#include "diamond_formula.h"

template<class M>
diamond_formula<M>::diamond_formula(const agent *ag, const formula *f) : m_ag(ag), m_f(f) {
    m_modal_depth = 1 + m_f->get_modal_depth();
}

template<class M>
bool diamond_formula<M>::is_entailed(const kstate &state, const kworld_ptr &world) const {
    kworld_ptr_set ag_worlds = state.get_edges().at(world).at(*m_ag);
    auto it_kws = ag_worlds.begin();

    while (it_kws != ag_worlds.end()) {
        if (m_f->is_entailed(state, *it_kws)) {
            return true;
        }
        ++it_kws;
    }
    return false;
}

template<class M>
bool diamond_formula<M>::is_entailed(const pstate &state, const pworld_ptr &world) const {
    pworld_ptr_set ag_worlds = state.get_beliefs().at(world).at(*m_ag);
    auto it_pws = ag_worlds.begin();

    while (it_pws != ag_worlds.end()) {
        if (m_f->is_entailed(state, *it_pws)) {
            return true;
        }
        ++it_pws;
    }
    return false;
}

template<class M>
bool diamond_formula<M>::is_propositional() const {
    return false;
}

template<class M>
bool diamond_formula<M>::operator==(const diamond_formula<M> &to_compare) const {
    return m_ag == to_compare.m_ag && m_f == to_compare.m_f;
}

template<class M>
bool diamond_formula<M>::operator<(const diamond_formula &to_compare) const {
    return (m_ag <  to_compare.m_ag) ||
           (m_ag == to_compare.m_ag && m_f < to_compare.m_f);
}