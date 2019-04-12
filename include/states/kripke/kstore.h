/* 
 * File:   kstore.h
 * Author: Francesco
 *
 * Created on April 9, 2019, 3:22 PM
 */

#pragma once

#include <set>
#include "kworld.h"
#include "kedge.h"

typedef std::set<kedge> kedge_set;
typedef std::set<kworld> kworld_set;

class kstore
{
private:
    //STATIC??
    kedge_set m_created_edges;
    kworld_set m_created_worlds;
    
    kstore();
public:

    static kstore& get_instance();

    //@TODO: Param ok because set makes copy. Ret?
    
    kedge_ptr add_edge(const kedge &);
    kworld_ptr add_world(const kworld &);
    
    //@TODO: It is better to create a new world each time or to search for the key and in case they don't exist create the world and add the world (double insert basically)
    //const kworld& add_world(const fluent_formula &); //Use this in case building world is way more expensive than searching the set

    kstore(kstore const&) = delete;
    void operator=(kstore const&) = delete;
    
};