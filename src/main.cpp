/*
 * \brief The main file.
 *
 * \copyright GNU Public License.
 *
 * \author Francesco Fabiano.
 * \date Aprile 1, 2019
 */

#include <iostream>
#include <stdio.h>
#include <string.h>
#include <memory>
#include <vector>



#include "../include/search/planner.ipp"
#include "../include/utilities/reader.h"
#include "../include/domain/domain.h"

#define VERSION "2.0"


/**\todo: Understand this, why out of main?*/
auto domain_reader = std::make_shared<reader>();

void print_usage(char* prog_name)
{
	std::cout << "USAGE:" << std::endl;
	std::cout << prog_name << " input_domain [options]" << std::endl << std::endl;
	std::cout << "OPTIONS:" << std::endl;

	std::cout << "-debug" << std::endl;
	std::cout << "	Makes the solving process verbose." << std::endl;

	std::cout << "-st @state_type" << std::endl;
	std::cout << "	Select the @state_type for the planner." << std::endl;
	std::cout << "	Possible @state_type:" << std::endl;
	std::cout << "		KRIPKE: The States are represented with Kripke structures and the transition function follows the paper definition. (Default)" << std::endl;
	std::cout << "		KRIPKE_OPT: The States are represented with Kripke structures and the transition function is optimized." << std::endl;
	std::cout << "		POSS: The States are represented with Possibilities (NWF-SET)." << std::endl;
	std::cout << "		OBDD: The States are represented with OBDDs." << std::endl;

	std::cout << "-ir @restriction" << std::endl;
	std::cout << "	Set the @restriction that the Initial state must encode." << std::endl;
	std::cout << "	Possible @restriction:" << std::endl;
	std::cout << "		S5: The Initial state must encode an S5 model. (Default)" << std::endl;
	std::cout << "		K45: The Initial state must encode a K45 model." << std::endl;
	std::cout << "		NONE: The Initial state does not have restrictions." << std::endl;

	std::cout << "-gr @restriction" << std::endl;
	std::cout << "	Set the @restriction for the Goal description." << std::endl;
	std::cout << "	Possible @restriction:" << std::endl;
	std::cout << "		NONE: The Goal does not have restrictions. (Default)" << std::endl;
	std::cout << "		NONEG: The Goal does not accept negative belief formula (-B(i,phi))." << std::endl;

	std::cout << "-act_obsv @action_observability" << std::endl;
	std::cout << "	Set the type of @action_observability for the action execution." << std::endl;
	std::cout << "	Possible @action_observability:" << std::endl;
	std::cout << "		GLOBAL: The observability frame is globally defined for each state (Default)." << std::endl;
	std::cout << "		RELATIVE: The observability frame is defined for each world of the state." << std::endl;

	std::cout << "-act_check @action_check" << std::endl;
	std::cout << "	Set the type of @action_check for the action execution." << std::endl;
	std::cout << "	Possible @action_check:" << std::endl;
	std::cout << "		PW: The executability is checked only on the state but the conditonal effects are cheched in every worlds (Default)." << std::endl;
	std::cout << "		PP: Both the executability and the conditonal effects are cheched only on the state." << std::endl;
	std::cout << "		WW: Both the executability and the conditonal effects are cheched in every world." << std::endl;

	/*std::cout << "-ini_build @building_tech" << std::endl;
	std::cout << "	Set the @building_tech that describes how the initial state will be build." << std::endl;*/

	std::cout << "-check_visited" << std::endl;
	std::cout << "	The planner will check for visited states (by default it does not)." << std::endl;

	std::cout << "-h @heuristic" << std::endl;
	std::cout << "	Set the @heuristic to use to perform the search." << std::endl;
	std::cout << "	Possible @heuristic:" << std::endl;
	std::cout << "		L_PG: A planning graph is used to calculate the distance of each state from the goal." << std::endl;
	std::cout << "		S_PG: A planning graph is used to calculate the sum of each sub-goal distance starting from the state." << std::endl;
	std::cout << "		SUBGOALS: We select the state with the highest number of satisfied subgoals." << std::endl;



	std::cout << "-e action1 action2 action3 ..." << std::endl;
	std::cout << "	Perform a sequence of actions and print out" << std::endl;
	std::cout << "	results step by step. The planner does not" << std::endl;
	std::cout << "	search for a plan." << std::endl << std::endl;
	std::cout << "	If the @debug option is on the states are printed as pdf in out/states/@domain_name/." << std::endl << std::endl;


	std::cout << "-results_file" << std::endl;
	std::cout << "	Print the plan time in a file to make the tests confrontations easier." << std::endl << std::endl;


	std::cout << "EXAMPLES:" << std::endl;
	std::cout << prog_name << " ex.al" << std::endl;
	std::cout << "	Find a plan for ex.al. The system will" << std::endl;
	std::cout << "	automatically detect necessary fluents to be considered." << std::endl;
	std::cout << prog_name << " ex.al -e \"move(2,table)\" \"move(1,2)\" -p" << std::endl;
	std::cout << "	Execute the plan [move(2,table);move(1,2)]." << std::endl;
	std::cout << "	The possible world semantics is used." << std::endl << std::endl;

	exit(1);
}

