#include "diamond_formula.h"

template<class M>
diamond_formula<M>::diamond_formula(const agent &ag, const formula &f) {
    m_ag = ag;
    m_f = f;
    m_modal_depth = diamond_formula::calc_modal_depth();
}

template<class M>
bool diamond_formula<M>::is_entailed(const kstate &state, const kworld_ptr &world) const {
    kworld_ptr_set ag_worlds = state.get_edges().at(world).at(m_ag);
    kworld_ptr_set::const_iterator it_kws;

    for (it_kws = ag_worlds.begin(); it_kws != ag_worlds.end(); it_kws++) {
        if (m_f.is_entailed(state, world)) {
            return true;
        }
    }

    return false;
}

template<class M>
bool diamond_formula<M>::is_entailed(const pstate &state, const pworld_ptr &world) const {
    pworld_ptr_set ag_worlds = state.get_beliefs().at(world).at(m_ag);
    pworld_ptr_set::const_iterator it_pws;

    for (it_pws = ag_worlds.begin(); it_pws != ag_worlds.end(); it_pws++) {
        if (m_f.is_entailed(state, world)) {
            return true;
        }
    }

    return false;
}

template<class M>
unsigned long diamond_formula<M>::calc_modal_depth() const {
    return 1 + m_f.calc_modal_depth();
}

template<class M>
bool diamond_formula<M>::is_propositional() const {
    return false;
}
