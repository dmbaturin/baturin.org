baturin.org
===========

This is the, ahem, source code of the www.baturin.org website.


You probably have no reason to build or modify it. Leave it to me, I know what I'm doing.

If for whatever reason you still want to build it, just run "make" (you need GNU Make).
You'll also need the OCaml compiler, fileutils module and the MPP template processor.

```
opam install fileutils mpp
```

The result will be in the build/ directory.


It uses an ad-hoc website generator. Because you know, there are so many of them it's easier
to make a new one than to choose. Besides, most of them want you to use markdown or another
limited markup language that doesn't even allow you to insert <marquee> tags.
