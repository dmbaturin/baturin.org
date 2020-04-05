# Duck typing

<time id="last-modified">2018-02-21</time>
<tags>programming, python, ocaml</tags>

<p id="summary">
So called &ldquo;duck typing&rdquo; is often poorly explained and thus often misunderstood.
Its name and the adage associated with it (&ldquo;if it walks like a duck and quacks like a duck,
then it is a duck&rdquo;) don't do it any favors either. 
</p>

I've always found that analogy strange and misleading because values in programs do not quack or walk
or do anything else unprovoked, it's programmers who do something with them. The key idea can be explained
without any avian analogies: objects that provide the same interface can be viewed as interchangeable.

I suppose one of the reasons for controversy over what duck typing is and whether it's merely a synonym for dynamic binding
or anything else is that there are really two levels of duck typing that programming systems (that is, languages and their standard libraries) can implement.
They often come together, but the first level can come without the second.
Since duck typing is typically associated with object oriented programming, I will use object terminology
throughout, though it should be possible to generalize it to non-object systems.

The first level allows using objects interchangeably if they provide the same method. This expectation is never
true in type systems that are static and nominal where objects are only interchangeable if they are instances
of the same class. This level can be achieved by dynamic binding or a special
variant of structural typing, and it's the only part of duck typing that has anything to do with language design _per se_.

The second level fulfills the expectation that, for example, any value that is supposed to have a human-readable
representation can be used with a function that prints values on screen, or any iterable collection can be used
with a function that searches for values in collections. Implementation of this level is solely a library design
decision and is achieved by double dispatch conventions.

## Level 1: dynamic binding

One way to enable calling methods of objects irrespective of their type/class is to not check it at all.
This is what Python or Ruby are doing: they will blindly try to call a method and raise a runtime error
if that method doesn't exist.

The main downside of this approach is that you never know if it's safe to use a value or not, so you
have to add runtime checks.

Any language that can look up object methods by name at runtime automatically implements it.

```python
>>> class Foo(object):
...   def work_miracles(self):
...     print("Working miracles, please wait...")
... 
>>> class Bar(object):
...   pass
... 
>>> x = Foo()
>>> y = Bar()
>>> x.work_miracles()
Working miracles, please wait...
>>> y.work_miracles()
Traceback (most recent call last):
  File "<stdin>", line 1, in <module>
AttributeError: 'Bar' object has no attribute 'work_miracles'
```

## Level 1: structural typing

There aren't many statically typed languages that allow level 1 duck typing. The necessary condition is support for
structural typing, but it's not sufficient.

The only language with a structural object system I have experience with is OCaml. In fact it has type inference for
objects that allows type safe level 1 duck typing.

```ocaml
(* The # operator means attribute access *)
let foo x y = x#work_miracles (y + 1) |> print_endline

```

The inferred type of this function will be
```ocaml
val foo : < work_miracles : int -> string; .. > -> int -> unit
```
whch means _x_ can be any object that has method _work_miracles_ that takes an integer and returns a string,
no matter what other attributes it may have. Trying to use it with an object that doesn't have such a method
will cause a compile time error.

## Level 2 duck typing

To make most of duck typing, the standard library should use double dispatch and follow a set of conventions
for method names consistently. The idea of double dispatch, if you are not familiar with it, is to make
functions call a specific method of their argument value instead of inspecting that value and trying to
determine what to do with it.

Python standard library a large set of &ldquo;magic methods&rdquo; and third party library maintainers usually
provide them in their classes as well.

For example, the built in `print(x)` function doesn't know how to convert anything to a string for printing,
instead it calls `x.__repr__()`. It is easy to make any class print-friendly by defining that method in it.

```python
class Polar(object):
  def __init__(self, r, phi):
    self._r = r
    self._phi = phi
  
  def __repr__(self):
    return "r={0} phi={1}".format(self._r, self._phi)

>>> print(Polar(3, 2))
r=3 phi=2

```

We could easily implement a similar set of conventions on top of OCaml object system if we really wanted to.
Let's make a pythonesque print function:
```ocaml
let print obj =	print_endline (obj#repr	())
(* val print : < repr : unit -> string; .. > -> unit *)

class cartesian	x y =
  object
    method repr	() = Printf.sprintf "(x=%f y=%f)" x y
  end

class polar r phi =
  object (self)
    method repr	() = Printf.sprintf "(r=%f phi=%f)" r phi
  end

let a =	new cartesian 1.0 1.0
let b =	new polar 1.0 45.0

let () = print a
let () = print b

(* Output:

(x=1.000000 y=1.000000)
(r=1.000000 phi=45.000000)

*)

```
