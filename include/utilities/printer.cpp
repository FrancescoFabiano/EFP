
/* 
 * File:   printer.cpp
 * Author: Francesco
 *
 * Created on April 7, 2019, 12:40 PM
 */
#include <iostream>
#include "printer.h"


    void printer::print_list(const string_list& to_print)
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

    void printer::print_list(const string_list_list& to_print)
    {
    	bool first_print = true;
	string_list_list::const_iterator it_sll;

	for (it_sll = to_print.begin(); it_sll != to_print.end(); it_sll++) {
		if (!first_print) {
			std::cout << "AND";
			first_print = false;
		}
		print_list(*it_sll);

	}
    }

