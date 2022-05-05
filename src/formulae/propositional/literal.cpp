#include "literal.h"

literal::literal(const fluent *fluent, const bool is_negated) : m_fluent(fluent), m_is_negated(is_negated) {
    m_modal_depth = 0;
}

bool literal::is_entailed(const kstate &state, const kworld_ptr &world) const {
    return (world.get_fluent_set().find(*m_fluent) != world.get_fluent_set().end()) != m_is_negated;
}

bool literal::is_entailed(const pstate &state, const pworld_ptr &world) const {
    return (world.get_fluent_set().find(*m_fluent) != world.get_fluent_set().end()) != m_is_negated;
}
