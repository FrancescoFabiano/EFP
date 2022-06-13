#include "literal.h"
#include "../../states/kripke/kstate.h"
#include "../../states/possibilities/pstate.h"

literal::literal(const fluent *fluent, const bool is_negated) : m_fluent(fluent), m_is_negated(is_negated) {
    m_modal_depth = 0;
}

bool literal::is_entailed(const fluent_ptr_set *fluent_set) const {
    assert(m_modal_depth == 0);
    return (fluent_set->find(m_fluent) != fluent_set->end()) != m_is_negated;
}

bool literal::is_entailed(const kstate *state, const kworld *world) const {
    return (world->get_fluent_set()->find(m_fluent) != world->get_fluent_set()->end()) != m_is_negated;
}

bool literal::is_entailed(const pstate *state, const pworld *world) const {
    return (world->get_fluent_set()->find(m_fluent) != world->get_fluent_set()->end()) != m_is_negated;
}

bool literal::is_propositional() const {
    return true;
}

const fluent *literal::get_fluent() const {
    return m_fluent;
}

bool literal::is_positive() const {
    return !m_is_negated;
}

bool literal::operator==(const literal &to_compare) const {
    return m_is_negated == to_compare.m_is_negated && m_fluent == to_compare.m_fluent;
}

bool literal::operator<(const literal &to_compare) const {
    return (m_is_negated <  to_compare.m_is_negated) ||                                     // Positive literals < negative literals
           (m_is_negated == to_compare.m_is_negated && m_fluent < to_compare.m_fluent);
}
