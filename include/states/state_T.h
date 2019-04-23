/**
 * \class state
 * \brief Templatic Class that encodes a state of the *planner*.
 *
 * \details  This is the *TEMPLATE* and will be used as black box from \ref planner:
 * its implementation will depend on the initial choices.
 *
 * This class should be used to check entailment and to produce successors.
 * 
 * Template and not virtual to keep the pointer and, since the type of search is decided
 * at compile-time virtual overhead is not necessary.
 *
 * \todo Move to goal class bool is_goal();
 * 
 * \todo Check if is the correct way to usa a templatic class.
 * \copyright GNU Public License.
 *
 * \author Francesco Fabiano.
 * \date April 11, 2019
 */

#pragma once

#include "kripke/kstate.h"

#include "../utilities/define.h"
#include "../actions/action.h"
#include "../domain/initially.h"

/**
 * @tparam T:
 *    - \ref KRIPKE
 *    - \ref POSSIBILITIES
 *    - \ref OBDD*/
template <class T>
class state
{
private:
    /**
     * \brief The type of state representation.
     * 
     * One of:
     *    - \ref KRIPKE
     *    - \ref POSSIBILITIES
     *    - \ref OBDD*/
    T representation;

    /**
     * \brief The list of executed \ref action to get from the initial state to *this*.
     * 
     * Is a std::vector because we can repeat the same action.
     * @see action and action::m_id.
     */
    action_id_list m_executed_actions_id;
    /**
     * \brief The length of the plan up to this point.
     * 
     * \todo we can retrieve it from \ref m_executed_actions_list.
     *          */
    unsigned short m_plan_length;
    /**
     * \brief The heuristic value of the *this*.
     *
     * This value is given by the chosen implementation of \ref heuristic.*/
    int m_heuristic_value;

    /**
     * \brief Setter that behaves like the assignment operator.
     *
     * @param[in] given: the \ref state to copy in *this*.*/
    void set_state(const state<T> & given);
    /**
     * \brief Setter for the field \ref representation.
     *
     * @param[in] given: the **T** object to copy in \ref representation.
     * 
     * \todo implement the operator = in all the representations **T**.*/
    void set_state(const T & given);

    /**
     * \brief Setter for the field \ref m_executed_actions_id.
     *
     * @param[in] act_lis: the list of \ref action_id object to copy in \ref m_executed_actions_id.*/
    void set_executed_actions(const action_id_list & act_lis);
    /**
     * \brief Setter for the field \ref m_plan_length.
     *
     * @param[in] length: the length of the plan to copy in \ref m_plan_length.*/
    void set_plan_length(unsigned short length);
    /**
     * \brief Setter for the field \ref m_heuristic_value.
     *
     * @param[in] h_value: the int to copy in \ref m_heuristic_value.*/
    void set_heuristic_value(int h_value);

public:
    /**
     *\brief Constructor without parameters.
     * 
     * It creates \ref representation calling its **T** constructor.
     */
    state<T>();
    /**
     *\brief Constructor with a given \ref state.
     * 
     * @param given_state: the \ref state to copy into *this*.
     */
    state<T>(const state<T> & given_state);
    /**
     *\brief Constructor with a given state-representation object (**T**).
     * 
     * @param given_state: the **T** object state to copy into \ref representation.
     */
    state<T>(const T & given_representation);
    /**
     *\brief Constructor with that set *this* as successor of the given one.
     * 
     * @param prev_state: the \ref state that is the predecessor of *this*.
     *  @param act: the \ref action applied to \p prev_state.
     *
     * \todo what if the action is not executable?
     */
    state<T>(const state<T> & prev_state, const action & act);
    /*state<T>(const action_id_list &, unsigned short);
    state<T>(const action_id_list &, unsigned short, int);*/

    /**
     * \brief Getter of \ref m_executed_actions_id.
     *     
     * @return the \ref action_id_list that represents all the executed \ref action before to obtain *this*.*/
    const action_id_list & get_executed_actions();
    /**
     *\brief Getter of \ref m_plan_length.
     *     
     * @return the length of the plan up to *this*.*/
    unsigned short get_plan_length();
    /**
     *\brief Getter of \ref m_heuristic_value.
     *     
     * @return the heuristic value *this*.*/
    int get_heuristic_value();

    /**
     *\brief Function that checks if *this* entails a conjunctive set of \ref fluent.
     * 
     * The actual entailment is left to the specific state-representation (\ref representation).
     * 
     * @see kstate
     * 
     * @param to_check: the conjunctive set of \ref fluent to check if is entailed by *this*.
     * 
     * @return true if \p to_check is entailed by *this*.
     * @return false if \p -to_check is entailed by *this*.
     * 
     * \todo add in see also the other state representations.
     * 
     * \bug Is not supposed to override the method but it does.
     */
    bool entails(const fluent_set & to_check);
    /**
     *\brief Function that checks if *this* entails a DNF \ref fluent_formula.
     * 
     * The actual entailment is left to the specific state-representation (\ref representation).
     * 
     * @see kstate
     *
     * @param to_check: the DNF \ref fluent_formula to check if is entailed by *this*.
     * 
     * @return true if \p to_check is entailed by *this*.
     * @return false if \p -to_check is entailed by *this*.
     * 
     * \todo add in see also the other state representations.
     * 
     * \bug Is not supposed to override the method but it does.
     */
    bool entails(const fluent_formula & to_check);
    /**
     *\brief Function that checks if *this* entails a CNF \ref formula_list.
     * 
     * The actual entailment is left to the specific state-representation (\ref representation).
     * 
     * @see kstate
     *
     * @param to_check: the CNF \ref formula_list to check if is entailed by *this*.
     *
     *       
     * @return true if \p to_check is entailed by *this*.
     * @return false if \p -to_check is entailed by *this*.
     * 
     * \todo add in see also the other state representations.
     *
     * \bug Is not supposed to override the method but it does.
     */
    bool entails(const formula_list & to_check);

    /**
     *\brief Function that builds the initial \ref state and set *this* with it.
     * 
     * The actual construction of the \ref state is left to the specific state-representation (\ref representation).
     * 
     * @see kstate, initially
     *
     * @param ini_conditions: the conditions that the initial state must respect.
     * 
     * \todo add in see also the other state representations.
     *
     * \bug Is not supposed to override the method but it does.
     */
    void build_initial(const initially & ini_conditions);
    /**
     *\brief Function that compute the successor of *this* given an \ref action.
     * 
     * The actual implementation is left to the specific state-representation (\ref representation).
     * 
     * @see kstate, action
     *
     * @param act: the \ref action to execute on *this*.
     * @return The state-representation that encodes the successor.
     * 
     * \todo add in see also the other state representations.
     *
     * \bug Is not supposed to override the method but it does.
     */
    T compute_succ(const action & act);

};

