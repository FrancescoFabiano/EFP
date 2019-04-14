/**
 * \brief Used to store the domain information read by the parsing process. 
 * 
 * \details   This class is used during the parsing process to store all the read information,
 *            i.e., the complete description of the domain (as std::string) is found here.
 *            All the domain information stored are yet to ground (\ref grounder).
 * 
 * \warning   The fields of the class are public but should be private and accessed through getters and setters.
 * 
 * \copyright GNU Public License.
 * 
 * \author Francesco Fabiano.
 * \date 3/31/2019
 */

#pragma once

#include "define.h"
#include "../formulae/belief_formula.h"
#include "../formulae/proposition.h"

class reader
{
public:
    /**
     * \brief Name of all the fluents (only positive) in the domain.
     */
    string_set m_fluents;
    /**
     * \brief Name of all the actions in the domain.
     */
    string_set m_actions;
    /**
     * \brief String description of all the agents in the domain.
     */
    string_set m_agents;

    /**
     * \brief String description of all the initial conditions (initially in the domain).
     */
    string_set_set m_initially;

    /**
     * \brief String description of all the goal conditions.
     */
    string_set m_goal;

    /**
     * \brief \ref m_initially conditions described as \ref belief_formula (yet to ground).
     */
    formula_list m_bf_initially;

    /**
     * \brief \ref m_goal conditions described as \ref belief_formula (yet to ground).
     */
    formula_list m_bf_goal;

    /**
     * \brief String description of propositions, each one of these specifies an action conditions.
     */
    proposition_list m_propositions;


    /**
     * \brief Function that reads the info from the domain file.
     * 
     * Function called to parse the file containing the domain
     * and store the information into the fields of the \ref reader class. 
     * @return The same int value returned from the parsing process.
     */
    int read();


    /**
     * \brief Function used to print all the information stored inside the reader object.
     */
    void print() const;


    //std::string name(fluent x);
    //@TODO: Check if is the best type of parameters pass.
    //string_list name(const fluent_list&);
};
