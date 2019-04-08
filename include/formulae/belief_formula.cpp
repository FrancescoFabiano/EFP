/* 
 * File:   belief_formula.cpp
 * Author: Francesco
 *
 * Created on March 31, 2019, 7:43 PM
 */

#include "belief_formula.h"

//@TODO: Check if there are no side changes -- maybe better to pass another and then dereferenciate it (nullptr?)

void belief_formula::set_flu(const fluent_formula& ff)
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
		if (m_bf2 != nullptr){
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
		if (m_bf2 != nullptr)
			m_bf2->print_grounded(grounder);
		std::cout << ")";
		break;

	case EMPTY: //Static
		break;

	default: //Static
		break;

	} //switch
}; // print*/

void belief_formula::ground(const grounder & gr)
{
	switch (m_formula_type) {

	case FLUENT_FORMULA:
		m_fluent_formula = gr.ground_fluent(m_string_fluent_formula);
		break;

	case BELIEF_FORMULA:
		m_agent_op = gr.ground_agent(m_string_agent_op);
		m_bf1->ground(gr);
		break;

	case C_FORMULA:
		m_group_agents = gr.ground_agent(m_string_group_agents);
		m_bf1->ground(gr);
		break;

	case E_FORMULA:
		m_group_agents = gr.ground_agent(m_string_group_agents);
		m_bf1->ground(gr);
		break;

	case PROPOSITIONAL_FORMULA:
		m_bf1->ground(gr);
		m_bf2->ground(gr);
		break;

	case EMPTY: //Static
		break;

	default: //Static
		break;

	} //switch
} // print*/

/*belief_formula::belief_formula()
{
	m_string_agent_op = "";
	m_agent_op = -1;
	m_formula_type = EMPTY;
	fluent fail = -1;
	fluent_list fail_l;
	fail_l.insert(fail);
	//m_fluent_formula.insert(fail_l);
	m_bf1 = nullptr;
	m_bf2 = nullptr;
	m_operator = BF_NONE;
	m_group_agents.insert(fail);
}

belief_formula::belief_formula(const belief_formula& bf)
{
	m_string_agent_op = bf.m_string_agent_op;
	m_agent_op = bf.m_agent_op;
	m_formula_type = bf.m_formula_type;
	m_string_fluent_formula = bf.m_string_fluent_formula;
	m_fluent_formula = bf.m_fluent_formula;
	m_bf1 = bf.m_bf1;
	m_bf2 = bf.m_bf2;
	m_operator = bf.m_operator;
	m_string_group_agents = bf.m_string_group_agents;
	m_group_agents = bf.m_group_agents;
}

belief_formula::belief_formula(belief_formula* bf)
{
	m_string_agent_op = bf->m_string_agent_op;
	m_agent_op = bf->m_agent_op;
	m_formula_type = bf->m_formula_type;
	m_string_fluent_formula = bf->m_string_fluent_formula;
	m_fluent_formula = bf->m_fluent_formula;
	m_bf1 = bf->m_bf1;
	m_bf2 = bf->m_bf2;
	m_operator = bf->m_operator;
	m_string_group_agents = bf->m_string_group_agents;
	m_group_agents = bf->m_group_agents;
}*/