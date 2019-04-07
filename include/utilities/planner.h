/* 
 * File:   planner.h
 * Author: Francesco
 *
 * Created on March 31, 2019, 5:08 PM
 */

#pragma once

#include "domain.h"

class planner
{
private:
    //Utilities
    timer* m_timer; // timer
    domain m_given_domain;
      
    state generate_initial();
    //Queue management
    
    //Support    
    bool is_visited();
    bool is_goal();
    int retrieve_heuristic();
       
public:
    //@TODO: constructors
    planner(timer*);
    
    bool search();
};