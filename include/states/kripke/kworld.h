/* 
 * File:   kworld.h
 * Author: Francesco
 *
 * Created on March 31, 2019, 1:00 PM
 */
#pragma once

#include <string>
#include <vector>
#include <memory>

#include "../../utilities/define.h"
#include "kedge.h"


//Use a unique key that identifies the world??? The hash of the literal list
typedef std::string kworld_id;
typedef std::shared_ptr<kworld> kworld_ptr;
//@TODO: is maybe better set? Duplicate? if switch add < for pointer
typedef std::vector<kworld_ptr> kworld_ptr_list;

class kworld
{
private:
    fluent_list m_fluent_list;
    kworld_id m_world_id;
    kedge_ptr_list m_edges_out;
    kedge_ptr_list m_edges_in;
    
    map<agent, kworld_ptr_list> map_edge_next_state; //is used to get the next states via the edges of label agent.

    
    kworld_id hash_fluents_into_id(const fluent_list&);
    kworld_id hash_fluents_into_id(); //generate an unique id given the state information -> the literal
    
    //@TODO: Is reference correct????? In theory we refer to element of the kstore
    void set_edges_in(const kedge_ptr_list &);
    void set_edges_out(const kedge_ptr_list &);
    void set_literal_list(const fluent_list &);
    void set_id();
    
public:
    kworld();
    kworld(const fluent_list &);


    kedge_ptr_list get_edges_in();
    kedge_ptr_list get_edges_out();
    fluent_list get_fluent_list();
    kworld_id get_id();
    map< agent, kworld_ptr_list > get_map_edge_next_state();

    
    
    void add_out(const kedge &);
    void add_in(const kedge &);
    void print_info();
    void add_to_map(agent, kworld_ptr);
    void reset_map();

    bool entail_fluent_formula(fluent_formula) const;
    bool entail_literal(literal) const;
    bool entail_literal_list(literal_list) const;
};