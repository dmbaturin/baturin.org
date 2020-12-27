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

