#include "formula.h"

bool formula::is_entailed(const kstate &state, const kworld_ptr &world) const {
    return false;
}

bool formula::is_entailed(const pstate &state, const pworld_ptr &world) const {
    return false;
}

unsigned long formula::calc_modal_depth() const {
    return 0;
}

unsigned long formula::get_modal_depth() const {
    return m_modal_depth;
}

bool formula::is_propositional() const {
    return m_modal_depth == 0;
}
