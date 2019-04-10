/* 
 * File:   kworld.h
 * Author: Francesco
 *
 * Created on March 31, 2019, 1:00 PM
 */
#pragma once

#include <string>
#include <set>
#include <memory>
#include <map>


#include "../../utilities/define.h"
#include "kedge.h"


//Use a unique key that identifies the world??? The hash of the literal list
typedef std::string kworld_id;
typedef std::shared_ptr<kworld> kworld_ptr;
//@TODO: is maybe better set? Duplicate? if switch add < for pointer
typedef std::set<kworld_ptr> kworld_ptr_set;

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
    
    const fluent_list & get_fluent_list();
    kworld_id get_id();

    void print_info();

    bool entails(fluent) const;
    bool entails(const fluent_list &) const;
    bool entails(const fluent_formula &) const;
    
    bool operator<(const kworld&) const;

};