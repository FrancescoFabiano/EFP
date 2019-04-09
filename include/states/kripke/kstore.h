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
    static kedge_set m_created_edges;
    static kworld_set m_created_worlds;

    kstore();
public:

    static kstore& get_instance();

    //@TODO: smart pointer instead?
    const kedge& add_edge(const kedge &edge);    
    const kworld& add_world(const kworld &world);

    store(store const&) = delete;
    void operator=(store const&) = delete;
    
};