#include "and_formula.h"

and_formula::and_formula(const formula &f1, const formula &f2) {
    m_f1 = f1;
    m_f2 = f2;
    m_modal_depth = and_formula::calc_modal_depth();
}

bool and_formula::is_entailed(const kstate &state, const kworld_ptr &world) const {
    return m_f1.is_entailed(state, world) && m_f2.is_entailed(state, world);
}

bool and_formula::is_entailed(const pstate &state, const pworld_ptr &world) const {
    return m_f1.is_entailed(state, world) && m_f2.is_entailed(state, world);
}

unsigned long and_formula::calc_modal_depth() const {
    return std::max(m_f1.calc_modal_depth(), m_f2.calc_modal_depth());
}
