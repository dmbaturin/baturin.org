<h1>What I wish I knew when learning OCaml</h1>

<p>This is a poorly structured list of common questions and answers about OCaml
   I had to ask myself or have been asked often.</p>

<div>
<div id="generated-toc"> </div>

<h2>The language</h2>

<h3>The double semicolon</h3>
<p>The answer to the question &ldquo;When do I need the <strong>;;</strong> within
   OCaml source code?&rdquo; is <strong>never</strong>.</p>
<p>It's not a part of the language and is only used by the interpreter as an end of input mark.</p>

<p>If you want to execute something for its side effects and don't care about its return value,
   you should wrap it in a let-expression and match it against the wildcard pattern:</p>
<pre>
let _ = print_endline "hello world"

let _ = exit 0
</pre>

<p>If you call a function that returns unit, you can also match the result against the unit value, <tt>()</tt>.</p>
<pre>
let () = print_endline "hello world"
</pre>
<p>The advantage over wildcard is that if you accidentally write an expression that has type other than
   unit (e.g. forget the argument), the compiler will throw a type error.</p>

<p><strong>Note:</strong> If you use ocaml as a non-interactive script interpreter  (i.e. <tt>ocaml /path/to/file.ml</tt>),
   you do need the ;; after directives, as in <tt>#use "topfind";;</tt>. But the directives are not part
   of the language either.</p>
<p><strong>Historical note:</strong> In CAML Light, the predecessor of OCaml, double semicolons were mandatory.
   For this reason they are quite common in old code originally written in CAML Light or written in the early days
   of OCaml. These days they are considered a bad style.</p>

<h3>Semicolons and expression sequences</h3>
<p>Semicolon is an expression separator, not a statement terminator.</p>
<p>This code</p>
<pre>
let foo x y =
    print_endline "Now I'll add up two numbers";
    print_endline "Yes, seriously";
    x + y
</pre>
<p>is semantically equivalent to</p>
<pre>
let foo x y =
    let _ = print_endline "Now I'll add up two numbers" in
    let _ = print_endline "Yes, seriously" in
    x + y
</pre>
<p>Therefore, if you end a sequence of expressions with a semicolon, whatever comes next will
   be treated as the last expression of the sequence, and will likely cause a syntax error
   (e.g. if the next thing is a let binding) or, worse, incorrect behaviour.</p>

<h3>Shadowing is not mutation</h3>

<p>Some uses of top level let bindings may look like mutation, but they actually aren't.</p>

<pre>
let x = 10
let x = x + 10
let () = Printf.printf "%d\n" x
</pre>

<p>In this example, the <em>x</em> in the printf expression is 20. Did we redefine <em>x</em> for the <em>whole program</em>?</p>
<p>The answer is no. Every let binding opens a new scope. In case of top level bindings, the scope they open continues until the
   end of the file. So &ldquo;let x = x + 10&rdquo; takes the value of <em>x</em> from the outer scope, adds 10 to it,
   and opens a new scope where <em>x</em> is bound to 20.</p>
<p>However, it has no effect on the outer scope. We can demonstrate it by creating some closures that use the old <em>x</em>.</p>

<pre>
let x = 10
let print_old_x () = Printf.printf "%d\n" x

let x = x + 10
let () = Printf.printf "%d\n" x  (* prints 20 *)
let () = print_old_x ()          (* prints 10 *)
</pre>

<h3>Functions and currying</h3>
<p>The good thing about currying in OCaml and other ML-style languages is that you don't need to know what
   currying is to use it. Every time you create a function of &ldquo;multiple arguments&rdquo;,
   you really create a curried function. Which means that</p>
<pre>
let add x y = x + y
</pre>
<p>is a sugar for</p>
<pre>
let add =
  fun x -&gt;
    fun y -&gt; x + y
</pre>
<p>This makes partial application very easy: just omit some of the arguments 
   and what you get is a new function.</p>
<pre>
# let add1 = add 1 ;;
val add1 : int -&gt; int = &lt;fun&gt;
</pre>
<p>To be precise, what you get is a closure: a function plus an environment
   where name <em>x</em> is bound to 1, but you don't need to think about it
   to use it.</p>
<p><strong>Note:</strong> this is also the reason OCaml never gives you
   &ldquo;too few arguments&rdquo; errors if you forget an argument. Only when you try to use
   that value, it tells you that your value has type <em>foo &rarr; bar</em> but was expected
   to have type <em>bar</em>. If you see an error like that, check for
   missing arguments. Example:</p>
<pre>
# (add 1) + 9 ;;
Error: This expression has type int -&gt; int but an expression was expected of type int 
</pre>

