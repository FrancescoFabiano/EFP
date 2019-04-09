/* 
 * File:   initially.cpp
 * Author: Francesco
 *
 * Created on April 6, 2019, 5:01 PM
 */

#include <iostream>

#include "initially.h"

 initially::initially(){
	 m_ini_restriction = FAIL;
 }

initially::initially(domain_restriction ini_restriction){
	 m_ini_restriction = ini_restriction;
 }

    
bool initially::check_restriction(const belief_formula & bf)//Apply the restriction
{
	bool ret = false;
	switch(m_ini_restriction){
	//We only admit C(belief)
	//@TODO: Check if all the agents have to be in the C
	case S5:
		if (bf.m_formula_type == C_FORMULA){
			ret = true;
		}
		else if (bf.m_formula_type == PROPOSITIONAL_FORMULA)
		{
			if(bf.m_operator != BF_AND)
			{
				ret = false;
			}
			else{
				//To admit initially [C(phi2), C(phi2)]
				ret = check_restriction(*bf.m_bf1) && check_restriction(*bf.m_bf2);
			}
		}
		break;
	case K45:
		ret = false;
		break;
	case NONE:
		ret = true;
		break;
	default: /* static */
		ret = false;
		break;
	}
	
	return ret;

}
//This type of parameter is fine because of we add the return value and not ff
void initially::add_pointed_condition(const fluent_formula & ff)
{
	//Is in DNF form so you have to add these to the fluent of before (all of them)
	 m_pointed_world_conditions = formula_manipulation::and_ff(m_pointed_world_conditions,ff);
}
//This type of parameter is fine because of the push back (makes a copy))
void initially::add_initial_condition(const belief_formula& bf)
{
//This is a CNF form because the OR is taken inside
	if(check_restriction(bf)){
		m_bf_intial_conditions.push_back(bf);
	}
	else{
		std::cerr << "The initial state does not respect the conditions\n";
		exit(1);
	}
}

const fluent_formula& initially::get_pointed_wordl_conditions()
{
	return m_pointed_world_conditions;
}
const formula_list& initially::get_initial_conditions()
{
	return m_bf_intial_conditions;
}
