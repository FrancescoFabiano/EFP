#ifndef EFP_DELPHIC_H
#define EFP_DELPHIC_H

#include <cstdio>
#include <map>

#include "domain_def.h"
#include "actions.h"

//#include "../../src/actions/custom_event_models/cevent.h"
//#include "../../src/actions/custom_event_models/cem.h"

/****************** Delphic *******************/

// EVENTS

enum e_meta_condition {
    act_eff,
    neg_act_eff,
    act_pre,
    neg_act_pre,
    none
};

typedef std::set<e_meta_condition> event_metacond;
typedef fluent_formula event_postconditions;

typedef std::map<agent, agent_group_id> agent_group_map;

/**
 * \TODO: RICONTROLLA DEPENDENCIES DEGLI HEADER
 */
// Possibilities
//class cevent;
typedef short event_id;

class cevent_ptr;
typedef std::set<cevent_ptr> cevent_ptr_set;


//class cem;
typedef act_type cem_id;

class cem_ptr;

typedef std::map<agent_group_id, cevent_ptr_set> cevent_map;
typedef std::map<cevent_ptr, cevent_map> cem_edges;

class pworld_ptr;
typedef std::map<std::pair<pworld_ptr, cevent_ptr>, pworld_ptr> cupdate_map;

//class cem_store;
//typedef std::set<cevent> cevent_set; /**< \brief A set of \ref cevent, used to store all the created ones.*/
//typedef std::set<cem> cem_set; /**< \brief A set of \ref cem, used to store all the created ones.*/

#endif //EFP_DELPHIC_H
