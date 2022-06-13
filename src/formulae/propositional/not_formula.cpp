#include "not_formula.h"

not_formula::not_formula(const formula *f) : m_f(f) {
    m_modal_depth = f->get_modal_depth();
}

bool not_formula::is_entailed(const fluent_ptr_set *fluent_set) const {
    assert(m_modal_depth == 0);
    return !m_f->is_entailed(fluent_set);
}

bool not_formula::is_entailed(const kstate *state, const kworld *world) const {
    return !m_f->is_entailed(state, world);
}

bool not_formula::is_entailed(const pstate *state, const pworld *world) const {
    return !m_f->is_entailed(state, world);
}

bool not_formula::operator==(const not_formula &to_compare) const {
    return m_f == to_compare.m_f;
}

bool not_formula::operator<(const not_formula &to_compare) const {
    return m_f < to_compare.m_f;
}
