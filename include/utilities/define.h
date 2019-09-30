/**
 * \class define
 * \brief Class containing most of the custom types and symbols defined for the planner.
 *
 *
 * \copyright GNU Public License.
 *
 * \author Francesco Fabiano.
 * \date March 31, 2019
 */

#pragma once

#include <string>
#include <iostream>
#include <vector>
#include <set>
#include <map>
#include <queue>
#include <memory>


/*****************************************************************
 * Constants
 ****************************************************************/
#define NEGATION_SYMBOL "-" /**< \brief The negation symbol to negate the \ref fluent */

/****************************************************************
 * Non class specific
 ****************************************************************/
typedef std::set<std::string> string_set; /**< \brief A representation of a conjunctive set of \ref fluent.
                                            *
                                            * Each element of the formula is a std::string (not grounded) */
typedef std::set<string_set> string_set_set; /**< \brief A representation of a formula in DNF formula.
                                              *
                                              * Each element of the formula is a \ref string_set (not grounded) */

/****************************************************************
 * Domain Related
 ****************************************************************/
typedef unsigned short fluent; /**< \brief A representation of a fluent through an unique id (short).
                                * 
                                * This representation is derived by applying \ref grounder::ground_fluent(const std::string&) const
                                * to the elements of \ref reader::m_fluents.
                                *
                                * The unique id of the readed fluent is always an odd number while to
                                * obtain the negated version is necessary to add 1.
                                *
                                * \todo is maybe better to store only the positive?
                                */
typedef std::set<fluent> fluent_set; /**< \brief A representation of a conjunctive set of \ref fluent.
                                      * 
                                      * This representation is used, for example, to represent
                                      * a world \ref kworld in a Kripke structure (\ref kstate).
                                      *
                                      * Each element of the set is a \ref fluent*/
typedef std::set<fluent_set> fluent_formula; /**< \brief A representation of a fluent formula in DNF.
                                      *
                                      * This representation is used, for example, to represent
                                      * a set of world (\ref kworld) of a Kripke structure (\ref kstate).
                                      *
                                      * \todo How the set of fluent has < implemented?
                                      * Each element of the set is a \ref fluent_set*/

typedef unsigned short agent; /**< \brief A representation of an agent through an unique id (short).
                                      *
                                      *  This representation is derived by applying \ref grounder::ground_agent(const std::string&) const
                                      * to the element of \ref reader::m_agents.*/
typedef std::set<agent> agent_set; /**< \brief A set of \ref agent.*/
typedef std::vector<agent> agent_list; /**< \brief A list of \ref agent.*/

typedef unsigned short action_id; /**< \brief The unique id (short) associated with each action.
                                      *
                                      *  This id is derived by applying \ref grounder::ground_action(const std::string&) const
                                      * to the element of \ref reader::m_actions.*/
typedef std::vector<action_id> action_id_list; /**< \brief A list of \ref action_id.*/

/*****************Grounder*****************/
typedef std::map<std::string, fluent> fluent_map; /**< \brief The map that associates to each \ref fluent name its grounded value.
                                                           * 
                                                           * @see grounder.*/
typedef std::map<std::string, agent> agent_map; /**< \brief The map that associates to each \ref agent name its grounded value.
                                                           * 
                                                           * @see grounder.*/
typedef std::map<std::string, action_id> action_name_map; /**< \brief The map that associates to each \ref agent name its grounded value.
                                                           * 
                                                           * @see grounder.*/

typedef std::map<fluent, std::string> reverse_fluent_map; /**< \brief The map that associates to each \ref fluent its name (for speed).
                                                           * 
                                                           * @see grounder.*/
typedef std::map<agent, std::string> reverse_agent_map; /**< \brief The map that associates to each \ref agent its name (for speed).
                                                           * 
                                                           * @see grounder.*/
typedef std::map<action_id, std::string> reverse_action_name_map; /**< \brief The map that associates to each \ref action_id its name (for speed).
                                                           * 
                                                           * @see grounder.*/

/** \brief The possible restriction applicable to the domain.
 *
 * These restrictions have mostly to do with the Kripke representation and
 * with the type of inference that you could do on them.
 *
 * It has been introduced because it helps with the computation of the initial state (Finitary S5).*/
enum domain_restriction
{
    S5, /**< \brief Restricts only to S5 models.
         *
         * This is the default Initial description (\ref initially).
         * When this flag is on only C(phi) are accepted and they must cover all the fluents.
         * The possible cases are:
         * - *phi* -> all worlds must entail *phi*.
         * - C(B(i,*phi*)) -> all worlds must entail *phi*.
         * - C(B(i,*phi*) \ref BF_OR B(i,-*phi*)) -> only edges conditions.
         * - C(-B(i,*phi*) \ref BF_AND -B(i,-*phi*)) -> only edges conditions.
         *
         * With this type of model is associate the concept of *Knowledge* given
         * that the model gurantee the relations to be equivalent.*/
    K45, /**< \brief Restricts only to K45 models.
         *
         * With this type of model is associate the concept of *Belief*.*/
    NONE, /**< \brief No restrictions applied.
         */
    NONEG, /**< \brief No negative belief_formula are accepted.
            *
            * This is used when the planning graph heuristic is involved
            * because it cannot deal with negative \ref belief_formula
            * in the goal (no mutex yet).*/
    RESTRICTION_FAIL, /**< \brief The default case to guarantee consistency.*/
};

