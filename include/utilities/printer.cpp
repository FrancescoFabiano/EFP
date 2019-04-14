/*
 * \brief Implementation of \ref printer.h
 * \copyright GNU Public License.
 *
 * \author Francesco Fabiano.
 * \date 4/7/2019
 */

#include <iostream>
#include "printer.h"

void printer::print_list(const string_set& to_print)
{
	bool first_print = true;

	//Iterate on the \p to_print and print its content separated by a comma to represent a CNF.
	string_set::const_iterator it_sl;
	for (it_sl = to_print.begin(); it_sl != to_print.end(); it_sl++) {
		if (!first_print) {
			std::cout << ",";
		}
		first_print = false;
		std::cout << *it_sl;
	}
}

void printer::print_list(const string_set_set& to_print)
{
	bool first_print = true;
	string_set_set::const_iterator it_sll;
	//Iterate on the \p to_print and print its content using ::to_print(const string_set&) seprated by a OR to represent a DNF.

	for (it_sll = to_print.begin(); it_sll != to_print.end(); it_sll++) {
		if (!first_print) {
			std::cout << "OR";
		}
		first_print = false;
		print_list(*it_sll);

	}
}

