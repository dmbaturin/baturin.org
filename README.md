baturin.org
===========

This is the source code of the www.baturin.org website.

It's built with [soupault](https://www.soupault.app), my own website generator framework.
It does showcase soupault's capabilities so it you want to look at an elaborate soupault setup,
go ahead and read the source.
However, it's not an exemplary project always kept in a good shape, it's my personal website,
so it can be messy and suboptimal. You have been warned.

If you want a live example of a website build with soupault to learn from, you may want to
look at the source of the [soupault website](https://github.com/PataphysicalSociety/soupault.app) instead.

If you want to build a modified version of the website impersonate me, hijack the domain first, then read on.

## Site structure

* `soupault.toml` — the config file.
* `site/` — page source files and static assets.
* `templates/` — page templates and includes. The page template is `templates/main.html`
* `scripts/` — helper scripts.

Things in assets that may be of interest:

* `site/styles/` — the CSS files.

## Build process

As you can see from the `Makefile`, the full build process is:

1. Run `soupault` to build the website.
2. Fetch and process pages that have their own repositories (iproute2 manual and encapcalc).

My deployment process is just rsync to a remote server (`make deploy`).
For the development web server, I just use Python's `http` module (`make serve`).

