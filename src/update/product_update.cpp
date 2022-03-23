/**
 * \brief Implementation of \ref product_update.h.
 *
 * \copyright GNU Public License.
 *
 * \author Francesco Fabiano, Alessandro Burigana.
 * \date April 29, 2021
 */

#include "product_update.h"
#include "../actions/custom_event_models/cevent.h"
#include "../actions/custom_event_models/cem.h"
#include "../actions/custom_event_models/cem_store.h"
//
//agent_group_map product_update::build_agent_group_map()
//{
//	agent_group_map a_map;
//	return a_map;
//}
//
//const pstate & product_update::update(const pstate & state, const action & act)
//{
//	pstate ret;
//	cem_ptr cem = cem_store::get_instance().get_cem(act.get_type());
//	const pworld_ptr& p_pw = state.get_pointed();
//	cevent_ptr cev = cem_store::get_instance().get_event(cem.get_pointed_id());
//	cupdate_map u_map;
//	agent_group_map a_map;
//
//	if (!state.entails(cev.get_precondition(state, act), p_pw)) {
//		std::cerr << "Action is not executable." << std::endl;
//		return ret;
//	}
//
//	pworld_ptr new_pointed = update_helper(ret, state, act, cem, p_pw, cev, u_map, a_map);
//	ret.set_pointed(new_pointed);
//
//	return ret;
//}
//
//const pworld_ptr & product_update::update_helper(pstate & ret, const pstate & state, const action & act, const cem_ptr & cem, const pworld_ptr & pw, const cevent_ptr & ev, cupdate_map & u_map, const agent_group_map & a_map)
//{
//	if (ev.get_meta_precondition().find(e_meta_condition::none) != ev.get_meta_precondition().end() && ev.get_ontic_change()) {
//		u_map.insert(cupdate_map::value_type({pw, ev}, pw));
//		return pw;
//	}
//
//	fluent_set world_description = pw.get_fluent_set();
//	// Execute the all the effects
//	if (ev.get_ontic_change()) {
//		fluent_formula effects = ev.get_postconditions(state, act);
//		fluent_formula::const_iterator it_eff;
//
//		for (it_eff = effects.begin(); it_eff != effects.end(); it_eff++) {
//			helper::apply_effect(*it_eff, world_description);
//		}
//	}
//
//	pworld_ptr new_pw = ret.add_world(pworld(world_description));
//	u_map.insert(cupdate_map::value_type({pw, ev}, new_pw));
//
//	//	event_information_state::const_iterator it_eis;
//	//
//	//	agent_group_id agg;
//
//	information_state::const_iterator it_pis;
//	pworld_ptr_set::const_iterator it_pws;
//	cevent_ptr_set::const_iterator it_evs;
//	agent_group_map::const_iterator it_agm;
//
//	cevent_ptr_set evs;
//	agent ag;
//
//	//    for (it_eis = ev.get_information_state().begin(); it_eis != ev.get_information_state().end(); it_eis++) {
//	//        agg = it_eis->first;
//	//
//	//        for (it_agm = )
//	//    }
//
//	for (it_pis = pw.get_information_state().begin(); it_pis != pw.get_information_state().end(); it_pis++) {
//		ag = it_pis->first;
//		it_agm = a_map.find(ag);
//
//		if (it_agm != a_map.end()) {
//			for (it_pws = it_pis->second.begin(); it_pws != it_pis->second.end(); it_pws++) {
//				evs = ev.get_information_state().at(it_agm->second);
//
//				for (it_evs = evs.begin(); it_evs != evs.end(); it_evs++) {
//					if (u_map.find({*it_pws, *it_evs}) == u_map.end() &&
//						state.entails(it_evs->get_precondition(state, act), *it_pws)) {
//						pworld_ptr believed_pw = update_helper(ret, state, act, cem, *it_pws, *it_evs, u_map, a_map);
//						ret.add_edge(new_pw, believed_pw, ag);
//					}
//				}
//			}
//		}
//	}
//	return new_pw;
//}
//

