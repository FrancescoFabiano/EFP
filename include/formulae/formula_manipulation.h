/**
 * \brief Class used to check properties of formulae and modify them.
 * 
 *  The class implements static methods to facilitate
 *  the modification of the formulae.
 *
 * \see fluent_formula, belief_formula.
 * 
 * \todo public fields into private and getter and setter.
 * 
 * \copyright GNU Public License.
 *
 * \author Francesco Fabiano.
 * \date April 7, 2019
 */
#pragma once

#include "belief_formula.h"

#include "../utilities/define.h"

class formula_manipulation
{
private:

    /** \brief Function that checks if two \ref fluent_set are consistent.
     * 
     * Two \ref fluent_set are consistent if they not contains a \ref fluent and its negation together.*/
    static bool is_consistent(const fluent_set &, const fluent_set &);

    /** \brief Function that returns the negation of a given \ref fluent.
     * 
     * @param[in] to_negate: the \ref fluent to negate
     * 
     * @return the negation of \p to_negate.*/
    static fluent negate_fluent(const fluent to_negate);

    /* Set has == operator
     * \brief Function that checks if two \ref fluent_set are the same.
     * 
     * @param[in]  to_check_1: the first \ref fluent to check.
     * @param[in]  to_check_2: the second \ref fluent to check.
     * 
     * @return true: if each \ref fluent in \p to_check_1 exists in \p to_check_2 and vice-versa.
     * @return false: otherwise.*/
    /*  static bool is_the_same_ff(const fluent_set& to_check_1, const fluent_set& to_check_2);*/
    /* \brief Function that checks if two \ref fluent_formula are syntacticly the same.
     * 
     * This function uses recursively \ref is_the_same_ff(const fluent_set&, const fluent_set&).
     * 
     * @param[in]  to_check_1: the first \ref fluent_formula to check.
     * @param[in]  to_check_2: the second \ref fluent_formula to check.
     * 
     * @return true: if each \ref fluent_set in \p to_check_1 exists in \p to_check_2 and vice-versa.
     * @return false: otherwise.*/
    /* static bool is_the_same_ff(const fluent_formula& to_check_1, const fluent_formula& to_check_2);*/

public:

    /** \brief Function that merges two conjunctive set of \ref fluent into one.
     *   
     * @param[in]  to_merge_1: the first conjunctive set of \ref fluent to merge.
     * @param[in]  to_merge_2: the second conjunctive set of \ref fluent to merge.
     * 
     * @return the union of all the \ref fluent in \p to_merge_1\2 if is consistent (exit otherwise).*/
    static fluent_set and_ff(const fluent_set& to_merge_1, const fluent_set& to_merge_2);

    /** \brief Function that merges two \ref fluent_formula into one.
     * 
     * @param[in]  to_merge_1: the first \ref fluent_formula to merge.
     * @param[in]  to_merge_2: the second \ref fluent_formula to merge.
     * 
     * @return the union of all the \ref fluent_set in \p to_merge_1\2 if is consistent (exit otherwise).*/
    static fluent_formula and_ff(const fluent_formula& to_merge_1, const fluent_formula& to_merge_2);

    /** \brief Function that checks if two \ref belief_formula are of the form B(i, *phi*) -- B(i, -*phi*) where *phi* is a \ref fluent_formula.
     * 
     * This function is useful to identify when an agent \p i knows the true value of *phi*. 
     * Is one of the accepted formulae in \ref S5.
     * 
     * @param[in]  to_check_1: the first \ref belief_formula to check.
     * @param[in]  to_check_2: the second \ref belief_formula to check.
     * @param[out] ret: the \ref fluent_formula containing *phi*.
     * 
     * @return true: if the two \ref belief_formula contain \ref fluent_formula that are the negation of each other.
     * @return false: otherwise.*/
    static bool check_Bff_Bnotff(const belief_formula& to_check_1, const belief_formula& to_check_2, std::shared_ptr<fluent_formula> ret);
};



