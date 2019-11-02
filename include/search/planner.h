/**
 * \class planner
 * 
 * \brief Templatic class that implements the searching technique (BFS).
 *
 * The class is templatic because it searches with different state representation.
 *
 * \todo Templatic with priority queue?.
 *
 * \copyright GNU Public License.
 *
 * \author Francesco Fabiano.
 * \date May 6, 2019
 */
#pragma once
#include <queue>
#include <stdlib.h>
#include <chrono>
#include <ctime>

//THIS IS BECAUSE IS A TEMPLATIC CLASS AND IT IS A DEPENDECY
#include "../states/state_T.ipp"
#include "../heuristics/planning_graph.ipp"

/** @tparam T:
 *    - \ref KRIPKE
 *    - \ref POSSIBILITIES
 *    - \ref OBDD*/
template <class T>
class planner
{
private:
    /**The queue that contains all the states<T> yet to be visited.*/
    std::queue< T > m_search_space;

    /* \brief The \ref state_type.*/
    //state_type m_state_type;
public:
    /*Constructor that from the initialized domain creates the initial state and place it inside \ref m_search_space
     *
     * @param[in] given_state_type: the type of state representation to set as m_state_type.*/
    // planner(state_type given_state_type);


    /**Function that searches on m_search_space using BFS on all the actions.
     * 
     * @param[in] old_check: if true print the plan time in a file to easy the confrontation with the old version.
     * @return true if a plan is found.
     * @return false otherwise.*/
    bool search_BFS(bool old_check);

    /**Function print out the solution time.
     * 
     * @param[in] elapsed_seconds: the time of the search.
     * @param[in] goal: the goal state.
     * @param[in] old_check: if true print the plan time in a file to easy the confrontation with the old version.
     * @param[in] givenplan: if true changes the name of the output folder for the comparison time.*/
    void print_results(std::chrono::duration<double> elapsed_seconds, T goal, bool old_check, bool givenplan);


    /**Function that searches on m_search_space using the given actions.
     *@param[in] act_name: the names of the \ref action to execute (ordered).*/
    void execute_given_actions(const std::vector<std::string> act_name);

    /**Function that searches on m_search_space using the given actions and print out the execution time.
     * 
     * Every useless I\O step is removed for time accuracy
     * 
     *@param[in] act_name: the names of the \ref action to execute (ordered).*/
    void execute_given_actions_timed(const std::vector<std::string> act_name);
};