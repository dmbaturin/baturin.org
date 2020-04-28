# Introduction to OCaml, part 2
<time id="last-modified">2018-08-07</time>
<tags>programming, ocaml</tags>

*Note:*	new chapters are not posted in the blog anymore! Improved and expanded versions of these posts
and new	chapters can be	found on the [OCaml Book](https://ocaml-book.baturin.org/) project website.

<p id="summary">
In the previous chapter we've learnt how to use variables and arithmetic functions.
Now it's time to learn how to make our own functions.
</p>

## Anonymous functions

We will start with anonymous functions. The reason is that in OCaml, names functions are simply
anonymous functions bound to name, and the special syntax for creating named functions is merely
syntactic sugar.

The syntax of the anonymous functions is the following: `fun <formal parameter> -> <expression>`.

Let's write a simple program using an anonymous function:

```ocaml
let a = (fun x -> x * x) 3

let () = print_int a
```

The `fun x -> x * x` function takes a single argument and squares it. Since we already know that
the `*` operator works with integer, we can infer that this function has type `int -> int`.
In the program it is applied to 3 and the result is bound to `a`, which is later printed.

## Named functions

Now, before we learn of syntactic sugar for named functions, let's create a named function
the hard way:

```ocaml
let square = fun x -> x * x

let a = square 3 (* 9 *)
```

As you can see, the syntax is exactly the same for variable and function bindings.
In OCaml, they are both expressions. Like constants, functions (if not applied to
any arguments), evaluate to themselves. 

## Bindings, scopes, and closures

Now let's examine how functions interacts with other bindings. Unlike other expressions,
functions contain _free variables_. A free variable is a variable not bound to any value.
In our `square` example, `x` is a free variable, and the expression in the right hand side
contains no other variables. When that function is applied to an argument, `x` will be substituted
with that argument in the `x * x` expression and that expression will be evaluated.
We can visualize that process like this:

```
square 3 =
(fun x -> x * x) 3 =
(fun 3 -> 3 * 3) =
3 * 3 =
9
```

Of course, if we have previously created any bindings, we can refer to them in our functions.

```ocaml
let a = 2

let plus_two = fun x -> x + a

let b = plus_two 3 (* b = 5 *)
```

However, what happens is the name `a` is shadowed by another binding? Let's try and see what happens:

```ocaml
let a = 2

let plus_two = fun x -> x + a

let a = 3

let () = print_int (plus_two a)
```

This program will print 5 (i.e. 2 + 3, not 3 + 3). The reason is that functions capture bound variables from the scope
where they were created &mdash; forever. This is called _lexical scoping_.
The alternative approach would be _dynamic scoping_ where the value of `a` is be determined
at the time when the `plus_two` function is applied. But in OCaml, values of bound variables
are determined when functions are created, so we don't need to worry about it.

We can rewrite the `plus_two` function using a `let ... in` binding instead:

```ocaml
let plus_two =
  let a = 2 in
  fun x -> x + a

let b = plus_two 3
```

Here, the variable `a` doesn't even exist for the rest of the program because it was a local
binding only visible to the `fun x -> x + a` expression, but it continues to exist for the
`plus_two` function.

A function stored together with its environment is called a _closure_. Since you cannot prevent
functions from capturing their environment in OCaml, there is no distinction between functions
and closures, and for simplicity they are all called just functions. You should always
remember about the effect though, and it has important practical uses.

For example, it can be used to create functions of multiple arguments using only functions
of one argument. In fact, this is how functions of multiple arguments work in OCaml.
Any function &ldquo;of two arguments&rdquo; really takes one argument and produces another
function of one argument.

## Functions of multiple arguments and currying

Let's write a function for calculating the average of two values.

```ocaml
let average = fun x -> (fun y -> (x +. y) /. 2.0)
  
```

As we can see, when applied to one argument, this function creates another function,
where `x` is bound to the argument &mdash; a closure.
So the expression `let f = average 3.0` will be equivalent to `let f = fun y -> (3.0 +. y) /. 2.0)`,
because functions capture the bound variables from the scope where they are created, as we discussed already.
The function `f` is thus a function that calculates
the average of 3.0 and another arbitrary number, and can be applied to another argument.
We could also avoid the intermediate binding and write it as `(average 3.0) 4.0` is we wanted
the average of 3.0 and 4.0. The type of the `average` function is therefore `float -> (float -> float)`.

That's a lot of parentheses as you can see. To simplify it, OCaml and most other functional languages
use a convention where arrows associate to the right. Therefore the type of the `average` function
can be written as `float -> float -> float`, and it is assumed to mean `float -> (float -> float)`.
Likewise, you can apply it without any parentheses too: `let a = average 3.0 4.0`.

The process of creating a function &ldquo;of multiple arguments&rdquo; from functions of one argument
is called _currying_, after Haskell Curry who was already mentioned in the history section,
even though he wasn't the first to invent it, as it often happens with named laws.

## Partial application

A big advantage of curried functions is that they make partial application especially easy.
In many languages, partial application either needs a special syntax or isn't possible at all.
In languages that use closures and currying, it is very easy: just use only the first argument(s)
and omit the rest, and you get a function with first argument(s) fixed that can be applied to different
remaining arguments as needed.

To see some real examples, let's introduce the OCaml's `Printf.printf` function that is used for
formatted output. The reason we used `print_int`, `print_string` and similar in the first chapter
is that we pretended that we know nothing about functions with more than one argument until we
had a chance to learn about them systematically. In practice, people almost always use `Printf.printf`
instead because it's much more powerful and convenient. Its name means that it belongs to the `Printf`
module that comes with the standard library, but we will discuss modules later, for now let's consider
it just an unusual name.

The type of that function is rather complicated and we will not discuss it right now. Let's just say that
its first argument is a format string. Format string syntax is very similar to that of C and all languages
inspired by its `printf`. The Hello World program could be written:

```ocaml
let () = Printf.printf "%s\n" "hello world"

```

When applied to a format string, that function will produce functions or one or more arguments depending
on the format sting. For example, in `let f = Printf.printf "%s %d"`, `f` will be a function of type `string -> int -> unit`.

Now let's write a simple program using `Printf.printf` and partial application of it:

```ocaml
let greet = Printf.printf "Hello %s!\n"

let () = greet "world"
```

The `"Hello %s!\n"` is stored in a closure together with the function that `Printf.printf` produced from it.
A similar idiom in an object oriented language might have been `greeter = new Formatter("Hello %s!\n"); greeter.format("world")`.
As Peter Norvig put it in <a href="http://norvig.com/design-patterns/design-patterns.pdf">one talk</a>, objects
are data with attached behaviour, while closures are behaviours with attached state data.

A danger of curried functions, however, is that failing to supply enough arguments is not a syntax error,
but a valid expression, just not of the type that might be expected. Luckily, since OCaml is statically typed,
this kind of errors rarely goes unnoticed and programs fail to compile.

Consider this program:

```ocaml
let add = fun x -> (fun y -> x + y)

let x = add 3 (* forgot second argument *)

let () = Printf.printf "%d\n" x
```

It will fail to compile because `add 3` expression has type `int -> int`, while `Printf.printf "%d\n"` is `int -> unit`.

## The syntactic sugar for function definitions

Of course, creating functions by binding anonymous functions to names can quickly get cumbersome, especially when
multiple arguments are involved, so OCaml provides syntactic sugar for it.

Let's rewrite the functions we've already written in a simpler way:

```ocaml
let plus_two x = x + 2

let average x y = (x +. y) /. 2.0
```

OCaml also supports multiple arguments to the `fun` keyword too, so anonymous functions of multiple arguments can be
easily created to: `let f = fun x y -> x + y`.

## Formal parameter is a pattern

We have already seen that the left hand side of a `let`-expression can be not only a name, but any valid _pattern_,
including the wildcard or any valid constant. This also applied to the left hand side of function definitions.

For example, we can create a function that ignores its argument using the wildcard pattern:

```ocaml
let always_zero _ = 0

let always_one = fun _ -> 1
```

We can also use `()`, which is a constant of type `unit`, as a function argument. Since functions with
no arguments cannot exist in OCaml, this is often done for functions used solely for their side effects:

```ocaml
let print_hello () = print_endline "hello world"

let () = print_hello ()
```

The `print_hello` function in this example has type `unit -> unit`.

## Operators are functions

It is actually not true that we haven't seen functions of multiple arguments in the first chapter,
we just pretended that operators are special. While in many languages they are indeed special,
in most functional languages operators are just functions that can be used in infix form.

In OCaml, every infix operator can also be used in prefix form if enclosed in parentheses:

```ocaml
let a = (+) 2 3
let b = (/.) 5 2
let c = (^) "hello " "world"
```

You can also define your own operators like any other functions using the same parentheses syntax:

```ocaml
let (^^) x y = x ^ x ^ y ^ y

let s = "foo" ^^ "bar" (* foofoobarbar *)

```

You can also define a prefix operator if you start it with the tilde:

```ocaml
let (~*) x = x * x

let a = ~* 2 (* 4 *)
```

The first character of the operator defines its associativity and precedence.

## Named and optional arguments

OCaml supports named and optional arguments. Named arguments are preceded with the tilde symbol:

```ocaml
let greet ~greeting ~name = Printf.printf "%s %s!\n" greeting name

let () = greet ~name:"world" ~greeting:"hello"
```

If you look at the inferred type of the `greet` function, you will see that argument labels
are embedded in its type: `greeting:string -> name:string -> unit`.
Named arguments can be used in any order as long as you specify the labels, but if argument come
in the same order as they are defined in the function, you can omit the labels:

```ocaml
let greet ~greeting ~name = Printf.printf "%s %s!\n" greeting name

let () = greet "hello" "world"
```

The syntax of optional argument is a bit more complicated. Suppose we want to write a function
that takes a string and prints `hello <string>` by default, but allows us to use a different greeting,
for example, &ldquo;hi&rdquo;
This is how we can do it:


```ocaml
let greet ?(greeting="hello") name = Printf.printf "%s %s!\n" greeting name

let () = greet "world" ~greeting:"hi"
```

The inferred type of this `greet` function will look like this: `?greeting:string -> string -> unit`.
It is recommended to put optional arguments first, because otherwise you will not be able to use
your function with optional arguments omitted.

## Exercises

Using the `Printf.printf` function, make a `join_strings` function that takes two strings and an optional
separator argument that defaults to space and joins them. What is the type of that function?

Write a function that has type `int -> float -> string` using any functions we already encountered.


Continue to the [part 3](/blog/introduction-to-ocaml-part-3).
