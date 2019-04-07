/* 
 * File:   main.cpp
 * Author: Francesco
 *
 * Created on April 1, 2019, 1:18 PM
 */

#include <iostream>
#include <stdio.h>
#include <string.h>
#include "../include/utilities/reader.h"
#include "../include/utilities/domain.h"

#define VERSION "2.0"

//@TODO: Understand this, why out of main?
reader domain_reader;


void print_usage(char* prog_name)
{
	std::cout << "USAGE:" << std::endl;
	std::cout << "  " << prog_name << " input_domain [options]" << std::endl << std::endl;
	std::cout << "OPTIONS:" << std::endl;
	std::cout << "  -ir @restriction" << std::endl;
	std::cout << "     Set the @restriction that the initial state must encode." << std::endl;
	std::cout << "     Possible @restriction:" << std::endl;
	std::cout << "	     S5: The initial state must encode an S5 model (Default)." << std::endl;
	std::cout << "	     K45: The initial state must encode a K45 model." << std::endl;
	std::cout << "	     NONE: The initial state does not have restrictions." << std::endl;
	std::cout << " -spg" << std::endl;
	std::cout << "     Find the plan using the sum planning graph heuristic." << std::endl;
	std::cout << " -mpg" << std::endl;
	std::cout << "     Find the plan using the max planning graph heuristic." << std::endl;
	std::cout << "  -e action1 action2 action3 ..." << std::endl;
	std::cout << "     Perform a sequence of actions and print out" << std::endl;
	std::cout << "     results step by step. The planner does not" << std::endl;
	std::cout << "     search for a plan." << std::endl << std::endl;
	std::cout << "EXAMPLES:" << std::endl;
	std::cout << "  " << prog_name << " ex.al" << std::endl;
	std::cout << "     Find a conformant plan for ex.al. The system will" << std::endl;
	std::cout << "     automatically detect necessary fluents to be considered." << std::endl;
	std::cout << "  " << prog_name << " ex.al -e \"move(2,table)\" \"move(1,2)\" -p" << std::endl;
	std::cout << "     Execute the plan [move(2,table);move(1,2)]." << std::endl;
	std::cout << "     The possible world semantics is used." << std::endl << std::endl;

	exit(1);
}

int main(int argc, char** argv)
{	
	bool debug = false;
	domain_restriction ini_restriction = S5; //default
		
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
		}
		//No case sensitivity
		if (strcmp(argv[i], "-ir") == 0) {
			i++;
			if (i >= argc)
			{
				std::cerr << "-ir needs specification (S5, K45, NONE)." << std::endl;
				exit(1);
			}
			else if (strcmp(argv[i], "S5") == 0) {
				std::cout << "Initial state must be an S5." << std::endl;
				ini_restriction = K45; //default
			}
			else if (strcmp(argv[i], "K45") == 0) {
				std::cout << "Initial state must be a K45." << std::endl;
				ini_restriction = K45;
			}
			else if(strcmp(argv[i], "NONE") == 0) {
				std::cout << "Initial state does not have restrictions." << std::endl;
				ini_restriction = NONE;
			}
			else
			{
				std::cerr << "Wrong specification (S5, K45, NONE)." << std::endl;
				exit(1);
			}
		}
		/* else if (strcmp(argv[i], "-mpg") == 0) {
			std::cout << " search with max planning graph heuristic " << std::endl;
			planner.m_heuristic = PLANNINGGRAPH;
			planner.useHeuristic = true;
		} else if (strcmp(argv[i], "-spg") == 0) {
			std::cout << " search with sum planning graph heuristic " << std::endl;
			planner.m_heuristic = SUMPLANNINGGRAPH;
			planner.useHeuristic = true;
		} else if (strcmp(argv[i], "-e") == 0) {
			planner.m_task = DOPLAN;

			if (i == argc - 1)
				print_usage(argv[0]);
			i++;

			while (i < argc && argv[i][0] != '-') {
				planner.m_plan.push_back(string(argv[i++]));
			}
		} else
			print_usage(argv[0]);*/
		i++;
	}
	if (freopen(argv[1], "r", stdin) == NULL) {
		std::cerr << argv[0] << ": File " << argv[1] << " cannot be opened.\n";
		exit(1);
	}
	
	domain domain(&domain_reader);

	//timer.start(READ_TIMER);
	domain_reader.read();
	if (debug){
		domain_reader.print();
	}
	if(!domain.build(debug, ini_restriction))
	{
		std::cerr << "Error in building the domain.";
		exit(1);
	}
	//timer.end(READ_TIMER);
	//planner.main();

	exit(0);
}