/** \brief The possible way of checking an action effects executability.*/
enum action_check
{
    EXE_POINTED__COND_POINTED,/**< \brief Both the executability and the conditional effects are checked only on the state.*/
    EXE_POINTED__COND_WORLD,/**< \brief The executionability is checked only on the state but the conditional effects are checked in every worlds.*/
    EXE_WORLD__COND_WORLD/**< \brief Both the executability and the conditional effects are checked in every world.*/
};

/****************************************************************
 * Actions Related
 ****************************************************************/
class belief_formula;
typedef std::map<agent, fluent_formula> observability_map; /**< \brief Used to express the obsverbability conditions.
                                * 
                                * Each element associates an \ref agent to the observability conditions for an \ref action.*/
//Associate each effect the condition
typedef std::map<fluent_formula, belief_formula> effects_map; /**< \brief Used to express the conditions of an action effects.
                                * 
                                * Each element associates an \ref action effect to its conditions.*/

enum event_type
{
    EPSILON,/**< \brief The null event.*/
    SIGMA,/**< \brief Event corresponding to ... */
    TAU/**< \brief  Event corresponding to ... */
};

typedef std::set<event_type> event_type_set;
typedef std::set<std::pair<event_type, event_type>> event_type_relation;

/****************************************************************
 * States Related
 ****************************************************************/

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

/*****************Kripke States*****************/
class kedge;
typedef std::string kedge_id; /**< \brief The id of an \ref kedge in a \ref kstate.
                                * 
                                * The id is calculated through an hash (kedge::hash_info_into_id()) of the info of the \ref kedge.
                                *
                                * This id is used to extract the \ref kedge from the \ref kstore to not create a new every time
                                * but to access the already created ones.
                                * @see kedge and kstore*/
/*typedef std::shared_ptr<const kedge> kedge_ptr;*/ /*< \brief A std::shared_pointer to a \ref kedge usually stored in \ref kstore.
                                                * @see kedge and kstore.
                                                * \todo transform into class for < into set, also add = and empty constructor, operator*,operator ->, = with string.*/
class kedge_ptr;
typedef std::set<kedge_ptr> kedge_ptr_set; /**< \brief A set of \ref kedge_ptr.
                                            * 
                                            * Mainly used to store all the \ref kedge of a \ref kstate without wasting memory.
                                            *
                                            * \todo The operator < for std::shared_ptr must be implemented
                                            * @see kedge and kstate*/

class kworld;
typedef std::string kworld_id; /**< \brief The id of a \ref kworld in a \ref kstate.
                                * 
                                * The id is calculated through an hash (kworld::hash_fluents_into_id()) of the info of the \ref kworld.
                                *
                                * This id is used to extract the \ref kworld from the \ref kstore to not create a new every time
                                * but to access the already created ones.
                                * @see kworld and kstore*/
/*typedef std::shared_ptr<const kworld> kworld_ptr;*/ /*< \brief A std::shared_pointer to a \ref kworld usually stored in \ref kstore.
                                                   * 
                                                   * @see kworld and kstore.
                                                   * \todo transform into class for < into set.*/
class kworld_ptr;
typedef std::set<kworld_ptr> kworld_ptr_set; /**< \brief A set of \ref kworld_ptr.
                                            * 
                                            * Mainly used to store all the \ref kworld of a \ref kstate without wasting memory.
                                            *
                                            * \todo The operator < for std::shared_ptr must be implemented.
                                            * @see kworld and kstate.*/

typedef std::map<std::pair<kworld_ptr, event_type>, kworld_ptr> kstate_map;

/*****************Possibilities*****************/
class pworld;
typedef std::string pworld_id; /**< \brief The id of a \ref pworld in a \ref pstate.
                                *
                                * The id is calculated through an hash (pworld::hash_fluents_into_id()) of the info of the \ref pworld.
                                *
                                * This id is used to extract the \ref pworld from the \ref pstore to not create a new every time
                                * but to access the already created ones.
                                * @see pworld and pstore*/
/*typedef std::shared_ptr<const pworld> pworld_ptr;*/ /*< \brief A std::shared_pointer to a \ref pworld usually stored in \ref pstore.
                                                   *
                                                   * @see pworld and pstore.
                                                   * \todo transform into class for < into set.*/
class pworld_ptr;
typedef std::set<pworld_ptr> pworld_ptr_set; /**< \brief A set of \ref pworld_ptr.
                                            *
                                            * Mainly used to store all the \ref pworld of a \ref pstate without wasting memory.
                                            *
                                            * \todo The operator < for std::shared_ptr must be implemented.
                                            * @see pworld and pstate.*/

typedef std::map<agent, pworld_ptr_set> pworld_map; /**< \brief A map between agents and set of \ref pworld_ptr.
                                                    *
                                                    * Mainly used to store all the \ref pworld of a \ref pstate without wasting memory,
                                                    * while keeping information about the agents' beliefs.
                                                    *
                                                    * @see pworld and pstate.*/

typedef std::queue<pworld_ptr> pworld_queue; /**< \brief A queue of \ref pworld_ptr.
                                            *
                                            * \todo The operator < for std::shared_ptr must be implemented.
                                            * @see pworld and pstate.*/

typedef std::map<pworld_ptr, pworld_ptr> transition_map; /**< \brief A map that keeps track of the results of the transition function.
                                                          *
                                                          * @see pworld and pstate.*/

typedef std::vector<std::tuple<pworld_ptr, pworld_ptr, agent>> beliefs_vector;/**< \brief A vector of tuples (pw1, pw2, ag) that represent that in \ref pworld pw1 the \agent ag believes that the \ref \pworld pw2 is possible
                                                                               *
                                                                               * @see pworld and pstate.*/
