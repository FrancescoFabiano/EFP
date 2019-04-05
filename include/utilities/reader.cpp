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
	string_list::const_iterator it;
	string_list_list::const_iterator it2;
	proposition_list::const_iterator it3;
	bool first;
	unsigned int i;

	std::cout << std::endl;
	std::cout << std::endl;

	std::cout << "AGENT DECLARATION" << std::endl;
	std::cout << "---------------------------" << std::endl;
	first = true;
	for (it = m_agents.begin(); it != m_agents.end(); it++) {
		if (!first)
			std::cout << ",";
		first = false;
		std::cout << *it;
	}
	//std::cout << " DONE PRINTING AGENTS" << std::endl;
	std::cout << std::endl;
	std::cout << std::endl;


	std::cout << "FLUENT DECLARATION" << std::endl;
	std::cout << "----------------------------" << std::endl;
	first = true;
	for (it = m_fluents.begin(); it != m_fluents.end(); it++) {
		if (!first)
			std::cout << ",";
		first = false;
		std::cout << *it;
	}

	std::cout << std::endl;
	std::cout << std::endl;

	std::cout << "PROPOSITIONS" << std::endl;
	std::cout << "----------------------------" << std::endl;
	for (it3 = m_propositions.begin(); it3 != m_propositions.end(); it3++) {
		it3->print();
		std::cout << std::endl;
	}

	// print init cstate
	formula_list::const_iterator it5;
	std::cout << "INIT" << std::endl;
	std::cout << "----------------------------" << std::endl;

	for (it5 = m_bf_initially.begin(); it5 != m_bf_initially.end(); it5++) {
		it5->print();


		std::cout << std::endl;
	}

	std::cout << std::endl;

	// print goal state
	std::cout << "GOAL " << std::endl;
	std::cout << "----------------------------" << std::endl;
	for (it5 = m_bf_goal.begin(); it5 != m_bf_goal.end(); it5++) {
		it5->print();
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
	i = 0;
	for (it2 = m_initially.begin(); it2 != m_initially.end(); it2++) {
		std::cout << "\tState " << i++ << ": ";
		std::cout << m_fluents.size() - (*it2).size();
		std::cout << std::endl;
	}
	//std::cout << "done" << std::endl;
}
