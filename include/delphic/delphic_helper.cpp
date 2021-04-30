/**
 * \brief Implementation of \ref delphic_helper.h.
 *
 * \copyright GNU Public License.
 *
 * \author Francesco Fabiano.
 * \date April 29, 2021
 */

#include "delphic_helper.h"
#include "pem.h"

pem delphic_helper::get_pem(const pstate & u, const action & act)
{
    pem ret;
//    switch (act.get_type()) {
//        case ONTIC: {

            //This finds all the worlds that are reachable from the initial state following
            //the edges labeled with fully observant agents.
//            agent_set agents = domain::get_instance().get_agents();
//            agent_set fully_obs_agents = get_agents_if_entailed(act.get_fully_observants(), u.get_pointed());
//
//            agent_set oblivious_obs_agents = agents;
//            minus_set(oblivious_obs_agents, fully_obs_agents);
//        }
//    }
    return ret;
}

pstate delphic_helper::union_update(const pstate & u, const pem & e)
{
    return u;
}
