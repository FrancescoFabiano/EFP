#include "ck_formula.h"

template<class M>
ck_formula<M>::ck_formula(const agent_set *ags, const formula *f) : m_ags(ags), m_f(f) {
    m_modal_depth = 1 + f->get_modal_depth();
}

template<class M>
bool ck_formula<M>::is_entailed(const kstate &state, const kworld_ptr &world) const {
    std::queue<kworld_ptr> to_visit_worlds;
    kworld_ptr_set visited_worlds, ag_worlds, to_visit;

    kworld_ptr current_world;
    std::map<agent, kworld_ptr_set> world_edges;

    agent_set::const_iterator it_ags;
    kworld_ptr_set::const_iterator it_kws;

    to_visit_worlds.push(world);

    while (!to_visit_worlds.empty()) {
        current_world = to_visit_worlds.front();
        world_edges = state.get_edges().at(current_world);

        visited_worlds.insert(current_world);

        if (!m_f->is_entailed(state, current_world)) {
            return false;
        } else {
            for (it_ags = m_ags->begin(); it_ags != m_ags->end(); ++it_ags) {
                ag_worlds = world_edges.at(*it_ags);
//                to_visit = std::set_difference(ag_worlds, visited_worlds);
//                for (it_kws = to_visit.begin(); it_kws != to_visit.end(); it_kws++) {
                for (it_kws = ag_worlds.begin(); it_kws != ag_worlds.end(); ++it_kws) {
                    if (visited_worlds.find(*it_kws) == visited_worlds.end()) {
                        to_visit_worlds.push(*it_kws);
                    }
                }
            }
            to_visit_worlds.pop();
        }
    }
    return true;
}

template<class M>
bool ck_formula<M>::is_entailed(const pstate &state, const pworld_ptr &world) const {
    std::queue<pworld_ptr> to_visit_worlds;
    pworld_ptr_set visited_worlds, ag_worlds;

    pworld_ptr current_world;
    std::map<agent, pworld_ptr_set> world_edges;

    agent_set::const_iterator it_ags;
    pworld_ptr_set::const_iterator it_pws;

    to_visit_worlds.push(world);

    while (!to_visit_worlds.empty()) {
        current_world = to_visit_worlds.front();
        world_edges = state.get_beliefs().at(current_world);

        visited_worlds.insert(current_world);

        if (!m_f->is_entailed(state, current_world)) {
            return false;
        } else {
            for (it_ags = m_ags->begin(); it_ags != m_ags->end(); ++it_ags) {
                ag_worlds = world_edges.at(*it_ags);

                for (it_pws = ag_worlds.begin(); it_pws != ag_worlds.end(); ++it_pws) {
                    if (visited_worlds.find(*it_pws) == visited_worlds.end()) {
                        to_visit_worlds.push(*it_pws);
                    }
                }
            }
            to_visit_worlds.pop();
        }
    }
    return true;
}

template<class M>
bool ck_formula<M>::is_propositional() const {
    return false;
}

template<class M>
bool ck_formula<M>::operator==(const ck_formula<M> &to_compare) const {
    return m_ags == to_compare.m_ags && m_f == to_compare.m_f;
}

template<class M>
bool ck_formula<M>::operator<(const ck_formula<M> &to_compare) const {
    return (m_ags <  to_compare.m_ags) ||
           (m_ags == to_compare.m_ags && m_f < to_compare.m_f);
}