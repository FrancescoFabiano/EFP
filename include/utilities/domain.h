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

enum domain_restriction
{
    S5, //Knowledge, S5 finitary
    K45, //Belief
    NONE, //sensing action (peek_a)
};

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

    
    domain(reader*);

    
    bool build(bool, domain_restriction);
    

    

   

    //@TODO: Check if pointer is the best type
    formula_list get_initial_description();
    formula_list get_goal_description();
    
    //@TODO: setter properly and constructors
    void set_initial_description();
    void set_goal_description();
    
};
