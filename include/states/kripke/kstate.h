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
 *
 * \copyright GNU Public License.
 *
 * \author Francesco Fabiano.
 * \date April 9, 2019
 */
#pragma once

#include "kedge.h"
#include "kworld.h"
#include "kstore.h"

#include "../../utilities/define.h"
#include "../../domain/initially.h"
#include "../../actions/action.h"

class kstate
{
private:
    /**
     * \brief The set containing a pointer to each edge of the structure.
     * 
     * @see kstore and kedge.
     */
    kedge_ptr_set m_edges;
    /**
     * \brief The set containing a pointer to each world of the structure.
     * 
     * @see kworld and kstore.
     */
    kworld_ptr_set m_worlds;
    /**
     * \brief The pointer to the *pointed world*.
     * 
     * @see kworld and kstore.
     */
    kworld_ptr m_pointed;
    /**
     *\brief Function that check the entailment of a \ref fluent in a given \ref kworld.
     *     @see \ref kworld::entails(fluent) const
     *  
     * @param[in] to_check: the \ref fluent that has to be checked if entailed in \p world.
     * @param[in] world: the pointer to the  \ref kworld where to check the entailment.
     *
     * @return true: \p to_check is entailed in \p world;
     * @return false: \p -to_check is entailed in \p world.*/
    bool entails(fluent to_check, kworld_ptr world) const;
    /**
     *\brief Function that check the entailment of a conjunctive set of \ref fluent in a given \ref kworld.
     *     @see \ref kworld::entails(fluent_set) const
     *  
     * @param[in] to_check: the conjunctive set of \ref fluent that has to be checked if entailed in \p world.
     * @param[in] world: the pointer to the  \ref kworld where to check the entailment.
     *
     * @return true: \p to_check is entailed in \p world;
     * @return false: \p -to_check is entailed in \p world.*/
    bool entails(const fluent_set & to_check, kworld_ptr world) const;
    /**
     *\brief Function that check the entailment of a conjunctive set of \ref fluent in a given \ref kworld.
     *     @see \ref kworld::entails(fluent_formula) const
     *  
     * @param[in] to_check: the DNF \ref fluent_formula that has to be checked if entailed in \p world.
     * @param[in] world: the pointer to the \ref kworld where to check the entailment.
     *
     * @return true: \p to_check is entailed in \p world;
     * @return false: \p -to_check is entailed in \p world.*/
    bool entails(const fluent_formula & to_check, kworld_ptr world) const;

    /**
     *\brief Function that check the entailment of a \ref belief_formula.
     * 
     * The concept of entailment of a \ref belief_formula is related to reachability
     * and so an initial state is required, the given \ref kworld \p world is the starting point
     * (it simulates the pointed world) where the entailment check starts from. The
     * world needs to be parametric to not create a new \ref kstate for every iteration of the formula;
     * now we can just change the starting point and keep the same \ref kstate when testing, for example,
     * *nested knowledge*. The function is private because the entailment always starts from the \ref m_pointed and then
     * moves to the reachable (with the operators) \ref kworld.
     *  
     * @see \ref belief_formula and entails(const belief_formula &) const.
     * 
     * @param[in] to_check: the \ref belief_formula that has to be checked if entailed from \p world.
     * @param[in] world: the pointer to the \ref kworld where to start to check the entailment.
     *
     * @return true: \p to_check is entailed starting from \p world;
     * @return false: \p -to_check is entailed starting from \p world.
     * 
     * \warning self-loop?
     */
    bool entails(const belief_formula & to_check, kworld_ptr world) const;
    /**
     *\brief Function that check the entailment of a \ref belief_formula on several possible starting points.
     * 
     * This function eases the task to check the entailment from several starting \ref kworld simultaneously.
     * This happens when we want to check the transitive closure of the \ref agent ag and to test the formula B(ag, phi).
     * 
     * @see belief_formula.
     * 
     * @param[in] to_check: the \ref belief_formula that has to be checked if entailed from \p world.
     * @param[in] world: the pointers to the set of \ref kworld where to start to check the entailment.
     *
     * @return true: \p to_check is entailed starting from all the \ref kworld in \p worlds;
     * @return false: \p -to_check is entailed starting from all the \ref kworld in \p worlds.
     * 
     * \warning self-loop?
     */
    bool entails(const belief_formula & to_check, const kworld_ptr_set & worlds) const;

