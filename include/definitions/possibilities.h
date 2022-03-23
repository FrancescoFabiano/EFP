#ifndef EFP_POSSIBILITIES_H
#define EFP_POSSIBILITIES_H

#include <cstdio>
#include <set>
#include <map>
#include <queue>

#include "domain_def.h"

/*****************Possibilities*****************/
class belief_formula;

class pstate;

class pworld;
typedef std::size_t pworld_id; /**< \brief The id of a \ref pworld in a \ref pstate.
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
                                                    * @see pworld and pstate.*/

typedef std::map<pworld_ptr, pworld_map> pedges; /**< \brief A map, for every \ref pworld, of the beleifs of its agents.
//                                                                 *
//                                                                 * @see pworld and pstate.*/

typedef std::queue<pworld_ptr> pworld_queue; /**< \brief A queue of \ref pworld_ptr.
                                            *
                                            * \todo The operator < for std::shared_ptr must be implemented.
                                            * @see pworld and pstate.*/

typedef std::map<pworld_ptr, pworld_ptr> transition_map; /**< \brief A map that keeps track of the results of the transition function.
                                                          *
                                                          * @see pworld and pstate.*/

typedef std::vector<std::tuple<pworld_ptr, pworld_ptr, agent> > beliefs_vector; /**< \brief A vector of tuples (pw1, pw2, ag) that represent that in \ref pworld pw1 the \agent ag believes that the \ref \pworld pw2 is possible
                                                                               *
                                                                               * @see pworld and pstate.*/
typedef std::map<belief_formula, unsigned short> pg_bfs_score; /** A map to store the info of the classical planning graph*/


/*****************Possibilities Optimized*****************/
class pstate_opt;
typedef std::set<pstate_opt> pstate_opt_set; /**< \brief A set of \ref pstate_opt, used to store all the created ones.*/

typedef std::string pstate_opt_id; /**< \brief The id of a \ref pstate_opt.
                                *
                                * The id is calculated through an hash (pstate_opt::hash_fluents_into_id()) of the info of the \ref pstate_opt.
                                *
                                * This id is used to extract the \ref pstate_opt from the \ref p_opt_store to not create a new every time
                                * but to access the already created ones.
                                * @see pstate_opt and p_opt_store*/
class pstate_opt_ptr;
typedef std::set<pstate_opt_ptr> pstate_opt_ptr_set; /**< \brief A set of \ref pstate_opt_ptr.
                                            *
                                            * Mainly used to store all the reachable \ref pstate_opt of a \ref pstate_opt without wasting memory.
                                            *
                                            * \todo The operator < for std::shared_ptr must be implemented.
                                            * @see pstate_opt.*/
typedef std::map<agent, pstate_opt_ptr_set> pedges_opt; /**< \brief A map between agents and set of \ref pstate_opt_ptr
                                                       *
                                                       * Each element consists of <\ref agent, \ref pstate_opt_ptr_set> and link each
                                                       * \ref agent to a set of possibilities;
                                                       * remember that each possibility is a state itself.
                                                       *
                                                       * @see pstate_opt.*/

/****************** Bisimulation *******************/

typedef unsigned short bis_label;
typedef std::set<bis_label> bis_label_set;

typedef std::map<pworld_ptr, std::map<pworld_ptr, bis_label_set> > pbislabel_map;

#endif //EFP_POSSIBILITIES_H
