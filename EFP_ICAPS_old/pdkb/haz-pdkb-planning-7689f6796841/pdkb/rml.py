
class RML(object):
    def __init__(self):
        # Make sure all of the right methods are there
        assert 'is_lit' in dir(self)

        # Set the initial attributes required
        self.hash_val = False

    def __hash__(self):
        if not self.hash_val:
            self.make_hash()
        return self.hash_val

    def __eq__(self, other):
        return hash(self) == hash(other)

    def __repr__(self):
        return str(self)

    def is_ak(self, akprops):
        return self.is_lit() and (self.proposition.replace('!','') in akprops)

    def make_hash(self):
        self.hash_val = hash(str(self))


class BeliefModality(RML):
    def __init__(self, agent, rml):
        RML.__init__(self)
        self.agent = agent
        self.rml = rml
        self.reset()

    def reset(self):
        self.hash_val = False
        self._prop = False
        self._seq = False

    def is_lit(self):
        return False

    @property
    def proposition(self):
        if not self._prop:
            self._prop = self.rml.proposition
        return self._prop

    @property
    def agent_sequence(self):
        if not self._seq:
            self._seq = str(self.agent) + self.rml.agent_sequence
        return self._seq

    def merge_modalities(self):

        self.rml = self.rml.merge_modalities()
        self.reset()

        if self.rml.agent == self.agent:
            return self.rml
        else:
            return self

    def get_depth(self):
        return 1 + self.rml.get_depth()


class Belief(BeliefModality):

    def negate(self):
        return Possible(self.agent, self.rml.negate())

    def pddl(self):
        return "B%s_%s" % (str(self.agent), self.rml.pddl())

    def entails_rml(self, other):
        return self.rml.entails_rml(other.rml)

    def __str__(self):
        return "B%s %s" % (str(self.agent), str(self.rml))


class Possible(BeliefModality):

    def negate(self):
        return Belief(self.agent, self.rml.negate())

    def pddl(self):
        return "P%s_%s" % (str(self.agent), self.rml.pddl())

    def entails_rml(self, other):
        if isinstance(other, Belief):
            return False
        else:
            return self.rml.entails_rml(other.rml)

    def __str__(self):
        return "P%s %s" % (str(self.agent), str(self.rml))


class Literal(RML):
    def __init__(self, name, negated=False):
        RML.__init__(self)
        assert isinstance(name, str)
        self.name = name
        self.negated = negated

    @property
    def proposition(self):
        return str(self)

    @property
    def agent(self):
        return -1

    @property
    def agent_sequence(self):
        return ''

    def negate(self):
        return Literal(self.name, not self.negated)

    def merge_modalities(self):
        return self

    def is_lit(self):
        return True

    def get_prop(self):
        return Literal(self.name)

    def get_depth(self):
        return 0

    def entails_rml(self, other):
        assert other.is_lit()
        return str(self) == str(other)

    def pddl(self):
        return {False:'', True:'not_'}[self.negated] + self.name

    def __str__(self):
        return {False:'', True:'!'}[self.negated] + self.name


########################
# Helper functions
####################

def neg(rml):
    assert isinstance(rml, RML), "'%s' is not an RML -- type is %s" % (str(rml), str(type(rml)))
    return rml.negate()

def parse_rml(s):
    if '!' == s[0]:
        return neg(parse_rml(s[1:]))
    elif 'B' == s[0]:
        return Belief(s[1:].split(' ')[0], parse_rml(' '.join(s.split(' ')[1:])))
    elif 'P' == s[0]:
        return Possible(s[1:].split(' ')[0], parse_rml(' '.join(s.split(' ')[1:])))
    else:
        return Literal(s)
