/* 
 * File:   kstructure.h
 * Author: Francesco
 *
 * Created on April 9, 2019, 10:44 AM
 */

#include "..\state.h"

#include "kedge.h"
#include "kworld.h"

#pragma once
class kstate:public state 
{
    private:
        //@TODO: optimization -> use shared pointer and create a class that stores edges and worlds where you refer to
        kedge_list m_edges;
        kworld_list m_worlds;
        kworld m_pointed;
    public:
        kstate(const state &);
    
        state compute_succ (const action &);
        bool entails (fluent);
        bool entails (const belief_formula &);
};

