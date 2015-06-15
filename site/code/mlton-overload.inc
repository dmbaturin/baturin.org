{% set date 2015 June 10 %}

<p>StandardML doesn't allow function overloading, and for a good reason (it makes type inference
   decidable and allows you to never write any type annotations).
   However, for convenience arithmetic functions are magically overloaded and work with both
   integers and floating point numbers.
   Compilers don't expose this functionality to the end user,
   but deep inside they can do it, and I decided to dig into MLton to find out
   what it looks like and how to do it myself.</p>
<p>It turned out that they didn't hide it too deep. Reading the standard library source revealed that there
   is <tt>_overload</tt> keyword that is not allowed unless you build your program with a special compiler
   switch.</p>
<p>For demonstration I wrote a program that uses a <tt>^+</tt> operator that works like <tt>^</tt> (concatenation)
   for string and like <tt>+</tt> (addition) for integers.</p>

<pre>
signature BIZZAREOP_INT = sig
    val ^+ : int * int -&gt; int
end

structure BizzareOpInt : BIZZAREOP_INT = struct
    val ^+ = fn (x, y) =&gt; x + y
end

signature BIZZAREOP_STRING = sig
    val ^+ : string * string -&gt; string
end

structure BizzareOpString : BIZZAREOP_STRING = struct
    val ^+ = fn (x, y) =&gt; x ^ y
end

(* Magic begins here *)

_overload ^+ : ('a * 'a -&gt; 'a) as BizzareOpInt.^+ and BizzareOpString.^+

(* Magic ends here *)

infix 6 ^+

val () = print ("hello " ^+ "world\n")
val () = print ((Int.toString (3 ^+ 4)) ^ "\n")
</pre>

<p>Compiling it with default options fails with a very clear error message:</p>
<pre>
$ mlton ./test.sml 
Error: test.sml 17.1.
  _overload disallowed.
</pre>

<p>In build scripts I've found another bit of magic required to get it to work, a special build option.</p>

<pre>
$ mlton -default-ann 'allowOverload true' ./test.sml

$ ./test 
hello world
7
</pre>

