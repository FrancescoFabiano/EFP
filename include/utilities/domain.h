/* 
 * File:   domain.h
 * Author: Francesco
 *
 * Created on March 31, 2019, 5:41 PM
 */

#pragma once

#include "reader.h"
#include "define.h"
#include "../interfaces/action.h"
#include "grounder.h"


class domain
{
private:
    //@TODO: Check if ref or pointer is better std::unique_ptr<reader>
    reader* m_reader;  
    grounder m_grounder;
    
    std::vector<std::string> fluents;
    std::vector<std::string> actions;
    std::vector<std::string> agents;
    
    formula_list m_intial_description;
    formula_list m_goal_description;
    
public:
    
    //fluent_map m_fluents_map;
    fluent_list m_fluents;
    
    //action_name_map m_actions_name_map;
    action_set m_actions;
    
    //agent_map m_agent_map;
    agent_set m_agents;

    
    domain(reader*);
    
    bool build(bool);
   

    //@TODO: Check if pointer is the best type
    formula_list get_initial_description();
    formula_list get_goal_description();
    
    //@TODO: setter properly and constructors
    void set_initial_description();
    void set_goal_description();
    
};
