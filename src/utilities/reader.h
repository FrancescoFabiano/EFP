/**
 * \brief Class used to store the domain information read by the parsing process. 
 * 
 * \details   This class is used during the parsing process to store all the read information,
 *            i.e., the complete description of the domain (as std::string) is found here.
 *            All the domain information stored are yet to ground (\ref grounder).
 * 
 * \todo   The fields of the class are public but should be private and accessed through getters and setters.
 * 
 * \todo   Maybe remove the string description after this, make them not accessible and give outside only the grounded ones and check that grounder is called only here and in printer.
 * 
 * \bug the () extra causes weird errors
 * \bug the - before B is not accepted
 * \bug the - before fluent_formula is not accepted
 * 
 * \copyright GNU Public License.
 * 
 * \author Francesco Fabiano, Alessandro Burigana.
 * \date March 31, 2019
 */

#pragma once

#include "../../include/definitions/define.h"
#include "../formulae/belief_formula.h"
#include "../actions/proposition.h"
#include "../actions/custom_event_models/cem_store.h"
#include "../parse/cem_parser.h"

class reader {
public:
    reader();

    reader(reader const&) = delete;
    reader(reader const&&) = delete;

    reader& operator=(reader const&) = delete;
    reader& operator=(reader const&&) = delete;

    /** \brief Name of all the fluents (only positive) in the domain.*/
    string_set m_fluents;

    /** \brief Name of all the actions in the domain.*/
    string_set m_actions;
    /** \brief String description of all the agents in the domain.*/
    string_set m_agents;

    /** \brief String description of all the initial conditions (initially in the domain).*/
    string_set_set m_initially;

    /** \brief String description of all the goal conditions.*/
    string_set m_goal;

    /** \brief \ref m_initially conditions described as \ref belief_formula (yet to ground).*/
    formula_list m_bf_initially;

    /** \brief \ref m_goal conditions described as \ref belief_formula (yet to ground).*/
    formula_list m_bf_goal;

    /** \brief String description of propositions, each one of these specifies an action conditions (yet to ground).*/
    proposition_list m_propositions;

    const string_set &get_fluents() const;

    const string_set &get_actions() const;

    const string_set &get_agents() const;

    const formula_list &get_bf_initially() const;

    const formula_list &get_bf_goal() const;

    const proposition_list &get_propositions() const;

    void set_fluents(const string_set &fluents);

    void set_actions(const string_set &actions);

    void set_agents(const string_set &agents);

    void set_bf_initially(const formula_list &bf_initially);

    void set_bf_goal(const formula_list &bf_goal);

    void set_propositions(const proposition_list &propositions);

    int read_mar();
    /** \brief Function that reads the info from the domain file.
     * 
     * Function called to parse the file containing the domain
     * and store the information into the fields of the \ref reader class. 
     * @param[in] cem_filename: The name of the file that contains the specification of the update models.
     * @return The same int value returned from the parsing process.*/
    int read_cem();

    void generate(const std::string & file);

    /** \brief Function used to print all the information stored inside the reader object.*/
    void print() const;
};
