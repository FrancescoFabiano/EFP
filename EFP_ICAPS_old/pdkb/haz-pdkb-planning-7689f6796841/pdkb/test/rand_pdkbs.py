

import os, sys, random

from pdkb.kd45 import PDKB, project, kd_closure

from pdkb.rml import Belief, Possible, Literal, neg

from pdkb.test.utils import random_rml, random_pdkb, parse_test_pdkb, test_kripke

from krrt.utils import read_file, write_file, get_file_list


def test0():
    test_files = get_file_list('examples/', match_list=['.pdkb'])
    for f in test_files:
        print "\n Testing file: %s" % f
        parse_test_pdkb(f)
        print "***"
        print "*******************************************" * 2
        print "*******************************************" * 2
        print "***\n"

def test1():
    print "\n----\n"

    l1 = Literal('p')
    l2 = Belief(2, Belief(1, l1))
    l3 = Belief(2, neg(Belief(1, neg(l1))))
    l4 = neg(l3)

    l5 = neg(Belief(1, neg(Belief(2, l1))))
    l6 = neg(Belief(1, neg(Belief(2, Possible(3, Belief(4, l1))))))

    print "KD closure operation:"
    print "%s -> %s" % (str(l2), str(map(str, kd_closure(l2))))
    print "%s -> %s" % (str(l3), str(map(str, kd_closure(l3))))

    print "\n----\n"

    kb = PDKB(2, [1,2], map(Literal, ['p', 'q']))
    kb.add_rml(l2)
    print "Initial KB:"
    print kb
    print "\nClosing..."
    kb.logically_close()
    print kb

    print "\n----\n"

    kb.add_rml(l4)
    print "Initial Closed KB:"
    kb.logically_close()
    print kb
    print "\nConsistent: %s" % str(kb.is_consistent())

    print "\n----\n"

    kb = PDKB(2, [1,2], map(Literal, ['p', 'q']))
    kb.add_rml(Belief(1, Literal('p')))
    kb.add_rml(Belief(2, Literal('q')))
    kb.add_rml(Belief(1, Belief(2, Literal('q'))))
    kb.add_rml(neg(Belief(1, Literal('q'))))
    print "Initial KB:"
    print kb
    print "\n...projecting on agent 1\n"
    print project(kb.rmls, 1)

    print "\n----\n"




def test2():

    kb = PDKB(1, [1], map(Literal, ['p', 'q']))

    l1 = neg(Literal('p'))
    l2 = Belief(1, Literal('p'))
    l3 = Belief(1, neg(Literal('q')))

    kb.add_rml(l1)
    kb.add_rml(l2)
    kb.add_rml(l3)

    test_kripke(kb, [l1, l2, l3])

def test3():
    kb = PDKB(1, [1], map(Literal, ['p', 'q']))

    l1 = Literal('p')
    l2 = Belief(1, Literal('q'))

    kb.add_rml(l1)
    kb.add_rml(l2)

    test_kripke(kb, [l1, l2])

def test4():
    kb = PDKB(1, [1,2], map(Literal, ['p', 'q', 'r']))

    rmls = [
        Literal('p'),
        Belief(1, Literal('q')),
        Belief(2, neg(Literal('q')))
    ]

    for rml in rmls:
        kb.add_rml(rml)

    test_kripke(kb, rmls)

def test5():
    kb = PDKB(2, [1,2], map(Literal, ['p', 'q']))

    rmls = [
        Literal('p'),
        Belief(1, Literal('q')),
        Belief(1, Belief(2, neg(Literal('q'))))
    ]

    for rml in rmls:
        kb.add_rml(rml)

    test_kripke(kb, rmls)




