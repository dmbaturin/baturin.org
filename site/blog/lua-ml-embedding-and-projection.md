# Embedding and projection in Lua-ML

<time id="last-modified">2020-08-28</time>
<tags>programming, ocaml, lua-ml</tags>

<p id="summary">
One thing I find odd about many interpreter projects is that they are designed as standalone and can't be used as embedded
scripting languages. Lua-ML is completely different in that regard: it's designed as an extension language first and offers
some unique features for that use case, including a reconfigurable runtime library. You can remove modules from its standard library or replace
them with your own implementations. Of course, you can also pass OCaml values to Lua code and take them back in a type-safe manner too.
That aspect isn't very obvious or well-documented, so in this post we'll try to uncover it.
</p>

## Before we start

A small, ready to use example of using Lua-ML is provides by the [luaclient.ml](https://github.com/lindig/lua-ml/blob/master/example/luaclient.ml) file.

You can either compile it with `ocamlfind ocamlopt -package lua-ml -linkpkg -o luaclient luaclient.ml` or paste its module definitions into the toplevel
and play with it interactively.

You can also find a comprehensive example in the [soupault source code](https://github.com/dmbaturin/soupault/blob/master/src/plugin_api.ml).

## Embedding and projection

To make OCaml values accessible for Lua code, you need to _embed_ them in the Lua-ML's type system. To make them available to OCaml code again,
you need to _project_ them.

Embedding OCaml values into Lua is straightforward and "lossless". Taking them back is tricky and needs some caution.

Lua's native type system is very simplistic. It has numbers, strings, tables, and nil. There's also an "abstract" `userdata` type
used for opaque values from the host program.

The "number" type is internally a float, and there are no real integers. Tables allow keys of any type, there's no dedicated "array" type
that guarantees that indices are numeric and strictly sequential. 

For exchanging data between OCaml and Lua, the [Value](https://github.com/lindig/lua-ml/blob/master/src/luavalue.mli) module provides a bunch of "map" records. Their simplified type looks like this:

```ocaml
type 'a map = {
  is : 'a -> bool;
  embed : 'a -> value
  project : value -> a
}
```

The `embed` function converts an OCaml value to a Lua value. It never fails.

```
# I.Value.int.embed 3  ;;
- : I.value = I.Value.LuaValueBase.Number 3.

# I.Value.string.embed "hello" ;;
- : I.value = I.Value.LuaValueBase.String "hello"
```

The `project` function does the opposite. It follows the same type conversion rules as Lua itself: for example, a number
can always be projected as a string. It may fail with an exception if type conversion fails.

```
# I.Value.int.embed 3 |> I.Value.string.project ;;
- : string = "3"

# I.Value.string.embed "hello" |> I.Value.float.project ;;
Exception: Luavalue.Make(U).Projection (_, "float").

# I.Value.float.embed 3.5 |> I.Value.int.project ;;
Exception: Luavalue.Make(U).Projection (_, "int").
```

Thus, you should always use the `project` function with caution. That's what the third `is` function is for.
It tells you whether using `project` on that value would be safe.

```
# I.Value.int.embed 3 |> I.Value.float.is ;;
- : bool = true

# I.Value.float.embed 3.4 |> I.Value.int.is ;;
- : bool = false
```

Too bad there's no function that would just tell you the type of a value, but with a long chain of conditionals
we can write a function that can project any primitive type from Lua back to OCaml. However, we need to be careful
with Lua's subtyping.

For example, the fact that everything has a "truth value" in Lua means that everything "is" a boolean there,
and `I.Value.bool.is` _never_ returns false. Likewise, integer is a subtype of float from Lua's point of view,
so every int "is" a float. We need to sequence the conditionals to reflect the subtyping order.

```ocaml
let value_of_lua v =
  if V.int.is v then `Float (V.int.project v |> float_of_int)
  (* float is a supertype of int, so int "is" a float, and order of checks is important *)
  else if V.float.is v then `Float (V.float.project v)
  else if V.string.is v then `String (V.string.project v)
  else if V.unit.is v then `Null
  (* Everything in Lua has a truth value, so V.bool.is appears to never fail *)
  else if V.bool.is v then `Bool (V.bool.project v)
  (* Not sure if can happen *)
  else failwith "Unimplemented projection"
```

### Type mapping combinators

For values that can be `nil` in Lua, there's an option combinator.

```
# (I.Value.option I.Value.int).embed (Some 4) |> (I.Value.option I.Value.float).project ;;
- : float option = Some 4.

# I.Value.string.embed "hello" |> (I.Value.option I.Value.string).project ;;
- : string option = Some "hello"
```

Note that it _only_ handles the case when a value is `nil`, and still fails if types cannot be converted:

```
# I.Value.string.embed "hello" |> (I.Value.option I.Value.float).project  ;;
Exception: Luavalue.Make(U).Projection (_, "float").
```

Embedding linked lists is also simple:

```
# (I.Value.list I.Value.int).embed [1;2;3] ;;
- : I.value = I.Value.LuaValueBase.Table <abstr>
```

To wrap it up, let's write a function that embeds a Ezjsonm-compatible polymorphic variant type
into Lua:

```ocaml
let rec embed_anything v =
  match v with
  | `Bool b -> I.Value.bool.embed b
  | `Int i -> I.Value.int.embed i
  | `Float f -> I.Value.float.embed f
  | `String s -> I.Value.string.embed s
  | `A vs -> (List.map embed_anything vs) |> (I.Value.list I.Value.value).embed
  | `O vs ->
    List.map (fun (k, v) -> (k, embed_anything v)) vs |>
    I.Value.Table.of_list |> I.Value.table.embed
  | `Null -> I.Value.unit.embed ()
```

## Conclusion

Well, this isn't exactly as simple as I hoped it would be, but it's definitely possible to pass anything to Lua.

I'm not sure why Lua-ML didn't "just" use polymorphic variants internally. However, before I can even consider reworking Lua-ML
or writing a new embeddable interpreter, I need to really study and understand the prior art.
