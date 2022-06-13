/**
 * \brief Class that stores all the domain information.
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
#include <boost/shared_ptr.hpp>
#include <boost/make_shared.hpp>

//#include "../../include/definitions/domain_def.h"
#include "../../include/definitions/actions.h"
#include "../formulae/formula.h"
#include "domain_config.h"
#include "grounder.h"
#include "initially.h"
#include "../utilities/reader.h"
#include "../formulae/finitary_theory/finitary_theory.h"
//#include "../actions/custom_event_models/cem_store.h"

class finitary_theory;

class domain {
private:
    /*All the useful info of the domain are store here:
     m_fluents -> all the fluents;
     m_actions -> all the actions 
     m_agents -> all the agents.*/
    /** \brief Set containing all the (grounded) \ref fluent of the domain.*/
    fluent_set m_fluent_set;
    /** \brief Set containing all the (grounded) \ref agent of the domain.*/
    agent_set m_agent_set;
    /** \brief Set containing all the \ref action (with effects, conditions, obsv etc.) of the domain.*/
    action_set m_action_set;

    fluent_ptr_set m_fluent_ptr_set;
    agent_ptr_set m_agent_ptr_set;

    finitary_theory m_finitary_theory;

    cem_store m_store;

    domain_config m_config;

    grounder m_grounder;

    /** \brief The description of the initial state.*/
    initially m_intial_description;
    /** \brief The formula that describes the goal.*/
    formula_list m_goal_description;


    /** \brief Function that from the file stores the \ref agent information.*/
    void build_agents(const reader &reader);
    /** \brief Function that from the file stores the \ref fluent information.*/
    void build_fluents(const reader &reader);

    /** \brief Function that from the file stores the \ref action information.*/
    void build_actions(const grounder &grounder, reader &reader);


    /** \brief Function that adds to the right \ref action each \ref proposition.*/
    void build_propositions(const grounder &grounder, reader &reader);

    /** \brief Function that builds \ref m_intial_description.*/
    void build_initially(const grounder &grounder, reader &reader, const printer &printer);

    /** \brief Function that builds \ref m_goal_description.
     * \todo move to the goal class.*/
    void build_goal(const grounder &grounder, reader &reader);

public:
    domain(domain_config config, cem_store store, finitary_theory theory);

    domain(domain const&) = delete;
    domain(domain const&&) = delete;

    domain& operator=(domain const&) = delete;
    domain& operator=(domain const&&) = delete;

    const domain_config & get_config() const;

    const cem_store get_store() const;

    void set_config(const domain_config & to_set_config);

    void set_store(const cem_store &store);

    /** \brief Function that builds all the domain information.
     *
     * This function calls \ref build_fluents, \ref build_agents and \ref build_actions.*/
    void build(const grounder &grounder, reader &reader, const printer &printer);

    /** \brief Getter of the field \ref m_grounder.
     *
     * @return the ref to \ref m_grounder.*/
    const grounder & get_grounder() const;
    /** \brief Getter of the field \ref m_fluents.
     *
     * @return the ref to \ref m_fluents.*/
    const fluent_ptr_set & get_fluent_set() const;

    /** \brief Getter of the field \ref m_agents.
     *
     * @return the ref to \ref m_agents.*/
    const agent_ptr_set & get_agent_set() const;

    /** \brief Function that return the number of \ref fluent in the domain.
     *
     * @return the number of \ref fluent.*/
    unsigned int get_fluent_number() const;

    unsigned int get_size_fluent() const;

    /** \brief Getter of the field \ref m_actions.
     *
     * @return the ref to \ref m_actions.*/
    const action_set & get_actions() const;

    const finitary_theory & get_finitary_theory() const;

    /** \brief Getter of the field \ref m_intial_description.
     *
     * @return the ref to \ref m_intial_description.*/
    const initially & get_initial_description() const;
    /** \brief Getter of the field \ref m_goal_description.
     *
     * @return the ref to \ref m_goal_description.*/
    const formula_list & get_goal_description() const;
};
