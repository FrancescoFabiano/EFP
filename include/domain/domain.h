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

public:

    /** \brief To get always (the same instance of) *this* and the same instantiated fields.
     * \warning the \ref set_config has to called in the main file only.*/
    static domain& get_instance();

    /** \brief Copy constructor removed since is Singleton class. */
    domain(domain const&) = delete;
    /** \brief Copy operator removed since Singleton class. */
    void operator=(domain const&) = delete;

    const domain_config & get_config() const;

    void set_config(const domain_config & to_set_config);

    /** \brief Function that builds all the domain information.
     *
     * This function calls \ref build_fluents, \ref build_agents and \ref build_actions.*/
    void build();

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

    /** \brief Getter of the field \ref m_intial_description.
     *
     * @return the ref to \ref m_intial_description.*/
    const initially & get_initial_description();
    /** \brief Getter of the field \ref m_goal_description.
     *
     * @return the ref to \ref m_goal_description.*/
    const formula_list & get_goal_description();
};
