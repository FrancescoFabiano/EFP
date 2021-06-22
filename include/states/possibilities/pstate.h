/**
 * \brief Class that represents a possibility.
 *
 * \details  A possibility is one of the possible states description for EFP. It represent a single state in the
 *  mA^rho language.
 * 
 * @see pworld and pstore.
 *
 * \todo general: the && parameter in set (move) and in other places, is it better?
 *
 * \copyright GNU Public License.
 *
 * \author Francesco Fabiano.
 * \date September 16, 2019
 */
#pragma once

#include "pworld.h"
#include "pstore.h"

#include "../../utilities/define.h"
#include "../../domain/initially.h"
#include "../../actions/action.h"
#include "../../../lib/bisimulation/bisimulation.h"

class pstate
{
private:
    /** \brief The set containing a pointer to each world of the structure.
     *
     *  \todo: forse basta solo m_pointed.
     *
     * @see pworld and pstore.*/
    pworld_ptr_set m_worlds;
    /** \brief The pointer to the *pointed world*.
     * 
     * @see pworld and pstore.*/
    pworld_ptr m_pointed;
    //    /** \brief The beliefs of each \ref agent in every \ref pworld.
    //     *
    //     * @see pworld.*/
    //    pedges m_beliefs;

    /** \brief Function that returns all the reachable \ref pworld given the \ref agent and the staring \ref pworld.
     * 
     * This function finds all the world that are transitively reachable from the starting world following
     * the edges labelled with the given \ref agent. It returns all the worlds where is necessary to check if
     * *phi* is entailed in order to verify B(\p ag, *phi*) and \p world is the starting point.
     * 
     * @see \ref belief_formula.
     * 
     * @param[in] ag: the label of the \ref kedge that the function follows to check the transitivity.
     * @param[in] world: the pointers to the set of \ref pworld where to start to check the entailment.
     *
     * @return a set of pointers to all the reachable worlds.
     *
     * 
     * \todo self-loop?*/
    static pworld_ptr_set get_B_reachable_worlds(const agent& ag, const pworld_ptr & world);

    /** \brief Function that returns all the reachable \ref pworld given the \ref agent and the staring \ref pworld.
     * 
     * This function modify the parameter so it's easy to check if a fixed point is reached.
     * This is useful for the operator C. Otherwise its better to use \ref get_B_reachable_worlds(agent, pworld_ptr) const.
     * 
     * @see belief_formula, get_B_reachable_worlds(agent, pworld_ptr) const and get_C_reachable_worlds(const agent_set &, pworld_ptr world) const.
     * 
     * @param[in] ag: the label of the \ref kedge that the function follows to check the transitivity.
     * @param[in] world: the pointers to the set of \ref pworld where to start to check the entailment.
     *
     * @param[out] ret: the set of pointers to all the reachable worlds.
     *
     * 
     * \todo self-loop?*/
    static bool get_B_reachable_worlds_recursive(const agent& ag, const pworld_ptr & world, pworld_ptr_set & ret);

    /** \brief Function that returns all the reachable \ref pworld given a set of \ref agent and the staring \ref pworld.
     * 
     * This function applies get_E_reachable_worlds(agent , pworld_ptr) with all the agents inside the
     * given set and return the union of the reached \ref pworld. It returns all the worlds where is necessary to check if
     * *phi* is entailed in order to verify E(\p ags, *phi*) and \p world is the starting point.
     * 
     * @see \ref belief_formula and get_B_reachable_worlds(agent, pworld_ptr).
     * 
     * @param[in] ags: the set of label of the \ref kedge that the function follows to check the transitivity.
     * @param[in] world: the pointers to the set of \ref pworld where to start to check the entailment.
     *
     * @return a set of pointers to all the reachable worlds.
     *
     * 
     * \todo self-loop?*/
    static pworld_ptr_set get_E_reachable_worlds(const agent_set & ags, const pworld_ptr & world);

