/**
 * \brief Singleton class that stores all the domain information.
 * 
 * All the information are read from the file given in input and are then processed into 
 * appropriate data structures.
 * 
 * \todo for planner and creates the initial state.
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

    /** \brief The pointer to a \ref reader object.
     * 
     * This is used to read the information from the the input file.
     * The same object is used in the **main** class.
     * 
     * \todo Can be std::unique_ptr<reader>.*/
    std::shared_ptr<reader> m_reader;
    /** \brief A \ref grounder object used to store the name of the information.
     * 
     * This is the \ref grounder obj used across the solver to ground and
     * deground the information.*/
    grounder m_grounder;

    /*All the useful info of the domain are store here:
     m_fluents -> all the fluents;
     m_actions -> all the actions 
     m_agents -> all the agents.
     */
    /** \brief Set containing all the (grounded) \ref fluent of the domain.*/
    fluent_set m_fluents;
    /** \brief Set containing all the \ref action (with effects, conditions, obsv etc.) of the domain.*/
    action_set m_actions;
    /** \brief Set containing all the (grounded) \ref agent of the domain.*/
    agent_set m_agents;

    /** \brief The \ref state_type.*/
    state_type m_state_type;

    /** \brief The description of the initial state.*/
    initially m_intial_description;
    /** \brief The restriction to apply to the goal description.*/
    domain_restriction m_goal_restriction;
    /** \brief The formula that describes the goal.*/
    formula_list m_goal_description;

    /** \brief If the \ref action consider a global visibility frame for their execution.*/
    bool m_is_global_obsv;
    /** \brief The \ref action_check used in *this*.*/
    action_check m_act_check;

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
    /** \brief Function that checks if \ref belief_formula respects \ref m_goal_restriction.
     *
     * @param[in] to_check: the \ref belief_formula to check.
     * 
     * @return true: if \p to_check respects \ref m_goal_restriction.
     * @return false: if \p to_check doesn't respect \ref m_goal_restriction.
     * \todo move to the goal class.
     */
    bool check_goal_restriction(const belief_formula & to_check);

    /** \brief Private constructor since it is a Singleton class.*/
    domain();



public:

    /** \brief To get always (the same instance of) *this* and the same instantiated fields.
     * \warning the \ref set_domain has to called in the main file only.*/
    static domain& get_instance();


    /** \brief Setter for the domains parameters.
     *
     * @param[in] reader: the \ref reader pointer to assign to \ref m_reader.
     * @param[in] state_repr: the \ref state_type to assign to \ref m_state_type.
     * @param[in] ini_res: the restriction to apply to \ref m_intial_description.
     * @param[in] goal_res: the \ref domain_restriction to assign to \ref m_goal_restriction.
     * @param[in] is_global_obsv: the \ref domain_restriction to assign to \ref m_is_global_obsv.
     * @param[in] act_check: the \ref action_check to assign to \ref m_act_check.*/
    void set_domain(std::shared_ptr<reader> reader, state_type state_repr, domain_restriction ini_res, domain_restriction goal_res, bool is_global_obsv, action_check act_check);

    /** \brief Function that builds all the domain information.
     *
     * This function calls \ref build_fluents, \ref build_agents and \ref build_actions.
     *
     * @param[in] debug: if true makes the function verbose.*/
    bool build(bool debug);

    /** \brief Getter of the field \ref m_grounder.
     *
     * @return the ref to \ref m_grounder.*/
    const grounder & get_grounder();
    /** \brief Getter of the field \ref m_fluents.
     *
     * @return the ref to \ref m_fluents.*/
    const fluent_set & get_fluents();
    /** \brief Function that return the number of \ref fluent in the domain.
     *
     * @return the number of \ref fluent.*/
    unsigned int get_fluent_number();
    /** \brief Getter of the field \ref m_actions.
     *
     * @return the ref to \ref m_actions.*/
    const action_set & get_actions();
    /** \brief Getter of the field \ref m_agents.
     *
     * @return the ref to \ref m_agents.*/
    const agent_set & get_agents();
    /** \brief Getter of the field \ref m_state_type.
     *
     * @return the field \ref m_state_type.*/
    state_type get_state_type();
    /** \brief Getter of the field \ref m_is_global_obsv.
     *
     * @return the field \ref m_is_global_obsv.*/
    bool get_is_global_obsv();
    /** \brief Getter of the field \ref m_act_check.
     *
     * @return the field \ref m_act_check.*/
    action_check get_act_check();

    /** \brief Getter of the field \ref m_intial_description.
     *
     * @return the ref to \ref m_intial_description.*/
    const initially & get_initial_description();
    /** \brief Getter of the field \ref m_goal_restriction.
     *
     * @return the field \ref m_goal_restriction.*/
    domain_restriction get_goal_restriction();
    /** \brief Getter of the field \ref m_goal_description.
     *
     * @return the ref to \ref m_goal_description.*/
    const formula_list & get_goal_description();



    /** \brief Copy constructor removed since is Singleton class. */
    domain(domain const&) = delete;
    /** \brief Copy operator removed since Singleton class. */
    void operator=(domain const&) = delete;

    /*formula_list get_initial_description();
    formula_list get_goal_description();

    //@TODO: setter properly and constructors
    void set_initial_description();
    void set_goal_description();*/

};
