#include "box_whether_formula.h"
#include "../../states/kripke/kstate.h"
#include "../../states/possibilities/pstate.h"

template<class M>
box_whether_formula<M>::box_whether_formula(const agent *ag, const formula *f) : m_ag(ag), m_f(f) {
    m_modal_depth = 1 + m_f->get_modal_depth();
}

template<class M>
bool box_whether_formula<M>::is_entailed(const kstate *state, const kworld *world) const {
    auto ag_worlds = state->get_edges().at(world).at(m_ag);
    auto it_kws = ag_worlds.begin();
    bool is_entailed = m_f->is_entailed(state, *it_kws);

    while (++it_kws != ag_worlds.end()) {
        if (m_f->is_entailed(state, *it_kws) != is_entailed) {
            return false;
        }
    }
    return true;
}

template<class M>
bool box_whether_formula<M>::is_entailed(const pstate *state, const pworld *world) const {
    auto ag_worlds = state->get_edges().at(world).at(m_ag);
    auto it_pws = ag_worlds.begin();
    bool is_entailed = m_f->is_entailed(state, *it_pws);

    while (++it_pws != ag_worlds.end()) {
        if (m_f->is_entailed(state, *it_pws) != is_entailed) {
            return false;
        }
    }
    return true;
}

template<class M>
bool box_whether_formula<M>::is_propositional() const {
    return false;
}

template<class M>
const agent* box_whether_formula<M>::get_agent() const {
    return m_ag;
}

template<class M>
const formula* box_whether_formula<M>::get_formula() const {
    return m_f;
}

template<class M>
bool box_whether_formula<M>::operator==(const box_whether_formula<M> &to_compare) const {
    return m_ag == to_compare.m_ag && m_f == to_compare.m_f;
}

template<class M>
bool box_whether_formula<M>::operator<(const box_whether_formula<M> &to_compare) const {
    return (m_ag <  to_compare.m_ag) ||
           (m_ag == to_compare.m_ag && m_f < to_compare.m_f);
}
