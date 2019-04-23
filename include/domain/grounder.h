/**
 * \class grounder
 * \brief Class that grounds all the string of the domain in their numerical id.
 * 
 *  - \ref actions are associated to an \ref action_id;
 *  - agents (string) are associated to a \ref agent;
 *  - fluents (string) are associated to a \ref fluent;
 *
 * \see define, action.
 * 
 * \todo Singleton class?.
 * 
 * \copyright GNU Public License.
 *
 * \author Francesco Fabiano.
 * \date April 5, 2019
 */
#pragma once

#include "../utilities/define.h"
#include "../utilities/printer.h"
#include <string>


//@TODO: Double definition watch out
//typedef unsigned short action_id;

class grounder
{
private:
    /** \brief A map that links each \ref fluent name to its grounded value.*/
    fluent_map m_fluent_map;

    /** \brief A map that links each \ref agent name to its grounded value.*/
    agent_map m_agent_map;

    /** \brief A map that links each \ref action name to its grounded value.*/
    action_name_map m_action_name_map;

    /** \brief A map that links each \ref fluent grounded value to its name (printing value)*/
    reverse_fluent_map r_fluent_map;
    /** \brief A map that links each \ref agent grounded value to its name (printing value)*/
    reverse_agent_map r_agent_map;
    /** \brief A map that links each \ref action grounded value to its name (printing value)*/
    reverse_action_name_map r_action_name_map;

    /** \brief Boolean to check if the \ref r_fluent_map has been created.
     *
     * printing function that is why we don't always create it, only in debug mode.
     */
    bool m_reversed_fl = false;
    /** \brief Boolean to check if the \ref r_agent_map has been created.
     *
     * printing function that is why we don't always create it, only in debug mode.
     */
    bool m_reversed_ag = false;
    /** \brief Boolean to check if the \ref r_action_name_map has been created.
     *
     * printing function that is why we don't always create it, only in debug mode.
     */
    bool m_reversed_ac = false;

    /** \brief Function that reverses the given map.
     *
     * @param[in] given_fluent_map: the \ref fluent_map to reverse.
     */
    void create_reverse_fl(const fluent_map& given_fluent_map);

    /** \brief Function that reverses the given map.
     *
     * @param[in] given_agent_map: the \ref agent_map to reverse.
     */
    void create_reverse_ag(const agent_map& given_agent_map);

    /** \brief Function that reverses the given map.
     *
     * @param[in] given_action_name_map: the \ref action_name_map to reverse.
     */
    void create_reverse_ac(const action_name_map& given_action_name_map);
public:

    /** \brief Constructor without parameters.*/
    grounder();
    /** \brief Constructor with parameters.
     *
     * @param[in] given_fluent_map: the map to copy into \ref m_fluent_map.
     * @param[in] given_agent_map: the map to copy into \ref m_agent_map.
     * @param[in] given_action_name_map: the map to copy into \ref m_action_name_map.
     * 
     * \todo Is parameter passing the best one? Is maybe better to use const reference?
     */
    grounder(fluent_map given_fluent_map, agent_map given_agent_map, action_name_map given_action_name_map);

    /** \brief Setter for the parameter \ref m_fluent_map.
     *
     * @param[in] given_fluent_map: the map to copy into \ref m_fluent_map.
     *
     * \todo Is parameter passing the best one? Is maybe better to use const reference?
     */
    void set_fluent_map(fluent_map given_fluent_map);
    /** \brief Setter for the parameter \ref given_agent_map.
     *
     * @param[in] given_agent_map: the map to copy into \ref m_agent_map.
     * 
     * \todo Is parameter passing the best one? Is maybe better to use const reference?
     */
    void set_agent_map(agent_map given_agent_map);
    /** \brief Setter for the parameter \ref given_action_name_map.
     *
     * @param[in] given_action_name_map: the map to copy into \ref m_action_name_map.
     * 
     * \todo Is parameter passing the best one? Is maybe better to use const reference?
     */
    void set_action_name_map(action_name_map given_action_name_map);