<h3>Mutable record fields vs. references</h3>

<p>They are the same thing. A reference is a record with the sole mutable field called &ldquo;contents&rdquo;,
   as you can see from the Pervasives source code.</p>

<pre>
type 'a ref = { mutable contents : 'a }

let (:=) r v = r.contents &lt;- v
let (!) r = r.contents
</pre>

<p>Therefore the choice between references and mutable fields is mostly aesthetic. However, if you store
   references in record fields, converting them to mutable records may make the code slightly faster.</p>

<h3>Is there any difference between begin/end and parentheses?</h3>

<p>
Not really. The real difference between begin/end in OCaml and identical looking construct in Pascal (or curly braces in C)
is that in Pascal, they create a <em>compound statement</em>, while in OCaml they work as expression delimiters.
In other words, it's just a more readable alternative to parentheses.
</p>
<p>Consider these examples:</p>
<pre>
let foo () =
  (for i = 1 to 3 do
     print_endline "I'll return 0, I tell you!"
   done);
  0

let x = 3 + begin 2 end 
</pre>

<h3>Type definitions</h3>

<p>The ML term for a tuple is <em>product type</em>. A natural example of a product type
   is a point on a plane that has X and Y coordinates.</p>
<pre>
type point = float * float
</pre>
<p>Product types are rarely named, and the compiler will not start calling everything that
   is float * float a point after you declare this type. They are usually implicit or
   incorporated into definitions of sum types and records.</p>
<p>Another kind of types that has no direct analogy in most other languages is
   <em>sum type</em> (variant records in Pascal and Ada are pretty close). Easier to show than
   to explain. An example: in color printing, dot color is either the key color (usually black)
   or a combination of cyan, yellow, and magenta. We can represent colors with this type:</p>
<pre>
type color = Key | Color of int * int * int
</pre>
<p>Sum and product types are referred to as <em>algebraic data types</em>.</p>
<br />
<p>Some jargon you should know:</p>
<img src="/images/caml-faq/ocaml_type_definition.png" alt="Type definition diagram"/>
<p>A type is <em>polymorphic</em> if there are free <em>type variables</em> &mdash;
   placeholders that can be substituted for any type. Type names are
   often referred to as <em>type constructors</em> (a polymorphic type <tt>'a list</tt>
   can produce <tt>int list</tt>, <tt>string list</tt> and so on).</p>
<p>Type variables are always prefixed with an apostrophe, names can be arbitrary.
   This is perfectly acceptable:</p>
<pre>
type ('result, 'error) result = Success of 'result | Error of 'error
</pre>
<p>The elements of a sum type, such as Left and Right above, are called <em>data constructors</em>.
   Their names must be capitalized. Data constructors can be nullary, this is equivalent to enumerated
   types in other languages such as C enum.</p>
<pre>
type ml = OCaml | SML | FSharp (* Nullary constructors *)
</pre>

<p>There are two types for which special syntactic sugar is provided: unit and list.
   In imaginary syntax their definitions can be written as:</p>
<pre>
type unit = ()

type 'a list = 'a :: 'a list | []
</pre>
<p>The <strong>()</strong> and <strong>::</strong> are special only syntactically,
   otherwise they are just data constructors like any other and can be used in pattern
   matching and anywhere else where normal constructors are allowed.</p>

<p><strong>Note:</strong> in print and blackboard writing people often use greek letters
   for type variables, e.g. <em>&alpha; list</em> rather than <tt>'a list</tt>.
   Likewise, the <tt>*</tt> in product types is often substituted for the &times; sign.</p>

<h3>let vs. let rec</h3>

<p>At first the <strong>rec</strong> keyword may look like syntactic noise, but it isn't.</p>
<p>If the name of your binding is unique within the scope, <strong>rec</strong> simply allows you
   to refer to its name within your expression, but if you are shadowing another binding,
   it allows you to choose whether that name will refer to the older binding or to the newly defined one.</p>
<p>An example: suppose you want to redefine the <strong>incr</strong> function from Pervasives so that it
   prints a debug message when called.</p>
<p>Let's define it. Note the lack of <strong>rec</strong>.</p>
<pre>
let incr i =
    print_endline "Counter was incremented";
    incr i
</pre>
<p>Now let's try it.</p>
<pre>
# let x = ref 0 ;;
val x : int ref = {contents = 0}

# incr x ;;
Counter was incremented
- : unit = ()

# !x ;;
- : int = 1
</pre>
<p>This works as expected, our new incr calls the old incr, and everything within our new scope refers to the new incr definition.</p>
<p>However, if we use a definition with <strong>rec</strong>, <tt>incr i</tt> within our definition will refer to the <em>new definition</em>
   of <tt>incr</tt> rather than <tt>incr</tt> from Pervasives, thus creating a useless infinite loop.</p>

