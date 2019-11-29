/**
 * \class heuristic_manager
 * \brief Class that set the score to each state by using the given heuristics.
 *
 * \copyright GNU Public License.
 * 
 * \author Francesco Fabiano.
 * \date November 28, 2019
 */
//#include "satisfied_goals.h"
#include "planning_graph.ipp"
#include "satisfied_goals.ipp"

#pragma once

template <class T>
class heuristic_manager
{
private:
    /**The type of heuristic used*/
    heuristics m_used_heur;
    satisfied_goals<T> m_sat_goal;

public:
    /** \brief Class constructor that uses the chosen heuristic to perform the operation
     * 
     * @param[in] used_heur: Which heuristic has to be used.*/
    heuristic_manager(heuristics used_heur);
    /** \brief The function that compute heuristic value
     * 
     * @param[out] eState: The state to update with the calculated heuristic value.*/
    void set_heuristic_value(T & eState);

    /**Setter for the field \ref m_used_heur
     *
     * @param[in] used_h: The heuristics to assign to \ref m_used_heur
     */
    void set_used_h(heuristics used_h);
    /**Getter for the field \ref m_used_heur
     *
     * @return: The heuristics assigned to \ref m_used_heur
     */
    heuristics get_used_h() const;

    /**Setter for the field \ref m_sat_goal
     *
     * @param[in] used_h: The value to assign to \ref m_sat_goal
     */
    void set_sat_goal(const satisfied_goals<T> & sat_goal);

    /**Getter for the field \ref m_sat_goal
     *
     * @return: The value assigned to \ref m_sat_goal
     */
    const satisfied_goals<T> & get_sat_goal() const;


    /** \brief The = operator.
     *   
     * @param [in] to_copy: the \ref heuristic_manager to assign to *this*.
     * @return true: if \p the assignment went ok.
     * @return false: otherwise.*/
    bool operator=(const heuristic_manager& to_copy);
};