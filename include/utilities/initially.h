/* 
 * File:   initially.h
 * Author: Francesco
 *
 * Created on April 6, 2019, 3:48 PM
 */
#pragma once

#include "define.h"
#include "belief_formula.h"
#include "formula_manipulation.h"

class initially {
    
private:
    fluent_formula m_pointed_world_conditions; //The conditions on the fluents that define the pointed world(s);
    formula_list m_bf_intial_conditions; //The initially belief_formula; for now just Common Knowledge is admissed to respect S5
    domain_restriction m_ini_restriction;
    
    bool check_restriction(const belief_formula &); //Apply the restriction
        
public:
    
    initially();
    initially(domain_restriction);
    
     //not const & because we want this to survive after reader?
     void add_pointed_condition(const fluent_formula &);
     void add_initial_condition(const belief_formula &);

     const fluent_formula& get_pointed_wordl_conditions();
     const formula_list& get_initial_conditions();  
};