    /**
     *\brief Function that return all the reachable \ref kworld given the \ref agent and the staring \ref kworld.
     * 
     * This function finds all the world that are transitively reachable from the starting world following
     * the edges labelled with the given \ref agent. It returns all the worlds where is necessary to check if
     * *phi* is entailed in order to verify B(\p ag, *phi*) and \p world is the starting point.
     * 
     * @see \ref belief_formula.
     * 
     * @param[in] ag: the label of the \ref kedge that the function follows to check the transitivity.
     * @param[in] world: the pointers to the set of \ref kworld where to start to check the entailment.
     *
     * @return a set of pointers to all the reachable worlds.
     *
     * 
     * \warning self-loop?
     */
    const kworld_ptr_set get_B_reachable_worlds(agent ag, kworld_ptr world);

    /**
     *\brief Function that return all the reachable \ref kworld given the \ref agent and the staring \ref kworld.
     * 
     * This function modify the parameter so it's easy to check if a fixed point is reached.
     * This is useful for the operator C. Otherwise its better to use \ref get_B_reachable_worlds(agent, kworld_ptr).
     * 
     * @see \ref belief_formula, get_B_reachable_worlds(agent, kworld_ptr) and get_C_reachable_worlds(const agent_set &, kworld_ptr world).
     * 
     * @param[in] ag: the label of the \ref kedge that the function follows to check the transitivity.
     * @param[in] world: the pointers to the set of \ref kworld where to start to check the entailment.
     *
     * @param[out] reached: the set of pointers to all the reachable worlds.
     *
     * 
     * \warning self-loop?
     */
    bool get_B_reachable_worlds(agent ag, kworld_ptr world, kworld_ptr_set& reached);

    /**
     *\brief Function that return all the reachable \ref kworld given a set of \ref agent and the staring \ref kworld.
     * 
     * This function applies get_E_reachable_worlds(agent , kworld_ptr) with all the agents inside the
     * given set and return the union of the reached \ref kworld. It returns all the worlds where is necessary to check if
     * *phi* is entailed in order to verify E(\p ags, *phi*) and \p world is the starting point.
     * 
     * @see \ref belief_formula and get_B_reachable_worlds(agent, kworld_ptr).
     * 
     * @param[in] ags: the set of label of the \ref kedge that the function follows to check the transitivity.
     * @param[in] world: the pointers to the set of \ref kworld where to start to check the entailment.
     *
     * @return a set of pointers to all the reachable worlds.
     *
     * 
     * \warning self-loop?
     */
    const kworld_ptr_set get_E_reachable_worlds(const agent_set & ags, kworld_ptr world);

    /**
     *\brief Function that return all the reachable \ref kworld given a set of \ref agent and the staring \ref kworld.
     * 
     * This function modify the parameter so it's easy to check if a fixed point is reached.
     * This is useful for the operator C. Otherwise its better to use \ref get_E_reachable_worlds(agent_set, kworld_ptr).
     * 
     * @see \ref belief_formula, get_B_reachable_worlds(agent, kworld_ptr, kworld_ptr_set) and get_C_reachable_worlds(const agent_set &, kworld_ptr world).
     * 
     * @param[in] ags: the set of label of the \ref kedge that the function follows to check the transitivity.
     * @param[in] world: the pointers to the set of \ref kworld where to start to check the entailment.
     *
     * @param[out] reached: the set of pointers to all the reachable worlds.
     *
     * 
     * \warning self-loop?
     */
    bool get_E_reachable_worlds(const agent_set &ags, kworld_ptr world, kworld_ptr_set& reached);

        /**
     *\brief Function that return all the reachable \ref kworld (in the *Common Knowledge* sense) given a \ref agent and the staring \ref kworld.
     * 
    * This function applies get_E_reachable_worlds(const agent_set &, kworld_ptr) on its on result until a fixed point is found.
        * It returns all the worlds where is necessary to check if*phi* is entailed in order to verify C(\p ags, *phi*) and \p world is the starting point.
     * 
     * @see \ref belief_formula, and get_E_reachable_worlds(const agent_set &, kworld_ptr world).
     * 
     * @param[in] ags: the set of label of the \ref kedge that the function follows to check the *Common Knowledge* reachability.
     * @param[in] world: the pointers to the set of \ref kworld where to start to check the entailment.
     *
     * @return a set of pointers to all the reachable worlds.
     * 
     * \warning self-loop?
     */
    const kworld_ptr_set get_C_reachable_worlds(const agent_set &, kworld_ptr world);

public:

    bool entails(fluent) const;
    bool entails(const fluent_set &) const;
    bool entails(const fluent_formula &) const;

    bool entails(const belief_formula &) const;

    void add_world(const kworld &);
    void add_edge(const kedge &);

    void build_initial(const initially &);
    const kstate& compute_succ(const action &);
};

