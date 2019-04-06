/* 
 * File:   initially.h
 * Author: Francesco
 *
 * Created on April 6, 2019, 5:01 PM
 */

#include "initially.h"
    
bool initially::check_restriction(belief_formula bf); //Apply the restriction
        
void initially::add_pointed_condition(fluent_formula ff)
{
	
}
void initially::add_initial_condition(belief_formula bf);

const fluent_formula& initially::get_pointed_wordl_conditions();
const formula_list& initially::get_initial_conditions();