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
     *\brief Function that checks the entailment of a \ref fluent in a given \ref kworld.
     *     @see \ref kworld::entails(fluent) const
     *  
     * @param[in] to_check: the \ref fluent that has to be checked if entailed in \p world.
     * @param[in] world: the pointer to the  \ref kworld where to check the entailment.
     *
     * @return true: \p to_check is entailed in \p world;
     * @return false: \p -to_check is entailed in \p world.*/
    bool entails(fluent to_check, kworld_ptr world) const;
    /**
     *\brief Function that checks the entailment of a conjunctive set of \ref fluent in a given \ref kworld.
     *     @see \ref kworld::entails(const fluent_set &) const
     *  
     * @param[in] to_check: the conjunctive set of \ref fluent that has to be checked if entailed in \p world.
     * @param[in] world: the pointer to the  \ref kworld where to check the entailment.
     *
     * @return true: \p to_check is entailed in \p world;
     * @return false: \p -to_check is entailed in \p world.*/
    bool entails(const fluent_set & to_check, kworld_ptr world) const;
    /**
     *\brief Function that checks the entailment of a conjunctive set of \ref fluent in a given \ref kworld.
     *     @see \ref kworld::entails(const fluent_formula &) const
     *  
     * @param[in] to_check: the DNF \ref fluent_formula that has to be checked if entailed in \p world.
     * @param[in] world: the pointer to the \ref kworld where to check the entailment.
     *
     * @return true: \p to_check is entailed in \p world;
     * @return false: \p -to_check is entailed in \p world.*/
    bool entails(const fluent_formula & to_check, kworld_ptr world) const;

    /**
     *\brief Function that checks the entailment of a \ref belief_formula.
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
     * \todo self-loop?
     */
    bool entails(const belief_formula & to_check, kworld_ptr world) const;
    /**
     *\brief Function that checks the entailment of a \ref belief_formula on several possible starting points.
     * 
     * This function eases the task to check the entailment from several starting \ref kworld simultaneously.
     * This happens when we want to check the transitive closure of the \ref agent ag and to test the formula B(ag, phi).
     * 
     * @see belief_formula.
     * 
     * @param[in] to_check: the \ref belief_formula that has to be checked if entailed from \p world.
     * @param[in] worlds: the pointers to the set of \ref kworld where to start to check the entailment.
     *
     * @return true: \p to_check is entailed starting from all the \ref kworld in \p worlds;
     * @return false: \p -to_check is entailed starting from all the \ref kworld in \p worlds.
     * 
     * \todo self-loop?
     */
    bool entails(const belief_formula & to_check, const kworld_ptr_set & worlds) const;

    /**
     *\brief Function that returns all the reachable \ref kworld given the \ref agent and the staring \ref kworld.
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
     * \todo self-loop?
     */
    const kworld_ptr_set get_B_reachable_worlds(agent ag, kworld_ptr world) const;

    /**
     *\brief Function that returns all the reachable \ref kworld given the \ref agent and the staring \ref kworld.
     * 
     * This function modify the parameter so it's easy to check if a fixed point is reached.
     * This is useful for the operator C. Otherwise its better to use \ref get_B_reachable_worlds(agent, kworld_ptr) const.
     * 
     * @see belief_formula, get_B_reachable_worlds(agent, kworld_ptr) const and get_C_reachable_worlds(const agent_set &, kworld_ptr world) const.
     * 
     * @param[in] ag: the label of the \ref kedge that the function follows to check the transitivity.
     * @param[in] world: the pointers to the set of \ref kworld where to start to check the entailment.
     *
     * @param[out] reached: the set of pointers to all the reachable worlds.
     *
     * 
     * \todo self-loop?
     */
    bool get_B_reachable_worlds(agent ag, kworld_ptr world, kworld_ptr_set& reached) const;

    /**
     *\brief Function that returns all the reachable \ref kworld given a set of \ref agent and the staring \ref kworld.
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
     * \todo self-loop?
     */
    const kworld_ptr_set get_E_reachable_worlds(const agent_set & ags, kworld_ptr world) const;

    /**
     *\brief Function that returns all the reachable \ref kworld given a set of \ref agent and the staring \ref kworld.
     * 
     * This function modify the parameter so it's easy to check if a fixed point is reached.
     * This is useful for the operator C. Otherwise its better to use \ref get_E_reachable_worlds(const agent_set &, kworld_ptr) const.
     * 
     * @see \ref belief_formula, get_B_reachable_worlds(agent, kworld_ptr, kworld_ptr_set) const and get_C_reachable_worlds(const agent_set &, kworld_ptr world) const.
     * 
     * @param[in] ags: the set of label of the \ref kedge that the function follows to check the transitivity.
     * @param[in] world: the pointers to the set of \ref kworld where to start to check the entailment.
     *
     * @param[out] reached: the set of pointers to all the reachable worlds.
     *
     * 
     * \todo self-loop?
     */
    bool get_E_reachable_worlds(const agent_set &ags, kworld_ptr world, kworld_ptr_set& reached) const;

    /**
     *\brief Function that returns all the reachable \ref kworld (in the *Common Knowledge* sense) given a \ref agent and the staring \ref kworld.
     * 
     * This function applies get_E_reachable_worlds(const agent_set &, kworld_ptr) on its on result until a fixed point is found.
     * It returns all the worlds where is necessary to check if *phi* is entailed in order to verify C(\p ags, *phi*) and \p world is the starting point.
     * 
     * @see \ref belief_formula, and get_E_reachable_worlds(const agent_set &, kworld_ptr world).
     * 
     * @param[in] ags: the set of label of the \ref kedge that the function follows to check the *Common Knowledge* reachability.
     * @param[in] world: the pointers to the set of \ref kworld where to start to check the entailment.
     *
     * @return a set of pointers to all the reachable worlds.
     * 
     * \todo self-loop?
     */
    const kworld_ptr_set get_C_reachable_worlds(const agent_set &, kworld_ptr world) const;

