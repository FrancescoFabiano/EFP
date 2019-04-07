/* 
 * File:   domain.cpp
 * Author: Francesco
 *
 * Created on April 1, 2019, 10:41 AM
 */
#include "domain.h"

domain::domain(reader* reader, domain_restriction ini_restriction)
{
	m_reader = reader;
	m_intial_description = initially(ini_restriction);

}

bool domain::build(bool debug)
{

	build_agents(debug);
	build_fluents(debug);
	build_actions(debug);
	build_initially(debug);

	return true;
}

void domain::build_agents(bool debug)
{
	agent_map domain_agent_map;
	std::cout << "\nBuilding agent list..." << std::endl;
	string_list::const_iterator it_agents;
	int i = 0;
	for (it_agents = m_reader->m_agents.begin(); it_agents != m_reader->m_agents.end(); it_agents++) {
		domain_agent_map.insert(agent_map::value_type(*it_agents, i));
		m_agents.insert(i++);
		if (debug) {
			std::cout << "Agent " << *it_agents << " is " << (i - 1) << std::endl;
		}
	}
	m_grounder.set_agent_map(domain_agent_map);
}

void domain::build_fluents(bool debug)
{
	fluent_map domain_fluent_map;

	std::cout << "\nBuilding fluent literals..." << std::endl;
	string_list::const_iterator it_fluents;
	int i = 0;
	for (it_fluents = m_reader->m_fluents.begin();
		it_fluents != m_reader->m_fluents.end(); it_fluents++) {
		domain_fluent_map.insert(fluent_map::value_type(*it_fluents, i));
		m_fluents.insert(i++);
		if (debug) {
			std::cout << "Literal " << *it_fluents << " is " << (i - 1) << std::endl;
		}
		domain_fluent_map.insert(fluent_map::value_type(NEGATION_SYMBOL + *it_fluents, i));
		m_fluents.insert(i++);
		if (debug) {
			std::cout << "Literal not " << *it_fluents << " is " << (i - 1) << std::endl;
		}
	}
	m_grounder.set_fluent_map(domain_fluent_map);
}

void domain::build_actions(bool debug)
{

	action_name_map domain_action_name_map;
	std::cout << "\nBuilding action list..." << std::endl;
	string_list::const_iterator it_actions_name;
	int i = 0;
	for (it_actions_name = m_reader->m_actions.begin();
		it_actions_name != m_reader->m_actions.end(); it_actions_name++) {
		action tmp_action(*it_actions_name, i);
		domain_action_name_map.insert(action_name_map::value_type(*it_actions_name, i));
		i++;
		m_actions.insert(tmp_action);
		if (debug) {
			std::cout << "Action " << tmp_action.get_name() << " is " << tmp_action.get_id() << std::endl;
		}
	}

	m_grounder.set_action_name_map(domain_action_name_map);

	build_propositions();

	if (debug) {
		std::cout << "\nPrinting complete action list..." << std::endl;
		action_set::const_iterator it_actions;
		i = 0;
		for (it_actions = m_actions.begin();
			it_actions != m_actions.end(); it_actions++) {
			it_actions->print(m_grounder);
		}
	}

}

void domain::build_propositions()
{
	//Adding propositions to actions list
	std::cout << "\nAdding propositions to actions..." << std::endl;
	proposition_list::const_iterator it_prop;
	action_id action_to_modify;
	for (it_prop = m_reader->m_propositions.begin();
		it_prop != m_reader->m_propositions.end(); it_prop++) {
		action_to_modify = m_grounder.ground_action(it_prop->m_action_name);
		if (m_actions.size() > action_to_modify) {
			//To change remove and add the updated --> @TODO: find better like queue
			action_set::iterator it_action_set = std::next(m_actions.begin(), action_to_modify);
			action tmp = *it_action_set;
			tmp.add_proposition(*it_prop, m_grounder);
			m_actions.erase(it_action_set);
			m_actions.insert(tmp);
		}
	}
}

void domain::build_initially(bool debug)
{
		
	formula_list::iterator it_fl;

	for (it_fl = m_reader->m_bf_initially.begin(); it_fl != m_reader->m_bf_initially.end(); it_fl++) {
		switch (it_fl->m_formula_type) //initially phi
		{
			//S5 -> pointed world
		case(FLUENT_FORMULA):
			m_intial_description.add_pointed_condition(m_grounder.ground_fluent(it_fl->m_string_fluent_formula));
			if (debug) {
				std::cout << "Added to pointed world condition: ";
				printer::print_list(it_fl->m_string_fluent_formula);
				std::cout << std::endl;
			}

			break;

			//S5 -> Edge
		case(C_FORMULA):
			m_intial_description.add_initial_condition(*it_fl);
			if (debug) {
				std::cout << "Added to initial conditions: ";
				it_fl->print();
				std::cout << std::endl;
			}
			break;

			//Possible S5 -> initally [C(phi2), C(phi2)]
		case(PROPOSITIONAL_FORMULA):
			m_intial_description.add_initial_condition(*it_fl);
			if (debug) {
				std::cout << "Added to initial conditions: ";
				it_fl->print();
				std::cout << std::endl;
			}
			break;

			//No more S5
		case(BELIEF_FORMULA):
			m_intial_description.add_initial_condition(*it_fl);
			if (debug) {
				std::cout << "Added to initial conditions: ";
				it_fl->print();
				std::cout << std::endl;
			}
			break;

			//No more S5
		case(E_FORMULA):
			m_intial_description.add_initial_condition(*it_fl);
			if (debug) {
				std::cout << "Added to initial conditions: ";
				it_fl->print();
				std::cout << std::endl;
			}
			break;

		default:
			std::cerr << "Error in the 'initially' conditions." << std::endl;
			exit(1);
			break;
		}

	}
	/* Deleted because new version of initial state
	if(itn->node_type == CForm && itn->bfnode1->node_type == BForm) //initially C(B(i,phi))
	{
	if(a_map.find(itn->bfnode1->agentPro) == a_map.end())
	{
	cout << "-------ERROR: AGENT NOT DECLARED------- " << endl;
	exit(1);
	}

	Agent ag = a_map.find(itn->bfnode1->agentPro)->second;
	FluentFormula ff = *(convert(itn->bfnode1->bfnode1->flu_form));
	ini.add_correct(ag,ff);
	continue;
	}*/
}


/*std::string domain::get_name();
formula_list domain::get_initial_description();
formula_list domain::get_goal_description();
    
void domain::set_name(std::string domain_name);
//@TODO: setter properly and constructors
void domain::set_initial_description();
void domain::set_goal_description();*/
