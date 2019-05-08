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

//THIS IS BECAUSE IS A TEMPLATIC CLASS AND IT IS A DEPENDECY
#include "../states/state_T.ipp"

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
     * @return true if a plan is found.
     * @return false otherwise.
     */
    bool search_BFS();

    /**Function that searches on m_search_space using the given actions.
     *@param[in] act_name: the names of the \ref action to execute (ordered).*/
    void execute_given_actions(const std::vector<std::string> act_name);
};