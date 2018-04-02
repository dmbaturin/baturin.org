{% set date 2018 April 02 %}

<h1>Object-oriented Peano arithmetic in Python</h1>

<p>I'm not sure what would be the best way to implement Peano arithmetics in Python
   or another language without algebraic data types, but I think I've found the
   <em>worst</em> (and the funniest) way to do it.</p>
<p>Python has introspection and for every class we can find its base class(es).
   It can also create classes on the fly and attach string representation functions to classes.
   So we can assume that some base class is zero, its subclasses are S(O),
   subclasses of its subclasses is S(S(O)) and so on.</p>

<p>Download the file: <a href="/code/files/oo-peano.py">oo-peano.py</a> </p>
<p>
The code on this page and in that file only works with Python3. There are
differences in metaclass implementation in 2 and 3, for a legacy version that works with
Python 2.7 see <a href="/code/files/oo-peano.py">oo-peano2.py</a>
</p>

<pre>
#!/usr/bin/env python3

import uuid

# Axiom 0: every language feature can
# and will be misused.

class Nat(type):
    def __repr__(self):
        if self == Zero:
            return "O"
        else:
            return "S({0})".format(repr(self.__bases__[0]))

# Axiom 1: Zero is a natural number
class Zero(object, metaclass=Nat):
    # Axiom 2: for every natural number N,
    # succ(N) is a natural number
    @classmethod
    def succ(self):
        # Class names don't matter but must be unique
        name = str(uuid.uuid4())

        # By successor Peano meant a subclass, right?
        t = type(name, (self,), {"__metaclass__": Nat})
        return t

    @classmethod
    def add(self, x):
        if self == Zero:
            return x
        else:
            return self.__bases__[0].add(x.succ())

    @classmethod
    def mul(self, x):
        if self == Zero:
            return self
        else:
            return x.add(self.__bases__[0].mul(x))


if __name__ == '__main__':
    one = Zero.succ()
    two = Zero.succ().succ()
    three = Zero.succ().succ().succ()

    print("{0} + {1} = {2}".format(Zero, Zero, Zero.add(Zero)))
    print("{0} + {1} = {2}".format(Zero, one, Zero.add(one)))
    print("{0} + {1} = {2}".format(two, three, two.add(three)))
    print("{0} + {1} = {2}".format(three, two, three.add(two)))
    print()
    print("{0} * {1} = {2}".format(Zero, Zero, Zero.mul(Zero)))
    print("{0} * {1} = {2}".format(Zero, one, Zero.mul(one)))
    print("{0} * {1} = {2}".format(two, one, two.mul(one)))
    print("{0} * {1} = {2}".format(two, three, two.mul(three)))
    print("{0} * {1} = {2}".format(three, two, three.mul(two)))
</pre>

<p>The output demonstrates that zero and one are indeed the additive and multiplicative identity elements
   respectively, and that addition and multiplication as we defined them are commutative.</p>
<pre>
$ python3 peano.py
O + O = O
O + S(O) = S(O)
S(S(O)) + S(S(S(O))) = S(S(S(S(S(O)))))
S(S(S(O))) + S(S(O)) = S(S(S(S(S(O)))))

O * O = O
O * S(O) = O
S(S(O)) * S(O) = S(S(O))
S(S(O)) * S(S(S(O))) = S(S(S(S(S(S(O))))))
S(S(S(O))) * S(S(O)) = S(S(S(S(S(S(O))))))
</pre>

<hr />

<p>If you've never heard of Peano arithmetic before, here's an informal introduction to what's going on.
   To define natural numbers axiomatically, we postulate that there's
   a thing called Zero (we'll render it as <em>O</em>) and it's a natural number.
   Then we postulate that there's a <em>successor</em>
   function <em>S(n)</em> (also called <em>succ(n)</em>) such that for any natural number <em>n</em>, <em>S(n)</em>
   is also a natural number, and if two natural numbers are equal, then their
   successors are also equal.</p>
<p>Thus we can construct any number from Zero by using the <em>S</em> function.
   0 = O, 1 = S(O), 2 = S(S(O)) and so on.</p>
<p>We also assume that there is	a way to extract the <em>n</em>	from <em>S(n)</em>,
   that	is, that there is a <em>predecessor</em> function <em>P</em> such that <em>P(S(n)) = n</em>.
   We also assume that the predecessor of Zero is Zero,	although we don't really need this fact in the program above.</p>
<p>Now we can define addition and multiplication. We'll use inductive definitions with Zero as the base case:</p>
<pre>
N + O = N            (1)
N + S(M) = S(N) + M  (2)

N * O =	O            (1')
N * S(M) = N + N*M   (2')
</pre>
<p>Since every natural number if either Zero or can be represented as <em>S...(S(O))</em>,
   we know that	for every number <em>M</em> this process will eventually hit the base case and
   terminate.</p>

