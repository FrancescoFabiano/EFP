#ifndef EFP_GROUNDER_DEF_H
#define EFP_GROUNDER_DEF_H

#include <cstdio>
#include <map>

#include "define.h"

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


#endif //EFP_GROUNDER_DEF_H
