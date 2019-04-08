/* 
 * File:   belief_formula.h
 * Author: Francesco
 *
 * Created on March 31, 2019, 7:03 PM
 */

#pragma once

#include <list>
#include <memory>

#include "../utilities/printer.h"
#include "../utilities/define.h"

#include "../domain/grounder.h"


enum bf_type
{
    FLUENT_FORMULA,
    BELIEF_FORMULA,
    //@TODO: change -> this represent composed formulae
    PROPOSITIONAL_FORMULA,
    E_FORMULA,
    C_FORMULA,
    EMPTY
};

enum bf_operator
{
    BF_AND,
    BF_OR,
    BF_NOT,
    BF_NONE
};

class belief_formula
{   
public:
    std::string m_string_agent_op;    
    agent m_agent_op;    
    bf_type m_formula_type; //Which case of belief formula this is

    //Fluent formula
    string_list_list m_string_fluent_formula;
    //Pointer because it may be empty 
    fluent_formula m_fluent_formula;

    //@TODO: Unique or shared pointers??
    //Belief formula -> B(i,\phi)
    std::shared_ptr<belief_formula> m_bf1;
    //propositional formula -> B1 op B2 where B1,B2 are beliefs formulae
    std::shared_ptr<belief_formula> m_bf2;
    bf_operator m_operator;
    //Agent for group formulae
    string_list m_string_group_agents;
    agent_set m_group_agents;
    
    //@TODO: Check if is the best type of parameters pass. (Also the ones below)    
    void set_flu(const fluent_formula&);

    //belief_formula();
    //belief_formula(const belief_formula&);
    //belief_formula(belief_formula*);
    
    void print() const;
    void print_grounded(const grounder&) const;
    
    void ground(const grounder &);
    

};

//This is a CNF form because the OR is taken inside
typedef std::list<belief_formula> formula_list;