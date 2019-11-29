/**
 * \class satisfied_goals
 * \brief Class that implements the heuristic "number of satisfied sub_goals".
 *
 * \details In this class we simply check how many sub_goals 
 * (elements of domain::get_instance.get_goal_description()) each state satisfies
 * and we pick the states which satisfies the highest number of sub goals as the next to
 * expand.
 * As extension we should break each complex belief formula in more than one so we
 * have more sub goals and more accuracy. Especially we should break C in multiple components.
 * 
 *
 * \copyright GNU Public License.
 * 
 * \author Francesco Fabiano.
 * \date November 28, 2019
 */

#pragma once

template <class T>
class satisfied_goals
{
private:
    /** \brief The number of subgoals.*/
    unsigned short m_goals_number;

    /** \brief A local copy of the goal that is possibly modified (Added more subgoals).*/
    formula_list m_goals;

    /** \brief Function that expands the group formulae to have more sub_goals
     * 
     * For example: We have C([a,b], \phi) we then transform it in:
     *      - B(a, \phi);
     *      - B(b, \phi);
     * FUTURE DEVELOPEMENT
     *      - B(a, (B(b, \phi)));
     *      - B(b, (B(a, \phi)));
     *      - B(a, (B(b, B(a, \phi))));
     *      - etc. (Following \ref depth)
     * FUTURE DEVELOPEMENT
     *      - C([a,b], \phi).
     * 
     */
    void expand_goals();


public:

    /** \brief Class constructor that takes care of the goals rewriting.*/
    satisfied_goals();

    /** \brief Function that return the number of unsatisifed goals of \ref eState
     * 
     * @param[in] eState: The (templatic) state where to check for unsatisfied goals
     * @return: The number of unsatisfied goals in \ref eState.*/
    unsigned short get_unsatisfied_goals(const T & eState) const;


    /** \brief Function that return the number of satisifed goals of \ref eState
     * 
     * @param[in] eState: The (templatic) state where to check for satisfied goals
     * @return: The number of satisfied goals in \ref eState.*/
    unsigned short get_satisfied_goals(const T & eState) const;

    /**Getter of the field \ref m_goals
     *
     * @return: the cnf of expanded sub goals.
     */
    const formula_list & get_goals() const;
    /**Setter of the field \ref m_goals
     *
     * @param[in] to_set: the cnf of expanded sub goals to set as \ref m_goals.
     */
    void set_goals(const formula_list & to_set);

    /**Getter of the field \ref m_goals_number
     *
     * @return: the number of expanded sub goals.
     */
    unsigned short get_goals_number() const;
    /**Setter of the field \ref m_goals_number
     *
     * @return: the number of expanded sub goals to set.
     */
    void set_goals_number(unsigned short to_set);

    /** \brief The = operator.
     *   
     * @param [in] to_copy: the \ref s to assign to *this*.
     * @return true: if \p the satisfied_goals went ok.
     * @return false: otherwise.*/
    bool operator=(const satisfied_goals<T>& to_copy);
};