    /** \brief Function that returns all the reachable \ref pworld given a set of \ref agent and the staring \ref pworld.
     * 
     * This function modify the parameter so it's easy to check if a fixed point is reached.
     * This is useful for the operator C. Otherwise its better to use \ref get_E_reachable_worlds(const agent_set &, pworld_ptr) const.
     * 
     * @see \ref belief_formula, get_B_reachable_worlds(agent, pworld_ptr, pworld_ptr_set) const and get_C_reachable_worlds(const agent_set &, pworld_ptr world) const.
     * 
     * @param[in] ags: the set of label of the \ref kedge that the function follows to check the transitivity.
     * @param[in] worlds: the pointers to the set of \ref pworld where to start to check the entailment.
     *
     * @param[out] reached: the set of pointers to all the reachable worlds.
     *
     * 
     * \todo self-loop?*/
    static bool get_E_reachable_worlds_recoursive(const agent_set &ags, const pworld_ptr_set & worlds, pworld_ptr_set & reached);

    /** \brief Function that returns all the Distributed reachable \ref pworld given a set of \ref agent and the staring \ref pworld.
     * 
     * This function finds the intersection of the worlds reachable by all the agents in \ref ags starting from \ref world
     * @param[in] ags: the set of label of the \ref kedge that the function follows to check the transitivity.
     * @param[in] world: the pointers to the set of \ref pworld where to start to check the entailment.
     *
     * @return a set of pointers to all the D_reachable worlds.
     *
     * 
     * \todo self-loop?*/
    static pworld_ptr_set get_D_reachable_worlds(const agent_set & ags, const pworld_ptr & world);

    /** \brief Function that returns all the reachable \ref pworld (in the *Common Knowledge* sense) given a \ref agent and the staring \ref pworld.
     * 
     * This function applies get_E_reachable_worlds(const agent_set &, pworld_ptr) on its on result until a fixed point is found.
     * It returns all the worlds where is necessary to check if *phi* is entailed in order to verify C(\p ags, *phi*) and \p world is the starting point.
     * 
     * @see \ref belief_formula, and get_E_reachable_worlds(const agent_set &, pworld_ptr world).
     * 
     * @param[in] ags: the set of label of the \ref kedge that the function follows to check the *Common Knowledge* reachability.
     * @param[in] world: the pointers to the set of \ref pworld where to start to check the entailment.
     *
     * @return a set of pointers to all the reachable worlds.
     * 
     * \todo self-loop?*/
    pworld_ptr_set get_C_reachable_worlds(const agent_set & ags, const pworld_ptr & world) const;

    /** \brief Function that builds the initial Kripke structure given the initial conditions in a structural way.
     *
     *         -# Compute the set of *phi* not know by \ref agent ag for each \ref agent;
     *         -# for each set compute the all the consistent \ref pworld where *phi* changes;
     *         -# create a \ref kedge between all this \ref pworld (transitively closed) labeled with ag;
     *         -# link all the classes of \ref pworld with an ongoing \ref kedge labeled with the ag of the reaching class.
     * 
     * @see pworld, kedge and initially.*/
    void build_initial_structural();

    /** \brief Function that builds the initial Kripke structure given the initial conditions pruning from a complete K structure.
     *
     *    - Thanks to \ref generate_initial_pworlds we generate all the possible worlds.
     *  We than store all the ones that respect \p ini_conditions *this* and all the other only in \ref pstore.
     * 
     * @see pworld, kedge and initially.
     *  
     * \todo If this method is the one is implemented the add functions in \ref pstore must be changed to not create the object
     *       every time but just to retrieve the object since all of them are already created.*/
    void build_initial_prune();
    /** \brief Function used to build all the possible permutations of the \ref domain's \ref fluent.
     *  
     * The function creates all the permutation recursively and adds to *this* only the ones that respect \p ini_conditions.
     * All the other ones are stored in \ref pstore for future uses.
     * 
     * @param[out] permutation: the permutation in construction.
     * @param[in] index: the index of the \ref fluent to add.
     * @param[in] index: the set of initialky known \ref fluent to reduce the possible permutations.*/

    void generate_initial_pworlds(fluent_set& permutation, int index, const fluent_set & initially_known);
    /** \brief Function check if the \ref pworld respect the initial conditions and adds it to *this* if it does.
     *  
     * The function checks if the \ref pworld created by \ref generate_initial_pworlds(fluent_set&, int)
     * entails the initial conditions and:
     * - If it does the \ref pworld is added to *this* (and to \ref pstore).
     * - If it doesn't the \ref pworld is added only to \ref pstore.
     * 
     * @param[in] possible_add: the \ref pworld to check.*/
    void add_initial_pworld(const pworld & possible_add);

