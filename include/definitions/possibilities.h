#ifndef EFP_POSSIBILITIES_H
#define EFP_POSSIBILITIES_H

#include <cstdio>
#include <set>
#include <map>
#include <queue>

#include "domain_def.h"

/*****************Possibilities*****************/

class pworld;
typedef std::size_t pworld_id; /**< \brief The id of a \ref pworld in a \ref pstate.
                                *
                                * The id is calculated through an hash (pworld::hash_fluents_into_id()) of the info of the \ref pworld.
                                *
                                * This id is used to extract the \ref pworld from the \ref pstore to not create a new every time
                                * but to access the already created ones.
                                * @see pworld and pstore*/
/*typedef std::shared_ptr<const pworld> pworld*;*/ /*< \brief A std::shared_pointer to a \ref pworld usually stored in \ref pstore.
                                                   *
                                                   * @see pworld and pstore.
                                                   * \todo transform into class for < into set.*/
//class pworld_ptr;
typedef std::set<const pworld*> pworld_set; /**< \brief A set of \ref pworld*.
                                            *
                                            * Mainly used to store all the \ref pworld of a \ref pstate without wasting memory.
                                            *
                                            * \todo The operator < for std::shared_ptr must be implemented.
                                            * @see pworld and pstate.*/

typedef std::map<const agent*, pworld_set> pworld_map; /**< \brief A map between agents and set of \ref pworld*.
                                                    *
                                                    * @see pworld and pstate.*/

typedef std::map<const pworld*, pworld_map> pedges; /**< \brief A map, for every \ref pworld, of the beleifs of its agents.
//                                                                 *
//                                                                 * @see pworld and pstate.*/


typedef std::map<pworld*, pworld*> transition_map; /**< \brief A map that keeps track of the results of the transition function.
                                                          *
                                                          * @see pworld and pstate.*/

/****************** Bisimulation *******************/

typedef unsigned short bis_label;
typedef std::set<bis_label> bis_label_set;

typedef std::map<pworld*, std::map<pworld*, bis_label_set> > pbislabel_map;

#endif //EFP_POSSIBILITIES_H
