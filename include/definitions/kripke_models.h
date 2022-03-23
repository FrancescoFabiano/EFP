#ifndef EFP_KRIPKE_MODELS_H
#define EFP_KRIPKE_MODELS_H

#include <cstdio>
#include <set>
#include <map>

#include "domain_def.h"

class kstore;

class kworld;
///\todo: remove string from code because they have terrible performances
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

enum event_type {
    EPSILON, /**< \brief The null cevent.*/
    SIGMA, /**< \brief Event corresponding to ... */
    TAU /**< \brief Event corresponding to ... */
};

typedef std::set<event_type> event_type_set;
typedef std::set<std::pair<event_type, event_type> > event_type_relation;

typedef std::map<std::pair<kworld_ptr, event_type>, kworld_ptr> kstate_map;

class kstate;

typedef std::map<kworld_ptr, std::map<agent, kworld_ptr_set>> kedge_map;

/****************** Bisimulation *******************/

typedef unsigned short bis_label;
typedef std::set<bis_label> bis_label_set;

typedef std::map<kworld_ptr, std::map<kworld_ptr, bis_label_set> > kbislabel_map;

#endif //EFP_KRIPKE_MODELS_H
