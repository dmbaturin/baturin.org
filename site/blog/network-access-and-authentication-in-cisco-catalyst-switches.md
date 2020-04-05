# Network access and authentication in Cisco Catalyst switches

<time id="last-modified">2018-03-31</time>
<tags>networks, cisco</tags>

<p id="summary">
From the fact that I'm a <a href="https://vyos.io">VyOS</a> maintainer, you can guess that I'm not a fan of Cisco routers,
but I'm a big fan of Cisco Catalyst switches. They have never failed me yet, and among all L2 switches they have
the least annoying CLI, even though it does have its issues. For L3 switches there may be better alternatives,
but for L2, I haven't seen anything better than Catalyst so far.
</p>

The first thing I do when I need to setup a new switch (or revamp a configuration of an old one) is configuring network access to it and user authentication.
For a long time, IOS has had a rather sane authentication framework known as `aaa new-model`, but I don't see it used as often
as I would like to. Cisco IOS has a bit too many legacy authentication mechanisms that one can use in combinations, which makes
things confusing and often less secure. The oldest approach with per-line passwords and enable passwords is still widespread,
maybe out of habit, maybe because it's conceptually simple and easy to explain to newcomers. The old approach is either annoying because it
requires that you remember two passwords for login and for enable, or adds no security if the password is the same. I recommend switching to
the new approach right away and never looking back.

In this post I'll attempt to explain the modern way in a beginner-friendly fashion rather than just give a sequence of commands to be used without understanding.
Before we make a recipe, let's review the concepts IOS uses for access and authentication. In the end we'll create a recipe for secure and
convenient local authentication that works on Catalyst switches, though it should be applicable to routers as well.

## &ldquo;Lines&rdquo; in Cisco IOS

Most server and desktop operating systems, as well as many newer operating systems of routers and switches, apply the same
authentication rules to all users no matter how exactly they access the system, whether it's a network connection, serial console, or anything else.
Even if they can do it differently, they do not do it by default.

Cisco IOS uses a different approach and has a concept of &ldquo;lines&rdquo;. This concept comes from big multi-user computers that had
multiple terminal lines (e.g. RS232 ports) where dumb terminals (e.g. DEC VT102) could be connected. These days people use terminal emulators
rather than physical terminals for serial console access of course. Cisco IOS extended the concept of lines to network connections &mdash;
rather than setting up SSH and telnet servers to use certain authentication and source address filtering options, you create virtual terminal
lines and set access options there, just like you do for the physical serial console line.

The default behavior of physical and virtual lines is not the same: physical lines allow connections without authentication when no authentication
options are set, while virtual lines deny it.

For virtual lines, you can set available protocols. If you want to enable both telnet and SSH, you can do:
```
line vty 0
  transport input telnet ssh
```

Since telnet is unencrypted, for IOS versions that support SSH, it's a good idea to leave telnet out.

## Authentication and authorization models

The old model, that is active by default, always uses separate passwords for login and for entering the privileged exec mode (`enable`).
It does support user accounts, but after logging in, a user still needs to enter the enable password. It also supports not having any users
at all, and instead setting a password for each terminal line. That model doesn't support RADIUS or granular privilege separation.

The new model, which is activated by `aaa new-model` commands always relies on user accounts and allows entering the privileged exec mode
automatically if the user account has sufficient privileges. The new model also supports all of the old methods and you can configure it
to use line passwords for authentication and enable passwords for privileged exec authorization if you really want to.

The new model allows you to create authentication method lists that can be assigned to different services, such as terminal lines,
remote access VPN services and so on. It includes a special list called `default` that defines methods used by default.

The method that uses user accounts created with the `username` command is called `local`. Other methods include RADIUS or TACACS+
server groups (that are created with `aaa group` commands) and the old methods such as `line` for line passwords.
In a typical small network the `local` method is what you want.
To enable local authentication by default, do:
```
aaa authentication login default local
```

Authorization is configured separately. For authorization based on locally assigned privilege levels, use the method called `local`.
You can also allow everything for every authenticated user with `if-authenticated` method.
To use local privilege levels for the privileged exec mode authorization, do:
```
aaa authorization exec default local
```

## Privilege levels

Cisco IOS supports up to 16 privilege levels. Level 1 allows a user nothing but entering the unprivileged mode where you cannot even view most
of the system information, much less modify anything.
Level 15 allows entering the priviliged exec mode, and thus gives a user full control over the system.

It is possible to assign special meanings to the levels in between, for example, to allow users with privilege 10 reboot the device,
you can do:

```
privilege exec level 10 reload
```

To set a privilege level for a local user, do:
```
username jrandomhacker privilege 15
```

An important thing is that every line has a maximum privilege level, and will not allow a user to raise the privilege above that level even
if the account privilege level is higher. Apparently that maximum level is set to 1 by default, which means no user will be able to enter
the privileged exec mode unless the maximum level is set to 15, so we need to do it:
```
line vty 0
  privilege level 15
```

While the CLI help calls the line privilege level a default level, in fact users with lower privilege levels will not be able to escalate
the privilege level above the level set for their account settings. To the contrary, not setting it to 15 is a perfect way to lock yourself
out of the device.

## The recipe

So, let's consider the simplest case that is very common in small networks: no remote AAA services such as RADIUS are used, all users are allowed
full access, and people are supposed to use SSH.

Assuming `ip ssh` is already setup, we need to do the following.

Enable the new model:
```
aaa new model
```

Set the system to use local user accounts and privilege levels for authentication and authorization:
```
aaa authentication login default local
aaa authorization exec default local 
```

Create a local user with the privilege level 15 that allows entering privileged exec mode:
```
username admin secret 0 mylongpassword
username admin privilege 15
```

Configure the virtual terminal line to only allow SSH:
```
line vty 0
  transport input ssh  
```

Optionally, explicitly set the line to use the default authentication method list:
```
line vty 0
  login authentication default
```

And, very important, set the maximum level of the line to 15 to allow users enter priviliged exec mode from that line:
```
line vty 0
  privileve level 15
```

## Afterword

I do not claim complete understanding of the authentication options, and the official documentation is not always
easy to follow, besides, IOS behaviour tend to vary slightly from one product line to another. To avoid locking
yourself out of your device, always make sure you have serial console access, or use the `reload in 5` commands
or similar before making a potentially risky change (then do `reload cancel` if it works).