<h3>let vs. let ... and (mutually recursive bindings)</h3>

<p>
At first the <tt>and</tt> keyword may look like syntactic sugar that allows one to avoid writing <tt>let</tt> second time.
While it can be used this way (though arguably shouldn't), there's a deeper distinction between a sequence of <tt>let ... in</tt>
bindings and <tt>let ... and ...</tt>
</p>
<p>The distinction is that the <tt>and</tt> keyword allows mutually recursive bindings.</p>
<p>Consider this inefficient but illustrative example of functions that check if given number is even or odd:</p>
<img src="/images/caml-faq/even.png" alt="even(x) = true if x = 0, odd(x-1) otherwise"/>
<br />
<img src="/images/caml-faq/odd.png" alt="odd(x) = false if x = 0, even(x-1) otherwise"/>

<p>
None of them refers to itself, but they refer to each other, which makes them <em>mutually recursive</em>. The naive attempt
to translate that definition to OCaml:
</p>
<pre>
let even x =
    match x with
    | 0	-> true
    | _	-> odd (x - 1)

let odd	x = 
    mach x with
    | 0	-> false
    | _	-> even	(x - 1)
</pre>
<p>
will fail with <tt>Error: Unbound value odd</tt> error. That's because in the scope where <tt>even</tt> is defined, 
<tt>odd</tt> is not defined yet (remember that every let binding opens a new scope).</p>
<p>
This is where the <tt>and</tt> keyword comes into play: by using it, we can make OCaml treat multiple bindings
in a single new scope. This version will work as expected:
</p>
<pre>
let rec even x =
    match x with
    | 0	-> true
    | _	-> odd (x - 1)

and odd x =
    match x with
    | 0	-> false
    | _	-> even	(x - 1)
;;

# odd 19 ;;
- : bool = true

# even 42 ;;
- : bool = true

</pre>

<h3>Abstract types</h3>
<p>Abstract types is a way to implement encapsulation. A type is treated as abstract if module interface doesn't
   expose anything but its name.</p>
<p>In object oriented programming, hiding values inside objects serve two purposes: to abstract from implementation details,
   and to prevent invariant violations. These tasks are not really related to each other though, and it can be done with 
   different mechanisms.</p>
<p>In ML modules, we may hide (rather, not expose) functions to avoid cluttering the interface with implementation details.
   But if those functions are used with values that come from the outside world, how do we protect invariants?
   ML gives us ability to hide a <em>type</em> implementation inside a module and make functions work with that type
   rather than types known to the outside world. Then we can provide a function that creates values of that type.</p>
<p>Suppose we want to write a module for working with natural numbers whose values cannot be negative.
   Internally, the type we are working with will be <em>int</em>, but no one outside the module will know this,
   and therefore will not be able to use it with our functions without converting it first.</p>
<pre>
module type NATURAL = sig
    exception Negative_argument

    type t (* No implementation *)

    val from_int : int -> t (* Convert an int to the mysterious type t *)
    val to_int : t -&gt; int   (* Convert back to int *)
    val add : t -&gt; t -&gt; t   (* Note: t -&gt; t, not int -&gt; int *)
end

module Natural : NATURAL = struct
    type t = int

    exception Negative_argument

    let from_int x = 
        if x &lt; 0 then raise Negative_argument else x

    let to_int x = x

    let add x y = x + y
end
</pre>
<p>Let's see how it works:</p>
<pre>
# Natural.add 3 2 ;;
Error: This expression has type int but an expression was expected of type Natural.t   

# Positive.from_int (-10) ;;
Exception: Natural.Negative_argument. 

# let a = Natural.add (Natural.from_int 4) (Natural.from_int 3) ;;
val a : t = &lt;abstr&gt;

# Natural.to_int a ;;
- : int = 7
</pre>
<p>The type <em>t</em> is indeed <em>int</em> internally, but leaving it abstract makes OCaml treat it as
   a distinct type. If we change the signature to use <em>type t = int</em>, the <tt>add</tt>
   function will work with any values of the type <em>int</em>. If we make the <tt>add</tt> function <em>int &rarr; int</em>
   rather than <em>t &rarr; t</em>, it will have the same effect:</p>
<pre>
# Natural.add (-9) 8 ;;
- : int = -1
</pre>

<h2>The toolchain</h2>

<h3>Compiled? Interpreted?</h3>
<p>OCaml is a good demonstration that the terms &ldquo;compiled language&rdquo; and &ldquo;interpreted language&rdquo;
   are meaningless. Right in the distribution, there are:</p>
<ul>
  <li>An interpreter</li>
  <li>A bytecode compiler and bytecode interpreter</li>
  <li>A native code compiler</li>
</ul>

<p>The interpreter can work in both interactive mode (read-eval-print loop) and batch mode, like shell or Perl or Python.</p>
<p>The bytecode compiler is useful on platforms where the native compiler is not available (e.g. MIPS), or for bootstrapping the native code
   compiler on new machines (the bytecode interpreter is written in C). You can also produce Javascript from OCaml bytecode files
   with js_of_ocaml.</p>
<p> Bytecode executables are a lot slower than native ones, so most of the time people either use the interpreter for exploratory programming
and scripting or compile to native code. </p>
<p>Due to this, there are multiple executables:</p>
<dl>
  <dt>ocaml</dt>
  <dd>The interpreter</dd>
  <dt>ocamlc</dt>
  <dd>The bytecode executable of the bytecode compiler</dd>
  <dt>ocamlc.opt</dt>
  <dd>The native executable of the bytecode compiler</dd>
  <dt>ocamlopt</dt>
  <dd>The bytecode executable of the native code compiler</dd>
  <dt>ocamlopt.opt</dt>
  <dd>The native executable of the native code compiler
</dl>
<p>Fortunately, you don't have to remember it in most cases.</p>
<p><strong>Note:</strong> the REPL is often referred to as &ldquo;toplevel&rdquo;.</p>

<h3>Interactive interpreter is annoying to use</h3>
<p>No history, no completion, almost no line editing. A perfect environment for exploratory programming indeed.</p>
<p>There are two things you can do: use ocaml with rlwrap, or install utop.</p>
<p>If you want function name completion, utop is a better choice.</p>

<h3>How do I load source code files into the REPL?</h3>
<p>In the REPL you can load source files and binary libraries with interpreter directives.
   Directives start with <tt>#</tt> character, which sometimes confuses readers
   because <tt>#</tt> is also the REPL prompt.</p>
<p>If you want to load all definitions from <tt>file.ml</tt> into the REPL:</p>
<pre>#use "program.ml";;</pre>

<p>If you want to load a file as a module:</p>
<pre>#mod_use "program.ml";;</pre>

<p>For demonstration, suppose you have a file named <tt>program.ml</tt> in your current directory
with contents:</p>
<pre>
let hello () = print_endline "hello world"
</pre>
<p>Here's a session transcript that demonstrates how these directives work:</p>
<pre>
$ ocaml
        OCaml version 4.02.1

# #use "program.ml";;
val hello : unit -&gt; unit = &lt;fun&gt;

# #mod_use "program.ml";;
module Program : sig val hello : unit -&gt; unit end

# hello () ;;
hello world
- : unit = ()

# Program.hello () ;;
hello world
- : unit = ()
</pre>

<h3>How do I load compiled libraries in the REPL?</h3>
<p>If you have findlib installed and the library you want to load was installed with e.g. OPAM,
   the easiest way is to load the findlib package. For this purpose, findlib installs a handler
   for <tt>#require</tt> directive that can automatically load a package and its dependencies.</p>
<p>In utop, the topfind library is loaded by default, in standard OCaml REPL you'll have to load it
   yourself with <tt>#use "topfind";;</tt></p>
<p>Then you can list available packages with <tt>#list;;</tt> and load packages you want, e.g.
   <tt>#require "unix";;</tt>.</p>

<p>If you want to load a library that is not installed, use <tt>#load</tt> directive, as in
   <tt>#load "somedir/mylib.cma";;</tt>. Note that unlike findlib's <tt>#require</tt>,
   <tt>#load</tt> does not automatically load dependencies.</p>


<h3>How do I link libraries</h3>
<p>The easiest way to compile a program that uses libraries is to use ocamlfind from findlib.</p>
<pre>
ocamlfind ocamlopt -package unix -o prog.native prog.ml
ocamlfind ocamlc -package unix -o prog.byte prog.ml
</pre>
<p>You can do it by hand, but you'll have to specify different library file names (.cma vs .cmxa) for bytecode
   and native versions, while ocamlfind will do it automatically for you.</p>

<h3>How do I produce module signatures automatically?</h3>
<p>Global type inference makes it possible.</p>
<pre>
ocamlc -i mymodule.ml > mymodule.mli
</pre>
<p>Sadly, it can't automatically update existing signatures (add what's missing and preserve what's already
   there if it was edited by hand), but copying autogenerated lines is still faster than writing them by hand.</p>

</div>

<time id="last-modified">2018-02-11</time>
