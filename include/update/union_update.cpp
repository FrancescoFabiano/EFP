/**
 * \brief Implementation of \ref union_update.h.
 *
 * \copyright GNU Public License.
 *
 * \author Francesco Fabiano.
 * \date April 29, 2021
 */

#include "union_update.h"
#include "../utilities/helper_t.ipp"
#include "../actions/pevent.h"
#include "../actions/pem.h"
#include "../actions/pem_store.h"

pem_ptr union_update::build_pem(const action & act)
{
	/*pem_id p_id;
	switch ( act.get_type() ) {
	case ONTIC:
	{
		//Hard-coded
		p_id = 2;
		break;
	}
	case SENSING:
	case ANNOUNCEMENT:
	{
		//Hard-coded
		p_id = 1;
		break;
	}
	default:
		std::cerr << "\nError: wrong action specification.\n";
		break;
	}*/

	return pem_store::get_instance().get_pem(act.get_type());

}

agent_group_map union_update::build_agent_group_map()
{
	agent_group_map a_map;
	return a_map;
}

const pstate & union_update::u_update(const pstate & state, const action & act)
{
	pstate ret;
	pem_ptr pem = union_update::build_pem(act);
	const pworld_ptr& p_pw = state.get_pointed();
	pevent_ptr p_ev = pem_store::get_instance().get_event(pem.get_pointed_id());
	pupdate_map u_map;
	agent_group_map a_map;

	if (!state.entails(get_total_pre(state, act, p_ev), p_pw)) {
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
		fluent_formula effects = get_total_effects(state, act, ev);
		fluent_formula::const_iterator it_eff;

		for (it_eff = effects.begin(); it_eff != effects.end(); it_eff++) {
			helper::apply_effect(*it_eff, world_description);
		}
	}

	pworld_ptr new_pw = ret.add_world(pworld(world_description));
	u_map.insert(pupdate_map::value_type({pw, ev}, new_pw));

	information_state::const_iterator it_pwm;
	pworld_ptr_set::const_iterator it_pws;

	for (it_pwm = pw.get_information_state().begin(); it_pwm != pw.get_information_state().end(); it_pwm++) {
		auto it_eve = pem.get_edges().find(ev);

		if (it_eve != pem.get_edges().end()) {
			agent ag = it_pwm->first;
			auto it_agm = a_map.find(ag);

			if (it_agm != a_map.end()) {
				auto it_evm = it_eve->second.find(it_agm->second);

				if (it_evm != it_eve->second.end()) {
					event_ptr_set::const_iterator it_evs;

					for (it_pws = it_pwm->second.begin(); it_pws != it_pwm->second.end(); it_pws++) {
						for (it_evs = it_evm->second.begin(); it_evs != it_evm->second.end(); it_evs++) {
							if (u_map.find({*it_pws, *it_evs}) == u_map.end() &&
								state.entails(get_total_pre(state, act, *it_evs), *it_pws)) {
								pworld_ptr believed_pw = u_update_helper(ret, state, act, pem, *it_pws, *it_evs, u_map, a_map);
								ret.add_edge(new_pw, believed_pw, ag);
							}
						}
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
	pem_ptr pem = union_update::build_pem(act); // <- event_model
	const kworld_ptr & p_kw = state.get_pointed();
	pevent_ptr p_ev = pem_store::get_instance().get_event(pem.get_pointed_id()); // <- event_model
	kupdate_map u_map;
	agent_group_map a_map;

	if (!state.entails(get_total_pre(state, act, p_ev), p_kw)) {
		std::cerr << "Action is not executable." << std::endl;
		return ret;
	}

	kworld_ptr_set::const_iterator it_kws;
	pem_edges::const_iterator it_eve;
	fluent_formula::const_iterator it_eff;

	fluent_set world_description;
	fluent_formula effects;

	for (it_kws = state.get_worlds().begin(); it_kws != state.get_worlds().end(); it_kws++) {
		for (it_eve = pem.get_edges().begin(); it_eve != pem.get_edges().end(); it_eve++) {
			if (state.entails(get_total_pre(state, act, it_eve->first), *it_kws)) {
				world_description = it_kws->get_fluent_set();
				// Execute the all the effects
				if (it_eve->first.get_ontic_change()) {
					effects = get_total_effects(state, act, it_eve->first);

					for (it_eff = effects.begin(); it_eff != effects.end(); it_eff++) {
						helper::apply_effect(*it_eff, world_description);
					}
				}

				kworld_ptr new_kw = ret.add_world(kworld(world_description));
				u_map.insert(kupdate_map::value_type({*it_kws, it_eve->first}, new_kw));

				if (*it_kws == state.get_pointed() && it_eve->first.get_id() == pem.get_pointed_id()) {
					ret.set_pointed(new_kw);
				}
			}
		}
	}

	event_map::const_iterator it_evm;
	event_ptr_set::const_iterator it_evs;

	kworld_ptr kfirst, ksecond;
	pevent_ptr efirst, esecond;
	agent ag;

	kedge_map::const_iterator it_kem;
	std::map<agent, kworld_ptr_set>::const_iterator it_agkw;

	for (it_kem = state.get_edges().begin(); it_kem != state.get_edges().end(); it_kem++) {
		kfirst = it_kem->first;

		for (it_eve = pem.get_edges().begin(); it_eve != pem.get_edges().end(); it_eve++) {
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

template <class T>
fluent_formula union_update::get_total_effects(const T & s, const action & act, const pevent_ptr & e)
{
	fluent_formula action_eff = helper_t::get_effects_if_entailed(act.get_effects(), s);
	auto meta_post = e.get_meta_postconditions();

	if (meta_post.size() > 1) {
		std::cerr << "Error: malformed action postcondition in pevent " << e.get_id() << std::endl;
		exit(1);
	} else if (meta_post.empty()) {
		fluent_formula empty;
		return empty;
	} else {
		switch ( *meta_post.begin() ) {
		case act_eff:
			return action_eff;
		case neg_act_eff:
			return helper::negate_fluent_formula(action_eff);
			//Not break because it means it is empty
			//break;
		case none:
		default:
			fluent_formula empty;
			return empty;
			break;
		}
	}

	//Need to insert the merge with specific postcondition
}

template <class T>
formula_list union_update::get_total_pre(const T & s, const action & act, const pevent_ptr & e)
{
	formula_list ret;
	formula_list action_pre = act.get_executability();
	fluent_formula action_eff = helper_t::get_effects_if_entailed(act.get_effects(), s);

	auto meta_pre = e.get_meta_precondition();

	for (auto it_meta_pre = meta_pre.begin(); it_meta_pre != meta_pre.end(); ++it_meta_pre) {
		belief_formula tmp_bf;

		switch ( *it_meta_pre ) {
		case act_eff:
			tmp_bf.set_from_ff(action_eff);
			ret.push_back(tmp_bf);
			break;
		case neg_act_eff:
			tmp_bf.set_from_ff(helper::negate_fluent_formula(action_eff));
			ret.push_back(tmp_bf);
			break;
		case act_pre:
			for (auto it_act_pre = action_pre.begin(); it_act_pre != action_pre.end(); ++it_act_pre) {
				ret.push_back(*it_act_pre);
			}
			break;
		case none:
		default:
			ret.clear();
			return ret;
			break;
		}
	}

	return ret;
}
