#ifndef EFP_FORMULAE_H
#define EFP_FORMULAE_H

#include <cstdio>
#include <list>
#include <set>

class belief_formula;

/** \brief The possible types of \ref belief_formula.*/
enum bf_type {
    FLUENT_FORMULA, /**< \brief A \ref belief_formula is also a \ref fluent_formula.
                     *
                     *  This is the base case for the recursion.*/
    BELIEF_FORMULA, /**< \brief A \ref belief_formula of the form B(\ref agent, *phi*).
                     *
                     *  This represent the belief of an \ref agent about a \ref belief_formula *phi*.*/
    PROPOSITIONAL_FORMULA, /**< \brief A \ref belief_formula composed with logical operators and \ref belief_formula(e).
                            *
                            * This uses the operator: \ref BF_NOT, \ref BF_AND, \ref BF_OR
                            *
                            * @see bf_operator.*/
    E_FORMULA, /**< \brief A \ref belief_formula of the form E([set of \ref agent], *phi*).
                     *
                     *  This represent the belief of a set of \ref agent about a \ref belief_formula *phi*.*/
    C_FORMULA, /**< \brief A \ref belief_formula of the form C([set of \ref agent], *phi*).
                     *
                     *  This represent the Common Knowledge of a set of \ref agent of a \ref belief_formula *phi*.*/
    D_FORMULA, /**< \brief A \ref belief_formula of the form D([set of \ref agent], *phi*).
                     *
                     *  This represent the Distributed Knowledge of a set of \ref agent of a \ref belief_formula *phi*.*/
    BF_EMPTY, /**< \brief When the belief formula is empty.*/
    BF_TYPE_FAIL /**< \brief The failure case.*/
};

/** \brief The logical operator for \ref belief_formula(e).
 *
 * These are used in the case that the \ref bf_type of a \ref belief_formula is \ref PROPOSITIONAL_FORMULA.*/
enum bf_operator {
    BF_AND, /**< \brief The AND between \ref belief_formula(e).*/
    BF_OR, /**< \brief The OR between \ref belief_formula(e).*/
    BF_NOT, /**< \brief The NOT of a \ref belief_formula.*/
    BF_INPAREN, /**< \brief When the \ref belief_formula is only surrounded by "()".*/
    BF_FAIL /**< \brief When the \ref belief_formula is not set PROPERLY (shouldn't be accessed if not \ref PROPOSITIONAL_FORMULA).*/
};

typedef std::list<belief_formula> formula_list; /**< \brief A CNF formula of \ref belief_formula.
                                                 *
                                                 * Each element of the formula is a \ref belief_formula.*/

typedef std::set<belief_formula> bformula_set; /**< \brief A set of \ref belief_formula.*/


#endif //EFP_FORMULAE_H