    /** \brief Function used to build the \ref kedge of the initial \ref pstate.
     *  
     * The function creates all the edges and adds to *this* only the ones that respect \p ini_conditions.
     * All the other ones are stored in \ref pstore for future usees.*/
    void generate_initial_pedges();
    /** \brief Function that removes the belief of \ref agent "ag" from \ref pworld "from" to \ref pworld "to".
     * 
     * @param[in] from: the \ref pworld in which \ref agent "ag" currently is
     * @param[in] to: the \ref pworld to remove
     * @param[in] ag: the \ref agent.*/
    static void remove_edge(const pworld_ptr & from, const pworld_ptr & to, const agent & ag);
    /** \brief Function that removes the \ref pedge(s) that imply that \p ag is ignorant about \p known_ff from *this*.
     *  
     * @param[in] known_ff: the \ref fluent_formula known by \p ag.
     * @param[in] ag: the \ref agent that knows \p known_ff.*/
    void remove_initial_pedge(const fluent_formula &known_ff, const agent& ag);
    /** \brief Function check if a belief_formula removes some edges from the initial state.
     *  
     * For example whenever is B(i, *phi*) \/ B(i, -*phi*) is necessary to remove the edges
     * labeled with i that link \ref pworld that entails *phi* and \ref pworld -*phi*.
     * 
     * @param[in] to_check: the \ref belief_formula to check.*/
    void remove_initial_pedge_bf(const belief_formula &to_check);

    automaton pstate_to_automaton(std::vector<pworld_ptr> & pworld_vec, const std::map<agent, bis_label> & agent_to_label) const;

    void automaton_to_pstate(const automaton & a, const std::vector<pworld_ptr> & pworld_vec, const std::map<bis_label, agent> & label_to_agent);

    //  bool check_reached(agent ag, const pworld_ptr & start, const pworld_ptr & end) const;

public:

    /** \brief Setter of the field \ref m_worlds.
     *
     * @param[in] to_set: the \ref pworld_ptr_set to assign to \ref m_worlds.*/
    void set_worlds(const pworld_ptr_set & to_set);
    /** \brief Setter of the field \ref m_pointed.
     *
     * @param[in] to_set: the \ref pworld_ptr to assign to \ref m_pointed.*/
    void set_pointed(const pworld_ptr & to_set);
    //    /** \brief Setter of the field \ref m_beliefs.
    //     *
    //     * @param[in] to_set: the \ref pedges to assign to \ref m_beliefs.*/
    //    void set_beliefs(const pedges & to_set);

    /** \brief Getter of the field \ref m_worlds.
     *
     * @return: the value of \ref m_worlds.*/
    const pworld_ptr_set & get_worlds() const;
    /** \brief Getter of the field \ref m_pointed.
     *
     * @return: the value of \ref m_pointed.*/
    const pworld_ptr & get_pointed() const;
    /** \brief Getter of the field \ref m_beliefs.
     *
     * @return: the value of \ref m_beliefs.*/
    const information_state & get_beliefs(const pworld_ptr & world) const;

    /** \brief Function that checks the entailment of a \ref fluent in a given \ref pworld.
     *     @see \ref pworld::entails(fluent) const
     *
     * @param[in] to_check: the \ref fluent that has to be checked if entailed in \p world.
     * @param[in] world: the pointer to the  \ref pworld where to check the entailment.
     *
     * @return true: \p to_check is entailed in \p world;
     * @return false: \p -to_check is entailed in \p world.*/
    static bool entails(const fluent & to_check, const pworld_ptr & world);
    /**
     *\brief Function that checks the entailment of a conjunctive set of \ref fluent in a given \ref pworld.
     *     @see \ref pworld::entails(const fluent_set &) const
     *
     * @param[in] to_check: the conjunctive set of \ref fluent that has to be checked if entailed in \p world.
     * @param[in] world: the pointer to the  \ref pworld where to check the entailment.
     *
     * @return true: \p to_check is entailed in \p world;
     * @return false: \p -to_check is entailed in \p world.*/
    static bool entails(const fluent_set & to_check, const pworld_ptr & world);
    /**
     *\brief Function that checks the entailment of a conjunctive set of \ref fluent in a given \ref pworld.
     *     @see \ref pworld::entails(const fluent_formula &) const
     *
     * @param[in] to_check: the DNF \ref fluent_formula that has to be checked if entailed in \p world.
     * @param[in] world: the pointer to the \ref pworld where to check the entailment.
     *
     * @return true: \p to_check is entailed in \p world;
     * @return false: \p -to_check is entailed in \p world.*/
    static bool entails(const fluent_formula & to_check, const pworld_ptr & world);

