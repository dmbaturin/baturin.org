# Extending OCaml programs with Lua (soupault got plugin support)

<time id="last-modified">2019-08-10</time>
<tags>programming, ocaml</tags>

<p id="summary">
Most of the time, when people make extensible programs in typed functional languages,
they make a DSL, not least because it's much easier to make a DSL in a language with algebraic types
and pattern matching than in one without.<br />
Some use cases really require a general-purpose language though. That's where things get
more interesting. Commonly used embeddable interpreters such as Lua, Guile, or Chicken are written in C.
It's possible to make OCaml or Haskell bindings for them and such bindings do exist,
but that's two high level languages communicating through a low level one.<br>
It would be much better to be able to expose native types to the embedded language in a type-safe and more or less convenient
fashion. Here's my take at it.
</p>

## The use case: soupault website generator

[Soupault](https://baturin.org/projects/soupault) is a website generator based on HTML rewriting
instead of template processing. I made it for my own website out of conceptual disagreement
with the &ldquo;classic&rdquo; workflow with Markdown and front matter that no one seems to question.

I don't mind it for blogs like this one since the blog format itself is ridig, but for non-blog websites
it easily becomes limiting and forces you to either mix Markdown with HTML or invent custom extensions—both
approaches arguably defeat the purpose of Markdown.

Soupault works directly on HTML and uses CSS selectors for locating elements, for example,
&ldquo;insert file `site/about.html` into `<div id="content">`.
That allows you to use any imagineable formatting and also offers
features impossible with classic generators. You can make every page look different if you want,
how much of the page is a template and how much is content is up to you. Its TOC and footnotes widgets
can reuse existing ids and make the links persist even if the heading text changes completely.
It's also quite easy to use as a drop-in workflow upgrade for handwritten websites or other generators,
without losing original URLs—it doesn't force any workflow on you.

The cost of the templateless approach is that if something is not already supported, it cannot be done at all.
Generators that use logicless templates exclusively have the same problem. The usual way to solve that
problem is to support plugins.

Most generators fall into two categories: easily extensible but slow or fast but not extensible.
Those written in interpreted languages like Jekyll are trivial to add plugin support, and plugins
are easy to distribute.

Hugo is well known for speed thanks to being a native executable, but it's not extensible.

<a href="https://wyam.io/">Wyam</a> is a rare example of a middle ground, written for .Net.

Can we combine the native full speed of core components with extensibility? Soupault is written
in OCaml, which does support dynamic linking and it's quite easy to use, but the real problem
is distributing plugins. The users would need to compile them for their platform, and it's obviously
much harder than just dropping a file into the plugins directory.

## The Lua-ML project

I've discovered the [Lua-ML](https://github.com/lindig/lua-ml) project around the time
I started working on soupault, so I immediately started wondering if I can use it.

Lua-ML is a pure OCaml implementation of Lua. The great thing about it is that it's fully modular.
You can replace any part with your own module as long as the interface is compatible.
Link in new modules as if they were a part of the standard library. Sure.
Replace a part of the standard library with your module? That's possible.
Replace the AST interpreter but keep the library? That's possible too.
There are no black boxes.

These are the good parts. The bad part is that it comes from a now defunct research project—a compiler backend
named C--. One of the project members, [Christian Lindig](https://lindig.github.io/), salvaged it from C--
and published it on Github, but hadn't actively worked on it.

Last time any non-maintenance work was done on was around 2005 or so. I was to become its first real user, too.
It had a complicated build process due to its use of literate programming (even though most modules had no documentation)
and there was a fork of a 2004 version of the `Hashtbl` module from the standard library inside it.

Fortunately, Christian turned out to be very far from an unreachable maintainer. He gave me write access to the
original repository so that I could fix the issues, and answered all questions about the codebase he could answer.

After some long nights spent messing up with the code, and with a patch by [Gabriel Radanne](https://drup.github.io/)
that adds OPAM packaging, the build process was sane enough to make it a build dependency.

Another bad part is that it implements [Lua 2.5](https://github.com/lindig/lua-ml/blob/master/doc/lua-2.5-refman.pdf),
which was a rather limited language. Many improvements, including `for` loops were only made later.
But, that's a start.

For a simple interpreter example you can easily play with, check out [luaclient.ml](https://github.com/lindig/lua-ml/blob/master/example/luaclient.ml).
My goal for this post is to walk through are more realistic example from the soupault codebase
that can be found in [plugin_api.ml](https://github.com/dmbaturin/soupault/blob/master/src/plugin_api.ml).

## Plugin example

That was a rather long introduction. Let's see what using Lua-ML actually looks like.
We'll start with a plugin example. This is a very simple plugin replicating the `site_url`
feature of website generators that makes relative links into absolute URLs:

```lua
-- Converts relative links to absolute URLs
-- e.g. "/about" -> "https://www.example.com/about"

-- Get the URL from the widget config
site_url = config["site_url"]

if not Regex.match(site_url, "(.*)/$") then
  site_url = site_url .. "/"
end

links = HTML.select(page, "a")

-- That's Lua 2.5, hand-cranked iteration...
index, link = next(links)

while index do
  href = HTML.get_attribute(link, "href")
  if href then
    -- Check if URL schema is present
    if not Regex.match(href, "^([a-zA-Z0-9]+):") then
      -- Remove leading slashes
      href = Regex.replace(href, "^/*", "")
      href = site_url .. href
      HTML.set_attribute(link, "href", href)
    end
  end
  index, link = next(links, index)
end
```

As you can see, there's a `page` variable in the default environment.
There are also `HTML` and `Regex` modules made accessible to Lua. They are in fact
wrappers for [lambdasoup](https://github.com/aantron/lambdasoup) and
[ocaml-re](https://github.com/ocaml/ocaml-re) libraries.

## Assembling the interpreter

### Preparing modules

&ldquo;Abstract&rdquo; types are known as `userdata` in Lua. To expose our type to Lua,
we need to make a module matching this signature:

```ocaml
module type USERDATA = sig
  type 'a t (* type parameter will be Lua value *)
  val tname : string  (* name of this type, for projection errors *)
  val eq : ('a -> 'a -> bool) -> 'a t -> 'a t -> bool
  val to_string : ('a -> string) -> 'a t -> string
end
```

So, we need a type, a string name for it, and functions for equality and string conversion.

The lambdasoup library uses phantom types to distinguish between element nodes 
and non-elements (roots, text, and whitespace) for better type safety: internally all nodes have the same structure,
but their types are artificially made different so that you can't do things that make no sense,
like inserting a child into a text node. We'll artificially force that type to monomorphic with
a simple sum type wrapper and some conversion/coercion functions:

```ocaml
module Html = struct
  type soup_wrapper = 
    | GeneralNode of Soup.general Soup.node
    | ElementNode of Soup.element Soup.node
    | SoupNode of Soup.soup Soup.node

  type 'a t = soup_wrapper

  let tname = "html"
  let eq _ = fun x y -> Soup.equal_modulo_whitespace (to_general x) (to_general y)
  let to_string _ s = Soup.to_string (to_general s)

  let from_soup s = SoupNode s

  let from_element e = ElementNode e

  let to_element n =
    match n with
    | ElementNode n -> n
    | _ -> raise (Plugin_error "Expected an element, but found a document")

  let to_general n =
    match n with
    | GeneralNode n -> n
    | ElementNode n -> Soup.coerce n
    | SoupNode n -> Soup.coerce n

  let select soup selector =
    to_general soup |> Soup.select selector |> Soup.to_list |> List.map (fun x -> ElementNode x)

  let get_attribute node attr_name =
    to_element node |> Soup.attribute attr_name

  let set_attribute node attr_name attr_value =
    to_element node |> Soup.set_attribute attr_name attr_value
end

```

Now we need to make modules that provide embedding and projection for out types (that is, conversion
to and from Lua values). For that we need to feed our module to a `Lua.Lib.Combine` functor.
It provides multiple different functors for different number of modules to handle, we'll use the `Lua.Lib.Combine.T2`
one fo handle the built-in `Luaiolib.T` module (that provides I/O functions) and our module at once:

```ocaml
module T =
  Lua.Lib.Combine.T2 (Luaiolib.T) (Html)

module LuaioT = T.TV1
module HtmlT  = T.TV2
```

Now `LuaioT` and `HtmlT` modules are ready to use. Use for what exactly? For assembling a complete Lua library.

The regex module works with strings, which are supported by Lua-ML without resorting to custom types,
so it's just a simple wrapper for `ocaml-re` and we do not need to do anything special with it.

```ocaml
module Re_wrapper = struct
  let replace ?(all=false) s pat sub =
    try
      let re = Re.Perl.compile_pat pat in
      Re.replace ~all:all ~f:(fun _ -> sub) re s
    with Re__Perl.Parse_error | Re__Perl.Not_supported ->
      raise (Plugin_error (Printf.sprintf "Malformed regex \"%s\"" pat))
(* ... *)
```

### Assembling the library

This is the complicated part. The first stage is to create a functor that will convert our `Html` module
to a Lua library and register the Lua-visible `HTML` and `Regex` modules in the interpreter state.

The functor will take a `Lua.Lib.TYPEVIEW` module setup with type `'a Html.t` to make the module
with embedding and projection functions from it. 

Simply creating such a module will not yet expose it to Lua. For that we need to pass a list of function name
and function tuples to `register_module`.

Lua-friendly functions are created from OCaml functions using combinators from the `C` module create by the `Luavalue.Make`
functor.

```ocaml
module MakeLib
  (HtmlV: Lua.Lib.TYPEVIEW with type 'a t = 'a Html.t) :
  Lua.Lib.USERCODE with type 'a userdata' = 'a HtmlV.combined =
struct
  type 'a userdata' = 'a HtmlV.combined
  module M (C: Lua.Lib.CORE with type 'a V.userdata' = 'a userdata') = struct
    module V = C.V
    let ( **-> ) = V.( **-> )
    let ( **->> ) x y = x **-> V.result y
    module Map = struct
      let html = HtmlV.makemap V.userdata V.projection
    end (* Map *)
   
    let init g = 
      C.register_module "HTML" [
        "select", V.efunc (Map.html **-> V.string **->> (V.list Map.html)) Html.select;
        "get_attribute", V.efunc (Map.html **-> V.string **->> V.option V.string) Html.get_attribute;
        "set_attribute", V.efunc (Map.html **-> V.string **-> V.string **->> V.unit) Html.set_attribute;
        (* ... *)
      ] g;
      
      C.register_module "Regex" [
        "replace", V.efunc (V.string **-> V.string **-> V.string **->> V.string)
          (Re_wrapper.replace ~all:false);
        (* ... *)
      ] g
  end (* M *)
end (* MakeLib *)
```

Now we need to link those modules together:

```ocaml
module W = Lua.Lib.WithType (T)
module C  =
    Lua.Lib.Combine.C5
        (Luaiolib.Make(LuaioT))
        (Luacamllib.Make(LuaioT))
        (W (Luastrlib.M))
        (W (Luamathlib.M))
        (MakeLib (HtmlT))
```

And finally create an interpreter module:

```ocaml
module I =
    Lua.MakeInterp
        (Lua.Parser.MakeStandard)
        (Lua.MakeEval (T) (C))
```

## Passing values to the interpreter

That's all good, but to make it possible for plugins to modify internal values
of our program, we need to pass them to the interpreter.

This is where the `HtmlT` module we created is needed. It provides a `makemap`
function that creates a record whose fields are functions, among them the `embed`
and `project` we need:

```ocaml
let lua_of_soup s =
  let v = HtmlT.makemap I.Value.userdata I.Value.projection in
  v.embed s

let soup_of_lua l =
  let v = HtmlT.makemap I.Value.userdata I.Value.projection in
  v.project l
```

## Running the interpreter

Finally we can setup an environment and run Lua code in it:

```ocaml
let state = I.mk () in
let soup = Soup.parse "<p>hello world</p>" in
let () = I.register_globals ["page", lua_of_soup (Html.SoupNode soup)] state in
let _ = I.dostring state "print(page)" in

```

The `I.dostring` and `I.dofile` functions return a list of Lua values now.
It's not very easy to work with, and worse, execution errors are only logged
to `stderr` and the caller has no easy way to see if plugin execution succeeded
or failed. That's definitely one of the things to fix.


