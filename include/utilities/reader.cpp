/* 
 * File:   reader.cpp
 * Author: Francesco
 *
 * Created on March 31, 2019, 6:41 PM
 */

#include <iostream>

#include <algorithm>    // for set_intersection, set_union
#include <functional>   // for less
#include "reader.h"
#include "printer.h"


//prototype of bison-generated parser function
int yyparse();

string_list reader::name(const fluent_list &x)
{
	string_list y;
	fluent_list::iterator it;

	for (it = x.begin(); it != x.end(); it++) {
		y.insert(name(*it));
	}

	return y;
}

std::string reader::name(fluent x)
{
	unsigned short i = x / 2;
	string_list::iterator it;

	if (i >= m_fluents.size())
		return NULL;

	it = m_fluents.begin();

	for (i = 0; i < x / 2; i++) {
		it++;
	}

	if (x % 2 == 0)
		return *it;

	return(NEGATION_SYMBOL + (*it));
}

int reader::read()
{
	return yyparse();
}

void reader::print() const
{
	
	proposition_list::const_iterator it_porplist;

	std::cout << std::endl;
	std::cout << std::endl;

	std::cout << "AGENT DECLARATION" << std::endl;
	std::cout << "---------------------------" << std::endl;
	/*first = true;
	for (it = m_agents.begin(); it != m_agents.end(); it++) {
		if (!first)
			std::cout << ",";
		first = false;
		std::cout << *it;
	}*/
	printer::print_list(m_agents);

	//std::cout << " DONE PRINTING AGENTS" << std::endl;
	std::cout << std::endl;
	std::cout << std::endl;


	std::cout << "FLUENT DECLARATION" << std::endl;
	std::cout << "----------------------------" << std::endl;
	/*first = true;
	for (it = m_fluents.begin(); it != m_fluents.end(); it++) {
		if (!first)
			std::cout << ",";
		first = false;
		std::cout << *it;
	}*/
	printer::print_list(m_fluents);

	std::cout << std::endl;
	std::cout << std::endl;

	std::cout << "PROPOSITIONS" << std::endl;
	std::cout << "----------------------------" << std::endl;
	for (it_porplist = m_propositions.begin(); it_porplist != m_propositions.end(); it_porplist++) {
		it_porplist->print();
		std::cout << std::endl;
	}

	// print init cstate
	formula_list::const_iterator it_formlist;
	std::cout << "INIT" << std::endl;
	std::cout << "----------------------------" << std::endl;

	for (it_formlist = m_bf_initially.begin(); it_formlist != m_bf_initially.end(); it_formlist++) {
		it_formlist->print();
		std::cout << std::endl;
	}

	std::cout << std::endl;

	// print goal state
	std::cout << "GOAL " << std::endl;
	std::cout << "----------------------------" << std::endl;
	for (it_formlist = m_bf_goal.begin(); it_formlist != m_bf_goal.end(); it_formlist++) {
		it_formlist->print();
		std::cout << std::endl;
	}

	std::cout << std::endl;
	std::cout << std::endl << std::endl;

	// print statistics
	std::cout << "STATISTICS " << std::endl;
	std::cout << "----------------------------" << std::endl;
	//std::cout << "Total actions: " << m_actions.size() << std::endl;
	//std::cout << "\tSensing actions: " << sensing_actions.size() << std::endl;	//Ben
	//std::cout << "\tOntic actions: " << ontic_actions.size() << std::endl;		//Ben
	//std::cout << "\tAnnouncement actions: " << ann_actions.size() << std::endl;	//Ben
	std::cout << "Total fluents: " << m_fluents.size() << std::endl;
	//std::cout << "Unknown fluents: " << std::endl;
	unsigned int i = 0;
	string_list_list::const_iterator it_fll;
	
	for (it_fll = m_initially.begin(); it_fll != m_initially.end(); it_fll++) {
		std::cout << "\tState " << i++ << ": ";
		std::cout << m_fluents.size() - (*it_fll).size();
		std::cout << std::endl;
	}
	//std::cout << "done" << std::endl;
}
