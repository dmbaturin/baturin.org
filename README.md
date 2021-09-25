baturin.org
===========

This is the source code of the www.baturin.org website.

It doubles as a showcase for my pet project, the [soupault](https://www.soupault.app)
website generator.

If you want a live example of a website build with soupault to learn from, you may want to
look at the source of the [soupault website](https://github.com/dmbaturin/soupault-website) instead.
My website used to be a good example, but now it's a testbed for unusual features,
and its workflow its quite complicated for a variety of reasons
(including legacy of its old, pre-soupault versions).

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

1. Run `soupault --index-only` to extract the metadata.
2. Use extracted metadata to generate blog archive pages with the `scripts/blog-archive.py` script.
3. Run `soupault` to process all page source files and copy assets to `build/`
4. Fetch and process pages that have their own repositories (iproute2 manual and encapcalc).

My deployment process is just rsync to a remote server.


