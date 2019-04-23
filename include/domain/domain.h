/**
 * \brief Class that stores all the domain information and creates the initial state.
 * 
 * All the information are read from the file given in input and are then processed into 
 * appropriate data structures.
 * 
 * \copyright GNU Public License.
 *
 * \author Francesco Fabiano.
 * \date April 1, 2019
 */
#pragma once
#include <memory>

#include "grounder.h"
#include "initially.h"

#include "../utilities/define.h"
#include "../utilities/reader.h"
#include "../actions/action.h"
//THIS IS BECAUSE IS A TEMPLATIC CLASS AND IT IS A DEPENDECY
#include "../states/state_T.ipp"

class domain
{
private:
    /**
     * \brief The pointer to a \ref reader object.
     * 
     * This is used to read the information from the the input file.
     * The same object is used in the **main** class.
     * 
     * \todo Check if ref or pointer is better std::unique_ptr<reader>
     */
    std::shared_ptr<reader> m_reader;
    /**
     * \brief A \ref grounder object used to store the name of the information.
     * 
     * This is the \ref grounder obj used across the solver to ground and
     * deground the information.*/
    grounder m_grounder;

    /*All the useful info of the domain are store here:
     m_fluents -> all the fluents;
     m_actions -> all the actions 
     m_agents -> all the agents.
     */
    /**
     * \brief Set containing all the (grounded) \ref fluent of the domain. 
     */
    fluent_set m_fluents;
    /**
     * \brief Set containing all the \ref action (with effects, conditions, obsv etc.) of the domain. 
     */
    action_set m_actions;
    /**
     * \brief Set containing all the (grounded) \ref agent of the domain. 
     */
    agent_set m_agents;

    /**
     * \brief The \ref state_type. 
     */
    state_type m_state_type;

    /**
     * \brief The description of the initial state. 
     */
    initially m_intial_description;
    /**
     * \brief The restriction to apply to the goal description.
     */
    domain_restriction m_goal_restriction;
    /**
     * \brief The formula that describes the goal.
     */
    formula_list m_goal_description;

    /** \brief Function that from the file stores the \ref agent information.
     * 
     * @param[in] debug: if true makes the function verbose.*/
    void build_agents(bool debug);
    /** \brief Function that from the file stores the \ref fluent information.
     *
     * @param[in] debug: if true makes the function verbose.*/
    void build_fluents(bool debug);

    /** \brief Function that from the file stores the \ref action information.
     *
     * @param[in] debug: if true makes the function verbose.*/
    void build_actions(bool debug);
    /** \brief Function that adds to the right \ref action each \ref proposition.*/
    void build_propositions();

    /** \brief Function that builds \ref m_intial_description.
     *
     * @param[in] debug: if true makes the function verbose.*/
    void build_initially(bool debug);

    /** \brief Function that builds \ref m_goal_description.
     *
     * @param[in] debug: if true makes the function verbose.
     * \todo move to the goal class.
     */
    void build_goal(bool debug);
    /** \brief Function that checks if \ref belief formula respects \ref m_goal_restriction.
     *
     * @param[in] to_check: the \ref belief formula to check.
     * 
     * @return true: if \p to_check respects \ref m_goal_restriction.
     * @return false: if \p to_check doesn't respect \ref m_goal_restriction.
     * \todo move to the goal class.
     */
    bool check_goal_restriction(const belief_formula & to_check);

public:
    /** \brief Constructor with parameters.
     *
     * @param[in] reader: the \ref reader pointer to assign to \ref m_reader.
     * @param[in] state_repr: the \ref state_type to assign to \ref m_state_type.
     * @param[in] ini_res: the restriction to apply to \ref m_intial_description.
     * @param[in] goal_res: the \ref domain_restriction to assign to \ref m_goal_restriction.*/
    domain(std::shared_ptr<reader> reader, state_type state_repr, domain_restriction ini_res, domain_restriction goal_res);


    /** \brief Function that builds all the domain information.
     *
     * This function calls \ref build_fluents, \ref build_agents and \ref build_actions.
     *
     * @param[in] debug: if true makes the function verbose.*/
    bool build(bool debug);

    /*formula_list get_initial_description();
    formula_list get_goal_description();

    //@TODO: setter properly and constructors
    void set_initial_description();
    void set_goal_description();*/

};
