#include "not_formula.h"

not_formula::not_formula(const formula *f) : m_f(f) {
    m_modal_depth = f->get_modal_depth();
}

bool not_formula::is_entailed(const kstate &state, const kworld_ptr &world) const {
    return !m_f->is_entailed(state, world);
}

bool not_formula::is_entailed(const pstate &state, const pworld_ptr &world) const {
    return !m_f->is_entailed(state, world);
}

bool not_formula::operator==(const not_formula &to_compare) const {
    return m_f == to_compare.m_f;
}

bool not_formula::operator<(const not_formula &to_compare) const {
    return m_f < to_compare.m_f;
}
