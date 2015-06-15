{% set date 2015 June 10 %}

<h1>Object-oriented Peano arithmetics in Python</h1>

<p>I'm not sure what would be the best way to implement Peano arithmetics in Python
   or another language without algebraic data types, but I think I've found the
   <em>worst</em> (and the funniest) way to do it.</p>
<p>If you've never seen it, the idea is to use unary encoding. Instead of just assuming
   something called numbers exists, we postulate that there exists an object O and
   that is a natural number, and a <em>successor</em> function S(n) such that
   for every n, S(n) is a natural number. Then we can think that 0 = O, 1 = S(O),
   2 = S(S(O)) and so on.</p>
<p>Python has introspection and for every class we can find its base class(es).
   It can also create classes on the fly and attach string representation functions to classes.
   So we can assume that some base class is O, its subclass (any of them) is S(O),
   subclass of its subclass is S(S(O)) and so on.</p>
<p><strong>Note:</strong> this only works in Python2, Python3 made a lot of changes
   to metaclasses and I haven't found a way to add pretty print to classes there yet.</p>

<pre>
#!/usr/bin/env python

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
class Zero(object):
    __metaclass__ = Nat

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

if __name__ == '__main__':
    one = Zero.succ()
    two = Zero.succ().succ()
    three = Zero.succ().succ().succ()

    print("{0} + {1} = {2}".format(Zero, Zero, Zero.add(Zero)))
    print("{0} + {1} = {2}".format(Zero, one, Zero.add(one)))
    print("{0} + {1} = {2}".format(two, three, two.add(three)))
    print("{0} + {1} = {2}".format(three, two, three.add(two)))
</pre>

<p>The output demonstrates that zero is indeed the identity element
   and that addition is commutative.</p>
<pre>
$ python peano.py
O + O = O
O + S(O) = S(O)
S(S(O)) + S(S(S(O))) = S(S(S(S(S(O)))))
S(S(S(O))) + S(S(O)) = S(S(S(S(S(O)))))
</pre>
