/* 
 * File:   kworld.h
 * Author: Francesco
 *
 * Created on March 31, 2019, 1:00 PM
 */
#pragma once

#include <set>

#include "kedge.h"

#include "../../utilities/define.h"
#include "../../domain/grounder.h"

class kworld
{
private:
    fluent_list m_fluent_list;
    kworld_id m_world_id;
  
    kworld_id hash_fluents_into_id(const fluent_list&);
    kworld_id hash_fluents_into_id(); //generate an unique id given the state information -> the literal
    
    //@TODO: Is reference correct????? In theory we refer to element of the kstore
    void set_fluent_list(const fluent_list &);
    void set_id();
    
public:
    kworld();
    //@TODO: consistency
    kworld(const fluent_list &);
    
    const fluent_list & get_fluent_list() const;
    kworld_id get_id() const;

    bool entails(fluent) const;
    bool entails(const fluent_list &) const;
    bool entails(const fluent_formula &) const;
    
    bool operator<(const kworld&) const;
    void print_info(const grounder& gr);


};