int main(int argc, char** argv)
{

	bool debug = false;
	bool results_file = false;
	bool is_global_obsv = true;
	bool check_visited = false;
	heuristics used_heur = NO_H;
	state_type state_struc = KRIPKE; //default
	domain_restriction ini_restriction = S5; //default
	domain_restriction goal_restriction = NONE; //default
	action_check act_check = EXE_POINTED__COND_WORLD; //default

	bool execute_given_actions = false;
	bool kopt = false;
	std::vector<std::string> given_actions;



	std::cout << "EFP version " << VERSION <<
		" - Built date: " << BUILT_DATE << std::endl;

	if (argc < 2)
		print_usage(argv[0]);

	int i = 2;
	while (i < argc) {
		//No case sensitivity
		if (strcmp(argv[i], "-debug") == 0) {
			std::cout << "Debug is on." << std::endl;
			debug = true;
		} else if (strcmp(argv[i], "-results_file") == 0) {
			results_file = true;
		}//No case sensitivity
		else if (strcmp(argv[i], "-ir") == 0) {
			i++;
			if (i >= argc) {
				std::cerr << "-ir needs specification (S5, K45, NONE)." << std::endl;
				exit(1);
			} else if (strcmp(argv[i], "S5") == 0) {
				std::cout << "Initial state must be an S5. (Default)" << std::endl;
				ini_restriction = K45; //default
			} else if (strcmp(argv[i], "K45") == 0) {
				std::cout << "Initial state must be a K45." << std::endl;
				ini_restriction = K45;
			} else if (strcmp(argv[i], "NONE") == 0) {
				std::cout << "Initial state does not have restrictions." << std::endl;
				ini_restriction = NONE;
			} else {
				std::cerr << "Wrong specification for '-ir'; use 'S5' or 'K45' or 'NONE'." << std::endl;
				exit(1);
			}
		} else if (strcmp(argv[i], "-gr") == 0) {
			i++;
			if (i >= argc) {
				std::cerr << "-gr needs specification (NONE, NONEG)." << std::endl;
				exit(1);
			} else if (strcmp(argv[i], "NONE") == 0) {
				std::cout << "The Goal does not have restrictions. (Default)" << std::endl;
				goal_restriction = NONE; //default
			} else if (strcmp(argv[i], "NONEG") == 0) {
				std::cout << "The Goal does not accept negative belief formula (-B(i,phi))." << std::endl;
				goal_restriction = NONEG;
			} else {
				std::cerr << "Wrong specification for '-gr'; use 'S5' or 'K45' or 'NONE'." << std::endl;
				exit(1);
			}
		} else if (strcmp(argv[i], "-check_visited") == 0) {

			std::cout << "The planner will check for visited states" << std::endl;
			check_visited = true;

		} else if (strcmp(argv[i], "-st") == 0) {
			i++;
			if (i >= argc) {
				std::cerr << "-st needs specification (KRIPKE, KRIPKE_OPT, POSS, OBDD)." << std::endl;
				exit(1);
			} else if (strcmp(argv[i], "KRIPKE") == 0) {
				std::cout << "The States are represented with Kripke structures. (Default)" << std::endl;
				state_struc = KRIPKE; //default
				kopt = false;
			} else if (strcmp(argv[i], "KRIPKE_OPT") == 0) {
				std::cout << "KRIPKE_OPT: The States are represented with Kripke structures and the transition function is optimized" << std::endl;
				state_struc = KRIPKE;
				kopt = true;
			} else if (strcmp(argv[i], "POSS") == 0) {
				std::cout << "The States are represented with Possibilities (NWF-SET)." << std::endl;
				state_struc = POSSIBILITIES;
			} else if (strcmp(argv[i], "OBDD") == 0) {
				std::cout << "The States are represented with OBDDs." << std::endl;
				state_struc = OBDD;
			} else {
				std::cerr << "Wrong specification for '-st'; use 'KRIPKE' or 'POSS' or 'OBDD'." << std::endl;
				exit(1);
			}
		} else if (strcmp(argv[i], "-act_obsv") == 0) {
			i++;
			if (i >= argc) {
				std::cerr << "-act_obsv needs specification (GLOBAL, RELATIVE)." << std::endl;
				exit(1);
			} else if (strcmp(argv[i], "GLOBAL") == 0) {
				std::cout << "The observability frame is globally defined for each state. (Default)" << std::endl;
				is_global_obsv = true; //default
			} else if (strcmp(argv[i], "RELATIVE") == 0) {
				std::cout << "The observability frame is defined for each world of the state." << std::endl;
				is_global_obsv = false;
			} else {
				std::cerr << "Wrong specification for '-act_obsv'; use 'GLOBAL' or 'RELATIVE'." << std::endl;
				exit(1);
			}
		} else if (strcmp(argv[i], "-act_check") == 0) {
			i++;
			if (i >= argc) {
				std::cerr << "-act_check needs specification (PP, PW, WW)." << std::endl;
				exit(1);
			} else if (strcmp(argv[i], "PP") == 0) {
				std::cout << "Both the executability and the conditonal effects are checked only on the state." << std::endl;
				act_check = EXE_POINTED__COND_POINTED; //default
			} else if (strcmp(argv[i], "PW") == 0) {
				std::cout << "The executability is checked only on the state but the conditonal effects are checked in every worlds. (Default)" << std::endl;
				act_check = EXE_POINTED__COND_WORLD;
			} else if (strcmp(argv[i], "WW") == 0) {
				std::cout << "Both the executability and the conditonal effects are checked in every world." << std::endl;
				act_check = EXE_WORLD__COND_WORLD;
			} else {
				std::cerr << "Wrong specification for '-act_obsv'; use 'PP' or 'PW' or 'WW'." << std::endl;
				exit(1);
			}
		} else if (strcmp(argv[i], "-h") == 0) {
			i++;
			if (i >= argc) {
				std::cerr << "-h needs specification (NONE, PG)." << std::endl;
				exit(1);
			} else if (strcmp(argv[i], "NONE") == 0) {
				std::cout << "Breadth first search. (Default)" << std::endl;
				used_heur = NO_H; //default
			} else if (strcmp(argv[i], "L_PG") == 0) {
				std::cout << "A planning graph is used to calculate the distance of each state from the goal." << std::endl;
				used_heur = L_PG;
			} else if (strcmp(argv[i], "S_PG") == 0) {
				std::cout << "A planning graph is used to calculate the sum of each sub-goal distance starting from the state." << std::endl;
				used_heur = S_PG;
			} else if (strcmp(argv[i], "SUBGOALS") == 0) {
				std::cout << "We select the state with the highest number of satisfied subgoals." << std::endl;
				used_heur = SUBGOALS;
			} else {
				std::cerr << "Wrong specification for '-h'; use 'NONE' or 'L_PG' or 'S_PG' or 'SUBGOALS'." << std::endl;
				exit(1);
			}
		} else if (strcmp(argv[i], "-e") == 0) {
			execute_given_actions = true;

			if (i == argc - 1)
				print_usage(argv[0]);
			i++;

			while (i < argc && argv[i][0] != '-') {
				given_actions.push_back(std::string(argv[i++]));
			}

		} else {
			print_usage(argv[0]);
		}

		i++;
	}
	std::string domain_name = argv[1];
	if (freopen(argv[1], "r", stdin) == NULL) {

		std::cerr << argv[0] << ": File " << domain_name << " cannot be opened.\n";
		exit(1);
	}

	domain_name = domain_name.substr(domain_name.find_last_of("\\/") + 1);
	domain_name = domain_name.substr(0, domain_name.find_last_of("."));

	//timer.start(READ_TIMER);
	domain_reader->read();
	if (debug) {
		domain_reader->print();
	}

	//Domain building
	domain::get_instance().set_domain(domain_name, debug, state_struc, kopt, domain_reader, ini_restriction, goal_restriction, is_global_obsv, act_check, check_visited);
	domain::get_instance().build();


	switch ( state_struc ) {
	case KRIPKE:
	{
		planner< state<kstate> > m_planner;
		if (execute_given_actions) {
			if (results_file) {
				m_planner.execute_given_actions_timed(given_actions);
			} else {
				m_planner.execute_given_actions(given_actions);
			}
			std::cout << "\n\n\n*****THE END*****\n";
		} else {
			if (m_planner.search(results_file, used_heur)) {
				std::cout << "\n\n\n*****THE END*****\n";
			} else {
				std::cout << "\n\n\n*****THE SAD END*****\n";
			}
		}
		break;
	}
	case POSSIBILITIES:
	{
		planner< state<pstate> > m_planner;
		if (execute_given_actions) {
			if (results_file) {
				m_planner.execute_given_actions_timed(given_actions);
			} else {
				m_planner.execute_given_actions(given_actions);
			}
			std::cout << "\n\n\n*****THE END*****\n";
		} else {
			if (m_planner.search(results_file, used_heur)) {
				std::cout << "\n\n\n*****THE END*****\n";
			} else {
				std::cout << "\n\n\n*****THE SAD END*****\n";
			}
		}
		break;
	}
	case OBDD:
	default:
		std::cerr << "\nNot implemented yet\n";
		exit(1);
	}

	//timer.end(READ_TIMER);
	//planner.main();

	exit(0);
}