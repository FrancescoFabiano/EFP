/*
 * \brief Implementation of \ref reader.h.
 *
 *
 * \copyright GNU Public License.
 *
 * \author Francesco Fabiano, Alessandro Burigana.
 * \date March 31, 2019
 */

#include <iostream>

#include "reader.h"
#include "printer.h" //Used in \ref print as a support
#include "../actions/custom_event_models/cem_store.h"

//prototype of bison-generated parser function
int marparse();
int cemparse();

/*string_set reader::name(const fluent_set &x)
{
	string_set y;
	fluent_set::iterator it;

	for (it = x.begin(); it != x.end(); it++) {
		y.insert(name(*it));
	}

	return y;
}

std::string reader::name(fluent x)
{
	unsigned short i = x / 2;
	string_set::iterator it;

	if (i >= m_fluents.size())
		return NULL;

	it = m_fluents.begin();

	for (i = 0; i < x / 2; i++) {
		it++;
	}

	if (x % 2 == 0)
		return *it;

	return(NEGATION_SYMBOL + (*it));
}*/

reader::reader() = default;

reader& reader::get_instance() {
    static reader instance;
    return instance;
}

int reader::read()
{
    /*to dynamically generate id of groups and actions type*/
    cem_store::get_instance().add_cem_name("ontic");
    cem_store::get_instance().add_cem_name("sensing");
    cem_store::get_instance().add_cem_name("announcement");

    cem_store::get_instance().add_agent_group("fully");
    cem_store::get_instance().add_agent_group("partially");

    return marparse();
}

int reader::read(const std::string & filename)
{
	//Call to the parser function.
	//Generation of action groups and events (after agents declaration but before actions declaration)
	std::cout << "\nBuilding event models..." << std::endl;
	cem_store::get_instance().generate(filename);
	return cemparse();
}

/**
 This function print out all the information that are stored as std::string in the
 * reader object. @see printer, belief_formula::print() const, \ref proposition::print() const
 */
void reader::print() const
{

	//Printing of all the agent with the \ref kstore::get_instance().print functions
	proposition_list::const_iterator it_porplist;
	std::cout << std::endl << std::endl;
	std::cout << "AGENT DECLARATION" << std::endl;
	std::cout << "---------------------------" << std::endl;
	printer::get_instance().print_list(m_agents);
	std::cout << std::endl << std::endl;


	//Printing of all the fluents with the \ref printer functions
	std::cout << "FLUENT DECLARATION" << std::endl;
	std::cout << "----------------------------" << std::endl;
	printer::get_instance().print_list(m_fluents);
	std::cout << std::endl << std::endl;

	//Printing of all the proposition with the \ref proposition functions
	std::cout << "PROPOSITIONS" << std::endl;
	std::cout << "----------------------------" << std::endl;
	for (it_porplist = m_propositions.begin(); it_porplist != m_propositions.end(); it_porplist++) {
		it_porplist->print();
		std::cout << std::endl;
	}

	//Printing of all the proposition with the \ref belief_formula functions
	formula_list::const_iterator it_formlist;
	std::cout << "INIT" << std::endl;
	std::cout << "----------------------------" << std::endl;

	for (it_formlist = m_bf_initially.begin(); it_formlist != m_bf_initially.end(); it_formlist++) {
		it_formlist->print();
		std::cout << std::endl;
	}

	std::cout << std::endl;

	//Printing of all the goal with the \ref belief_formula functions
	std::cout << "GOAL " << std::endl;
	std::cout << "----------------------------" << std::endl;
	for (it_formlist = m_bf_goal.begin(); it_formlist != m_bf_goal.end(); it_formlist++) {
		it_formlist->print();
		std::cout << std::endl;
	}

	std::cout << std::endl << std::endl << std::endl;

	/*
	 * // print statistics
	std::cout << "STATISTICS " << std::endl;
	std::cout << "----------------------------" << std::endl;
	//std::cout << "Total actions: " << m_actions.size() << std::endl;
	//std::cout << "\tSensing actions: " << sensing_actions.size() << std::endl;	//Ben
	//std::cout << "\tOntic actions: " << ontic_actions.size() << std::endl;		//Ben
	//std::cout << "\tAnnouncement actions: " << ann_actions.size() << std::endl;	//Ben
	std::cout << "Total fluents: " << m_fluents.size() << std::endl;
	//std::cout << "Unknown fluents: " << std::endl;
	unsigned int i = 0;
	string_set_set::const_iterator it_fll;

	for (it_fll = m_initially.begin(); it_fll != m_initially.end(); it_fll++) {
		std::cout << "\tState " << i++ << ": ";
		std::cout << m_fluents.size() - (*it_fll).size();
		std::cout << std::endl;
	}
	//std::cout << "done" << std::endl;*/
}