pstate product_update::update(const pstate & state, const action & act)
{
	//std::cerr << "\nDEBUG: Executing " << act.get_name() << "..." << std::endl;
	pstate ret;
//    cem_ptr cem = cem_store::get_instance().get_cem(act.get_type());
//    cem_edges cem_edges = cem.get_edges();
//
//	cupdate_map u_map;
//	auto a_map = act.get_observants();
//
//	/*if (!state.entails(act.get_executability())) {
//		std::cerr << "Action " << act.get_name() << " is not executable." << std::endl;
//		return ret;
//	}*/
//
//
//	fluent_set world_description;
//	fluent_formula effects;
//
//
//	for (auto it_ws = state.get_worlds().begin(); it_ws != state.get_worlds().end(); it_ws++) {
//		for (auto it_eve = cem_edges.begin(); it_eve != cem_edges.end(); it_eve++) {
//			auto prec = it_eve->first.get_precondition(state, act);
//			if (state.entails(prec, *it_ws)) {
//
//				world_description = it_ws->get_fluent_set();
//				// Execute the all the effects
//				if (it_eve->first.get_ontic_change()) {
//					effects = it_eve->first.get_postconditions(state, act);
//
//					for (auto it_eff = effects.begin(); it_eff != effects.end(); it_eff++) {
//						helper::apply_effect(*it_eff, world_description);
//
//					}
//				}
//				unsigned short rep = state.get_max_depth()+1+it_eve->first.get_id()+it_ws->get_repetition();
//				auto new_pw = ret.add_rep_world(pworld(world_description),rep);
//				ret.set_max_depth(rep);
//				u_map.insert(cupdate_map::value_type({*it_ws, it_eve->first}, new_pw));
//
//				if (*it_ws == state.get_pointed() && it_eve->first.get_id() == cem.get_pointed_id()) {
//					ret.set_pointed(new_pw);
//				}
//			}
//		}
//	}
//
//	cevent_ptr efirst, esecond;
//	agent ag;
//	agent_group_id obs_group;
//	pworld_ptr pworld_first;
//	cevent_map event_map;
//	cevent_ptr_set connected_events;
//
//	auto pbeliefs = state.get_beliefs();
//	for (auto it_pedges = pbeliefs.begin(); it_pedges != pbeliefs.end(); it_pedges++) {
//		pworld_first = it_pedges->first;
//
//		for (auto it_eve = cem.get_edges().begin(); it_eve != cem.get_edges().end(); it_eve++) {
//			efirst = it_eve->first;
//			auto pefirst = u_map.find({pworld_first, efirst});
//
//
//			if (pefirst != u_map.end()) {
//
//				for (auto it_ags_map = it_pedges->second.begin(); it_ags_map != it_pedges->second.end(); ++it_ags_map) {
//					ag = it_ags_map->first;
//
//					obs_group = -1;
//
//					//finding correct events reached by the agent observability group
//					auto obs_list_ag = a_map.find(ag);
//					if (obs_list_ag != a_map.end()) {
//						for (auto it_obs_cond = obs_list_ag->second.begin(); it_obs_cond != obs_list_ag->second.end();) {
//							if (state.entails(it_obs_cond->second)) {
//								obs_group = it_obs_cond->first;
//								it_obs_cond = obs_list_ag->second.end();
//							} else {
//								++it_obs_cond;
//							}
//						}
//
//					}
//
//					if (obs_group == -1) {
//						std::cerr << "\nERROR: The agent " << (domain::get_instance().get_grounder().deground_agent(ag));
//						std::cerr << " in action " << act.get_name();
//						std::cerr << " does not have any observability type associated." << std::endl;
//						exit(1);
//					}
//
//					event_map = cem_edges.find(efirst)->second;
//					connected_events = event_map.find(obs_group)->second;
//
//					for (auto it_conn_eve = connected_events.begin(); it_conn_eve != connected_events.end(); ++it_conn_eve) {
//						esecond = *it_conn_eve;
//
//						for (auto it_ws = it_ags_map->second.begin(); it_ws != it_ags_map->second.end(); it_ws++) {
//
//							auto pesecond = u_map.find({*it_ws, esecond});
//
//							if (pesecond != u_map.end()) {
////								std::cerr << "\nDEBUG: Adding: (" << efirst.get_id() << ")+";
////								//printer::get_instance().print_list(pworld_first.get_fluent_set());
////								std::cerr << "--(" << esecond.get_id() << ")+";
////								//printer::get_instance().print_list(it_ws->get_fluent_set());
////								std::cerr << "--(" << ag << ")";
//								ret.add_edge(pefirst->second, pesecond->second, ag);
//							}
//						}
//					}
//				}
//			}
//		}
//	}
	return ret;
}
