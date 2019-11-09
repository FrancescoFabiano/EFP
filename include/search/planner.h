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

/** \brief Class used as comparator for the priority queue.*/
template <class T>
struct compare_heuristic
{

    bool operator()(const T & state1, const T & state2)
    {
        return state1.get_heuristic_value() > state2.get_heuristic_value();
    }
};

template <class T>
class planner
{
private:
    /**The queue that contains all the states<T> yet to be visited.*/
    std::queue< T > m_search_space;
    std::priority_queue<T, std::vector<T>, compare_heuristic<T> > m_heur_search_space;

    /**Function that searches on m_search_space using BFS on all the actions.
     * 
     * @param[in] old_check: if true print the plan time in a file to easy the confrontation with the old version.
     * @return true if a plan is found.
     * @return false otherwise.*/
    bool search_BFS(bool old_check);


    /**Function that searches on m_search_space using Best First Search.
     * 
     * @param[in] old_check: if true print the plan time in a file to easy the confrontation with the old version.
     * @return true if a plan is found.
     * @return false otherwise.*/
    bool search_heur(bool old_check);
    /* \brief The \ref state_type.*/
    //state_type m_state_type;
public:

    /**Function that searches on m_search_space.
     * 
     * Calls either \ref search_BFS(bool old_check) or \ref search_heur(bool old_check)
     * @param[in] old_check: if true print the plan time in a file to easy the confrontation with the old version.
     * @param[in] used_heur: used to determine if any heuristic has to be used and which one.
     * @return true if a plan is found.*/
    bool search(bool old_check, heuristics used_heur);

    /**Function print out the solution time.
     * 
     * @param[in] elapsed_seconds: the time of the search.
     * @param[in] goal: the goal state.
     * @param[in] old_check: if true print the plan time in a file to easy the confrontation with the old version.
     * @param[in] givenplan: if true changes the name of the output folder for the comparison time.*/
    void print_results(std::chrono::duration<double> elapsed_seconds, T goal, bool old_check, bool givenplan);


    /**Function that searches on m_search_space using the given actions.
     *@param[in] act_name: the names of the \ref action to execute (ordered).*/
    void execute_given_actions(const std::vector<std::string>& act_name);

    /**Function that searches on m_search_space using the given actions and print out the execution time.
     * 
     * Every useless I\O step is removed for time accuracy
     * 
     *@param[in] act_name: the names of the \ref action to execute (ordered).*/
    void execute_given_actions_timed(const std::vector<std::string>& act_name);

    /**Function that checks whether the given actions exist. 
     * 
     *@param[in] act_name: the names of the \ref action to execute (ordered).*/
    void check_actions_names(const std::vector<std::string>& act_name);
};