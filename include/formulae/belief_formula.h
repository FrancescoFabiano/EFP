/**
 * \class belief_formula
 * \brief Class that implements a Belief Formula.
 *
 * \details A \ref belief_formula can have several form:
 *    - \ref FLUENT_FORMULA -- \ref fluent_formula;
 *    - \ref BELIEF_FORMULA -- B(\ref agent, *phi*);
 *    - \ref PROPOSITIONAL_FORMULA -- \ref BF_NOT(*phi*) or (*phi_1* \ref BF_AND *phi_2*) or (*phi_1* \ref BF_OR *phi_2*);
 *    - \ref E_FORMULA -- E([set of \ref agent], *phi*);
 *    - \ref C_FORMULA -- C([set of \ref agent], *phi*);
 * 
 * 
 * @see reader, domain
 * 
 * \todo Change all the public fields into private and implement getters and setters.
 *
 * \copyright GNU Public License.
 * 
 * \author Francesco Fabiano.
 * \date March 31, 2019
 */
#pragma once

#include <list>
#include <memory>

#include "../utilities/printer.h"
#include "../utilities/define.h"

#include "../domain/grounder.h"

/** 
 * \brief The possible types of \ref belief_formula.
 *
 * \todo remove \ref EMPTY and set faiulure.
 */
enum bf_type
{
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
    EMPTY /**< \brief The default case.
                     * 
                     *  * \todo to remove and set faiulure.*/
};

/** 
 * \brief The logical operator for \ref belief_formula(e).
 *
 * These are used in the case that the \ref bf_type of a \ref belief_formula is \ref PROPOSITIONAL_FORMULA.
 */
enum bf_operator
{
    BF_AND, /**< \brief The AND between \ref belief_formula(e).*/
    BF_OR, /**< \brief The OR between \ref belief_formula(e).*/
    BF_NOT, /**< \brief The NOT of a \ref belief_formula.*/
    BF_NONE /**< \brief When the \ref belief_formula is not \ref PROPOSITIONAL_FORMULA.*/
};

class belief_formula
{
public:
    /**\brief The \ref bf_type of *this*.*/
    bf_type m_formula_type; //Which case of belief formula this is

    /**\brief If *this* is a \ref FLUENT_FORMULA in this field is contained the actual \ref fluent_formula.*/
    fluent_formula m_fluent_formula;
    /**
     * \brief If *this* is a \ref FLUENT_FORMULA in this field is contained the string description of the actual \ref fluent_formula.
     *
     * \todo remove and ground at its creation.*/
    string_set_set m_string_fluent_formula;

    /**
     * \brief If *this* is a \ref BELIEF_FORMULA in this field is contained the \ref agent of the formula.
     *
     * Given the \ref belief_formula B(**ag**, *phi*) this field contains **ag**.
     */
    agent m_agent_op;
    /**
     * \brief If *this* is a \ref BELIEF_FORMULA in this field is contained the string description of the \ref agent of the formula.
     *
     * \todo remove and ground at its creation.*/
    std::string m_string_agent_op;

    /**
     * \brief In this field is contained the first nested \ref belief_formula of *this*.
     *
     * - If *this* is \ref BELIEF_FORMULA, i.e., B(ag, *phi*), \ref m_bf1 contains *phi*;
     * - if *this* is \ref PROPOSITIONAL_FORMULA, i.e. *phi_1* **op** *phi_2*, \ref m_bf1 contains *phi_1*;
     * - if *this* is \ref E_FORMULA, i.e., E(ags, *phi*), \ref m_bf1 contains *phi*;
     * - if *this* is \ref C_FORMULA, i.e., C(ags, *phi*), \ref m_bf1 contains *phi*;
     * - null_ptr otherwise.
     */
    std::shared_ptr<belief_formula> m_bf1;
    /**
     * \brief In this field is contained the second nested \ref belief_formula of *this*.
     *
     * - If *this* is \ref PROPOSITIONAL_FORMULA, i.e. *phi_1* **op** *phi_2*, \ref m_bf2 contains *phi_2*;
     * - null_ptr otherwise.
     */
    std::shared_ptr<belief_formula> m_bf2;
    /**
     * \brief In this field is contained the operator if *this* is \ref PROPOSITIONAL_FORMULA.
     *
     * - Given the \ref PROPOSITIONAL_FORMULA *phi_1* **op** *phi_2* \ref m_operator contains **op**.
     */
    bf_operator m_operator;

    /**
     * \brief If this is a \ref E_FORMULA or a \ref C_FORMULA here is contained the relative set of \ref agent.
     *
     * - If *this* is \ref E_FORMULA, i.e., E(**ags**, *phi*), \ref m_group_agents contains **ags**;
     * - if *this* is \ref C_FORMULA, i.e., C(**ags**, *phi*), \ref m_group_agents contains **ags**.
     */
    agent_set m_group_agents;
    /**
     * \brief If this is a \ref E_FORMULA or a \ref C_FORMULA here is contained the string description of the relative set of \ref agent.
     *
     * \todo remove and ground at its creation.*/
    string_set m_string_group_agents;

    /**
     * \brief Setter for the field \ref m_fluent_formula.
     *
     * @param[in] to_set: the \ref fluent_formula object to copy in \ref m_fluent_formula.
     *
     * \todo Check if is the best type of parameters passing.
     */
    void set_flu(const fluent_formula& to_set);

    //belief_formula();
    //belief_formula(const belief_formula&);
    //belief_formula(belief_formula*);

    /**
     * \brief Function that print *this* (std::string parameters representation).
     *
     * \todo Since the string parameter should be removed print thanks to a \ref grounder.*/
    void print() const;

    /**
     * \brief Function that print *this* where *this* is grounded.
     *
     * @param[in] gr: the \ref grounder obj used to ground the fields of *this*.
     * 
     * \todo Make sure it's grounded at construction and remove the parameter.
     */
    void print_grounded(const grounder& gr) const;

    /**  
     * \brief Function that grounds the fields of *this*.
     * 
     * @param[in] gr: the \ref grounder obj used to ground the fields of *this*.
     * 
     * \todo Call on costruction from outside, maybe a static function in \ref grounder.
     */
    void ground(const grounder & gr);

    /** \brief The equal operator for\ref belief_formula.
     * 
     * @param[in]  to_compare: the \ref belief_formula to compare with *this*.
     * 
     * @return true: if the given \ref belief_formula is syntacticly equal to *this*.
     * @return false: otherwise.*/
    bool operator==(const belief_formula& to_compare) const;

};

typedef std::list<belief_formula> formula_list; /**< \brief A CNF formula of \ref belief_formula.
                                                 *
                                                 * Each element of the formula is a \ref belief_formula.*/