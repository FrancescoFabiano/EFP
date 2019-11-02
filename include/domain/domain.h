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
#include "../heuristics/planning_graph.h"

class domain
{
private:

    /**\brief The given to the *this*. (The name of the file that contains the description of *this*)*/
    std::string m_name;

    /**\brief If this parameter is setted to true makes the functions verbose.*/
    bool m_debug;
    /**\brief The selected type of state. */
    state_type m_stype;
    /**\brief If this parameter is setted to true then we use the optimized version for Kripke states.*/
    bool m_kopt;
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

    /** \brief Function that from the file stores the \ref agent information.*/
    void build_agents();
    /** \brief Function that from the file stores the \ref fluent information.*/
    void build_fluents();

    /** \brief Function that from the file stores the \ref action information.*/
    void build_actions();
    /** \brief Function that adds to the right \ref action each \ref proposition.*/
    void build_propositions();

    /** \brief Function that builds \ref m_intial_description.*/
    void build_initially();

    /** \brief Function that builds \ref m_goal_description.
     * \todo move to the goal class.*/
    void build_goal();
    /** \brief Function that checks if \ref belief_formula respects \ref m_goal_restriction.
     *
     * @param[in] to_check: the \ref belief_formula to check.
     * 
     * @return true: if \p to_check respects \ref m_goal_restriction.
     * @return false: if \p to_check doesn't respect \ref m_goal_restriction.
     * \todo move to the goal class.*/
    bool check_goal_restriction(const belief_formula & to_check);

    /** \brief Private constructor since it is a Singleton class.*/
    domain();



public:

    /** \brief To get always (the same instance of) *this* and the same instantiated fields.
     * \warning the \ref set_domain has to called in the main file only.*/
    static domain& get_instance();


    /** \brief Setter for the domains parameters.
     *
     * @param[in] name: the value to assign to \ref m_name.
     * @param[in] debug: the value to assign to \ref m_debug.
     * @param[in] stype: the value to assign to \ref m_stype.
     * @param[in] k_opt: the value to assign to \ref m_kopt.
     * @param[in] reader: the \ref reader pointer to assign to \ref m_reader.
     * @param[in] ini_res: the restriction to apply to \ref m_intial_description.
     * @param[in] goal_res: the \ref domain_restriction to assign to \ref m_goal_restriction.
     * @param[in] is_global_obsv: the \ref domain_restriction to assign to \ref m_is_global_obsv.
     * @param[in] act_check: the \ref action_check to assign to \ref m_act_check.*/
    void set_domain(std::string name, bool debug, state_type stype, bool k_opt, std::shared_ptr<reader> reader, domain_restriction ini_res, domain_restriction goal_res, bool is_global_obsv, action_check act_check);

    /** \brief Function that builds all the domain information.
     *
     * This function calls \ref build_fluents, \ref build_agents and \ref build_actions.*/
    void build();
    /** \brief Returns the selected type of state.
     *
     * @return the boolean \ref m_stype.*/
    const state_type & get_stype();
    /** \brief Returns whether the planner uses the optimized version of the transition function for Kripke states.
     *
     * @return the boolean \ref m_kopt.*/
    const bool & get_k_optimized();

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
    /** \brief Getter of the field \ref m_is_global_obsv.
     *
     * @return the field \ref m_is_global_obsv.*/
    bool get_is_global_obsv();

    /** \brief Getter of the field \ref m_debug.
     *
     * @return the field \ref m_debug.*/
    bool get_debug();
    /** \brief Getter of the field \ref m_name.
     *
     * @return the field \ref m_name.*/
    std::string get_name();
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
