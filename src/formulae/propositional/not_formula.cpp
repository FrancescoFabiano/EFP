#include "not_formula.h"

not_formula::not_formula(const formula &f) {
    m_f = f;
    m_modal_depth = not_formula::calc_modal_depth();
}

bool not_formula::is_entailed(const kstate &state, const kworld_ptr &world) const {
    return !m_f.is_entailed(state, world);
}

bool not_formula::is_entailed(const pstate &state, const pworld_ptr &world) const {
    return !m_f.is_entailed(state, world);
}

unsigned long not_formula::calc_modal_depth() const {
    return m_f.calc_modal_depth();
}
