#ifndef EFP_DOMAIN_CONFIG_DEF_H
#define EFP_DOMAIN_CONFIG_DEF_H

/** \brief The possible heuristics applicable to the domain.*/
enum Heuristic {
    NO_H, /**< Breadth first search is executed*/
    L_PG, /**< A planning graph is used to calculate the distance of each state from the goal.
                    *
                    * For now for every state is computed a planning graph and then the length of it is returned.
                    *
                    * \todo Optimize and maybe create only one planning graph and check where the state belongs.
                    * .*/
    S_PG, /**< A planning graph is used to calculate the sub-goals distance of each state from the goal.
                    *
                    * For now for every state is computed a planning graph and then the sum of the distances of each sub-goals is returned
                    *
                    * \todo Optimize and maybe create only one planning graph and check where the state belongs.
                    * .*/
    C_PG, /**< A single planning graph is used to calculate the distance of each grounded belief formula from initial state and goal.
                    *
                    * This follows the idea of converting belief formula in fluent and just execute classical planning graph.
                    * .*/
    SUB_GOALS /**< For each state is calculate the number of (found or) missing sub-goals
                    *
                    * The group operator C is split in more belief_formulae.*/
};

enum Search_Strategy {
    BFS, /** Breadth first search is executed*/
    DFS, /** Depth first search is executed*/
    ITER_DFS /** Iterative Depth first search is executed*/
};

/** \brief The possible types of bisimulation algorithms.*/
enum Bisimulation_Algorithm {
    NO_BISIMULATION, /**< No Bisimulation is used*/
    PAIGE_TARJAN, /**< The standard Paige-Tarjan Algorithm..*/
    FAST_BISIMULATION /**< The Fast-Bisimulation Algorithm introduced by Dovier et al, 2001.*/
};

/** \brief The possible restriction applicable to the domain.
 *
 * These restrictions have mostly to do with the Kripke representation and
 * with the type of inference that you could do on them.
 *
 * It has been introduced because it helps with the computation of the initial state (Finitary S5).*/

enum Initial_State_Mode {
    FINITARY_S5_THEORY,
    CUSTOM_INITIAL_STATE
};

/** \brief The possible states representation.
 *
 * Different state representation imply different advantages.
 *
 * This type is used to allocate the class \ref state with the chosen representation.
 *
 */
enum state_type
{
    KRIPKE, /**< \brief The default state representation, through Kripke structures.
            *
            * It encodes the mA* semantic and bases its entailment and its transition function
            * on reachability on Kripke structure. @see kstate.*/
    POSSIBILITIES, /**< \brief State representation based on possibilities and nwf-set theory.
            *
            * It encodes the mA^rho semantic and bases its entailment and its transition function
            * on possibilities. @see poss_state. */
    OBDD /**< \brief State representation based on OBDD.
            *
            *\todo Yet to study.*/
};

/** \brief The possible way of checking an action effects executability.*/
enum action_check
{
    EXE_POINTED__COND_POINTED, /**< \brief Both the executability and the conditional effects are checked only on the state.*/
    EXE_POINTED__COND_WORLD, /**< \brief The executionability is checked only on the state but the conditional effects are checked in every worlds.*/
    EXE_WORLD__COND_WORLD/**< \brief Both the executability and the conditional effects are checked in every world.*/
};

enum logic {
    K,
    KD45,
    S5
};

enum spec_lang_type
{
    EPDDL, /**< The E-PDDL language is used to speficy the input file*/
    MAR /**< The mAp language is used to speficy the input file*/
};

enum up_model_type
{
    STANDARD, /**< The transition function follows the standard ontic,sensing,announcement update models*/
    CUSTOM /**< The transition function follows the update models defined by the user*/
};

#endif //EFP_DOMAIN_CONFIG_DEF_H
