/* 
 * File:   grounder.h
 * Author: Francesco
 *
 * Created on April 5, 2019, 12:18 PM
 */

#include "grounder.h"

grounder::grounder()
{
}

grounder::grounder(fluent_map fluent_map, agent_map agent_map, action_name_map action_name_map)
{
	set_fluent_map(fluent_map);
	set_agent_map(agent_map);
	set_action_name_map(action_name_map);

	//@TODO:Remove for efficency; just for printing reasons (pass debug maybe)
	reverse();

}

void grounder::reverse()
{
	create_reverse_fl(m_fluent_map);
	create_reverse_ag(m_agent_map);
	create_reverse_ac(m_action_name_map);
}

void grounder::create_reverse_fl(const fluent_map& to_revert)
{
	fluent_map::iterator it;
	for (it = m_fluent_map.begin(); it != m_fluent_map.end(); it++)
		r_fluent_map[it->second] = it->first;
}

void grounder::create_reverse_ag(const agent_map& to_revert)
{
	agent_map::iterator it;
	for (it = m_agent_map.begin(); it != m_agent_map.end(); it++)
		r_agent_map[it->second] = it->first;
}

void grounder::create_reverse_ac(const action_name_map& to_revert)
{
	action_name_map::iterator it;
	for (it = m_action_name_map.begin(); it != m_action_name_map.end(); it++)
		r_action_name_map[it->second] = it->first;
}

void grounder::set_fluent_map(fluent_map fluent_map)
{
	m_fluent_map = fluent_map;
}

void grounder::set_agent_map(agent_map agent_map)
{
	m_agent_map = agent_map;
}

void grounder::set_action_name_map(action_name_map action_name_map)
{
	m_action_name_map = action_name_map;
}

fluent grounder::ground_fluent(const std::string& x) const
{
	fluent_map::const_iterator p = m_fluent_map.find(x);

	if (p != m_fluent_map.end()) {
		return(p->second);
	}

	std::cerr << "ERROR: Fluent " << x << " is undeclared." << std::endl;
	exit(1);
}

fluent_list grounder::ground_fluent(const string_list& x) const
{
	string_list::iterator it;
	fluent_list y;

	for (it = x.begin(); it != x.end(); it++) {
		y.insert(ground_fluent(*it));
	}

	return y;
}

fluent_formula grounder::ground_fluent(const string_list_list& x) const
{
	string_list_list::iterator it;
	fluent_formula y;

	for (it = x.begin(); it != x.end(); it++) {
		//@TODO: Controlla questo passaggio, schifo! Change all the pointers to ref
		y.insert(ground_fluent(*it));
	}

	return y;
}

agent grounder::ground_agent(const std::string& x) const
{
	agent_map::const_iterator p = m_agent_map.find(x);

	if (p != m_agent_map.end()) {
		return(p->second);
	}

	std::cerr << "ERROR: Agent " << x << " is undeclared." << std::endl;
	exit(1);
}

action_id grounder::ground_action(const std::string& x) const
{
	action_name_map::const_iterator p = m_action_name_map.find(x);

	if (p != m_action_name_map.end()) {
		return(p->second);
	}

	std::cerr << "ERROR: Action " << x << " is undeclared." << std::endl;
	exit(1);
}

std::string grounder::deground_fluent(fluent x) const
{
	reverse_fluent_map::const_iterator p = r_fluent_map.find(x);

	if (p != r_fluent_map.end()) {
		return(p->second);
	}

	std::cerr << "ERROR: Fluent " << x << " is undeclared." << std::endl;
	exit(1);
}

string_list grounder::deground_fluent(const fluent_list& x) const
{
	fluent_list::iterator it;
	string_list y;

	for (it = x.begin(); it != x.end(); it++) {
		y.insert(deground_fluent(*it));
	}

	return y;
}

string_list_list grounder::deground_fluent(const fluent_formula& x) const
{
	fluent_formula::iterator it;
	string_list_list y;

	for (it = x.begin(); it != x.end(); it++) {
		//@TODO: Controlla questo passaggio, schifo! Change all the pointers to ref
		y.insert(deground_fluent(*it));
	}

	return y;
}

std::string grounder::deground_agent(agent x) const
{
	reverse_agent_map::const_iterator p = r_agent_map.find(x);

	if (p != r_agent_map.end()) {
		return(p->second);
	}

	std::cerr << "ERROR: Agent " << x << " is undeclared." << std::endl;
	exit(1);
}

std::string grounder::deground_action(action_id x) const
{
	reverse_action_name_map::const_iterator p = r_action_name_map.find(x);

	if (p != r_action_name_map.end()) {
		return(p->second);
	}

	std::cerr << "ERROR: Action " << x << " is undeclared." << std::endl;
	exit(1);
}

void grounder::print_ff(const fluent_list& to_print) const
{
	print_ff(deground_fluent(to_print));
}

void grounder::print_ff(const string_list& to_print) const
{

	bool first_print = true;

	string_list::const_iterator it_sl;
	for (it_sl = to_print.begin(); it_sl != to_print.end(); it_sl++) {
		if (!first_print) {
			std::cout << ",";
			first_print = false;
		}
		std::cout << *it_sl;
	}
}

void grounder::print_ff(const fluent_formula& to_print) const
{
	print_ff(deground_fluent(to_print));
}

void grounder::print_ff(const string_list_list& to_print) const
{

	bool first_print = true;
	string_list_list::const_iterator it_sll;

	for (it_sll = to_print.begin(); it_sll != to_print.end(); it_sll++) {
		if (!first_print) {
			std::cout << "AND";
			first_print = false;
		}
		print_ff(*it_sll);

	}
}