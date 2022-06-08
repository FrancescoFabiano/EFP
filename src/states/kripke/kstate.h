/**
 * \brief Class that represents a pointed Kripke structure.
 *
 * \details  A pointed Kripke structure is one of the possible states description for EFP. It represent a single state in the
 *  mA* language. The entailment of a \ref belief_formula on this state representation is based on the concept of reachability.
 *  Each node of the structure represents a possible world (\ref kworld) and each edge (\ref kedge) represent the uncertainty
 *  of an \ref agent. In particular if two \ref kworld are connected with a \ref kedge labeled ag it means that the \ref agent
 *  ag cannot distinguish between the two worlds. The pointed kworld represents the real world.
 * 
 * @see kworld, kedge, and kstore.
 *
 * \todo general: the && parameter in set (move) and in other places, is it better?
 *
 * \copyright GNU Public License.
 *
 * \author Francesco Fabiano, Alessandro Burigana.
 * \date April 9, 2019
 */
#pragma once

#include "kworld.h"
#include "kstore.h"

#include "../../../include/definitions/define.h"
//#include "../../domain/initially.h"
//#include "../../actions/action.h"
#include "../../../external-lib/bisimulation/bisimulation.h"
#include "../../formulae/finitary_theory/finitary_theory.h"
#include "../../domain/domain.h"

class kstate {
private:
    /** \brief The set containing a pointer to each world of the structure.
     *
     * @see kworld and kstore.*/
    kworld_set m_worlds;
    /** \brief The set containing a pointer to each edge of the structure.
     * 
     * @see kstore and kedge.*/
    kedge_map m_edges;
    /** \brief The pointer to the *pointed world*.
     * 
     * @see kworld and kstore.*/
    kworld *m_pointed;

    /** \brief Function that builds the initial Kripke structure given the initial conditions.
     *
     * The building of the initial Kripke structure can be done in two different ways:
     *    - A method using a pruning techniques(\ref build_initial_prune).
     *    - A structural build (\ref build_initial_structural).
     *
     * @see kworld, kedge and initially.
     *
     * \todo if first method is implemented the add functions in \ref kstore must be changed to not create
     *       the object every time but just to retrieve the object since all of them are already created.*/
    template<class M>
    void generate_from_theory(const domain &domain, const finitary_theory<M> &theory);

    template<class M>
    void generate_initial_pointed(const finitary_theory<M> &theory);

    template<class M>
    void generate_initial_worlds(const domain &domain, const finitary_theory<M> &theory);

    template<class M>
    void generate_initial_edges(const domain &domain, const finitary_theory<M> &theory);

    /** \brief Function that checks the entailment of a \ref fluent in a given \ref kworld.
     *
     * @param[in] formula: the \ref fluent that has to be checked if entailed in \p world.
     * @param[in] world: the pointer to the  \ref kworld where to check the entailment.
     *
     * @return true: \p to_check is entailed in \p world;
     * @return false: \p -to_check is entailed in \p world.*/
    bool entails(const formula &formula, const kworld &world) const;

    void update(const kstate &previous, const action &action);

    /** Function that calculates all the reachable \ref kworld.
     * 
     * @param[in] world: the current \ref kworld.
     * @param[in] reached_worlds: the set of all the reached \ref kworld.
     * @param[in] adj_list: the adjacency list of thr \ref kstate.*/
    void get_all_reachable_worlds_edges(const kworld *world, kworld_set & reached_worlds, kedge_map & reached_edges) const;

    /** Function that cleares all the unreachable \ref kworld of *this*
     *
     * @param[in] adj_list: the adjacency list of thr \ref kstate.*/
    void clean_unreachable_kworlds();

    /** \brief Function that transforms *this* into an equivalent automaton.
     * 
     * @param[in] kworld_vec: the vector of all \ref kworld.
     * 
     * @return the automaton equivalent to *this*.*/
    //     * @param[in] index_map: a map that associates a unique id to each \ref kworld.
    //     * @param[in] compact_indices: we associate to the \ref kworld numerical ids a second kind of numerical ids. These new ids are consecutive numbers (hence, they are compact).
    automaton kstate_to_automaton(std::vector<kworld*> & kworld_vec, const std::map<agent, bis_label> & agent_to_label) const;
    /** \brief Function that transforms the given automaton into an equivalent \ref kstate.
     * 
     * @param[in] a: the automaton to transform.
     * @param[in] kworld_vec: the vector of all \ref kworld.
     * 
     * @return the \ref kstate.*/
    void automaton_to_kstate(const automaton & a, const std::vector<kworld*> & kworld_vec, const std::map<bis_label, agent> & label_to_agent);

public:
    kstate();

    template<class M>
    kstate(const domain &domain, const finitary_theory<M> &theory);

    kstate(const kstate &previous, const action &action);
    /** \brief Getter of the field \ref m_worlds.
     *
     * @return: the value of \ref m_worlds.*/
    const kworld_set &get_worlds() const;
    /** \brief Getter of the field \ref m_information_state.
     *
     * @return: the value of \ref m_information_state.*/
    const kedge_map &get_edges() const;
    /** \brief Getter of the field \ref m_pointed.
     *
     * @return: the value of \ref m_pointed.*/
    const kworld *get_pointed() const;

    /** \brief The copy operator.
     *
     * @param [in] to_copy: the \ref kstate to assign to *this*.
     * @return true: if \p the assignment went ok.
     * @return false: otherwise.*/
//    bool operator=(const kstate & to_copy);

    /**\brief The operator < for set operation in \ref planning_graph.
     *
     * The ordering is based on kworlds and the kedges and pointed.
     *
     * @param[in] to_compare: the \ref kstate to check for odering.
     *
     * @return true: if *this* is smaller than \p to_compare.
     * @return false: otherwise.*/
    bool operator<(const kstate & to_compare) const;
    bool operator==(const kstate & to_compare) const;

    /** \brief Function that applies the transition function on *this* given an action.
     *
     * The transition function is applied accordingly to the optimization or not given as input.
     * The non optimized one follows mA*. Check the paper for more information.
     * The other uses some optz to reduce the state dimension.
     *  
     * @see action.
     *  
     * @param[in] act: The action to be applied on *this*.
     * @return the \ref kstate that results after the execution.
     * 
     * \todo The action must be executable on *this* otherwise it will return a null_ptr.*/
    kstate compute_succ(const action & act) const;

    /** \brief Function that sets *this* as the mimimum \ref kstate that is bisimilar to the current one.
     *
     * The function follows the approach of the algorithm described in Paige and Tarjan (1986).*/
    void calc_min_bisimilar(Bisimulation_Algorithm algorithm);

    /** \brief Function that prints the information of *this*.*/
    void print() const;

    /** \brief Function that prints the information of *this* in a Graphviz file.
     *
     * @param[in] graphviz: the ostream where to print the info of *this*.*/
    void print_graphviz(std::ostream& graphviz) const;

    //    void debug_print(const kstate & to_compare);
    //    void automaton_to_kstate_debug(const automaton & a, std::vector<kworld*> & kworld_vec);


};
