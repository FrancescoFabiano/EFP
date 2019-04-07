/* 
 * File:   initially.h
 * Author: Francesco
 *
 * Created on April 6, 2019, 3:48 PM
 */

#include "domain.h"
#include "define.h"
#include "belief_formula.h"

#pragma once

class intially_conditions{
    
private:
    fluent_formula pointed_world_conditions; //The conditions on the fluents that define the pointed world(s);
    formula_list bf_intial_conditions; //The initially belief_formula; for now just Common Knowledge is admissed to respect S5
    domain_restriction m_ini_restriction;
    
    bool check_restriction(belief_formula); //Apply the restriction
        
public:
     //not const & because we want this to survive after reader?
     void add_pointed_condition(fluent_formula);
     void add_initial_condition(belief_formula);

     const fluent_formula& get_pointed_wordl_conditions();
     const formula_list& get_initial_conditions();  
};

