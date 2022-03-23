#ifndef EFP_FORMULAE_H
#define EFP_FORMULAE_H

#include <cstdio>
#include <list>
#include <set>

class belief_formula;

typedef std::list<belief_formula> formula_list; /**< \brief A CNF formula of \ref belief_formula.
                                                 *
                                                 * Each element of the formula is a \ref belief_formula.*/

typedef std::set<belief_formula> bformula_set; /**< \brief A set of \ref belief_formula.*/


#endif //EFP_FORMULAE_H
