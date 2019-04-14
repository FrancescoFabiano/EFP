/* 
 * File:   kstate.h
 * Author: Francesco
 *
 * Created on April 9, 2019, 10:44 AM
 */
#pragma once

#include "kedge.h"
#include "kworld.h"
#include "kstore.h"

#include "../../utilities/define.h"
#include "../../domain/initially.h"
#include "../../actions/action.h"

//typedef std::map<agent, kworld_ptr_set> reachable_worlds_ag;
class kstate{
private:
    //@TODO: optimization -> use shared pointer and create a class that stores edges and worlds where you refer to
    kedge_ptr_set m_edges;
    kworld_ptr_set m_worlds;
    kworld_ptr m_pointed;

    bool entails(fluent, kworld_ptr);
    bool entails(const fluent_set &, kworld_ptr);
    bool entails(const fluent_formula &, kworld_ptr);
    
    bool entails(const belief_formula &, kworld_ptr);
    bool entails(const belief_formula &, const kworld_ptr_set &);
    
    const kworld_ptr_set get_B_reachable_worlds(agent, kworld_ptr);
    bool get_B_reachable_worlds(agent, kworld_ptr, kworld_ptr_set&);
    
    const kworld_ptr_set get_E_reachable_worlds(const agent_set &, kworld_ptr);
    bool get_E_reachable_worlds(const agent_set &, kworld_ptr, kworld_ptr_set&);
    
    const kworld_ptr_set get_C_reachable_worlds(const agent_set &, kworld_ptr);

public:

    bool entails(fluent);
    bool entails(const fluent_set &);
    bool entails(const fluent_formula &);
    
    bool entails(const belief_formula &);

    void add_world(const kworld &);
    void add_edge(const kedge &);
    
    void build_initial(const initially &);
    const kstate& compute_succ(const action &);
};