    /** \brief Function that checks the entailment of a \ref belief_formula.
     *
     * The concept of entailment of a \ref belief_formula is related to reachability
     * and so an initial state is required, the given \ref pworld \p world is the starting point
     * (it simulates the pointed world) where the entailment check starts from. The
     * world needs to be parametric to not create a new \ref pstate for every iteration of the formula;
     * now we can just change the starting point and keep the same \ref pstate when testing, for example,
     * *nested knowledge*. The function is private because the entailment always starts from the \ref m_pointed and then
     * moves to the reachable (with the operators) \ref pworld.
     *
     * @see \ref belief_formula and entails(const belief_formula &) const.
     *
     * @param[in] to_check: the \ref belief_formula that has to be checked if entailed from \p world.
     * @param[in] world: the pointer to the \ref pworld where to start to check the entailment.
     *
     * @return true: \p to_check is entailed starting from \p world;
     * @return false: \p -to_check is entailed starting from \p world.
     *
     * \todo self-loop?*/
    bool entails(const belief_formula & to_check, const pworld_ptr & world) const;
    /** \brief Function that checks the entailment of a \ref belief_formula on several possible starting points.
     *
     * This function eases the task to check the entailment from several starting \ref pworld simultaneously.
     * This happens when we want to check the transitive closure of the \ref agent ag  to test the formula B(ag, phi).
     *
     * @see belief_formula.
     *
     * @param[in] to_check: the \ref belief_formula that has to be checked if entailed from \p world.
     * @param[in] worlds: the pointers to the set of \ref pworld where to start to check the entailment.
     *
     * @return true: \p to_check is entailed starting from all the \ref pworld in \p worlds;
     * @return false: \p -to_check is entailed starting from all the \ref pworld in \p worlds.
     *
     * \todo self-loop?*/
    bool entails(const belief_formula & to_check, const pworld_ptr_set & worlds) const;
    /** \brief Function that checks the entailment of a \ref formula_list (CNF of \ref belief_formula).
     *
     *
     * @param[in] to_check: the \ref formula_list that has to be checked if entailed starting from from \p world.
     * @param[in] world: the pointer to the \ref pworld where to start to check the entailment.
     *
     * @return true: \p to_check is entailed starting from \p world;
     * @return false: \p -to_check is entailed starting from \p world.
     *
     * \todo self-loop?*/
    bool entails(const formula_list & to_check, const pworld_ptr & world) const;

    pworld_ptr add_world(const pworld & world);

    /** \brief Function that adds the belief of agent "ag" for the \ref pworld "pw" *this*.
     *
     * @param[in] from: the \ref pworld in which \ref agent "ag" currently is
     * @param[in] to: the \ref pworld believed from \ref agent "ag"
     * @param[in] ag: the \ref agent.*/
    void add_edge(const pworld_ptr & from, const pworld_ptr & to, const agent& ag);

