#ifndef EFP_DOMAIN_DEF_H
#define EFP_DOMAIN_DEF_H

#include <cstdio>
#include <boost/dynamic_bitset.hpp>

typedef boost::dynamic_bitset<> fluent; /**< \brief A representation of a fluent through an unique id (short).
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

typedef boost::dynamic_bitset<> agent; /**< \brief A representation of an agent through an unique id (short).
                                      *
                                      *  This representation is derived by applying \ref grounder::ground_agent(const std::string&) const
                                      * to the element of \ref reader::m_agents.*/
typedef std::set<agent> agent_set; /**< \brief A set of \ref agent.*/
typedef std::vector<agent> agent_list; /**< \brief A list of \ref agent.*/

typedef boost::dynamic_bitset<> action_id; /**< \brief The unique id (short) associated with each action.
                                      *
                                      *  This id is derived by applying \ref grounder::ground_action(const std::string&) const
                                      * to the element of \ref reader::m_actions.*/
typedef std::vector<action_id> action_id_list; /**< \brief A list of \ref action_id.*/

#endif //EFP_DOMAIN_DEF_H
