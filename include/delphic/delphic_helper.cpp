/**
 * \brief Implementation of \ref delphic_helper.h.
 *
 * \copyright GNU Public License.
 *
 * \author Francesco Fabiano.
 * \date April 29, 2021
 */

#include "delphic_helper.h"
#include "../utilities/helper_t.ipp"
#include "event.h"
#include "pem.h"
#include "pem_store.h"

pem_ptr delphic_helper::get_pem(const action & act)
{
    pem_ptr ret;
    event_ptr eps = pem_store::get_instance().get_epsilon();

    switch (act.get_type()) {
        case ONTIC: {
            event_ptr sig = pem_store::get_instance().add_event(event(event_type::SIGMA, true));
            ret = pem_store::get_instance().add_pem(pem(action_type::ONT));

            pem_edges edges;
            edges.insert(pem_edges::value_type(agent_type::FUL, {{sig, sig}, {eps, eps}}));
            edges.insert(pem_edges::value_type(agent_type::OBL, {{sig, eps}, {eps, eps}}));

            ret.set_edges(edges);
            break;
        }
        case SENSING:
        case ANNOUNCEMENT: {
            event_ptr sig = pem_store::get_instance().add_event(event(event_type::SIGMA, false));
            event_ptr tau = pem_store::get_instance().add_event(event(event_type::TAU  , false));
            /** \todo creare formula_list con precondition ed effetto (eventualmente negato). */

            action_type a_type = act.get_type() == proposition_type::SENSING ? action_type::SEN : action_type::ANN;
            ret = pem_store::get_instance().add_pem(pem(a_type));

            pem_edges edges;
            edges.insert(pem_edges::value_type(agent_type::FUL, {{sig, sig}, {tau, tau}, {eps, eps}}));
            edges.insert(pem_edges::value_type(agent_type::PAR, {{sig, sig}, {tau, tau}, {eps, eps}, {sig, tau}, {tau, sig}}));
            edges.insert(pem_edges::value_type(agent_type::OBL, {{sig, eps}, {tau, eps}, {eps, eps}}));

            ret.set_edges(edges);
            break;
        }
        default:
            break;
    }
    return ret;
}

pstate delphic_helper::union_update(const pstate & state, const pem & e)
{
    return state;
}