    /** \brief Function that given a \ref fluent name returns its grounded value.
     * 
     * The function reads the field \ref m_fluent_map.
     *
     * @param[in] to_ground: the \ref fluent name to ground.
     * @return: the \p to_ground grounded value.*/
    fluent ground_fluent(const std::string& to_ground) const;
    /** \brief Function that given a \ref fluent set returns its grounded value.
     *
     * This function calls recursively \ref ground_fluent(const std::string&) const.
     * 
     * @param[in] to_ground: the set of \fluent name to ground.
     * @return: \p to_ground grounded.
     * 
     * \todo Is the return type the best one? Is maybe better to use const reference?*/
    fluent_set ground_fluent(const string_set& to_ground) const;
    /** \brief Function that given a \ref string_set_set returns its grounded value.
     *
     * This function calls recursively \ref ground_fluent(const std::string_set&) const.
     * 
     * @param[in] to_ground: the set of set of \fluent name to ground.
     * @return: \p to_ground grounded.
     * 
     * \todo Is the return type the best one? Is maybe better to use const reference?*/
    fluent_formula ground_fluent(const string_set_set& to_ground) const;

    /** \brief Function that given an \ref agent name returns its grounded value.
     * 
     * The function reads the field \ref m_agent_map.
     *
     * @param[in] to_ground: the \ref agent name to ground.
     * @return: the \p to_ground grounded value.*/
    agent ground_agent(const std::string& to_ground) const;
    /** \brief Function that given an \ref agent set returns its grounded value.
     * 
     * This function calls recursively \ref ground_agent(const std::string&) const.
     *
     * @param[in] to_ground: the set of \ref agent name to ground.
     * @return: the \p to_ground grounded value.*/
    agent_set ground_agent(const string_set& to_ground) const;


    /** \brief Function that given an \ref action name returns its grounded value.
     * 
     * The function reads the field \ref m_action_name_map.
     *
     * @param[in] to_ground: the \ref action name to ground.
     * @return: the \p to_ground grounded value.*/
    action_id ground_action(const std::string& to_ground) const;

    /** \brief Function that given a \ref fluent grounded value returns its name. 
     * 
     * The function reads the field \ref r_fluent_map.
     *
     * @param[in] to_deground: the \ref fluent name to deground.
     * @return: the name of \p to_deground.*/
    std::string deground_fluent(fluent to_deground) const;
    /** \brief Function that given a set of \ref fluent grounded value returns the set with their names. 
     * 
     * The function uses \ref deground_fluent(fluent) const.
     *
     * @param[in] to_deground: the set of \ref fluent names to deground.
     * @return: the set with the name of the element of \p to_deground.*/
    string_set deground_fluent(const fluent_set& to_deground) const;
    /** \brief Function that given a grounded \ref fluent_formula returns the same formula in string format. 
     * 
     * The function uses \ref deground_fluent(fluent_set) const.
     *
     * @param[in] to_deground: the \ref fluent_formula to deground.
     * @return: the \ref fluent_formulain string format.*/
    string_set_set deground_fluent(const fluent_formula& to_deground) const;

    /** \brief Function that given an \ref agent grounded value returns its name. 
     * 
     * The function reads the field \ref r_agent_map.
     *
     * @param[in] to_deground: the \ref agent name to deground.
     * @return: the name of \p to_deground.*/
    std::string deground_agent(agent to_deground) const;

    /** \brief Function that given an \ref action grounded value returns its name. 
     * 
     * The function reads the field \ref r_action_name_map.
     *
     * @param[in] to_deground: the \ref action name to deground.
     * @return: the name of \p to_deground.*/
    std::string deground_action(action_id to_deground) const;

    /** \brief Function that print a \ref fluent_set in string format. 
     *
     * @param[in] to_print: the \ref fluent_set to print.*/
    void print_ff(const fluent_set& to_print) const;
    /** \brief Function that print a \ref fluent_formula in string format. 
     *
     * @param[in] to_print: the \ref fluent_formula to print.*/
    void print_ff(const fluent_formula& to_print) const;
};
