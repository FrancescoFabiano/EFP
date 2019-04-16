/**
 * \brief Class with static functions used to facilitate the printing.
 *
 * \details   Class used to print \ref string_set and \ref string_set_set.
 *            Only std::string are printed because are more informative than their grounded counterpart,
 * e.g., if we want to print a set of fluents (that maybe represents a possible world) we print out its std::string
 * description and not the int one (that is the grounded one) @see grounder
 *
 *
 * \copyright GNU Public License.
 *
 * \author Francesco Fabiano.
 * \date April 7, 2019
 */

#pragma once
#include "define.h"

class printer
{
public:
    /**
     * \brief Function used to print all the std::string inside \p to_print.
     *
     * A \ref string_set represents a conjunctive set of \ref fluent format so the printing is executed accordingly.
     *
     * @param[in] to_print: the \ref string_set to be printed.
     *
     * \warning   Is the const reference the best type of parameter passing?
     */
    static void print_list(const string_set& to_print);

    /**
     * \brief Function used to print all the std::string inside \p to_print.
     *
     * A \ref string_set_set represents a formula in DNF format so the printing is executed accordingly.
     *
     * @param[in] to_print: the \ref string_set_set to be printed.
     *
     *  \warning   Is the const reference the best type of parameter passing?
     */
    static void print_list(const string_set_set& to_print);

};
