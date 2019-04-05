/* 
 * File:   domain.cpp
 * Author: Francesco
 *
 * Created on April 1, 2019, 10:41 AM
 */
#include "domain.h"

domain::domain(reader* reader)
{
	m_reader = reader;
}

bool domain::build(bool debug)
{
	int i;

	fluent_map domain_fluent_map;
	agent_map domain_agent_map;
	action_name_map domain_action_name_map;

	std::cout << "\nBuilding agent list..." << std::endl;
	string_list::const_iterator it_agents;
	i = 0;
	for (it_agents = m_reader->m_agents.begin(); it_agents != m_reader->m_agents.end(); it_agents++) {
		domain_agent_map.insert(agent_map::value_type(*it_agents, i));
		m_agents.insert(i++);
		if (debug) {
			std::cout << "Agent " << *it_agents << " is " << (i - 1) << std::endl;
		}
	}

	//build fluent literals;
	std::cout << "\nBuilding fluent literals..." << std::endl;
	string_list::const_iterator it_fluents;
	i = 0;
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

	// build action list literals;
	std::cout << "\nBuilding action list..." << std::endl;
	string_list::const_iterator it_actions_name;
	i = 0;
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

	m_grounder = grounder(domain_fluent_map, domain_agent_map, domain_action_name_map);


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
	if (debug) {
		std::cout << "\nPrinting complete action list..." << std::endl;
		action_set::const_iterator it_actions;
		i = 0;
		for (it_actions = m_actions.begin();
			it_actions != m_actions.end(); it_actions++) {
			it_actions->print(m_grounder);
		}
	}

	return true;
}

/*std::string domain::get_name();
formula_list domain::get_initial_description();
formula_list domain::get_goal_description();
    
void domain::set_name(std::string domain_name);
//@TODO: setter properly and constructors
void domain::set_initial_description();
void domain::set_goal_description();*/
