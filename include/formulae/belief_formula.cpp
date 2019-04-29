/**
 * \brief Class that implements a \ref belief_formula.h.
 *
 * \author Francesco Fabiano.
 * \date March 31, 2019
 */
#include "belief_formula.h"

void belief_formula::set_flu(const fluent_formula& to_set)
{
	/**
	 * \todo Check if there are no side changes with the pointer passing -- maybe better to use the copy operator.
	 * \todo check nullptr.
	 */
	m_fluent_formula = to_set;
}

void belief_formula::print() const
{
	/*bool first = true;
	string_set::const_iterator it;
	string_set_set::const_iterator it2;
	bool first2 = true;
	std::cout << "(";*/
	switch ( m_formula_type ) {

	case FLUENT_FORMULA:
		printer::get_instance().print_list(m_string_fluent_formula);
		break;
	case BELIEF_FORMULA:
		std::cout << "B(" << m_string_agent_op << ",(";
		m_bf1->print();
		std::cout << "))";
		break;

	case C_FORMULA:
		std::cout << "C([";
		printer::get_instance().print_list(m_string_group_agents);
		std::cout << "],";
		m_bf1->print();
		std::cout << ")";
		break;

	case E_FORMULA:
		std::cout << "E([";
		printer::get_instance().print_list(m_string_group_agents);
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
		if (m_bf2 != nullptr) {
			//std::cout << "(";
			m_bf2->print();
		}
		//std::cout << ")";
		break;

	case EMPTY: /* static */
		std::cerr << "\n Unknown belief_formula type.";
		exit(1);
		break;

	default: /* static */
		std::cerr << "\n Unknown belief_formula type.";
		exit(1);
		break;

	}
};

void belief_formula::print_grounded(const grounder& grounder) const
{

	bool first = true;
	string_set::const_iterator it;
	string_set_set::const_iterator it2;
	bool first2 = true;
	std::cout << "(";
	switch ( m_formula_type ) {

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
		std::cerr << "\n Unknown belief_formula type.";
		exit(1);
		break;

	default:
		std::cerr << "\n Unknown belief_formula type.";
		exit(1);
		break;

	} //switch
}; // print*/

void belief_formula::ground(const grounder & gr)
{
	switch ( m_formula_type ) {

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
		std::cerr << "\n Unknown belief_formula type.";
		exit(1);
		break;

	default: //Static
		std::cerr << "\n Unknown belief_formula type.";
		exit(1);
		break;
	} //switch
}

bool belief_formula::operator==(const belief_formula& to_compare) const
{

	if (m_formula_type == to_compare.m_formula_type) {

		switch ( m_formula_type ) {

		case FLUENT_FORMULA:
		{
			return(m_fluent_formula == to_compare.m_fluent_formula);
			break;
		}

		case BELIEF_FORMULA:
		{
			if (m_agent_op == to_compare.m_agent_op) {
				return *m_bf1 == *(to_compare.m_bf1);
			} else {
				return false;
			}
			break;
		}

		case PROPOSITIONAL_FORMULA:
		{
			if (m_operator == to_compare.m_operator) {
				switch ( m_operator ) {
				case BF_NOT:
				{
					return *m_bf1 == *(to_compare.m_bf1);
					break;
				}
				case BF_AND:
				case BF_OR:
				{
					if (*m_bf1 == *(to_compare.m_bf1)) {
						return *m_bf2 == *(to_compare.m_bf2);
					} else if (*m_bf1 == *(to_compare.m_bf2)) {
						return *m_bf2 == *(to_compare.m_bf1);
					} else {
						return false;
					}
					break;
				}
				default:
					return false;
					break;
				}
			} else {
				return false;
			}
			break;
		}
		case E_FORMULA:
		case C_FORMULA:
		{
			if (m_group_agents == to_compare.m_group_agents) {
				return *m_bf1 == *(to_compare.m_bf1);
			}
			break;
		}
		default:
		{
			std::cerr << "\n Unknown belief_formula type.";
			exit(1);
			break;
		}
		}
	}
	return false;
}

// print*/

/*belief_formula::belief_formula()
{
	m_string_agent_op = "";
	m_agent_op = -1;
	m_formula_type = EMPTY;
	fluent fail = -1;
	fluent_set fail_l;
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