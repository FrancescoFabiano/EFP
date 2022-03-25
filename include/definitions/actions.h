#ifndef EFP_ACTIONS_H
#define EFP_ACTIONS_H

#include "domain_def.h"

class action;
typedef std::set<action> action_set; /**< \brief A representation a set of \ref action.*/

typedef std::vector<action> action_list; /**< \brief A representation of a sequential executution of \ref action*/

class belief_formula;

typedef short agent_group_id;
/**< \brief id that captures a generic agent group
 *
 * In the specific case when we are instantiating mAl domains this is:
 * - 0 for fully observant
 * - 1 for partially observant
 * - 2 for oblivious
 * .*/
typedef short act_type; /**< \brief id that captures a generic action type
 *
 * In the specific case when we are instantiating mAl domains this is:
 * - 0 for ontic
 * - 1 for sensing
 * - 2 for announcement
 * .*/

typedef std::map<agent, agent_group_id> single_observability_map;
/**< \brief Used to map a single agent to his observability group w.r.t.an action execution.*/

typedef std::map<agent, std::map<agent_group_id, belief_formula> > observability_map; /**< \brief Used to express the obsverbability conditions.
                                *
                                * Each element associates an \ref agent to the observability conditions for an \ref eff.*/
//Associate each effect the condition
typedef std::map<fluent_formula, belief_formula> effects_map; /**< \brief Used to express the conditions of an action effects.
                                *
                                * Each element associates an \ref action effect to its conditions.*/

typedef std::map<agent, belief_formula> mal_observability_map; /**< \brief Used to express the obsverbability conditions in mal where the obs groups are fixed.
                                *
                                * Each element associates an \ref agent to the observability conditions for an \ref eff.*/

enum mal_type /* Not necessary, to clean up the code*/
{
    ONTIC, /**< \brief Used when the proposition specifies the effects of an ontic action -- *act* **causes** *f*.*/
    SENSING, /**< \brief Used when the proposition specifies the effects of a sensing action -- *act* **sensed** *f*.*/
    ANNOUNCEMENT, /**< \brief Used when the proposition specifies the effects of a announcement action -- *act* **annaunces** *ff* */
    OBSERVANCE, /**< \brief Used when the proposition specifies the full observability conditions of an action -- *ag* **observes** *act* */
    AWARENESS, /**< \brief Used when the proposition specifies the partial observability conditions of an action -- *ag* **aware** *act* */
};

typedef std::map<agent, belief_formula> mal_observability_map; /**< \brief Used to express the obsverbability conditions in mal where the obs groups are fixed.
                                *
                                * Each element associates an \ref agent to the observability conditions for an \ref eff.*/

#endif //EFP_ACTIONS_H
