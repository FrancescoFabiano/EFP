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
#include "../utilities/helper_t.ipp"
#include "pem_store.h"

pem delphic_helper::get_pem(const pstate & state, const action & act)
{
    pem ret;

    belief_formula TRUE;
    TRUE.set_formula_type(BF_EMPTY);

    agent_set agents = domain::get_instance().get_agents();
    agent_set fully_obs_agents = helper::get_agents_if_entailed(act.get_fully_observants(), state);
    agent_set partially_obs_agents = helper::get_agents_if_entailed(act.get_partially_observants(), state);

    agent_set oblivious_obs_agents = agents;
    helper_t::minus_set<agent>(oblivious_obs_agents, fully_obs_agents);
    helper_t::minus_set<agent>(oblivious_obs_agents, partially_obs_agents);

    switch (act.get_type()) {
        case ONTIC: {
            pem_ptr sigma   = pem_ptr(pem(SIGMA  , act.get_executability()));       // pem_store::get_instance().add_pem
            pem_ptr epsilon = pem_ptr(pem(EPSILON, {TRUE}));

            pem_postconditions post;
            // get_effects_if_entailed per calcolare effetti entailed
//            act.get_effects();
//            sigma.set_postconditions(post);
            break;
        }
        case SENSING:
        case ANNOUNCEMENT: {
            break;
        }
        default:
            break;
    }
    return ret;
}

pstate delphic_helper::union_update(const pstate & u, const pem & e)
{
    return u;
}
