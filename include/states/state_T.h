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
    /** \brief The type of state m_representation.
     * 
     * One of:
     *    - \ref KRIPKE
     *    - \ref POSSIBILITIES
     *    - \ref OBDD*/
    T m_representation;
 
    /** \brief The list of executed \ref action to get from the initial state to *this*.
     * 
     * Is a std::vector because we can repeat the same action.
     * @see action and action::m_id.*/
    action_id_list m_executed_actions_id;
    /** \brief The length of the plan up to this point.
     * 
     * \todo we can retrieve it from \ref m_executed_actions_id.*/
    unsigned short m_plan_length;
    /** \brief The heuristic value of the *this*.
     *
     * This value is given by the chosen implementation of \ref heuristic.*/
    int m_heuristic_value;

    /** \brief Setter for the field \ref m_executed_actions_id.
     *
     * @param[in] to_set: the list of \ref action_id object to copy in \ref m_executed_actions_id.*/
    void set_executed_actions(const action_id_list & to_set);
    /** \brief Setter for the field \ref m_plan_length.
     *
     * @param[in] to_set: the length of the plan to copy in \ref m_plan_length.*/
    void set_plan_length(unsigned short to_set);
    /** \brief Setter for the field \ref m_heuristic_value.
     *
     * @param[in] to_set: the int to copy in \ref m_heuristic_value.*/
    void set_heuristic_value(int to_set);

    /** \brief Setter of \ref m_representation.
     *     
     * @param[in] to_set: the m_representation to assign to \ref m_representation.*/
    void set_representation(const T & to_set);

public:
    /** \brief Constructor without parameters.
     * 
     * It creates \ref m_representation calling its **T** constructor.*/
    state<T>();
    /* \brief Constructor with a given \ref state.
     * 
     * @param given_state: the \ref state to copy into *this*.*/
    /*state<T>(const state<T> & given_state);
     \brief Constructor with a given state-representation object (**T**).
     * 
     * @param given_representation: the **T** object state to copy into \ref m_representation.*/
    /*state<T>(const T & given_representation);
     * \brief Constructor with that set *this* as successor of the given one.
     * 
     * @param prev_state: the \ref state that is the predecessor of *this*.
     *  @param act: the \ref action applied to \p prev_state.
     *
     * \todo what if the action is not executable?*/
    state<T>(const state<T> & prev_state, const action & act);
    /*state<T>(const action_id_list &, unsigned short);
    state<T>(const action_id_list &, unsigned short, int);*/

    /** \brief Getter of \ref m_executed_actions_id.
     *     
     * @return the \ref action_id_list that represents all the executed \ref action before to obtain *this*.*/
    const action_id_list & get_executed_actions() const;
    /** \brief Getter of \ref m_plan_length.
     *     
     * @return the length of the plan up to *this*.*/
    unsigned short get_plan_length() const;
    /** \brief Getter of \ref m_heuristic_value.
     *     
     * @return the heuristic value *this*.*/
    int get_heuristic_value() const;
    /** \brief Getter of \ref m_representation.
     *     
     * @return the m_representation of *this*.*/
    const T & get_representation() const;

    /** \brief Function that checks if *this* entails a conjunctive set of \ref fluent.
     * 
     * The actual entailment is left to the specific state-representation (\ref m_representation).
     * 
     * @see kstate
     * 
     * @param to_check: the conjunctive set of \ref fluent to check if is entailed by *this*.
     * 
     * @return true if \p to_check is entailed by *this*.
     * @return false if \p -to_check is entailed by *this*.
     * 
     * \todo add in see also the other state m_representations.
     * 
     * \bug Is not supposed to override the method but it does.*/
    bool entails(const fluent_set & to_check) const;
    /** \brief Function that checks if *this* entails a DNF \ref fluent_formula.
     * 
     * The actual entailment is left to the specific state-representation (\ref m_representation).
     * 
     * @see kstate
     *
     * @param to_check: the DNF \ref fluent_formula to check if is entailed by *this*.
     * 
     * @return true if \p to_check is entailed by *this*.
     * @return false if \p -to_check is entailed by *this*.
     * 
     * \todo add in see also the other state m_representations.
     * 
     * \bug Is not supposed to override the method but it does.*/
    bool entails(const fluent_formula & to_check) const;
    /** \brief Function that checks if *this* entails a CNF \ref formula_list.
     * 
     * The actual entailment is left to the specific state-representation (\ref m_representation).
     * 
     * @see kstate
     *
     * @param to_check: the CNF \ref formula_list to check if is entailed by *this*.
     *
     *       
     * @return true if \p to_check is entailed by *this*.
     * @return false if \p -to_check is entailed by *this*.
     * 
     * \todo add in see also the other state m_representations.
     *
     * \bug Is not supposed to override the method but it does.*/
    bool entails(const formula_list & to_check) const;

    /** \brief Function that builds the initial \ref state and set *this* with it.
     * 
     * The actual construction of the \ref state is left to the specific state-representation (\ref m_representation).
     * 
     * @see kstate, initially
     * 
     * \todo add in see also the other state m_representations.
     *
     * \bug Is not supposed to override the method but it does.*/
    /* @param ini_conditions: the conditions that the initial state must respect.
     * @param fluent_number: the number of fluent in the domain, used for the prune construction.
     * @param[in] agent_number: the number of \ref agent in the \ref domain, used for the prune construction.*/
    void build_initial();


    /** \brief Function that checks if a given action is executable in *this*.
     *  
     * @see action.
     *  
     * @param[in] act: The action to be checked on *this*.
     * @return true: \p act is executable in *this*;
     * @return false: \p act is not executable in *this**/
    bool is_executable(const action & act) const;

    /** \brief Function that compute the successor of *this* given an \ref action.
     * 
     * The actual implementation is left to the specific state-representation (\ref m_representation).
     * 
     * @see kstate, action
     *
     * @param act: the \ref action to execute on *this*.
     * @return The statethat encodes the successor.
     * 
     * \todo add in see also the other state m_representations.
     *
     * \bug Is not supposed to override the method but it does.*/
    state<T> compute_succ(const action & act) const;

    /** \brief The copy operator.
     *   
     * @param [in] to_copy: the \ref state to assign to *this*.
     * @return true: if \p the assignment went ok.
     * @return false: otherwise.*/
    bool operator=(const state<T> & to_copy);

};
