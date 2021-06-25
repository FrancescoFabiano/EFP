/**
 * \brief Implementation of \ref union_update.h.
 *
 * \copyright GNU Public License.
 *
 * \author Francesco Fabiano.
 * \date April 29, 2021
 */

#include "union_update.h"
#include "../actions/possibilities/pevent.h"
#include "../actions/possibilities/pem.h"
#include "../actions/possibilities/pem_store.h"
#include "../actions/kripke/kevent.h"
#include "../actions/kripke/kem.h"
#include "../actions/kripke/kem_store.h"

agent_group_map union_update::build_agent_group_map()
{
	agent_group_map a_map;
	return a_map;
}

const pstate & union_update::u_update(const pstate & state, const action & act)
{
	pstate ret;
	pem_ptr pem = pem_store::get_instance().get_pem(act.get_type());
	const pworld_ptr& p_pw = state.get_pointed();
	pevent_ptr p_ev = pem_store::get_instance().get_event(pem.get_pointed_id());
	pupdate_map u_map;
	agent_group_map a_map;

	if (!state.entails(p_ev.get_precondition(state, act), p_pw)) {
		std::cerr << "Action is not executable." << std::endl;
		return ret;
	}

	pworld_ptr new_pointed = u_update_helper(ret, state, act, pem, p_pw, p_ev, u_map, a_map);
	ret.set_pointed(new_pointed);

	return ret;
}

const pworld_ptr & union_update::u_update_helper(pstate & ret, const pstate & state, const action & act, const pem_ptr & pem, const pworld_ptr & pw, const pevent_ptr & ev, pupdate_map & u_map, const agent_group_map & a_map)
{
	if (ev.get_meta_precondition().find(e_meta_condition::none) != ev.get_meta_precondition().end() && ev.get_ontic_change()) {
		u_map.insert(pupdate_map::value_type({pw, ev}, pw));
		return pw;
	}

	fluent_set world_description = pw.get_fluent_set();
	// Execute the all the effects
	if (ev.get_ontic_change()) {
		fluent_formula effects = ev.get_postconditions(state, act);
		fluent_formula::const_iterator it_eff;

		for (it_eff = effects.begin(); it_eff != effects.end(); it_eff++) {
			helper::apply_effect(*it_eff, world_description);
		}
	}

	pworld_ptr new_pw = ret.add_world(pworld(world_description));
	u_map.insert(pupdate_map::value_type({pw, ev}, new_pw));

	//	event_information_state::const_iterator it_eis;
	//
	//	agent_group_id agg;

	information_state::const_iterator it_pis;
	pworld_ptr_set::const_iterator it_pws;
	pevent_ptr_set::const_iterator it_evs;
	agent_group_map::const_iterator it_agm;

	pevent_ptr_set evs;
	agent ag;

	//    for (it_eis = ev.get_information_state().begin(); it_eis != ev.get_information_state().end(); it_eis++) {
	//        agg = it_eis->first;
	//
	//        for (it_agm = )
	//    }

	for (it_pis = pw.get_information_state().begin(); it_pis != pw.get_information_state().end(); it_pis++) {
		ag = it_pis->first;
		it_agm = a_map.find(ag);

		if (it_agm != a_map.end()) {
			for (it_pws = it_pis->second.begin(); it_pws != it_pis->second.end(); it_pws++) {
				evs = ev.get_information_state().at(it_agm->second);

				for (it_evs = evs.begin(); it_evs != evs.end(); it_evs++) {
					if (u_map.find({*it_pws, *it_evs}) == u_map.end() &&
						state.entails(it_evs->get_precondition(state, act), *it_pws)) {
						pworld_ptr believed_pw = u_update_helper(ret, state, act, pem, *it_pws, *it_evs, u_map, a_map);
						ret.add_edge(new_pw, believed_pw, ag);
					}
				}
			}
		}
	}
	return new_pw;
}

const kstate & union_update::u_update(const kstate & state, const action & act)
{
	kstate ret;
	kem_ptr kem = kem_store::get_instance().get_kem(act.get_type());
	const kworld_ptr & p_kw = state.get_pointed();
	kevent_ptr p_ev = kem_store::get_instance().get_event(kem.get_pointed_id());
	kupdate_map u_map;
	agent_group_map a_map;

	if (!state.entails(p_ev.get_precondition(state, act), p_kw)) {
		std::cerr << "Action is not executable." << std::endl;
		return ret;
	}

	kworld_ptr_set::const_iterator it_kws;
	kem_edges::const_iterator it_eve;
	fluent_formula::const_iterator it_eff;

	fluent_set world_description;
	fluent_formula effects;

	for (it_kws = state.get_worlds().begin(); it_kws != state.get_worlds().end(); it_kws++) {
		for (it_eve = kem.get_edges().begin(); it_eve != kem.get_edges().end(); it_eve++) {
			if (state.entails(it_eve->first.get_precondition(state, act), *it_kws)) {
				world_description = it_kws->get_fluent_set();
				// Execute the all the effects
				if (it_eve->first.get_ontic_change()) {
					effects = it_eve->first.get_postconditions(state, act);

					for (it_eff = effects.begin(); it_eff != effects.end(); it_eff++) {
						helper::apply_effect(*it_eff, world_description);
					}
				}

				kworld_ptr new_kw = ret.add_world(kworld(world_description));
				u_map.insert(kupdate_map::value_type({*it_kws, it_eve->first}, new_kw));

				if (*it_kws == state.get_pointed() && it_eve->first.get_id() == kem.get_pointed_id()) {
					ret.set_pointed(new_kw);
				}
			}
		}
	}

	kevent_map::const_iterator it_evm;
	kevent_ptr_set::const_iterator it_evs;

	kworld_ptr kfirst, ksecond;
	kevent_ptr efirst, esecond;
	agent ag;

	kedge_map::const_iterator it_kem;
	std::map<agent, kworld_ptr_set>::const_iterator it_agkw;

	for (it_kem = state.get_edges().begin(); it_kem != state.get_edges().end(); it_kem++) {
		kfirst = it_kem->first;

		for (it_eve = kem.get_edges().begin(); it_eve != kem.get_edges().end(); it_eve++) {
			efirst = it_eve->first;
			auto kefirst = u_map.find({kfirst, efirst});

			if (kefirst != u_map.end()) {
				for (it_agkw = it_kem->second.begin(); it_agkw != it_kem->second.end(); it_agkw++) {
					ag = it_agkw->first;
					auto it_agm = a_map.find(ag);

					if (it_agm != a_map.end()) {
						for (it_kws = it_agkw->second.begin(); it_kws != it_agkw->second.end(); it_kws++) {
							ksecond = *it_kws;

							for (it_evs = it_evm->second.begin(); it_evs != it_evm->second.end(); it_evs++) {
								esecond = *it_evs;
								auto kesecond = u_map.find({ksecond, esecond});

								if (kesecond != u_map.end()) {
									ret.add_edge(kefirst->second, kesecond->second, ag);
								}
							}
						}
					}
				}
			}
		}
	}
	return ret;
}
