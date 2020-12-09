/**
 * \brief Class that store the info related to a the agents attitude.
 * 
 * This class stores the info of all the agents' attitude.
 * This attitude depends on both the action executors and on the state of the world.
 *  
 * An instance of this class contains the info for each agent.
 * Moreover, given a state and an executor, this class will return all the agents attitudes.
 *
 * \copyright GNU Public License.
 *
 * \author Francesco Fabiano.
 * \date December 9, 2020
 */

#pragma once

#include <vector>

typedef std::map<agent, std::map<agent, std::map<agents_attitudes, formula_list>>> attitudes_map;

template <class T>
class
{
private:

    /**A map that contains the fully_observants attitudes of m_agent when the exe_agent is the executor.
     * 
     * each attitude is associated to a condition. Only one attitude can be true at the time;
     * we have some order in the attitudes and this make us select the first one that is activated.
     * 
     * The order is the following: F_TRUSTY, F_MISTRUSTY, F_UNTRUSTY, F_STUBBORN
     */
    attitudes_map m_F_attitude_wrt_exec;

    /**A map that contains the partially_observants attitudes of m_agent when the exe_agent is the executor.
     * 
     * each attitude is associated to a condition. Only one attitude can be true at the time;
     * we have some order in the attitudes and this make us select the first one that is activated
     * 
     * The order is the following: P_KEEPER, P_INSECURE
     */
    attitudes_map m_P_attitude_wrt_exec;

    /**Function that add the attitude for \ref m_agent w.r.t, \ref executor and an \ref attitude_condition to meet
     * @param[in] m_agent: the agent of whom we want to analyze the attitude.
     * @param[in] executor: the agent with respects with \ref m_agent has \ref attitude when \ref attitude_condition is met.
     * @param[in] attitude: the \ref attitude that the agent will have w.r.t. \ref executor when \ref attitude_condition is met.
     * @param[in] attitude_condition: the condition that the state must require for the agent to have the attitude.
     * @param[in] table: the map where to add the attitudes (fully or partially).
     */
    void add_attitudes(agent m_agent, agent executor, agents_attitudes attitude, const formula_list & attitude_condition, attitudes_map table);


    /**Function that add retrieve the fully_obs attitude for \ref m_agent w.r.t \ref executor and the current state
     *
     * @param[in] m_agent: the agent of whom we want to discover the attitude.
     * @param[in] executor: the agent with respects with \ref m_agent has \ref attitude.
     * @param[in] curr: the state where the attitude's conditions are checked.
     * @param[in] table: the map where to check the attitude_condition (fully or partially).
     * @return: the attitude that the agent has
     */
    agents_attitudes get_attitude(agent m_agent, agent executor, const state<T> & curr, const attitudes_map & table) const;

    /*Function that add retrieve the fully_obs attitude for \ref m_agent w.r.t \ref executor and the current state
     *
     * @param[in] m_agent: the agent of whom we want to discover the attitude.
     * @param[in] executor: the agent with respects with \ref m_agent has \ref attitude.
     * @param[in] curr: the state where the attitude's conditions are checked.
     * @return: the attitude that the agent has
     */
    // agents_attitudes get_F_attitude(agent m_agent, agent executor, const state<T> & curr) const;

    /*Function that add retrieve the partially_obs attitude for \ref m_agent w.r.t \ref executor and the current state
     *
     * @param[in] m_agent: the agent of whom we want to discover the attitude.
     * @param[in] executor: the agent with respects with \ref m_agent has \ref attitude.
     * @param[in] curr: the state where the attitude's conditions are checked.
     * @return: the attitude that the agent has
     */
    //  agents_attitudes get_P_attitude(agent m_agent, agent executor, const state<T> & curr) const;


    /**Function that add retrieve the partially_obs attitude w.r.t \ref executor and the current state
     *
     * @param[in] executor: the agent executing the action.
     * @param[in] curr: the state where the attitude's conditions are checked.
     * @param[in] table: the map where to check the attitude_condition (fully or partially).
     * @return: the attitudes that the agents have
     */
    const std::map<agent, agents_attitudes> & get_attitudes(agent executor, const state<T> & curr, const attitudes_map & table) const;



public:
    /**Class constructor that initializes the maps*/
    attitudes_table();

    /**Function that add the fully_obs attitude for \ref m_agent w.r.t, \ref executor when \ref attitude_condition is met
     * @param[in] m_agent: the agent of whom we want to analyze the attitude.
     * @param[in] executor: the agent with respects with \ref m_agent has \ref attitude when \ref attitude_condition is met.
     * @param[in] attitude: the \ref attitude that the agent will have w.r.t. \ref executor when \ref attitude_condition is met.
     * @param[in] attitude_condition: the condition that the state must require for the agent to have the attitude.
     */
    void add_F_attitudes(agent m_agent, agent executor, agents_attitudes attitude, const formula_list & attitude_condition);

    /**Function that add the partially_obs attitude for \ref m_agent w.r.t, \ref executor when \ref attitude_condition is met
     *
     * @param[in] m_agent: the agent of whom we want to analyze the attitude.
     * @param[in] executor: the agent with respects with \ref m_agent has \ref attitude when \ref attitude_condition is met.
     * @param[in] attitude: the \ref attitude that the agent will have w.r.t. \ref executor when \ref attitude_condition is met.
     * @param[in] attitude_condition: the condition that the state must require for the agent to have the attitude.
     */
    void add_P_attitudes(agent m_agent, agent executor, agents_attitudes attitude, const formula_list & attitude_condition);

    /**Function that add retrieve the fully_obs attitude w.r.t \ref executor and the current state
     *
     * @param[in] executor: the agent executing the action.
     * @param[in] curr: the state where the attitude's conditions are checked.
     * @return: the fully_obs attitudes that the agents have
     */
    const std::map<agent, agents_attitudes> & get_F_attitudes(agent executor, const state<T> & curr) const;

    /**Function that add retrieve the partially_obs attitude w.r.t \ref executor and the current state
     *
     * @param[in] executor: the agent executing the action.
     * @param[in] curr: the state where the attitude's conditions are checked.
     * @return: the partially_obs attitudes that the agents have
     */
    const std::map<agent, agents_attitudes> & get_P_attitudes(agent executor, const state<T> & curr) const;
};