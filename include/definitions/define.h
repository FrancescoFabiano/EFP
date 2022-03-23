/**
 * \class define
 * \brief Class containing most of the custom types and symbols defined for the planner.
 *
 *
 * \copyright GNU Public License.
 *
 * \author Francesco Fabiano, Alessandro Burigana.
 * \date March 31, 2019
 */

#pragma once

#include <cstdio>
#include <string>
#include <set>

#define NEGATION_SYMBOL "-" /**< \brief The negation symbol to negate the \ref fluent */

typedef std::set<std::string> string_set; /**< \brief A representation of a conjunctive set of \ref fluent.
                                            *
                                            * Each element of the formula is a std::string (not grounded) */
typedef std::set<string_set> string_set_set; /**< \brief A representation of a formula in DNF formula.
                                              *
                                              * Each element of the formula is a \ref string_set (not grounded) */
