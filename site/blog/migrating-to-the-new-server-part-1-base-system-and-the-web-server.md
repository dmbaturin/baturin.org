# Migrating to the new server part 1: base system and the web server

<time id="last-modified">2018-03-09</time>
<tags>servers, self-hosted</tags>

<p id="summary">
My current VPS is running CentOS 6, and, frankly, it long started showing its age. Not very surprising if we remember that
it was released in 2011. Even with all efforts from EPEL, Remi, and Software Collections maintainers, running new applications
on new OS versions is obviously easier than on old verions. That's why I decided to migrate to Fedora, and it was much easier
than I thought.
</p>

## Choosing the platform

Despite the amount of time I spend messing up with Debian in the [VyOS](http://vyos.io) project, ultimately I'm a RedHat guy.
Or at least an RPM-based distro guy. I was a fan of the original RedHat back in early 2000's when I was a high school student,
then used OpenSuSE for a while, then switched my desktop to Debian solely because of my involvement with Vyatta, but eventually
switched to Fedora a few years ago and moved my VyOS development to dedicated Debian machines. Sorry Debian fans, nothing against
it, it's just not my cup of tea.

An obvious choice would be CentOS 7, but... It's already a few years old, in two years it will be switched from full support to
life support, it still only has Python 2.7 in its main repos, and due to being systemd-based it requires as much migration
as everything that came after it. The worst part, however, is that you cannot upgrade CentOS to the next version without
reinstalling the machine. My VPS went through miration from CentOS 5 to 6. This migration is unavoidable, but I would rather not
repeat it any soon afterwards.

What I liked about CentOS is that its long release cycle doesn't force you to migrate to a new version any soon. However,
I would be fine with just stable enough for my needs (doesn't break on updates) rather than ABI-stable for years. I don't run
enterprise applications or anything after all. However, going for a rolling release distro that would solve that problem by
continuous upgrades, doesn't feel safe, as they do break once in a while.

In the few years of using Fedora, I've updated it with `dnf system-upgrade` every time,
last time even skipping a version (from 25 to 27), and it never broke. So I thought this is a good compromise that is already
confirmed by my experience.

Ever since I got my first VPS in 2010, it's been something of a tradition that my friends who run small hosting companies
host it for me in exchange for help with network equipment configuration. So, [Alexander Norman](https://adminor.net/)
who hosts my current server generously setup a second machine for me and installed Fedora 27 server on it as per my request,
and the fun has begun.

## Setup choices

If this is a big migration anyway, it's also a good time to rethink the choice of software and setup. So far I decided to stick
with the same software for the most part, but I looked for alternatives.

I looked into Docker or plain LXC, but ultimately decided against it:
not quite the use case for them, and since the machine is self-contained and relatively limited in storage space, the added complexity
and storage overheard are probably not worth it. 

The most important decision was	to use SELinux in enforcing mode. Containers only isolate applications from one another,
but offer no additional hardening for applications themselves, so something would have to be done about it anyway.
Besides, I just wanted to learn more about SELinux. The operations side of my career has been mostly focused on networks
and virtualization for quite a long time, and I got a bit out of touch with the rest of the system administration world.

I also decided to enable HTTPS for all websites, because why not, everyone is doing it. The stuff I write doesn't pose
privacy concerns for anyone (unless being a nerd is illegal or immoral anywhere), but it's still a good thing to have.

My email system is mostly in sync with the latest security developments (DKIM etc.) so it likely won't need many changes.
I'm going to try to setup DNSSEC however.

I've also decided to keep the original hostname and the login banner associated with it:
```
haruko.baturin.org

-- When you're on a bike, the ocean is a lot closer than you think.
-- The autumn salt wind went right through to the back of my nose.
-- And maybe it's because, like Haruko said, my head was empty.
```
It comes from [FLCL](http://anidb.net/perl-bin/animedb.pl?show=anime&aid=117), which you should watch/read if you enjoy surreal coming of age stories.
Old nerds never grow up, you know.

But, in this part I'll describe the base system and web server setup as I go in the tradition of gonzo journalism, and leave the other parts for later.

## Filesystem layout fixup

My friend installed Fedora with some of the default disk layout options, and it created a separate logical volume for `/home`.
I assumed that the default would be everything in one partition, and didn't tell him how I want it setup,
so I can't blame him.

Still since a lot of data such as email, web content, and databases will live in /var, while /home will be relatively empty,
this setup just makes no sense for me. Good the default option used LVM. I've temporarily allowed root login in `/etc/ssh/sshd_config`
to login without opening any files in `/home`. Them, assuming that /boot is on `/dev/sda1` and the physical volume is on `/dev/sda2`,
it was just a matter of:

```
# mv /home/dmbaturin /tmp
# umount /home
# lvremove /dev/fedora/home
# lvextend /dev/fedora/root /dev/sda2
# resize2fs /dev/mapper/fedora-root
# mv /tmp/dmbaturin /home
```
Since `lvextend` automatically fills all free space on the physical volume if you specify the device it's on instead of size,
and `resize2fs` fill all free space on the volume if no size argument is given, I didn't even need to do any size calculations
in this case.

Compared to what we had to endure in the days of plain partitions and filesystems that could only be resized when not mounted,
LVM and ext4 make this part anticlimactic.

## Firewall

I can see why one may want `firewalld` on their desktop, but on servers, I think it just has no place there. It doesn't add any value
if the rules are all predefined, but does add complexity. So, normal `iptables` then, but the init scripts for loading rules on boot
are no longer there by default.

Luckily, the package that has systemd unit files is right there in the repos:
```
# iptables-services
# systemctl enable iptables.service
# systemctl enable ip6tables.service

```
Just put your rules in `/etc/sysconfig/iptables` and `/etc/sysconfig/ip6tables` respectively. You can use `iptables-save` to save them
in the loadable format.

My rules are pretty straightforward, for services I will configure first I need something like:
```
[root@haruko ~]# cat /etc/sysconfig/iptables
*filter
:INPUT ACCEPT [0:0]
:FORWARD ACCEPT [0:0]
:OUTPUT ACCEPT [0:0]

# Keep state
-A INPUT -m state --state ESTABLISHED,RELATED -j ACCEPT

# SSH
-A INPUT -m state --state NEW -m tcp -p tcp --dport 22 -j ACCEPT -m comment --comment "SSH"

# HTTP
-A INPUT -m state --state NEW -m tcp -p tcp --dport 80   -j ACCEPT -m comment --comment "HTTP"
-A INPUT -m state --state NEW -m tcp -p tcp --dport 443  -j ACCEPT -m comment --comment "HTTPS"

# ICMP
-A INPUT -p icmp -j ACCEPT

# Local traffic
-A INPUT -i lo -j ACCEPT

# Default actions
-A INPUT -j REJECT --reject-with icmp-host-prohibited
-A FORWARD -j REJECT --reject-with icmp-host-prohibited
COMMIT
```

It could benefit from `ipset` perhaps, but it can wait.

### What about nftables?

I want to love nftables, but at this point, I just can't. Yes, it's a promising tool, and I'm sure the kernel part is good already,
but when the most recent release of the userspace tool cannot display rule counters (even though it can configure rules to use counters),
that's... a tough sell at the very least. Also, its man page is one of the worst I've seen: the man pages of iproute2 give a complete and
correct command argument grammar in BNF at least.

I'll be watching the situation, but for now, I'm sticking with what actually works.

## Apache HTTPD and letsencrypt

My original plan was to prepare configs for all websites complete with HTTPS on the new server first and then move the data at once.
My old server was using StartSSL certificates originally. StartSSL was fun while it lasted I have to admit. Now it's gone, so I needed
an alternative, and Let's Encrypt seems like an obvious choice by now.

For better or worse, my original plan failed. Doing HTTP challenges by hand is a royal headache, and the `certonly` option in general
is not so nice either. At first I was sceptical and started thinking of getting certificates from a provider like Gandi for $50/year.
But I decided to try moving what's easy to move and give the automated workflow with certbot Apache HTTP plugin first,
and I wasn't disappointed.

On the old server, HTTPS was only enabled for applications that really need it, while public websites were HTTP only.
All sensitive applications were under a single SSL virtual host. It turned out my setup was based on an assumption that is no longer true:
that you cannot have name-based virtual hosts with SSL and use different certificates for them all.

Sure, it relies on a TLS extension, but as [this Apache wiki page](https://wiki.apache.org/httpd/NameBasedSSLVHostsWithSNI) suggests,
clients that do not support it have long reached EOS, and if they are not completely extinct by now, it's _their_ problem, not mine,
so there's no reason to care about them. Really, who in their right mind is still using Internet Explorer 7 and Windows XP, or pre-2.0 Firefox?

My Apache HTTPD config layout is a bit peculiar, I like to keep all virtual hosts configuration files in `/etc/httpd/vhosts.d/*.conf`
and include them with `Include vhosts.d/*.conf` from `/etc/httpd/config/httpd.conf`.
I was surprised to find out that certbot, when used as intended, picked that up and correctly created additional config files for HTTPS
virtual hosts under `vhosts.d`. So I've just ran `certbot --apache`, picked all websites, and it did its trick rather nicely.
Didn't even override anything in the original configs, just added a redirect to HTTPS as I've asked.

I'm still to migrate some other stuff and see if adding HTTPS for a new one will go just as smoothly, but I'm pretty confident that it will.

## LAMP and SELinux

Remember that I decided to keep SELinux on this time? Installing LAMP is trivial, but making it pass audit checks took me
a bit more time.

Before I tell you what've done, save this command somewhere, it's going to help you a lot.
```
ausearch -m avc -ts today | audit2why
```
It will always tell you _some_ solution for making the audit error go away. It will not always be the _best_ solution, however,
but rather a universal one. If you are into SELinux, you may want to use something more granular instead, for example,
correctly set file context instead of disabling checking it.

Matomo (formerly Piwik) was the first web application I decided to install, so I'll use it as an example.

### Allowing Apache to read files

So, as usual I copied the Matomo files to `/var/www/vhosts/matomo.baturin.org` and did `chown -R apache:apache` on it.
And right at the stage when it checks if it can write to `tmp/` directory, I got this:

```
In logs:
(13)Permission denied: [client ...:48310] AH00035: access to /index.php denied 
(filesystem path '/var/www/vhosts/.../index.php')
because search permissions are missing on a component of the path

In ausearch:

type=AVC msg=audit(1520619405.318:916444): avc:  denied  { getattr } for  pid=3961 comm="httpd"
 path="/var/www/vhosts/.../index.php" dev="dm-0"
ino=1575637 scontext=system_u:system_r:httpd_t:s0 
tcontext=unconfined_u:object_r:admin_home_t:s0 tclass=file permissive=0

Was caused by:
  Missing type enforcement (TE) allow rule.

```

The solution is to set the right context for those files so that Apache HTTP is allowed to read them.

```
chcon -R -t httpd_sys_content_t /var/www/vhosts/matomo.baturin.org/
```

Without looking further you can usually find out what the context should be by looking at the directories
created by packages with `ls -alZ`.

### Allowing Apache HTTPD to write to files

Now the Matomo installer page loads, but complains that `tmp/` directory is not writable. In ausearch we see this:
```
type=AVC msg=audit(1520627385.839:1094): avc:  denied  { write } for  pid=3829 comm="php-fpm" name="matomo.baturin.org"
dev="dm-0" ino=1704836 scontext=system_u:system_r:httpd_t:s0
tcontext=unconfined_u:object_r:httpd_sys_content_t:s0 tclass=dir permissive=0
```

The solution `audit2why` suggests is to set `httpd_unified` SELinux boolean to `true`. That would allow HTTPD to write
to any files with `httpd_sys_content_t` context though. Is it really what we want? I thought I like the idea of separating
application files from writeable data and instead set the context of those directories to one that allows writing: `httpd_sys_rw_content_t`.

```
chcon -t httpd_sys_rw_content_t /var/www/vhosts/matomo.baturin.org/tmp/
chcon -t httpd_sys_rw_content_t /var/www/vhosts/matomo.baturin.org/config/
```

### Allowing database connections

Now system checks pass, but Matomo installer complains that it cannot connect to MySQL.
There are two SELinux booleans that would allow it: `httpd_can_network_connect` and 
`httpd_can_network_connect_db`. The latter only allows connections to ports and sockets of well
known database servers such as PostgreSQL, MySQL, and Oracle. Since I don't have web applications
that would need generic network connections, I went for the second, more granular option:

```
setsebool -P httpd_can_network_connect_db=1
```

If you are wondering what exactly this or that boolean does, you can view the policy associated with it with
this command:
```
sesearch -A -b httpd_can_network_connect_db
```

## Conclusion?

Web server setup works, and that's a good progress. Next step is the email server, but that's for a later post.
