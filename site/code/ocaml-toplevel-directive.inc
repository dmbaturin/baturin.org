{% set date 2017 Nov 23 %}

<h1>Writing your own OCaml toplevel directives</h1>

<p>
The OCaml directives that we all know and love (<code>#use</code>, <code>#load</code> and so on) are literally not a part of the language.
They never even get to the OCaml parser and are processed by the toplevel binary (<kbd>bin/ocaml</kbd>) itself. This is why trying to use them in source code that
is to be compiled with ocamlc or ocamlopt will cause compilation errors.
</p>
<p>
Somewhat confusingly, they do work if you execute source files with ocaml non-interactively, but that's just because it's equivalent to
starting ocaml as a REPL and pasting the code into it.
</p>
<p>
Anyway, the most interesting thing about the directives is that they are not hardcoded and it's possible to make your own.
In fact, some of the most widely used directives such as <code>#require</code> are provided by separate modules, which is why <code>#require</code> doesn't work until you
load the findlib module.
</p>
<p>
There is one secret you need to know though: the libraries that provide that functions are not in the default search path
and you need to add the path explicitly with <code>#directory "+compiler-libs";;</code>.
</p>
<p>
The directives are simply functions that are stored in a hash table called <code>Toploop.directive_table</code> along with their help data.
You can add them directly with <code>Hashtbl.add Toploop.directive_table</code> but this is deprecated and you should better use the
<code>Toploop.add_directive</code> function.
</p>
<p>That function and the types it uses are defined as follows:</p>
<pre>
type directive_fun =
   | Directive_none of (unit -> unit)
   | Directive_string of (string -> unit)
   | Directive_int of (int -> unit)
   | Directive_ident of (Longident.t -> unit)
   | Directive_bool of (bool -> unit)

type directive_info = {
  section: string;
  doc: string;
}

val add_directive : string -> directive_fun -> directive_info -> unit
</pre>

<p>
The first argument is the directive name. This is enough knowledge to create some useless directives. Here's a snippet
you can paste into the REPL and play with:
</p> 

<pre>
(* Important! *)
#directory "+compiler-libs";;

let () = Toploop.add_directive "hello"
  (Toploop.Directive_none (fun () -> print_endline "Hello world"))
  Toploop.{section="Goofing around"; doc="Prints \"hello world\""} ;;

let () = 
  Toploop.add_directive "greet"
  (Toploop.Directive_string
     (fun s ->
        Printf.printf "Hello %s\n" s))
  Toploop.{section="Goofing around"; doc="Prints a greeting"} ;;
</pre>

<p>Now you can try:</p>
<pre>
#hello;;

#greet "jrandomhacker";;

#help;; (* You'll see your section and commands there *)
</pre>
