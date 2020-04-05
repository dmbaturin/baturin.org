# Introduction to OCaml, part 3
<time id="last-modified">2018-08-08</time>
<tags>programming, ocaml</tags>

*Note:*	new chapters are not posted in the blog anymore! Improved and expanded versions of these posts
and new	chapters can be	found on the [OCaml Book](https://ocaml-book.baturin.org/) project website.


## Boolean values and conditional expressions

<p id="summary">
This should have been covered earlier, but better late than even later.
In this chapter we'll learn about booleans and conditional expressions.
</p>

We already know that `true` and `false` are constants of the type `bool`.
Let's learn how to use them.

### Equality and comparison operators

OCaml provides the following equality and comparison operators: `=` (equal), `<>` (not equal),
and the obvious `<`, `>`, `<=`, `>=`.

Unlike arithmetic operators, they do work with values of any type, but the values on the left and
right hand side of the operator must be same type. Trying to compare values of different types
will cause a compilation error. The type of those functions is an interesting question, for now we will only say that all equality and
comparison expressions evaluate to `bool`. 

```ocaml
2 = 2 (* good *)
4 <> 3 (* good *)

4 > 2 (* good *)
4 <= 5.0 (* bad, comparing int with float *)
(float 4) <= 5.0 (* good, int converted to float *)

(int_of_float 5.5) = 5 (* good *)
(ceil 5.7) = 6 (* good *)
```

### Conditional expressions

The syntax of the conditional expression is `if <cond> then <expr> else <expr>`.
They are exactly expressions rather than statements, similar to the `<cond> ? <expr> : <expr>`
construct in the C family of languages.

However, since evaluation of expressions may produce side effects, nothing prevents us from using
it in both roles, where other languages may require conditional statements and condition expressions
for different situations.

We can use it inside a `let`-binding to save evaluation result in a variable:

```ocaml
let a = if (2 = 2) then 0 else 1
```

Or we can use a `let`-expression with a wildcard (`_`) or unit pattern to ignore its result where another language
would use a conditional statement:

```ocaml
let n = read_int ()

let () =
  if (n > 100) then print_endline "This is a large number"
  else print_endline "This is a small number"
```

Note that parentheses around the condition are optional and were added for readability.

## Recursive functions

A recursive definition is a definition that refers to itself. Recursive functions are
very widely used in functional languages, not only for processing data that is inherently
nested, such as filesystem directories, but also as control structures in the same role
where imperative languages use iteration.

Let's write a function that calculates a factorial of an integer number following
this definition: 0! is 1, and factorial of a number _n_ greater than zero
is _n * (n-1)!_.

```ocaml
let rec factorial n =
  if n = 0 then 1
  else n * (factorial (n-1))
```

Why the `rec` keyword is required? The compiler has its own reasons to have
recursive functions explicitly marked as such, but for programmers it's not just
a syntactic noise either, since it allows them to choose if they want to create
a new recursive binding or shadow (redefine) an existing name.

Remember that in OCaml, functions are values,
and function bindings are not different from value bindings. Let's see what would
happen if `rec` was the default. Consider this program:

```ocaml
let x = 1

let x = x + 10
```

The intent of the second expression is to redefine previously defined variable `x`
with a new value. However, if `rec` was the default and the compiler naively assumed
that if the name of the binding appears in its body then the binding is supposed to be
recursive, the name `x` in the right hand side
of that expression would be treated not as a reference to the `x` from the outer scope,
but a reference to `x` from the left hand side, thus making that expression meaningless and ill-typed.

The `rec` keyword allows you to control this behaviour. The issue is even more apparent
if you want to redefine a function. Suppose you want to redefine the `print_string`
function to behave like `print_endline`.

```ocaml
let print_string s = print_string s; print_newline ()
```

If `rec` was the default, the effect would be even worse. Unlike our previous example,
that code would type check, but instead of intended behaviour, it would be calling our
newly defined `print_string` endlessly, which is absolutely not the intended behaviour.

You can verify it by adding the `rec` keyword and pasting that line into the interactive
interpreter.

```
# let rec print_string s = print_string s; print_newline ();;
val print_string : 'a -> unit = <fun>

# print_string "foo";;
Stack overflow during evaluation (looping recursion?).

```

However, since `rec` is not the default, our first version of the redefined `print_string`
works as expected. If you write a function that is intended to be recursive but forget the
`rec` keyword, the compiler will complain about unbound name, since by default it requires
that all names must be already defined in the scope before they can be referred to.

### Mutually recursive functions

Sometimes you will want your function definitions to be _mutually recursive_, that is, refer
to one another. Real life use cases for it often arise in data parsing and formatting, as well
as many other fields. For example, in JSON, objects (dictionaries) may contain array and
vice versa, so if you are writing a JSON formatter, your functions for formatting objects
and arrays will need to refer to each other.

The problem with it in OCaml and many other statically typed languages is that all names
must be defined in advance. Some languages use forward declarations to let
you get around the issue. OCaml uses the `and` keyword, so mutually recursive definitions
have the following form: `let rec <name1> = <expr> and <name2> = <expr>`.

Let's demonstrate it using a popular contrived example:

```ocaml
let rec even x =
  if x = 0 then true
  else odd (x - 1)
and odd x =
  if x = 0 then false
  else even (x - 1)
```

### Recursion as a control structure and tail call optimization

In imperative programming languages, recursion is often avoided unless absolutely
necessary because of its performance and memory consumption impact. It is not an inherent
problem of recursion as such, but rather a limitation of programming language implementations.

In functional languages, including OCaml, those performance issues can be avoided and the compiler
will translate recursive functions to loops with constant memory consumptions, if you follow
certain guidelines.

Before we learn the guidelines, let's examine the root cause of memory consumption issues in recursive functions.
Let's repeat our original factorial definition:

```ocaml
let rec factorial n =
  if n = 0 then 1
  else n * (factorial (n-1))
```

Since the `n * (factorial (n-1))` expression refers to `(factorial (n-1))`, it cannot be evaluated until
the result of executing `(factorial (n-1))` is known, and `factorial 3` will produce four nested function calls
in the executable code. With large arguments it can cause considerable memory consumption, and eventually cause
a stack overflow. 

Now consider this program:

```ocaml
let rec loop () = print_endline "I'm a recursive function"; loop ()

let _ = loop ()
```

If you compile and run it or paste it into the REPL, you will notice that it keeps
printing `I'm a recursive function` forever without ever running into stack overflow.
This is the usual way to write an endless loop in OCaml.

How it is possible? If you look at the `loop` function body, you can see that it doesn't
use the result of `loop ()` in any way. This means that it can be evaluated correctly
without knowing what `loop ()` evaluated to when it was executed previous time.
The OCaml compiler knows that, and produced executable code where `loop ()` is translated
to an unconditional jump rather than a function call. 

But what if you do need the result of previous function calls? You can introduce an auxilliary
function argument (often called _accumulator_) and pass the result of previous computations
in it. This is often called _passing state around_ and together with function composition,
it's a very common functional programming technique.

The key is to rewrite the expression in the function body in such a way that 

```ocaml
let rec factorial acc n =
  if n = 0 then acc
  else factorial (acc * n) (n - 1)

let () = Printf.printf "%d\n" (factorial 1 5)
```

The additional argument `acc` is multiplied by `n` every time,
and the function always knows all the data it needs to calculate
the factorial of `n - 1`, and OCaml also knows that it needs no state data from
previous function calls. The call to `factorial` in the function body is said to be
in the _tail position_.

This implementation is much less convenient to use than the original though, and worse,
the user needs to know the correct initial value of `acc` to use it successfully.
For this reason tail recursive functions are usually implemented as nested functions to give
them convenient interface and hide the added complexity:

```ocaml
let factorial n =
  let rec aux acc n =
    if n = 0 then acc
    else aux (acc * n) (n - 1)
  in aux 1 n
```

Assuming _f_ is a recursive function, while _g_, _h_, _i_, and _j_ are some other functions,
you can use these three forms as blueprints for your tail-recursive functions:

```ocaml
let rec f acc x = f (g acc x) (h x)

let rec f x =
  if (g x) then f (h acc x) (j x)
  else f (i acc x) (j x)

let rec f x =
  let y = g x in
  f y
```

Or, if we put it another way, if you want your function _f_ to be rail recursive, never use `(f x)`
as an argument to any other function in the body of _f_.

If you are using Merlin with your editor, it will tell you if an expression is in tail position or not.
If you want to find out the hard way if compiler recognized a function as tail recursive, you can run `ocamlc -annot myfile.ml`
and look for `call ( tail )` at the required positions. This is how Merlin does it, although it uses a binary
rather than text annotation format for that.

### Practical limits of naively recursive functions

While the existence of the limit of functions that are not tail recursive is an undeniable fact,
in practice it's important to consider not only its existence, but also its size.

Experimentally tested stack depth limit, checked on an x86-64 and ARMv6 GNU/Linux machines appears to be around 260 000
for bytecode and around 520 000 for native executables. This is enough to safely use recusrive functions even for very
large datastructures, and seriously reduce the benefits of aggressive optimizing naively recursive functions by
rewriting them in tail recursive style unless they are intended to run forever or knowingly receive very large input.

It is important to learn how to use tail recusrion, but it's also important to know when you can get away with a simpler
naively recursive definition.

## Exercises

Write a function that checks if given integer number is prime (i.e. has no divisors other than 1 and itself).

Write a function that calculates the greatest common divisor of two integer numbers using the Euclides algorithm:
gcd n 0 = n, gcd n m = gcd m, (n mod m). Do it in both naive and tail recursive style.

Rewrite this function for multiplying non-zero numbers to be tail recursive:

```ocaml
let rec mul n m =
  if m = 1 then n
  else n + (mul n (m-1))
```

Verify that it is indeed tail recursive by using a value of _m_ greater than the call stack depth, e.g. `mul 2 600000`.

Write a program using two functions that print "I'm a recursive function" and "I'm also a recursive function" respectively
so that these two lines are printed in an infinite loop.

Continue to [part 4](/blog/introduction-to-ocaml-part-4-higher-order-functions-parametric-polymorphism-and-algebraic-data-types).
