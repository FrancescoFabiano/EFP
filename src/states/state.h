/**
 * \class state
 * \brief Template-based Class that encodes a state of planner.h.
 *
 * \details  This is the *TEMPLATE* and will be used as black box from planner.h:
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
 * \author Francesco Fabiano, Alessandro Burigana.
 * \date April 11, 2019
 */

#pragma once
#include <fstream>

#include "../actions/action.h"
#include "../formulae/formula.h"
#include "../formulae/finitary_theory/finitary_theory.h"

/**
 * @tparam T:
 *    - \ref KRIPKE
 *    - \ref POSSIBILITIES
 *    - \ref OBDD*/
template <class T>
class state {
private:
    /** \brief The type of state m_representation.
     * 
     * One of:
     *    - \ref KRIPKE
     *    - \ref POSSIBILITIES
     *    - \ref OBDD*/
    T m_state;
    const state<T> *m_previous_state;
    const action *m_action;
    
    /** \brief The length of the plan up to this point.
     * 
     * \todo we can retrieve it from \ref m_executed_actions_id.*/
    unsigned short m_plan_length;
    /** \brief The heuristic value of the *this*.
     *
     * This value is given by the chosen implementation of \ref heuristic.*/
    short m_heuristic_value = 0;

    template<class M>
    void generate_from_theory(const finitary_theory<M> &theory);

    void generate_from_update(const state<T> *previous_state, const action *action);

public:
    /** \brief Constructor without parameters.
     * 
     * It creates \ref m_representation calling its **T** constructor.*/
    state<T>() = default;   // todo: rimuovi

    template<class M>
    explicit state<T>(const finitary_theory<M> &theory);
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
    state<T>(const state<T> *previous_state, const action *action);

    /** \brief Getter of \ref m_representation.
     *
     * @return the m_representation of *this*.*/
    const T &get_state() const;

    const state<T> *get_previous_state() const;

    const action *get_action() const;

    /** \brief Getter of \ref m_plan_length.
     *
     * @return the length of the plan up to *this*.*/
    unsigned short get_plan_length() const;
    /** \brief Setter for the field \ref m_heuristic_value.
     *
     * @param[in] to_set: the int to copy in \ref m_heuristic_value.*/
    void set_heuristic_value(short to_set);
    /** \brief Getter of \ref m_heuristic_value.
     *
     * @return the heuristic value *this*.*/
    short get_heuristic_value() const;

    /** \brief Function that checks if *this* entails a \ref fluent.
     * 
     * The actual entailment is left to the specific state-representation (\ref m_representation).
     * 
     * @see kstate
     * 
     * @param formula: the \ref fluent to check if is entailed by *this*.
     * 
     * @return true if \p to_check is entailed by *this*.
     * @return false if \p -to_check is entailed by *this*.
     * 
     * \todo add in see also the other state m_representations.
     * 
     * \bug Is not supposed to override the method but it does.*/
    bool entails(const formula &formula) const;

    /** \brief Function that checks if a given action is executable in *this*.
     *
     * @see action.
     *
     * @param[in] act: The action to be checked on *this*.
     * @return true: \p act is executable in *this*.
     * @return false: \p act is not executable in *this*.*/
    bool is_executable(const action & act) const;

    /** \brief Function that checks if *this* is a goal state.
     *
     * @return true: if *this* is a goal state.
     * @return false: otherwise.*/
    bool is_goal(const formula_list& goal_description) const;

    /** \brief Function that determines the mimimum \ref e-state that is bisimilar to the current one.
     *
     * The function depends on the type of e-state.
     *
     * @return the minimum bisimilar e-state to *this*.*/
    void calc_min_bisimilar(Bisimulation_Algorithm algorithm);

    /** \brief Function that prints the information of *this*.*/
    void print() const;

    /** \brief Function that prints the information of *this* in a Graphviz file.
     *
     * @param [in] postfix: The postfix to give to the name of the file (default "").
     */
    void print_graphviz(const std::string& postfix = "") const;

    /** \brief The < operator for set operations.
     *
     * The results is left to the representations.
     *
     * @param [in] to_compare: the \ref state to to_compare to *this*.
     * @return true: if *this* is smaller then to_compare.
     * @return false: otherwise.*/
    bool operator<(const state<T> & to_compare) const;



//    /** \brief Function that checks if *this* entails a conjunctive set of \ref fluent.
//     *
//     * The actual entailment is left to the specific state-representation (\ref m_representation).
//     *
//     * @see kstate
//     *
//     * @param to_check: the conjunctive set of \ref fluent to check if is entailed by *this*.
//     *
//     * @return true if \p to_check is entailed by *this*.
//     * @return false if \p -to_check is entailed by *this*.
//     *
//     * \todo add in see also the other state m_representations.
//     *
//     * \bug Is not supposed to override the method but it does.*/
//    bool entails(const fluent_set & to_check) const;
//    /** \brief Function that checks if *this* entails a DNF \ref fluent_formula.
//     *
//     * The actual entailment is left to the specific state-representation (\ref m_representation).
//     *
//     * @see kstate
//     *
//     * @param to_check: the DNF \ref fluent_formula to check if is entailed by *this*.
//     *
//     * @return true if \p to_check is entailed by *this*.
//     * @return false if \p -to_check is entailed by *this*.
//     *
//     * \todo add in see also the other state m_representations.
//     *
//     * \bug Is not supposed to override the method but it does.*/
//    bool entails(const fluent_formula & to_check) const;
//
//    /** \brief Function that checks if *this* entails a \ref belief_formula.
//     *
//     * The actual entailment is left to the specific state-representation (\ref m_representation).
//     *
//     * @see kstate
//     *
//     * @param to_check: the \ref belief_formula to check if is entailed by *this*.
//     *
//     * @return true if \p to_check is entailed by *this*.
//     * @return false if \p -to_check is entailed by *this*.
//     *
//     * \todo add in see also the other state m_representations.
//     *
//     * \bug Is not supposed to override the method but it does.*/
//    bool entails(const belief_formula & to_check) const;
//
//    /** \brief Function that checks if *this* entails a CNF \ref formula_list.
//     *
//     * The actual entailment is left to the specific state-representation (\ref m_representation).
//     *
//     * @see kstate
//     *
//     * @param to_check: the CNF \ref formula_list to check if is entailed by *this*.
//     *
//     *
//     * @return true if \p to_check is entailed by *this*.
//     * @return false if \p -to_check is entailed by *this*.
//     *
//     * \todo add in see also the other state m_representations.
//     *
//     * \bug Is not supposed to override the method but it does.*/
//    bool entails(const formula_list & to_check) const;

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
//    void build_initial(const initially& initial_conditions);



    /** \brief Function that computes the successor of *this* given an \ref action.
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
//    state<T> compute_succ(const action & act) const;

//    fluent_set compute_succ2(const action & act) const;


//    /** \brief The copy operator.
//     *
//     * @param [in] to_copy: the \ref state to assign to *this*.
//     * @return true: if \p the assignment went ok.
//     * @return false: otherwise.*/
//    bool operator=(const state<T> & to_copy);



    /**Function that retrieves the observants w.r.t. the current state
     *
     * @return: the observability of each agent*/
//    single_observability_map get_observants(const observability_map & table) const;


    /**Function that retrieves the agent group of an agent w.r.t. the current state
     *
     * @return: the agent_group_id of \p ag*/
//    agent_group_id get_obs_group(const cem_store &store, agent ag, const observability_map &table) const;


    //DEBUG
    //    void min_with_print(state<T> tmp);
    //    T debug_get_representation();

};

