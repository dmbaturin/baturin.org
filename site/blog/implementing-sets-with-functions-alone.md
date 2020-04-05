# Implementing sets with functions alone

<time id="last-modified">2018-04-12</time>
<tags>programming, ocaml</tags>

<p id="summary">
Implementation of sets using nothing but functions would be one of the first tricks in the
&ldquo;100 Fun Things to Do With Functions and Closures&rdquo; book if that book existed.
It may not be very practical, but it may help people get into the functional mindset.
We'll use <a href="http://ocaml.org">OCaml</a> for demonstration.
</p>

Our implementation will support the following operations:

* Checking if a value belongs to a set
* Inserting new elements into existing sets
* Creating unions and complements of sets

We'll represent a set _S_ as a function _s x_ such that _s x = true_ if _x_ is a member of
_S_ and _false_ otherwise. This way the first goal of our project is already met.

The type of the set functions will be `'a -> bool` (if you are new to ML, the `'a`
here is a type variable that can be replaced by any type, as long as all occurences of `'a`
within an expression are replaced with the same type — this is known as parametric polymorphism).

In this approach, a set and a function that checks if a value belongs to it is the same,
but we can add a little syntactic sugar to make it look as if sets were somehow special:

```ocaml
let is_in s x = s x
```

The empty set can be represented as a function that returns _false_ for any argument,
since by definition it's a set that has no elements, and nothing belongs to it:

```ocaml
let empty x = false
```

How can we represent a non-empty set? Let's start with a set of just one element.
To find out if something belongs to it, we just need to compare it with a fixed value,
and return false if they are not equal:

```ocaml
let set_of_one x =
  if x = 1 then true
  else false
```

Or we can rewrite it using the `empty` function we've defined earlier as the base case for recursion:

```ocaml
let set_of_one x =
  if x = 1 then true
  else empty x
```

Note that we've just essentially created a non-empty set from the empty set. Now we have the template for set functions,
and it's easy to see how to make a set of two elements (say 1 and 2) from `set_of_one`:

```ocaml
let set_of_one_and_two x =
  if x = 2 then true
  else set_of_one x
```

Now we are just one step away from the second goal of creating a function for inserting elements into existing sets.
All we need is a simple higher order function that takes a set _s_ and value _e_ and returns a new function _s' x_ that returns
_true_ is _x = e_, or the value of _s x_ otherwise.

```ocaml
let insert e s =
  fun x -> if x = e then true else s x
```

We can build sets of any finite size from the empty set with it:

```ocaml
let one = insert 1 empty
let two = insert 2 one
let three = insert 3 two

let a = in_set three 2 (* true *)
let b = in_set three 4 (* false *)
```

Now to the union and the complement. You might have noticed that with our `insert` function, we'are essentially
representing finite sets as unions of single element sets, but here we are talking about a user-friendly way to create
a union of two arbitrary sets.

By definition, an element is in the union of sets A and B if it belongs
to at least one of them. We can implement the union function simply by following the definition.

```
let union s s' =
  fun x -> (is_in s x) || (is_in s' x)
```

The definition of the (relative) complement of sets A and B is a set that includes all elements of A that are not in B.

```ocaml
let complement s s' =
  fun x -> (is_in s x) && not (is_in s' x)
```

Let's test them:

```ocaml
let s = insert 3 empty |> insert 2
let s' = insert 1 empty

let u = union s s'
let c = complement u s'

let x = is_in u 3 (* true *)
let y = is_in u 1 (* true *)
let z = is_in u 0 (* false *)

let v = is_in c 1 (* false *)
let w = is_in c 3 (* true *)
```

Now that all goals are met, we can even wrap everything into a module:

```ocaml
module type MYSET = sig
  type 'a set = 'a -> bool
  val empty : 'a set
  val is_in : 'a set -> 'a -> bool
  val insert : 'a -> 'a set -> 'a set

  val union : 'a set -> 'a set -> 'a set
  val complement : 'a set -> 'a set -> 'a set
end

module MySet : MYSET = struct
  type 'a set = 'a -> bool

  let empty x = false

  let is_in s x = s x

  let insert e s =
    fun x -> if x = e then true else s x

  let union s s' =
    fun x -> (is_in s x) || (is_in s' x)

  let complement s s' =
    fun x -> (is_in s x) && not (is_in s' x)
end
```

