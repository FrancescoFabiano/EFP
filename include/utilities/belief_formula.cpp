/* 
 * File:   belief_formula.cpp
 * Author: Francesco
 *
 * Created on March 31, 2019, 7:43 PM
 */

#include "belief_formula.h"

//@TODO: Check if there are no side changes -- maybe better to pass another and then dereferenciate it (NULL?)

void belief_formula::set_flu(fluent_formula* ff)
{
	m_fluent_formula = ff;
}

//@TODO: MErge in an unique function with the one below or remove this

void belief_formula::print() const
{
	/*bool first = true;
	string_list::const_iterator it;
	string_list_list::const_iterator it2;
	bool first2 = true;
	std::cout << "(";*/
	switch (m_formula_type) {

	case FLUENT_FORMULA:
		/*for (it2 = m_string_fluent_formula.begin(); it2 != m_string_fluent_formula.end(); it2++) {
		    if (!first2)
			std::cout << " | ";
		    first2 = false;
		    first = true;
		    std::cout << "(";
		    for (it = it2->begin(); it != it2->end(); it++) {
			if (!first)
			    std::cout << " AND ";
			first = false;
			    std::cout << *it;
		    
		    }
		    std::cout << ")";
		}
		std::cout << ")";*/
		printer::print_list(m_string_fluent_formula);
		break;
	case BELIEF_FORMULA:
		std::cout << "B(" << m_string_agent_op << ",(";
		m_bf1->print();
		std::cout << "))";
		//std::cout << ")";
		break;

	case C_FORMULA:
		std::cout << "C([";
		printer::print_list(m_string_group_agents);
		std::cout << "],";
		m_bf1->print();
		std::cout << ")";
		// std::cout << ")";
		break;

	case E_FORMULA:
		std::cout << "E([";
		printer::print_list(m_string_group_agents);
		std::cout << "],";
		m_bf1->print();
		std::cout << ")";
		std::cout << ")";
		break;

	case PROPOSITIONAL_FORMULA:
		if (m_operator == BF_NOT)
			std::cout << "NOT(";
		//else if (m_operator == BF_NONE)
			//std::cout << "(";
		m_bf1->print();
		if (m_operator == BF_NOT)
			std::cout << ")";
		if (m_operator == BF_NONE)
			std::cout << ")";
		if (m_operator == BF_AND)
			std::cout << " AND ";
		else if (m_operator == BF_OR)
			std::cout << " OR ";
		if (m_bf2 != NULL){
			//std::cout << "(";
			m_bf2->print();
		}
		//std::cout << ")";
		break;

	case EMPTY: /* static */
		break;

	default: /* static */
		break;

	}
};

void belief_formula::print_grounded(const grounder& grounder) const
{

	bool first = true;
	string_list::const_iterator it;
	string_list_list::const_iterator it2;
	bool first2 = true;
	std::cout << "(";
	switch (m_formula_type) {

	case FLUENT_FORMULA:
		for (it2 = m_string_fluent_formula.begin(); it2 != m_string_fluent_formula.end(); it2++) {
			if (!first2)
				std::cout << " | ";
			first2 = false;
			first = true;
			std::cout << "(";
			for (it = it2->begin(); it != it2->end(); it++) {
				if (!first)
					std::cout << " AND ";
				first = false;
				std::cout << grounder.ground_fluent(*it);
			}
			std::cout << ")";
		}
		std::cout << ")";
		break;

	case BELIEF_FORMULA:
		std::cout << "B(" << grounder.ground_agent(m_string_agent_op) << ",";
		m_bf1->print_grounded(grounder);
		std::cout << ")";
		std::cout << ")";
		break;

	case C_FORMULA:
		std::cout << "C(";
		for (it = m_string_group_agents.begin(); it != m_string_group_agents.end(); it++) {
			if (!first)
				std::cout << ",";
			first = false;
			std::cout << grounder.ground_agent(*it);
		}
		std::cout << ",";
		m_bf1->print_grounded(grounder);
		std::cout << ")";
		std::cout << ")";
		break;

	case E_FORMULA:
		std::cout << "E(";
		for (it = m_string_group_agents.begin(); it != m_string_group_agents.end(); it++) {
			if (!first)
				std::cout << ",";
			first = false;
			std::cout << grounder.ground_agent(*it);
		}
		std::cout << ",";
		m_bf1->print_grounded(grounder);
		std::cout << ")";
		std::cout << ")";
		break;

	case PROPOSITIONAL_FORMULA:
		if (m_operator == BF_NOT)
			std::cout << " NOT ";
		else if (m_operator == BF_NONE)
			std::cout << "(";
		m_bf1->print_grounded(grounder);
		if (m_operator == BF_NONE)
			std::cout << ")";

		if (m_operator == BF_AND)
			std::cout << " & ";
		else if (m_operator == BF_OR)
			std::cout << " | ";
		if (m_bf2 != NULL)
			m_bf2->print_grounded(grounder);
		std::cout << ")";
		break;

	case EMPTY: //Static
		break;

	default: //Static
		break;

	} //switch
}; // print*/