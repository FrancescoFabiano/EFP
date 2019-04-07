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
#include "initially.h"

class domain
{
private:
    //@TODO: Check if ref or pointer is better std::unique_ptr<reader>
    reader* m_reader;  
    grounder m_grounder;
    
    std::vector<std::string> fluents;
    std::vector<std::string> actions;
    std::vector<std::string> agents;
    
    initially m_intial_description;
    formula_list m_goal_description;
    
    void build_agents(bool);
    void build_fluents(bool);
    void build_actions(bool);
    void build_propositions();

    
    void build_initially(bool);
       
public:
    
    /*All the useful info of the domain are store here:
     m_fluents -> all the fluents;
     m_actions -> all the actions (with effects, conditions, obsv etc.);
     m_agents -> all the agents.
     */
    fluent_list m_fluents;
    action_set m_actions;
    agent_set m_agents;

    
    domain(reader*, domain_restriction);

    
    bool build(bool);
    

    

   

    //@TODO: Check if pointer is the best type
    formula_list get_initial_description();
    formula_list get_goal_description();
    
    //@TODO: setter properly and constructors
    void set_initial_description();
    void set_goal_description();
    
};
