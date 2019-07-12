baturin.org
===========

This is the source code of the www.baturin.org website.

It doubles as a showcase for my pet project, the [soupault](https://baturin.org/projects/soupault)
website generator.

If you are here to see what you can do with soupault and what the workflow looks like, read on.

If you want to build a modified version of the website impersonate me, hijack the domain first, then read on.

## Site structure

* `soupault.conf` — the config file.
* `site/` — page source files.
* `templates/` — page templates and includes. The page template is `templates/main.html`
* `assets/` — files that are not managed by soupault but copied unchanges. CSS, scripts, pics, downloads...
* `scripts/` — helper scripts.

Things in assets that may be of interest:

* `assets/styles/` — the CSS files.

Generally, the `assets/` dir is a mess because a lot of the structure was moved from an old website
in a way that keeps URLs the same. Link rot mitigation measures ain't pretty, but link rot itself
is even worse.

## Build process

As you can see from the `Makefile`, the full build process is:

1. Run `soupault`. Generated pages end up in `build/`
2. Copy `assets/` to the `build/` dir.
3. That's all.

My deployment process is just rsync to a remote server.


