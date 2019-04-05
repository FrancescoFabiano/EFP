/* 
 * File:   kworld.h
 * Author: Francesco
 *
 * Created on March 31, 2019, 1:00 PM
 */
#pragma once

#include "kedge.h"

typedef unsigned short kworld_id;

class kworld
{
private:
    literal_list m_literal_list;
    kworld_id m_world_id;
    kedge_id_list m_edges_out;
    kedge_id_list m_edges_in;
    bool m_exists;
    
    map< agent, std::set<kworld_id> > map_edge_next_state; //is used to get the next states via the edges of label agent.
   
public:
    kworld();

    kedge_id_list get_edges_in();
    kedge_id_list get_edges_out();
    literal_list get_literal_list();
    kworld_id get_id();
    bool get_exists();
    map< agent, std::set<kworld_id> > get_map_edge_next_state();

    void set_edges_in(kedge_id_list);
    void set_edges_out(kedge_id_list);
    void set_literal_list(literal_list);
    void set_id(kworld_id);
    void set_exists(bool);
    
    void add_out(kedge_id);
    void add_in(kedge_id);
    void print_info();
    void add_to_map(agent, kworld_id);
    void reset_map();

    bool entail_fluent_formula(fluent_formula) const;
    bool entail_literal(literal) const;
    bool entail_literal_list(literal_list) const;
};

