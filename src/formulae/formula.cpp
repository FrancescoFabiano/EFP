#include "formula.h"

formula::formula() : m_modal_depth(0) {}

unsigned long formula::get_modal_depth() const {
    return m_modal_depth;
}

bool formula::is_propositional() const {
    return m_modal_depth == 0;
}
