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
 * \author Francesco Fabiano, Alessandro Burigana.
 * \date April 1, 2019
 */
#pragma once
#include <memory>

#include "domain_config.h"
#include "grounder.h"
#include "initially.h"

#include "../utilities/define.h"
#include "../utilities/reader.h"
#include "../actions/action.h"
#include "../actions/custom_event_models/cem_store.h"
#include <boost/shared_ptr.hpp>
#include <boost/make_shared.hpp>

class domain
{
private:
    /** \brief Private constructor since it is a Singleton class.*/
    domain();

    /*All the useful info of the domain are store here:
     m_fluents -> all the fluents;
     m_actions -> all the actions 
     m_agents -> all the agents.*/
    /** \brief Set containing all the (grounded) \ref fluent of the domain.*/
    fluent_set m_fluents;
    /** \brief Set containing all the \ref action (with effects, conditions, obsv etc.) of the domain.*/
    action_set m_actions;
    /** \brief Set containing all the (grounded) \ref agent of the domain.*/
    agent_set m_agents;


    domain_config config;
    grounder domain_grounder;

    /** \brief The description of the initial state.*/
    initially m_intial_description;
    /** \brief The formula that describes the goal.*/
    formula_list m_goal_description;


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

public:

    /** \brief To get always (the same instance of) *this* and the same instantiated fields.
     * \warning the \ref set_domain has to called in the main file only.*/
    static domain& get_instance();

    /** \brief Copy constructor removed since is Singleton class. */
    domain(domain const&) = delete;
    /** \brief Copy operator removed since Singleton class. */
    void operator=(domain const&) = delete;

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
     * @param[in] act_check: the \ref action_check to assign to \ref m_act_check.
     * @param[in] check_visited: If the \ref search process should check for already visited_states.
     * @param[in] bisimulation: If the \ref states are reduced in size with bisimulation.*/
    void set_domain(const domain_config & to_set_config); // std::string name, bool debug, state_type stype, bool k_opt, boost::shared_ptr<reader> reader, domain_restriction ini_res, domain_restriction goal_res, bool is_global_obsv, action_check act_check, bool check_visited, bis_type bisimulation);

    /** \brief Function that builds all the domain information.
     *
     * This function calls \ref build_fluents, \ref build_agents and \ref build_actions.*/
    void build();
    /** \brief Returns the selected type of state.
     *
     * @return the boolean \ref m_stype.*/
    state_type get_stype() const;
    /** \brief Returns whether the planner uses the optimized version of the transition function for Kripke states.
     *
     * @return the boolean \ref m_kopt.*/
    bool get_k_optimized() const;

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

    unsigned int get_size_fluent();

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
    bool is_debug();
    /** \brief Getter of the field \ref m_check_visited.
     *
     * @return the field \ref m_check_visited.*/
    bool check_visited();

    /** \brief Getter of the field \ref m_name.
     *
     * @return the field \ref m_name.*/
    std::string get_name();
    /** \brief Getter of the field \ref m_act_check.
     *
     * @return the field \ref m_act_check.*/
    action_check get_act_check();
    /** \brief Getter of the field \ref m_bisimulation.
     *
     * @return the field \ref m_bisimulation.*/
    bis_type get_bisimulation();


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





    /*formula_list get_initial_description();
    formula_list get_goal_description();

    //@TODO: setter properly and constructors
    void set_initial_description();
    void set_goal_description();*/

};
