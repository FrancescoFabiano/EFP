#! /usr/bin/env python
# -*- coding: latin-1 -*-

from __future__ import with_statement
from collections import defaultdict

import build_model
import pddl_to_prolog
from krrt.planning import pddl
import timers

def get_fluent_facts(task, model):
    fluent_predicates = set()
    for action in task.actions:
        for effect in action.effects:
            fluent_predicates.add(effect.literal.predicate)
    for axiom in task.axioms:
        fluent_predicates.add(axiom.name)
    return set([fact for fact in model
                if fact.predicate in fluent_predicates])

def get_objects_by_type(typed_objects, types):
    result = defaultdict(list)
    supertypes = {}
    for type in types:
        supertypes[type.name] = type.supertype_names
    for obj in typed_objects:
        result[obj.type].append(obj.name)
        for type in supertypes[obj.type]:
            result[type].append(obj.name)
    return result

def instantiate(task, model):
    relaxed_reachable = False
    fluent_facts = get_fluent_facts(task, model)
    init_facts = set(task.init)

    type_to_objects = get_objects_by_type(task.objects, task.types)

    instantiated_actions = []
    instantiated_axioms = []
    reachable_action_parameters = defaultdict(list)
    for atom in model:
        if isinstance(atom.predicate, pddl.Action):
            action = atom.predicate
            parameters = action.parameters
            inst_parameters = atom.args[:len(parameters)]
            reachable_action_parameters[action.name].append(inst_parameters)
            if isinstance(action.precondition, pddl.ExistentialCondition):
                parameters = list(parameters)
                parameters += action.precondition.parameters
            variable_mapping = dict([(par.name, arg)
                                     for par, arg in zip(parameters, atom.args)])
            inst_action = action.instantiate(variable_mapping, init_facts,
                                             fluent_facts, type_to_objects)
            if inst_action:
                instantiated_actions.append(inst_action)
        elif isinstance(atom.predicate, pddl.Axiom):
            axiom = atom.predicate
            parameters = axiom.parameters
            if isinstance(axiom.condition, pddl.ExistentialCondition):
                parameters = list(parameters)
                parameters += axiom.condition.parameters
            variable_mapping = dict([(par.name, arg)
                                     for par, arg in zip(parameters, atom.args)])
            inst_axiom = axiom.instantiate(variable_mapping, init_facts, fluent_facts)
            if inst_axiom:
                instantiated_axioms.append(inst_axiom)
        elif atom.predicate == "@goal-reachable":
            relaxed_reachable = True

    return (relaxed_reachable, fluent_facts, instantiated_actions,
           instantiated_axioms, reachable_action_parameters)

def explore(task):
    prog = pddl_to_prolog.translate(task)
    model = build_model.compute_model(prog)
    with timers.timing("Completing instantiation"):
        return instantiate(task, model)

if __name__ == "__main__":
    from krrt.planning import pddl

    task = pddl.open()
    relaxed_reachable, atoms, actions, axioms, _ = explore(task)
    print "goal relaxed reachable: %s" % relaxed_reachable
    print "%d atoms:" % len(atoms)
    for atom in atoms:
        print " ", atom
    print
    print "%d actions:" % len(actions)
    for action in actions:
        action.dump()
        print
    print
    print "%d axioms:" % len(axioms)
    for axiom in axioms:
        axiom.dump()
        print