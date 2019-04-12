/* 
 * File:   domain.h
 * Author: Francesco
 *
 * Created on March 31, 2019, 5:41 PM
 */

#pragma once
#include <memory>

#include "grounder.h"
#include "initially.h"

#include "../utilities/define.h"
#include "../utilities/reader.h"
#include "../actions/action.h"
//THIS IS BECAUSE IS A TEMPLATIC CLASS AND IT IS A DEPENDECY
#include "../states/state_T.icpp"

class domain
{
private:
    //@TODO: Check if ref or pointer is better std::unique_ptr<reader>
    std::shared_ptr<reader> m_reader;  
    grounder m_grounder;
    
    std::vector<std::string> fluents;
    std::vector<std::string> actions;
    std::vector<std::string> agents;
    
    state_type m_state_type;
    
    initially m_intial_description;
    domain_restriction m_goal_restriction;
    
    formula_list m_goal_description;
    
    void build_agents(bool);
    void build_fluents(bool);
    void build_actions(bool);
    void build_propositions();

    
    void build_initially(bool);
    void build_goal(bool);
    bool check_goal_restriction(const belief_formula &);

public:
    
    /*All the useful info of the domain are store here:
     m_fluents -> all the fluents;
     m_actions -> all the actions (with effects, conditions, obsv etc.);
     m_agents -> all the agents.
     */
    fluent_list m_fluents;
    action_set m_actions;
    agent_set m_agents;

    
    domain(std::shared_ptr<reader>, state_type, domain_restriction, domain_restriction);

    
    bool build(bool);
    

    

   

    //@TODO: Check if pointer is the best type
    formula_list get_initial_description();
    formula_list get_goal_description();
    
    //@TODO: setter properly and constructors
    void set_initial_description();
    void set_goal_description();
    
};