public:

    /**
     *\brief Function that checks the entailment of a \ref fluent in *this*.
     *
     * The entailment of a \ref fluent in a kstate is true if the \ref fluent
     * is entailed by its pointed world (\ref m_pointed).
     * 
     * @see \ref entails(fluent, kworld_ptr) const.
     *  
     * @param[in] to_check: the \ref fluent that has to be checked if entailed in *this*.
     *
     * @return true: \p to_check is entailed in *this*;
     * @return false: \p -to_check is entailed in *this*.*/
    bool entails(fluent to_check) const;
    /**
     *\brief Function that checks the entailment of a conjunctive set of \ref fluent in *this*.
     *
     * The entailment of a conjunctive set of \ref fluent in a kstate is true if the conjunctive set of \ref fluent
     * is entailed by its pointed world (\ref m_pointed).
     * 
     * @see \ref entails(const fluent_set &, kworld_ptr) const.
     *  
     * @param[in] to_check: the conjunctive set of \ref fluent that has to be checked if entailed in *this*.
     *
     * @return true: \p to_check is entailed in *this*;
     * @return false: \p -to_check is entailed in *this*.*/
    bool entails(const fluent_set & to_check) const;
    /**
     *\brief Function that checks the entailment of a DNF \ref fluent_formula in *this*.
     *
     * The entailment of a DNF \ref fluent_formula in a kstate is true if the DNF \ref fluent_formula
     * is entailed by its pointed world (\ref m_pointed).
     * 
     * @see \ref entails(const fluent_formula &, kworld_ptr) const.
     *  
     * @param[in] to_check: the DNF \ref fluent_formula that has to be checked if entailed in *this*.
     *
     * @return true: \p to_check is entailed in *this*;
     * @return false: \p -to_check is entailed in *this*.*/
    bool entails(const fluent_formula & to_check) const;

    /**
     *\brief Function that checks the entailment of a \ref belief_formula in *this*.
     *
     * The entailment of a \ref belief_formula in a kstate is true if the \ref belief_formula
     * is entailed by its pointed world (\ref m_pointed).
     * 
     * @see \ref entails(const belief_formula&, kworld_ptr) const.
     *  
     * @param[in] to_check: the \ref belief_formula that has to be checked if entailed in *this*.
     *
     * @return true: \p to_check is entailed in *this*;
     * @return false: \p -to_check is entailed in *this*.*/
    bool entails(const belief_formula & to_check) const;

    /**
     *\brief Function that adds a \ref kworld to the Kripke structure represented by *this*.
     *
     * The \ref kworld is added. The structure only stores the pointer to the \ref kworld so
     * it is necessary to store the \ref kworld in \ref kstore.
     * 
     * @see kworld and kstore.
     *  
     * @param[in] to_add: the \ref kworld that has to be added to *this*.*/
    void add_world(const kworld & to_add);
    /**
     *\brief Function that adds a \ref kedge to the Kripke structure represented by *this*.
     *
     * The \ref kedge is added. The structure only stores the pointer to the \ref kedge so
     * it is necessary to store the \ref kedge in \ref kstore.
     * 
     * @see kedge and kstore.
     *  
     * @param[in] to_add: the \ref kedge that has to be added to *this*.*/
    void add_edge(const kedge & to_add);

    /**
     *\brief Function that builds the initial Kripke structure given the initial conditions.
     *
     * The building of the initial Kripke structure can be done in two different ways:
     *    - We build the complete Kripke structure and then we remove all the \ref kedge
     *      and \ref kworld that are not entailed by the initial conditions.
     *        -# If this method is the one is implemented the add functions in \ref kstore
     *           must be changed to not create the object every time but just to retrieve
     *           the object since all of them are already created.
     *    - A structural build:
     *         -# Compute the set of *phi* not know by \ref agent ag for each \ref agent;
     *         -# for each set compute the all the consistent \ref kworld where *phi* changes;
     *         -# create a \ref kedge between all this \ref kworld (transitively closed) labeled with ag;
     *         -# link all the classes of \ref kworld with an ongoing \ref kedge labeled with the ag of the reaching class.
     * 
     * @see kworld, kedge and initially.
     *  
     * @param[in] ini_conditions: the conditions on the pointed world and on the belief of the \ref agent in the initial state.
     *
     * \todo if first method is implemented the add functions in \ref kstore must be changed to not create 
     *       the object every time but just to retrieve the object since all of them are already created.*/
    void build_initial(const initially & ini_conditions);

    /**
     *\brief Function that checks if a given action is executable in *this*.
     *  
     * @see action.
     *  
     * @param[in] act: The action to be checked on *this*.
     * @return true: \p act is executable in *this*;
     * @return false: \p act is not executable in *this*.*/
    bool is_executable(const action & act) const;

    /**
     *\brief Function that applies the transition function on *this* given an action.
     *
     * The transition function is applied accordingly to mA*. Check the paper for more information.
     *  
     * @see action.
     *  
     * @param[in] act: The action to be applied on *this*.
     * @return the \ref kstate that results after the execution.
     * 
     * \todo The action must be executable on *this* otherwise it will return a null_ptr.*/
    const kstate& compute_succ(const action & act);
};

