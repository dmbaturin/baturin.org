# If you think ReasonML compiles to JS, you are wrong

<time id="last-modified">2019-09-05</time>
<tags>programming, ocaml, js</tags>

<p id="summary">
In this post we'll examine what <a href="https://reasonml.github.io/">ReasonML</a> really is and what it compiles to.
Everyone coming from the ML community already knows the truth, but in the JS community, this misconception
seems surprisingly common. It's not just about giving credit, but about the true potential of the language
that is far greater than that of TypeScript or Elm.
</p>

## Some theory

Modern compilers are rarely monolithic. Instead, they are split into multiple independent parts
that deal with a specific compilation stage.

Typical workflow is:

```
Concrete syntax → Abstract syntax → Intermediate representation → Executable code
```

The concrete syntax is what the programmer writes. That's also what people are arguing about a lot,
but in reality, it's a rather trivial issue. There's no real difference between `if(x == 0) { y = 1 }`
and `if x = 0 then begin y := 1 end`. The way the language looks has little to do with the way it works.
It's quite easy to make a language look very different. In the simplest case, all you need to do is a bunch
of substitutions, that's how Steve Bourne made his C code of the original Bourne shell look like Algol
(`begin` → `{`, `end` → `}` etc.).

After translation to the abstract syntax, all that disappears. Both examples above could be translated to
the same abstract syntax like `COND(BINOP(=, x, 0), BINOP(ASSIGN, y, 1), NONE)`.
This approach makes it possible to modify and extend the concrete syntax (e.g. add &ldquo;syntactic sugar&rdquo;)
without modifying anything down that pipeline.

Operations like type checking are normally done on the abstract syntax.

If the program passes the type checks and so on, the abstract syntax is translated to an intermediate representation. That's a code
for an abstract machine that is easy to translate to the target—typically an assembly language, but it can as well
be another language, like JS. Optimizations are usually done on the intermediate representation.

## So, what ReasonML really is?

Just as its own [website](https://reasonml.github.io/docs/en/what-and-why) says (sadly, not on the front page),
ReasonML is an alternative concrete syntax for the [OCaml](https://ocaml.org) language.

It's translated to the abstract syntax of the INRIA's OCaml compiler and it relies on the OCaml compiler for type checking,
optimizations, and everything else past the concrete syntax stage.

Thus, it doesn't compile to anything executable at all. It's best viewed as a plugin for OCaml.

You don't need ReasonML to use the rest of the toolchain, whether you want to compile your code to JS or anything else.
And your options are far wider than JS: the mainline OCaml can produce native executables for x86, ARM, and a bunch of other
architectures as well as bytecode executables that can run on any platform, all for GNU/Linux, *BSD, macOS, and Windows.

Howver, mainline OCaml doesn't support compiling to JS.

## What is compiled to JS?

The intermediate representation produced by the OCaml compiler. Compilation to JS happens at the stage when the concrete
and even abstract syntax is irrelevant.

You can see it in the diagram in ReasonML's own [README](https://github.com/facebook/reason/tree/master/src#repo-walkthrough).

The alternative compiler backend that produces JS instead of native code is called [BuckleScript](https://bucklescript.github.io/).
The recommended ReasonML setup uses it, but they are separate, independent projects that can exist without each other
(but cannot exist without the upstream OCaml compiler).

BuckleScript is not the only way to compile OCaml to JS. The other way is [js_of_ocaml](https://ocsigen.org/js_of_ocaml/dev/manual/overview)
that works on bytecode and doesn't aim to produce human-readable JS, but is much easier to plug into an existing build pipeline.

## Corollaries

So, what you are missing by thinking that ReasonML is a language compiled to JS?

* The other, maintstream OCaml syntax that most OCaml projects (including ReasonML itself) use.
* Compilation of the same modules to native code.

In short:

* You don't need ReasonML to use the OCaml toolchain (native or BuckleScript) if you don't like it.
* You can use ReasonML with both native and BuckleScript toolchains if you like it.
