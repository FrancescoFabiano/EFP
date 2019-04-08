/* 
 * File:   reader.h
 * Author: Francesco
 *
 * Created on March 31, 2019, 6:30 PM
 */

#pragma once

#include "define.h"
#include "../formulae/belief_formula.h"
#include "../formulae/proposition.h"

class reader
{
public:
    /* reading from the input file */
    string_list m_fluents;
    string_list m_actions;
    //string_list m_sensing_actions;
    //string_list m_ontic_actions;
    //string_list m_ann_actions;
    string_list m_agents;
    string_list_list m_initially;
    string_list m_goal; // set of literals in the goal
    
    formula_list m_bf_initially;
    formula_list m_bf_goal;
    
    proposition_list m_propositions;

    std::string name(fluent x);
    //@TODO: Check if is the best type of parameters pass.
    string_list name(const fluent_list&);
    int read();
    void print() const;
};
