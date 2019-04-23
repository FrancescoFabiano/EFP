/**
 * \brief Class used to store the initial \ref state information.
 * 
 * \see domain.
 * 
 * \copyright GNU Public License.
 *
 * \author Francesco Fabiano.
 * \date April 6, 2019
 */
#pragma once

#include "../utilities/define.h"
#include "../formulae/belief_formula.h"
#include "../formulae/formula_manipulation.h"

class initially
{
private:
    /** \brief Formula containing all the \ref fluent that are required by the possible initial pointed \ref state.*/
    fluent_formula m_pointed_world_conditions;

    /** \brief \ref belief_formula list containing all the \ref belief_formula(e) that describe the initial beliefs conditions.*/
    formula_list m_bf_intial_conditions;

    /** \brief The restriction to apply to the initial state
     * 
     * @see domain_restriction.*/
    domain_restriction m_ini_restriction;

    /** \brief Function that check if a \ref belief_formula respects \ref m_ini_restriction.
     * 
     * @param[in] to_check: the \ref belief_formula that should respect \ref m_ini_restriction.
     * 
     * @return true: if \p to_check respects \ref m_ini_restriction.
     * @return false: if \p to_check doesn't respect \ref m_ini_restriction.*/
    bool check_restriction(const belief_formula & to_check);

public:

    /** \brief  Constructor without parameters.
     *
     * Initialize all the parameters with their default constructors.*/
    initially();

    /** \brief  Constructor with a given restriction.
     *
     * @param[in] ini_restriction: the restriction that *this* should entail.*/
    initially(domain_restriction ini_restriction);

    /** \brief  Function that adds a condition for the initial pointed \ref state.
     *
     * @param[in] to_add: a \ref fluent_formula that the initial pointed \ref state must entail.
     *
     * \todo is parameter passing ok?*/
    void add_pointed_condition(const fluent_formula & to_add);

    /** \brief  Function that adds a condition for the initial \ref state.
     *
     * @param[in] to_add: a \ref belief_formula that the initial \ref state must entail.
     *
     * \todo is parameter passing ok?*/
    void add_initial_condition(const belief_formula & to_add);

    /**
     * \brief getter for the field m_pointed_world_conditions.
     * 
     * @return the field m_pointed_world_conditions.
     * 
     * \todo is return type ok?*/
    const fluent_formula& get_pointed_wordl_conditions();

    /**
     * \brief getter for the field m_bf_intial_conditions.
     * 
     * @return the field m_bf_intial_conditions.
     * 
     * \todo is return type ok?*/
    const formula_list& get_initial_conditions();
};

