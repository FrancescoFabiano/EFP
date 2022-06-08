#include "or_formula.h"

or_formula::or_formula(const formula *f1, const formula *f2) : m_f1(f1), m_f2(f2) {
    m_modal_depth = std::max(m_f1->get_modal_depth(), m_f2->get_modal_depth());
}

bool or_formula::is_entailed(const fluent_set *fluent_set) const {
    assert(m_modal_depth == 0);
    return m_f1->is_entailed(fluent_set) || m_f2->is_entailed(fluent_set);
}

bool or_formula::is_entailed(const kstate *state, const kworld *world) const {
    return m_f1->is_entailed(state, world) || m_f2->is_entailed(state, world);
}

bool or_formula::is_entailed(const pstate *state, const pworld *world) const {
    return m_f1->is_entailed(state, world) || m_f2->is_entailed(state, world);
}

bool or_formula::operator==(const or_formula &to_compare) const {
    return m_f1 == to_compare.m_f1 && m_f2 == to_compare.m_f2;
}

bool or_formula::operator<(const or_formula &to_compare) const {
    auto min1= std::min(m_f1, m_f2),
         max1= std::max(m_f1, m_f2),
         min2= std::min(to_compare.m_f1, to_compare.m_f2),
         max2= std::max(to_compare.m_f1, to_compare.m_f2);

    return (min1 <  min2) || (min1 == min2 && max1 < max2);
}
