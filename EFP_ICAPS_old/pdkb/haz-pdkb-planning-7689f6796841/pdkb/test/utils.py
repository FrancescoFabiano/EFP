
import random, os

from krrt.utils import read_file, write_file, get_file_list

from pdkb.kd45 import PDKB, kd_closure
from pdkb.rml import neg, Belief, Literal

def random_rml(depth, agents, fluents):
    b = random.choice(fluents)
    d = random.randint(0, depth)

    def coin():
        return random.choice([True, False])

    if coin():
        b = neg(b)

    for i in range(d):
        if b.is_lit():
            options = agents
        else:
            options = list(set(agents) - set([b.agent]))
        b = Belief(random.choice(options), b)
        if coin():
            b = neg(b)

    return b

def random_pdkb(depth, numAg, fluents, numRMLs, output=True):

    agents = list(range(1, numAg+1))
    kb = PDKB(depth, agents, fluents)
    count = 0
    new_rmls = set()

    while count < numRMLs:
        rml = random_rml(depth, agents, fluents)
        if rml not in kb.rmls and neg(rml) not in kb.rmls:
            # Take advantage of the fact that we know the closure
            #  is just the union of the closure of each RML
            test_rmls = list(kd_closure(rml)) + [rml]
            cons = True
            for trml in test_rmls:
                if neg(trml) in kb.rmls:
                    cons = False

            if cons:
                if output:
                    print "Added %s" % rml
                for trml in test_rmls:
                    kb.add_rml(trml)
                new_rmls.add(rml)
                count += 1

    kb.rmls = new_rmls
    return kb

def test_random_pdkb(depth, numAg, fluents, numRMLs, output=True):

    if output:
        print "\nGenerating random PDKB...\n"

    fluents = [Literal(f) for f in fluents]
    kb = random_pdkb(depth, numAg, fluents, numRMLs, output)
    M = kb.generate_kripke(False)

    msg = ''
    if not M.is_kd45():
        msg = "Error: PDKB is not KD45"
    elif not all([M.assess_rml(rml) for rml in kb.perspectival_view()]):
        msg = "Error: Not all rmls are satisfied"
    else:
        msg = "All is A-Ok"

    if output:
        print
        print msg
        print
    return ('Error' in msg, msg)

def parse_test_pdkb(fname):
    lines = read_file(fname)
    (depth, num_ag, props) = lines.pop(0).split(' ')

    depth = int(depth)
    num_ag = int(num_ag)
    props = props.split(',')

    assert 'B' not in props, "Error: Can't use 'B' as a proposition."
    assert num_ag < 10, "Error: Can't use more than 10 agents."

    def parse_rml(s):
        if '!' == s[0]:
            return neg(parse_rml(s[1:]))
        elif 'B' == s[0]:
            return Belief(int(s[1]), parse_rml(s[2:]))
        else:
            return Literal(s)

    rmls = [parse_rml(l.replace(' ', '')) for l in lines[:lines.index('END')]]

    desc = "\n".join(lines[lines.index('END')+1:])
    if desc:
        print "Running test with the following description:"
        print "\n--------\n"
        print desc
        print "\n-----\n"

    kb = PDKB(depth, list(range(1, num_ag+1)), map(Literal, props))

    for rml in rmls:
        kb.add_rml(rml)

    test_kripke(kb, rmls)

def test_kripke(kb, rmls):
    print
    print kb

    print "\nClosing...\n"
    kb.logically_close()
    print kb

    print "Consistent: %s" % str(kb.is_consistent())

    print "\nGenerating compressed kripke..."
    M = kb.generate_kripke(True)

    print
    print "Generating dot export..."
    M.generate_dot("graph.dot", True)
    lines = read_file("graph.dot")
    lines = [lines[0], "        rankdir=LR;"] + lines[1:]
    write_file("graph.dot", lines)
    os.system("dot -Tpng graph.dot > graph.png")

    print "\nGenerating full kripke..."
    M = kb.generate_kripke(False)

    print
    print M.get_stats()

    print
    for rml in rmls:
        print "Assessing %s: %s" % (str(rml), M.assess_rml(Belief(0, rml)))

    print
    all_match = all([M.assess_rml(rml) for rml in kb.perspectival_view()])
    print "All rmls match: %s" % str(all_match)
    if not all_match:
        print "Non-matching:"
        for rml in kb.construct_closed_world():
            if not M.assess_rml(rml):
                print "  %s" % str(rml)

    print
