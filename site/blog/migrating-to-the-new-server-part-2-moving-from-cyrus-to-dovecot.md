# Migrating to the new server part 2: moving from Cyrus to Dovecot</h1>

<time id="last-modified">2018-08-19</time>
<tags>servers, self-hosted</tags>

<p id="summary">
It's been a while since I published the <a href="/blog/migrating-to-the-new-server-part-1-base-system-and-the-web-server">first part</a>.
In case you were worrying how the rest of migration went, well, I ended up with an operational system quickly, even if because I decided not to change
the rest of it yet. However, a bug in Cyrus IMAPd made me make perhaps the biggest change in my email setup since its inception.
This post chronicles my migration to Dovecot.
</p>

# Brief history of my email setup

If you are also migrating from Cyrus to Dovecot and just want to see what I had to do, just skip to the next part.

My email setup remained the same at its core ever since I first moved to self-hosted email in 2010. Between 2007 and 2010 I've been using first usual
Gmail, then Google Apps with my own domain. Then I once logged into the web client and noticed that it generates ads based on the message content.
Frankly, I was quite shocked. I understand that it's a free of charge service supported by revenue from ads, but analyzing messages you've been
trusted to deliver and store is a line that I think should not be crossed. I briefly reviewed the alternatives and went on to make my own setup.
I think [Proton](http://protonmail.ch/) didn't exist at the time, but it's the only alternative I would really consider.

It's also not just about privacy, but also about control. Most email is unencrypted and will end up visible to recipient's email provider,
and quite possibly anyone else on the way if the remote server doesn't use TLS. The guarantee that even if some component of the software stack
makes a change you do not like, you still have a way out is more important. One my friend one made a long rant about Gmail automatically changing
all his IMAP folders and filters to the &ldquo;labels&rdquo; nonsense. I'm completely responsible for maintenance of the email setup,
but at least no change can happen without my knowledge (unless someone manages to break into the server and decided to change the setup).

Contrary to the popular opinion, self-hosted email is not nearly as much of a maintenance burden as some make it to be. Sure, even with ready to use
email suites you should better know what you are doing if you want your mail to be accepted by anyone else, but once it's setup, it will work fine
without your attention.

The combination of Postfix for SMTP and Cyrus for IMAP was rock solid for years. But sometimes...

# The Cyrus bug

After I've moved to Fedora 27, everything looked fine for a while. Then I noticed that sometimes messages disappear. It was so unusual that I didn't
believe it initially, but the evidence was undeniable. Reconstructing the mailbox would get the lost messages back, so it was obviously a bug.
There was nothing in logs about the details however, and no obvious way to reproduce or debug it, I could only correlate it with 
`assertion failed: imap/index.c: 228: record->uid == im->uid` line in the logs, which is hardly useful for anyone not familiar with its source code,
but clearly indicative of a bug. Additionally, the CPU load of imapd would often go all the way to 100%, often coinciding with lost message events.

Then I've found two bug reports: [2171](https://github.com/cyrusimap/cyrus-imapd/issues/2171) and [2208](https://github.com/cyrusimap/cyrus-imapd/issues/2208)
with similar manifestations. It was reassuring to know that the maintainers already know, and I hoped they will fix it soon, and kept reconstructing
my mailbox as needed, but time went on, and the bugs were still open, and my mailbox metadada still kept getting corrupted.

It thought that I've had enough of it and started looking for the alternatives. Dovecot looked good, and I only heard good things about it,
it also turned out that my friend uses it, so the decision was made.

# Migrating to Dovecot

## Migrating the messages

The migration method was my main concern. I wanted to do it with minimal downtime, and ideally also have a way to roll back if migration goes wrong
so that I wouldn't be left without working email.

The [migration guide](https://wiki2.dovecot.org/Migration/Cyrus) suggested either converting the mailboxes with scripts, or using `dsync` for migration
via IMAP. The cyrus2dovecot script was last modified two years ago and I couldn't find any recent reports of successful migrations done with it.
In addition, it would have to be done offline, so that method didn't look attractive.

Migration with [dsync](https://wiki2.dovecot.org/Migration/Dsync) looked much more attractive since it could be done without touching the original mailbox
data at all, and I could just start over is it went wrong.

IMAP migration is mostly meant from migrating from another server, not just from another IMAP implementation. Since both Cyrus and Dovecot would be running
on the same machine, I had to make a simple workaround: make them listen on different addresses.

In `/etc/cyrus.conf` we need to prevent imapd from listening on `0.0.0.0` and `::` addresses so that Dovecot can listen on the same port on different address:

```
# UNIX sockets start with a slash and are put into /var/lib/imap/sockets
SERVICES {
  # add or remove based on preferences
  imap          cmd="imapd" listen="127.0.0.1:imap" prefork=5
  imaps         cmd="imapd -s" listen="$wanAddress:imaps" prefork=1
#  pop3         cmd="pop3d" listen="127.0.0.1:pop3" prefork=3
#  pop3s                cmd="pop3d -s" listen="pop3s" prefork=1
  sieve         cmd="timsieved" listen="sieve" prefork=0
```

Now in `/etc/dovecot/dovecot.conf` replace the default `listen` line with a loopback address other than `127.0.0.1`:

```
listen = 127.0.0.10
```

This way they both can run on the same machine without address conflict.

To have Dovecot automatically create mailboxes for users if they don't yet exist, I needed to specify `mail_location` explicitly.
In `/etc/dovecot/conf.d/10-mail.conf`:

```
mail_location = maildir:~/Maildir
```

I decided to store all mail in home directory of the user for ease of backup, hence `~/Maildir`.

Then I needed to add configuration for connecting to the original IMAP server. I use PAM auth and IMAP user names are just login names
(without the domain part), so the user format was `%u`. I had to specify plaintext password there, but for migration it's fine I guess.

I created a separate file for those settings, `/etc/dovecot/conf.d/90-migration.conf`:

```
imapc_host = 127.0.0.1
imapc_user = %u
imapc_password = <my password>
imapc_features = rfc822.size fetch-headers
imapc_port = 143

#mail_prefetch_count = 20
mail_shared_explicit_inbox = no

```

The `mail_prefetch_count` setting is recommended by Dovecot authors, but in the presence of that bug that caused metadata corruption
and high CPU load, it didn't work well, so I traded speed for stability.

The setup was ready for migration. First I reconstructed the original Cyrus mailboxes one more time just to be sure metadata is in order:

```
sudo -u cyrus reconstruct -V max
```

Then I ran the migration:
```
doveadm  -Dv sync -R -u $myUser imapc:
```

## Setting up Dovecot to work with Postfix

I only needed to make a few adjustments to the default setup from Fedora. It has very sensible defaults otherwise.

In `/etc/dovecot/conf.d/10-master.conf` I replaces the default `service lmtp` section with:

```
service lmtp {
 unix_listener /var/spool/postfix/private/dovecot-lmtp {
   group = postfix
   mode = 0600
   user = postfix
  }
}
```

...and made a corresponding change in `/etc/postfix/main.cf`:

```
mailbox_transport = lmtp:unix:private/dovecot-lmtp
```

The final change I had to make was to change the username format to "%Ln". Since Postfix sends messages to `$user@$domain`,
but IMAP user names in my setup are just system user names, the domain part had to be stripped, else it would result
in bounced messages and `User does not exist` errors.

In `/etc/dovecot/conf.d/10-auth.conf`:

```
auth_username_format = %Ln
```

## Setting up sieve

I had to install `dovecot-pigeonhole` package and change the `mail_plugins` line to `mail_plugins = $mail_plugins sieve`
in `/etc/dovecot/20-lmtp.conf` and `/etc/dovecot/20-lda.conf`. That's all I had to do.

## Outcomes

Well, first of all, there is no metadata corruption anymore! It also works much faster, and without unmotivated CPU load spikes.

I have to admit that while I've been using Cyrus since some 2006, I never liked it much, or attained complete command of it.
I started using it just because my friends were using it, and then just stuck with it. However, its complicated relationship
between system users and virtual users always annoyed me, and I never found managing it easy.

With Dovecot, my sieve script in `~/.dovecot.sieve` just worked, and when it didn't work, it saved a nice, readable log in
`~/.dovecot.sieve.log`. Much easier to use than sieveshell I have to admit.

So far it's working great and it's unlikely that I'm going to regret the decision.