    /** \brief Function that checks the entailment of a \ref fluent in *this*.
     *
     * The entailment of a \ref fluent in a pstate is true if the \ref fluent
     * is entailed by its pointed world (\ref m_pointed).
     * 
     * @see \ref entails(fluent, pworld_ptr) const.
     *  
     * @param[in] to_check: the \ref fluent that has to be checked if entailed in *this*.
     *
     * @return true: \p to_check is entailed in *this*;
     * @return false: \p -to_check is entailed in *this*.*/
    bool entails(const fluent & to_check) const;
    /** \brief Function that checks the entailment of a conjunctive set of \ref fluent in *this*.
     *
     * The entailment of a conjunctive set of \ref fluent in a pstate is true if the conjunctive set of \ref fluent
     * is entailed by its pointed world (\ref m_pointed).
     * 
     * @see \ref entails(const fluent_set &, pworld_ptr) const.
     *  
     * @param[in] to_check: the conjunctive set of \ref fluent that has to be checked if entailed in *this*.
     *
     * @return true: \p to_check is entailed in *this*;
     * @return false: \p -to_check is entailed in *this*.*/
    bool entails(const fluent_set & to_check) const;
    /**
     *\brief Function that checks the entailment of a DNF \ref fluent_formula in *this*.
     *
     * The entailment of a DNF \ref fluent_formula in a pstate is true if the DNF \ref fluent_formula
     * is entailed by its pointed world (\ref m_pointed).
     * 
     * @see \ref entails(const fluent_formula &, pworld_ptr) const.
     *  
     * @param[in] to_check: the DNF \ref fluent_formula that has to be checked if entailed in *this*.
     *
     * @return true: \p to_check is entailed in *this*;
     * @return false: \p -to_check is entailed in *this*.*/
    bool entails(const fluent_formula & to_check) const;

    /**
     *\brief Function that checks the entailment of a \ref belief_formula in *this*.
     *
     * The entailment of a \ref belief_formula in a pstate is true if the \ref belief_formula
     * is entailed by its pointed world (\ref m_pointed).
     * 
     * @see \ref entails(const belief_formula&, pworld_ptr) const.
     *  
     * @param[in] to_check: the \ref belief_formula that has to be checked if entailed in *this*.
     *
     * @return true: \p to_check is entailed in *this*;
     * @return false: \p -to_check is entailed in *this*.*/
    bool entails(const belief_formula & to_check) const;

    /** \brief Function that builds the initial Kripke structure given the initial conditions.
     *
     * The building of the initial Kripke structure can be done in two different ways:
     *    - A method using a pruning techniques(\ref build_initial_prune).
     *    - A structural build (\ref build_initial_structural).
     * 
     * @see pworld, kedge and initially.
     * 
     * \todo if first method is implemented the add functions in \ref pstore must be changed to not create 
     *       the object every time but just to retrieve the object since all of them are already created.*/
    void build_initial();

    /** \brief Function that applies the transition function on *this* given an action.
     *
     * The transition function is applied accordingly to mA^rho. Check the paper for more information.
     *  
     * @see action.
     *  
     * @param[in] act: The action to be applied on *this*.
     * @return the \ref pstate that results after the execution.
     * 
     * \todo The action must be executable on *this* otherwise it will return a null_ptr.*/
    pstate compute_succ(const action & act) const;

    /** \brief sub-Function of \ref compute_succ that checks if the successor respects the epistemic properties after an \ref action execution.
     * 
     * For the moment this is only applied to sensing/announcement
     * 
     * 
     * @param fully: the set of fully observant w.r.t. *this* e-state update.
     * @param partially: the set of partially observant w.r.t. *this* e-state update.
     * @param effects: the effects of the action execution on *this*.
     * @param updated: the e-state after the action execution.
     * @return true: \p *this* respects all the properties.
     * @return false: \p *this* does not respect all the properties.*/
    bool check_properties(const agent_set & fully, const agent_set & partially, const fluent_formula & effects, const pstate & updated) const;

    /** \brief Function that makes *this* the mimimum \ref poss that is bisimilar to the current one.
     *
     * Not yet implemented.*/
    void calc_min_bisimilar();

    /** \brief The copy operator.
     *   
     * @param [in] to_copy: the \ref pstate to assign to *this*.
     * @return true: if \p the assignment went ok.
     * @return false: otherwise.*/
    bool operator=(const pstate & to_copy);

    /**\brief The operator < for set operation in \ref planning_graph.
     *
     * The ordering is based on pworlds and on the beliefs map.
     * 
     * @param[in] to_compare: the \ref kstate to check for odering.
     * 
     * @return true: if *this* is smaller than \p to_compare.
     * @return false: otherwise.*/
    bool operator<(const pstate & to_compare) const;

    /** \brief Function that prints the information of *this*.*/
    void print() const;

    /** \brief Function that prints the information of *this* in a Graphviz file.
     *
     * @param[in] graphviz: the ostream where to print the info of *this*.*/
    void print_graphviz(std::ostream& graphviz) const;
};
