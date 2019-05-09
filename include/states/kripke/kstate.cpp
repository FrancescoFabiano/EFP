/*
 * \brief Implementation of \ref kstate.h
 *
 * \copyright GNU Public License.
 *
 * \author Francesco Fabiano.
 * \date April 9, 2019
 */

#include <iostream>
#include <tuple>


//int count = 0;

#include "kstate.h"
#include "../../domain/domain.h"
#include "../../utilities/helper.h"

void kstate::set_worlds(const kworld_ptr_set & to_set)
{
	m_worlds = to_set;
}

void kstate::set_edges(const kedge_ptr_set & to_set)
{
	m_edges = to_set;
}

void kstate::set_pointed(const kworld_ptr & to_set)
{
	m_pointed = to_set;
}

void kstate::set_max_depth(unsigned int to_set)
{
	m_max_depth = to_set;
}

const kworld_ptr_set & kstate::get_worlds() const
{
	return m_worlds;
}

const kedge_ptr_set & kstate::get_edges() const
{
	return m_edges;
}

const kworld_ptr & kstate::get_pointed() const
{
	return m_pointed;
}

unsigned int kstate::get_max_depth() const
{
	return m_max_depth;
}

bool kstate::entails(fluent f) const
{
	return entails(f, m_pointed);
}

bool kstate::entails(fluent f, kworld_ptr world) const
{
	return world.get_ptr()->entails(f);
}

bool kstate::entails(const fluent_set & fl) const
{
	return entails(fl, m_pointed);
}

bool kstate::entails(const fluent_set & fl, kworld_ptr world) const
{
	return world.get_ptr()->entails(fl);
}

bool kstate::entails(const fluent_formula & ff) const
{
	return entails(ff, m_pointed);
}

bool kstate::entails(const fluent_formula & ff, kworld_ptr world) const
{
	return world.get_ptr()->entails(ff);
}

bool kstate::entails(const belief_formula & bf) const
{
	return entails(bf, m_pointed);
}

bool kstate::operator=(const kstate & to_copy)
{
	set_edges(to_copy.get_edges());
	set_worlds(to_copy.get_worlds());
	set_max_depth(to_copy.get_max_depth());
	set_pointed(to_copy.get_pointed());
	return true;
}

bool kstate::entails(const belief_formula & to_check, const kworld_ptr_set & reachable) const
{
	kworld_ptr_set::const_iterator it_kwl;
	for (it_kwl = reachable.begin(); it_kwl != reachable.end(); it_kwl++) {
		/**\todo why setted contary?*/
		if (!entails(to_check, (*it_kwl)))
			return false;
	}
	return true;
}

bool kstate::entails(const belief_formula & bf, kworld_ptr world) const
{
	/*
	 The entailment of a \ref belief_formula just call recursively the entailment on all the reachable world with that formula.
	 */
	switch ( bf.get_formula_type() ) {

	case FLUENT_FORMULA:
	{
		/** \todo Make sure its grounded. Maybe add to \ref belief_formula a bool that store if grounded or not or maybe ground
		 * when \ref domain created.
		 * @see belief_formula::ground(const grounder &).*/
		return entails(bf.get_fluent_formula(), world);
		break;
	}

	case BELIEF_FORMULA:

		/** \todo what was the at_lest_one of the previous version?*/
		return entails(bf.get_bf1(), get_B_reachable_worlds(bf.get_agent(), world));
		break;

	case PROPOSITIONAL_FORMULA:
		switch ( bf.get_operator() ) {
		case BF_NOT:
			return !entails(bf.get_bf1(), world);
			break;
		case BF_OR:
			return entails(bf.get_bf1(), world) || entails(bf.get_bf2(), world);
			break;
		case BF_AND:
			return entails(bf.get_bf1(), world) && entails(bf.get_bf2(), world);
			break;
		case BF_FAIL:
		default:
			std::cerr << "Something went wrong in checking entailment for Propositional formula";
			exit(1);
		}
		break;

	case E_FORMULA:
		//Check the entails on the E-reachable worlds
		return entails(bf.get_bf1(), get_E_reachable_worlds(bf.get_group_agents(), world));
		break;

		//Check the entails on the C-reachable worlds
	case C_FORMULA:
		return entails(bf.get_bf1(), get_C_reachable_worlds(bf.get_group_agents(), world));
		break;
	case BF_EMPTY:
	{

		return true;
		break;
	}
	case BF_TYPE_FAIL:
	default:
		std::cerr << "Something went wrong in checking entailment for Belief formula";
		exit(1);
	}
}

bool kstate::entails(const formula_list & to_check, kworld_ptr world) const
{
	//formula_list expresses CNF formula
	formula_list::const_iterator it_fl;
	for (it_fl = to_check.begin(); it_fl != to_check.end(); it_fl++) {
		if (!entails((*it_fl), world)) {
			return false;
		}
	}
	return true;
}

const kworld_ptr_set kstate::get_B_reachable_worlds(agent ag, kworld_ptr world) const
{
	kworld_ptr_set ret;
	get_B_reachable_worlds(ag, world, ret);

	return ret;
}

bool kstate::get_B_reachable_worlds(agent ag, kworld_ptr world, kworld_ptr_set& ret) const
{
	/** \todo check: If a--i-->b, b--i-->c then a--i-->c must be there*/
	bool is_fixed_point = true;
	kedge_ptr_set::const_iterator it_kedge;
	for (it_kedge = m_edges.begin(); it_kedge != m_edges.end(); it_kedge++) {
		if (((*it_kedge).get_from() == world) && ((*it_kedge).get_label() == ag)) {
			//We use the pair of insert, if we add a new world (true in the set::insert) then is not a fixed point
			if (std::get<1>(ret.insert((*it_kedge).get_to()))) {

				is_fixed_point = false;
			}
		}
	}
	return is_fixed_point;
}

const kworld_ptr_set kstate::get_E_reachable_worlds(const agent_set & ags, kworld_ptr world) const
{
	kworld_ptr_set ret;
	get_E_reachable_worlds(ags, world, ret);

	return ret;
}

bool kstate::get_E_reachable_worlds(const agent_set & ags, kworld_ptr world, kworld_ptr_set& ret) const
{
	bool is_fixed_point = true;
	agent_set::const_iterator it_agset;
	for (it_agset = ags.begin(); it_agset != ags.end(); it_agset++) {
		if (!get_B_reachable_worlds(*it_agset, world, ret)) {

			is_fixed_point = false;
		}
	}
	return is_fixed_point;
}

const kworld_ptr_set kstate::get_C_reachable_worlds(const agent_set & ags, kworld_ptr world) const
{
	//Use of fixed point to stop.
	bool is_fixed_point = false;
	kworld_ptr_set ret;
	while (!is_fixed_point) {

		is_fixed_point = get_E_reachable_worlds(ags, world, ret);
	}
	return ret;
}

void kstate::add_world(const kworld & world)
{

	m_worlds.insert(kstore::get_instance().add_world(world));
}

kworld_ptr kstate::add_rep_world(const kworld & world, unsigned short repetition, bool& is_new)
{
	kworld_ptr tmp = kstore::get_instance().add_world(world);
	tmp.set_repetition(repetition);
	is_new = std::get<1>(m_worlds.insert(tmp));
	return tmp;
}

kworld_ptr kstate::add_rep_world(const kworld & world)
{
	bool tmp;
	return add_rep_world(world, get_max_depth(), tmp);
}

/*void kstate::add_copy_world(const kworld & world, unsigned short repetition)
{
	kworld_ptr tmp = kstore::get_instance().add_world(world);
	tmp.set_repetition(repetition);

	//If this kworld_ptr exist then add another, we need to add a copy
	if (!(std::get<1>(m_worlds.insert(tmp)))) {
		///arg = 0 in kworld constructor by default but when put to x it set the extra id to x
		add_copy_world(world, repetition + 1);
	}
}*/

void kstate::add_edge(const kedge & edge)
{

	m_edges.insert(kstore::get_instance().add_edge(edge));
}

void kstate::build_initial()
{
	/** \todo for now prune building.*/
	std::cout << "\nBuilding initial Kripke structure...\n";
	build_initial_prune();
}

void kstate::build_initial_structural()
{

}

void kstate::build_initial_prune()
{

	/*Building of all the possible consistent \ref kworld and setting the pointed world.
	 * Creation of all the \ref fluent combinations. All the consistent ones are added to \ref kstore.*/
	fluent_set permutation;
	generate_initial_kworlds(permutation, 0);


	/*Building of all the consistent \ref kedge.*/
	generate_initial_kedges();
}

/*From https://www.geeksforgeeks.org/generate-all-the-binary-strings-of-n-bits/ since is like generating all the binary numbers.*/
void kstate::generate_initial_kworlds(fluent_set& permutation, int index)
{
	int fluent_number = domain::get_instance().get_fluent_number();
	if (index / 2 == fluent_number) {
		kworld to_add(permutation);
		add_initial_kworld(to_add);

		return;
	}
	fluent_set permutation_2 = permutation;
	//Add the \ref fluent in positive version
	permutation.insert(index);
	generate_initial_kworlds(permutation, index + 2);

	//Add the \ref fluent in negative version
	permutation_2.insert(index + 1);
	generate_initial_kworlds(permutation_2, index + 2);
}

void kstate::add_initial_kworld(const kworld & possible_add)
{
	initially ini_conditions = domain::get_instance().get_initial_description();

	switch ( ini_conditions.get_ini_restriction() ) {
	case S5:
	{
		/* Since the common knowledge is on all the agent it means that every possible \ref kworld
		 * in the initial state must entail *phi* where C(*phi*) is an initial condition.*/

		//Already setted in \ref domain::build_initially(bool).
		if (possible_add.entails(ini_conditions.get_ff_forS5())) {
			add_world(possible_add);
			if (possible_add.entails(ini_conditions.get_pointed_world_conditions())) {
				m_pointed = kworld_ptr(possible_add);
				/*std::cout << "pointed world: ";
				printer::get_instance().print_list(possible_add.get_fluent_set());
				std::cout << std::endl;*/
			}
		} else {
			//Already generated so we save it on kstore
			kstore::get_instance().add_world(possible_add);
		}
		break;
	}
	case K45:
	{
		std::cerr << "\nNot implemented yet\n";
		exit(1);
		break;
	}
	case NONE:
	{
		std::cerr << "\nNot implemented yet\n";
		exit(1);
		break;
	}
	default:
	{
		std::cerr << "\nNot implemented yet\n";
		exit(1);

		break;
	}
	}
}

void kstate::generate_initial_kedges()
{
	kworld_ptr_set::const_iterator it_kwps_1, it_kwps_2;

	kworld_ptr kwptr_tmp1, kwptr_tmp2;

	/*This for add to *this* all the possible edges.*/
	for (it_kwps_1 = m_worlds.begin(); it_kwps_1 != m_worlds.end(); it_kwps_1++) {
		for (it_kwps_2 = it_kwps_1; it_kwps_2 != m_worlds.end(); it_kwps_2++) {
			for (unsigned int i = 0; i < domain::get_instance().get_agents().size(); i++) {
				kwptr_tmp1 = *it_kwps_1;
				kwptr_tmp2 = *it_kwps_2;

				add_edge(kedge(kwptr_tmp1, kwptr_tmp2, i));
				add_edge(kedge(kwptr_tmp2, kwptr_tmp1, i));

			}
		}
	}

	//std::cout << "Tot edges: " << m_edges.size() << std::endl;

	initially ini_conditions = domain::get_instance().get_initial_description();

	formula_list::const_iterator it_fl;
	for (it_fl = ini_conditions.get_initial_conditions().begin(); it_fl != ini_conditions.get_initial_conditions().end(); it_fl++) {

		remove_initial_kedge_bf(*it_fl);
	}
	//std::cout << "Removed edges: " << count << std::endl;

	//std::cout << "Final edges: " << m_edges.size() << std::endl;

}

void kstate::remove_kedge(const kedge & to_remove)
{

	m_edges.erase(kstore::get_instance().add_edge(to_remove));
	//if(m_edges.erase(kstore::get_instance().add_edge(to_remove)) > 0)count++;
}

void kstate::remove_initial_kedge(const fluent_formula & known_ff, agent ag)
{

	kworld_ptr_set::const_iterator it_kwps_1, it_kwps_2;

	kworld_ptr kwptr_tmp1, kwptr_tmp2;

	/** \todo maybe don't loop twice on the world but exploit using it_kwps_2 = it_kwps_1:
	 * - remove (_1, _2).
	 * - remove (_2, _1).*/
	for (it_kwps_1 = m_worlds.begin(); it_kwps_1 != m_worlds.end(); it_kwps_1++) {
		for (it_kwps_2 = it_kwps_1; it_kwps_2 != m_worlds.end(); it_kwps_2++) {
			/** \todo or entails(-known_ff)?*/
			kwptr_tmp1 = *it_kwps_1;
			kwptr_tmp2 = *it_kwps_2;
			if (kwptr_tmp1.get_ptr()->entails(known_ff) && !kwptr_tmp2.get_ptr()->entails(known_ff)) {
				remove_kedge(kedge(kwptr_tmp1, kwptr_tmp2, ag));
				remove_kedge(kedge(kwptr_tmp2, kwptr_tmp1, ag));
			} else if (kwptr_tmp2.get_ptr()->entails(known_ff) && !kwptr_tmp1.get_ptr()->entails(known_ff)) {

				remove_kedge(kedge(kwptr_tmp2, kwptr_tmp1, ag));
				remove_kedge(kedge(kwptr_tmp1, kwptr_tmp2, ag));
			}
		}
	}

}

void kstate::remove_initial_kedge_bf(const belief_formula & to_check)
{
	switch ( domain::get_instance().get_initial_description().get_ini_restriction() ) {
	case S5:
	{
		/* Just check whenever is B(--) \/ B(--) and remove that edge*/
		if (to_check.get_formula_type() == C_FORMULA) {

			belief_formula tmp = to_check.get_bf1();

			switch ( tmp.get_formula_type() ) {

				//Only one for edges -- expresses that someone is ignorant.
			case PROPOSITIONAL_FORMULA:
			{
				//We remove all the check on the formula since they have already been controlled when ini_conditions has been created
				if (tmp.get_operator() == BF_OR) {

					//fluent_formula known_ff;
					auto known_ff_ptr = std::make_shared<fluent_formula>();
					helper::check_Bff_Bnotff(tmp.get_bf1(), tmp.get_bf2(), known_ff_ptr);
					if (known_ff_ptr != nullptr) {
						//printer::get_instance().print_list(*known_ff_ptr);
						remove_initial_kedge(*known_ff_ptr, tmp.get_bf2().get_agent());
					}
					return;

				} else if (tmp.get_operator() == BF_AND) {
					//This case doesn't add knowledge.
					return;

				} else {
					std::cerr << "\nError in the type of initial formulae (FIFTH).\n";
					exit(1);
				}
				return;
				break;
			}
			case FLUENT_FORMULA:
			case BELIEF_FORMULA:
			{
				return;
				break;
			}
			case BF_EMPTY:
			{
				return;
				break;
			}

			default:
			{
				std::cerr << "\nError in the type of initial formulae (SIXTH).\n";
				exit(1);
				break;
			}
			}
		} else {
			std::cerr << "\nError in the type of initial formulae (SEVENTH).\n";
			exit(1);
		}

		return;
		break;
	}

	case K45:
	{
		std::cerr << "\nNot implemented yet\n";
		exit(1);
		break;
	}
	case NONE:
	{
		std::cerr << "\nNot implemented yet\n";
		exit(1);
		break;
	}
	default:
	{
		std::cerr << "\nNot implemented yet\n";
		exit(1);

		break;
	}
	}
}

/** \warning executability should be check in \ref state (or \ref planner).*/
kstate kstate::compute_succ(const action & act) const
{
	switch ( act.get_type() ) {
	case ONTIC:
	{
		/*kstate tmp = execute_ontic(act);
		std::cout << "\n*********************Printing this*********************\n";
		print();
		std::cout << "\n*********************Printing tmp*********************\n";
		tmp.print();*/
		return execute_ontic(act);
		break;
	}
	case SENSING:
	{
		return execute_sensing(act);
		break;
	}
	case ANNOUNCEMENT:
	{
		return execute_announcement(act);
		break;
	}
	default:
	{
		std::cerr << "Error in executing an action: ";
		std::cerr << "the type of the action is not defined correctly";
		std::cerr << std::endl;
		exit(1);

		break;
	}
	}
}

/**We keep the agent_set to not calculate every time the fully obsv if the obsv is global.*/
void kstate::add_ret_ontic_worlds_internal(const kworld_ptr & start, kworld_ptr_set &reached, kstate& ret, const fluent_formula & effects, agent_set & fully_obs_agents, const action & act, action_check act_check, std::map<kworld_ptr, kworld_ptr>& map_for_edges) const
{
	/**\todo Maybe pointed useless as parameter?*/
	agent_set::const_iterator it_agset;
	kworld_ptr_set reachable_by_ag;
	agent tmp_ag;
	kworld_ptr_set::const_iterator it_kwset;
	kworld_ptr tmp_kworld_ptr;
	fluent_formula::const_iterator it_eff;

	fluent_set world_description = start.get_fluent_set();
	//Execute the all the effects
	//std::cout << "\n\n*************************" << act.get_name() << "*************************\n";
	for (it_eff = effects.begin(); it_eff != effects.end(); it_eff++) {
		world_description = helper::ontic_exec(*it_eff, world_description);
		//printer::get_instance().print_list(*it_eff);
	}
	//std::cout << "\n*************************" << act.get_name() << "*************************\n";


	//Insert into a map the pair <old_world, new_world>.
	map_for_edges.insert(std::pair<kworld_ptr, kworld_ptr>(start, ret.add_rep_world(kworld(world_description))));
	for (it_agset = fully_obs_agents.begin(); it_agset != fully_obs_agents.end(); it_agset++) {

		tmp_ag = *it_agset;
		/**\bug the depth is increased also by agents, introduce a variable extra.*/

		reachable_by_ag = get_B_reachable_worlds(tmp_ag, start);
		minus_set(reachable_by_ag, reached);

		if (reachable_by_ag.size() != 0) {
			sum_set(reached, reachable_by_ag);

			for (it_kwset = reachable_by_ag.begin(); it_kwset != reachable_by_ag.end(); it_kwset++) {
				tmp_kworld_ptr = *it_kwset;
				//Find the new fully observant if visibility is relative
				if (!domain::get_instance().get_is_global_obsv()) {
					fully_obs_agents = get_agents_if_entailed(act.get_fully_observants(), tmp_kworld_ptr);
					std::cerr << "\nRELATIVE_OBSERVABILITY - Not fully implemented yet.\n";
					exit(1);
				}

				if (act_check == EXE_POINTED__COND_POINTED /*|| act.get_type() != ONTIC*/) {
					add_ret_ontic_worlds_internal(tmp_kworld_ptr, reached, ret, effects, fully_obs_agents, act, act_check, map_for_edges);
				} else {
					add_ret_ontic_worlds(tmp_kworld_ptr, reached, ret, fully_obs_agents, act, act_check, map_for_edges);
				}
			}
			ret.set_max_depth(ret.get_max_depth() + 1);
		}
	}
}

void kstate::add_ret_ontic_worlds(const kworld_ptr & start, kworld_ptr_set &reached, kstate& ret, agent_set & fully_obs_agents, const action & act, action_check act_check, std::map<kworld_ptr, kworld_ptr>& map_for_edges) const
{
	fluent_formula effects;
	switch ( act_check ) {
	case (EXE_POINTED__COND_POINTED):
	{
		//The action executability is already be checked in state_T
		//if (act.get_type() == ONTIC)
		effects = get_effects_if_entailed(act.get_effects(), get_pointed());
		//else
		//	effects = get_sensing_effects_if_entailed(act.get_effects(), get_pointed());
		break;
	}
	case (EXE_POINTED__COND_WORLD):
	{
		//The action executability is already be checked in state_T
		//if (act.get_type() == ONTIC)
		effects = get_effects_if_entailed(act.get_effects(), start);
		//else
		//	effects = get_sensing_effects_if_entailed(act.get_effects(), get_pointed());
		break;
	}
	case (EXE_WORLD__COND_WORLD):
	{
		//The action executability is already be checked in state_T
		if (entails(act.get_executability(), start)) {
			effects = get_effects_if_entailed(act.get_effects(), start);
		} else {
			return;
		}
		break;
	}
	default:
	{
		std::cerr << "\nType of action checking not correctly decalred.\n";
		exit(1);
	}
	}
	add_ret_ontic_worlds_internal(start, reached, ret, effects, fully_obs_agents, act, act_check, map_for_edges);
}

kstate kstate::execute(const action& act) const
{

	//The execution are all the same if we consider that false beliefs don't count.
	kstate ret;

	//This finds all the worlds that are reachable from the initial state following
	//the edges labeled with fully observant agents.
	agent_set fully_obs_agents = get_agents_if_entailed(act.get_fully_observants(), get_pointed());
	agent_set partially_obs_agents = get_agents_if_entailed(act.get_partially_observants(), get_pointed());
	agent_set oblivious_obs_agents = domain::get_instance().get_agents();
	minus_set(oblivious_obs_agents, fully_obs_agents);
	minus_set(oblivious_obs_agents, partially_obs_agents);

	//DEBUG PRINT
	/*if (act.get_name().compare("b_check_1") == 0) {
		std::cout << "\nFully observant: ";
		printer::get_instance().print_list_ag(fully_obs_agents);
		std::cout << "\nPartially observant: ";
		printer::get_instance().print_list_ag(partially_obs_agents);
		std::cout << "\nOblivious observant: ";
		printer::get_instance().print_list_ag(oblivious_obs_agents);
	}*/

	if (oblivious_obs_agents.size() > 0) {
		ret = (*this);
		ret.set_max_depth(ret.get_max_depth() + 1);
	}

	//pas tmp for entailment
	kworld_ptr_set reached;
	reached.insert(get_pointed());
	std::map<kworld_ptr, kworld_ptr> map_for_edges;
	add_ret_ontic_worlds(get_pointed(), reached, ret, fully_obs_agents, act, domain::get_instance().get_act_check(), map_for_edges);

	kedge_ptr_set::const_iterator it_kedptr;
	std::map<kworld_ptr, kworld_ptr>::const_iterator it_kwmap;

	//The updated edges
	kworld_ptr from_old, to_old;
	kworld_ptr from_new, to_new;
	agent label;
	bool is_new;
	agent_set::const_iterator it_agset;
	kworld_ptr_set for_oblivious_in_partial;
	kworld_ptr_set::const_iterator it_agset_obpa;
	for (it_kedptr = get_edges().begin(); it_kedptr != get_edges().end(); it_kedptr++) {

		from_old = it_kedptr->get_from();
		it_kwmap = map_for_edges.find(from_old);
		if (it_kwmap != map_for_edges.end()) {
			from_new = it_kwmap->second;

			to_old = it_kedptr->get_to();
			it_kwmap = map_for_edges.find(to_old);
			if (it_kwmap != map_for_edges.end()) {
				to_new = it_kwmap->second;
				label = it_kedptr->get_label();
				if (fully_obs_agents.find(label) != fully_obs_agents.end()) {
					ret.add_edge(kedge(from_new, to_new, label));
				} else if (partially_obs_agents.find(label) != partially_obs_agents.end()) {
					//Add the uncertainty for the partial observers
					is_new = false;
					to_new = ret.add_rep_world(*(to_old.get_ptr()), from_new.get_repetition(), is_new);
					ret.add_edge(kedge(from_new, to_new, label));
					if (is_new) {
						for (it_agset = fully_obs_agents.begin(); it_agset != fully_obs_agents.end(); it_agset++) {
							ret.add_edge(kedge(to_new, to_new, *it_agset));
						}
						for (it_agset = partially_obs_agents.begin(); it_agset != partially_obs_agents.end(); it_agset++) {
							label = *it_agset;
							ret.add_edge(kedge(to_new, to_new, label));
							ret.add_edge(kedge(to_new, from_new, label));
						}
						for (it_agset = oblivious_obs_agents.begin(); it_agset != oblivious_obs_agents.end(); it_agset++) {
							label = *it_agset;
							for_oblivious_in_partial = get_B_reachable_worlds(label, to_old);
							for (it_agset_obpa = for_oblivious_in_partial.begin(); it_agset_obpa != for_oblivious_in_partial.end(); it_agset_obpa++) {
								ret.add_edge(kedge(to_new, *it_agset_obpa, label));
							}
							//ret.add_edge(kedge(to_new, from_old, *it_agset));

						}
					}
				} else {
					//The edges that connects the update partial Kripke structure to the old one.
					ret.add_edge(kedge(from_new, to_old, label));
				}
			} else {
				label = it_kedptr->get_label();
				if (oblivious_obs_agents.find(label) != oblivious_obs_agents.end()) {
					ret.add_edge(kedge(from_new, to_old, label));
				}
			}
		}
	}

	it_kwmap = map_for_edges.find(get_pointed());
	if (it_kwmap != map_for_edges.end()) {
		ret.set_pointed(it_kwmap->second);
	} else {
		std::cerr << "\nInvestigate the case when pointed is not update\n";
		exit(1);
	}

	return ret;
}

kstate kstate::execute_ontic(const action & act) const
{
	/** \bug What happen if ontic removes ignorance?
	 * for example:
	 * - act-> i,g then (i,g,h),(-i,g,h),(i,-g,h),(-i,-g,-h) are all equal to (i,g,h) in this case is \ref add_world
	 * - act-> i,g then (-i,g,h) in a "different" k (i,-g,h) then two different states -- ontic shouldn't duplicate worlds
	 * because there are not partial but can inherit the duplicates so
	 *
	 * What if the action does nothing?*/
	return execute(act);

}

kstate kstate::execute_sensing(const action & act) const
{
	/** We assume that if you sense something that is in contrast with your belief the sensing wins.
	 * Executing a sensing then is just:
	 * - execute an ontic for the fully observants.
	 * - link the new worlds (derived from the ontic) with copies of old (replica of the old) state for the partially.
	 * - Link this copies with themselves for fully and partially.
	 * - Link the old state for oblivious.*/
	/**\bug Wrong because the fluent changes accordingly to the pointed world*/


	//The execution are all the same if we consider that false beliefs don't count.
	kstate ret;

	//This finds all the worlds that are reachable from the initial state following
	//the edges labeled with fully observant agents.
	agent_set fully_obs_agents = get_agents_if_entailed(act.get_fully_observants(), get_pointed());
	agent_set partially_obs_agents = get_agents_if_entailed(act.get_partially_observants(), get_pointed());
	agent_set oblivious_obs_agents = domain::get_instance().get_agents();
	minus_set(oblivious_obs_agents, fully_obs_agents);
	minus_set(oblivious_obs_agents, partially_obs_agents);

	//DEBUG PRINT
	/*if (act.get_name().compare("b_check_1") == 0) {
		std::cout << "\nFully observant: ";
		printer::get_instance().print_list_ag(fully_obs_agents);
		std::cout << "\nPartially observant: ";
		printer::get_instance().print_list_ag(partially_obs_agents);
		std::cout << "\nOblivious observant: ";
		printer::get_instance().print_list_ag(oblivious_obs_agents);
	}*/

	if (oblivious_obs_agents.size() > 0) {
		ret = (*this);
		ret.set_max_depth(ret.get_max_depth() + 1);
	}

	//pas tmp for entailment
	kworld_ptr_set reached;
	reached.insert(get_pointed());
	std::map<kworld_ptr, kworld_ptr> map_for_edges;
	add_ret_ontic_worlds(get_pointed(), reached, ret, fully_obs_agents, act, domain::get_instance().get_act_check(), map_for_edges);

	kedge_ptr_set::const_iterator it_kedptr;
	std::map<kworld_ptr, kworld_ptr>::const_iterator it_kwmap;

	//The updated edges
	kworld_ptr from_old, to_old;
	kworld_ptr from_new, to_new;
	agent label;
	bool is_new;
	agent_set::const_iterator it_agset;
	kworld_ptr_set for_oblivious_in_partial;
	kworld_ptr_set::const_iterator it_agset_obpa;
	for (it_kedptr = get_edges().begin(); it_kedptr != get_edges().end(); it_kedptr++) {

		from_old = it_kedptr->get_from();
		it_kwmap = map_for_edges.find(from_old);
		if (it_kwmap != map_for_edges.end()) {
			from_new = it_kwmap->second;

			to_old = it_kedptr->get_to();
			it_kwmap = map_for_edges.find(to_old);
			if (it_kwmap != map_for_edges.end()) {
				to_new = it_kwmap->second;
				label = it_kedptr->get_label();
				if (fully_obs_agents.find(label) != fully_obs_agents.end()) {
					ret.add_edge(kedge(from_new, to_new, label));
				} else if (partially_obs_agents.find(label) != partially_obs_agents.end()) {
					//Add the uncertainty for the partial observers
					is_new = false;
					to_new = ret.add_rep_world(*(to_old.get_ptr()), from_new.get_repetition(), is_new);
					ret.add_edge(kedge(from_new, to_new, label));
					if (is_new) {
						for (it_agset = fully_obs_agents.begin(); it_agset != fully_obs_agents.end(); it_agset++) {
							ret.add_edge(kedge(to_new, to_new, *it_agset));
						}
						for (it_agset = partially_obs_agents.begin(); it_agset != partially_obs_agents.end(); it_agset++) {
							label = *it_agset;
							ret.add_edge(kedge(to_new, to_new, label));
							ret.add_edge(kedge(to_new, from_new, label));
						}
						for (it_agset = oblivious_obs_agents.begin(); it_agset != oblivious_obs_agents.end(); it_agset++) {
							label = *it_agset;
							for_oblivious_in_partial = get_B_reachable_worlds(label, to_old);
							for (it_agset_obpa = for_oblivious_in_partial.begin(); it_agset_obpa != for_oblivious_in_partial.end(); it_agset_obpa++) {
								ret.add_edge(kedge(to_new, *it_agset_obpa, label));
							}
							//ret.add_edge(kedge(to_new, from_old, *it_agset));

						}
					}
				} else {
					//The edges that connects the update partial Kripke structure to the old one.
					ret.add_edge(kedge(from_new, to_old, label));
				}
			} else {
				label = it_kedptr->get_label();
				if (oblivious_obs_agents.find(label) != oblivious_obs_agents.end()) {
					ret.add_edge(kedge(from_new, to_old, label));
				}
			}
		}
	}

	it_kwmap = map_for_edges.find(get_pointed());
	if (it_kwmap != map_for_edges.end()) {
		ret.set_pointed(it_kwmap->second);
	} else {
		std::cerr << "\nInvestigate the case when pointed is not update\n";
		exit(1);
	}

	return ret;
}

kstate kstate::execute_announcement(const action & act) const
{
	/** We assume that if you announce something everyone believes at it, so:
	 * - execute an ontic for the fully observants.
	 * - link the new worlds (derived from the ontic) with copies of old (replica of the old) state for the partially.
	 * - Link this copies with themselves for fully and partially.
	 * - Link the old state for oblivious.*/
	/**\bug Wrong because the fluent changes accordingly to the pointed world*/

	return execute(act);
}

void kstate::print() const
{
	int counter = 1;
	std::cout << std::endl;
	std::cout << "The Pointed World has id ";
	printer::get_instance().print_list(get_pointed().get_fluent_set());
	std::cout << std::endl;
	std::cout << "*******************************************************************" << std::endl;
	;
	kworld_ptr_set::const_iterator it_kwset;
	std::cout << "World List:" << std::endl;

	for (it_kwset = get_worlds().begin(); it_kwset != get_worlds().end(); it_kwset++) {
		std::cout << "W-" << counter << ": ";
		printer::get_instance().print_list(it_kwset->get_fluent_set());
		std::cout << " rep:" << it_kwset->get_repetition();
		std::cout << std::endl;
		counter++;
	}
	counter = 1;
	std::cout << std::endl;
	std::cout << "*******************************************************************" << std::endl;
	kedge_ptr_set::const_iterator it_keset;
	std::cout << "Edge List:" << std::endl;
	for (it_keset = get_edges().begin(); it_keset != get_edges().end(); it_keset++) {

		std::cout << "E-" << counter << ": (";
		printer::get_instance().print_list(it_keset->get_from().get_fluent_set());
		std::cout << "," << it_keset->get_from().get_repetition();
		std::cout << ") - (";
		printer::get_instance().print_list(it_keset->get_to().get_fluent_set());
		std::cout << "," << it_keset->get_to().get_repetition();
		std::cout << ") ag:" << domain::get_instance().get_grounder().deground_agent(it_keset->get_label());
		std::cout << std::endl;
		counter++;

	}
	std::cout << "*******************************************************************" << std::endl;
}

void kstate::print_graphviz(std::ostream& graphviz) const
{
	fluent_set::const_iterator it_fl;

	std::map<std::string, std::string> map_name;

	graphviz << "	node [shape = doublecircle]; \"";
	for (it_fl = get_pointed().get_fluent_set().begin(); it_fl != get_pointed().get_fluent_set().end(); it_fl++) {
		graphviz << domain::get_instance().get_grounder().deground_fluent(*it_fl);
		graphviz << "_";
	}
	graphviz << get_pointed().get_repetition() << "\";\n";



	graphviz << "//WORLDS List:" << std::endl;
	graphviz << "	node [shape = circle];\n";
	kworld_ptr_set::const_iterator it_kwset;
	for (it_kwset = get_worlds().begin(); it_kwset != get_worlds().end(); it_kwset++) {

		graphviz << "	\"";
		for (it_fl = it_kwset->get_fluent_set().begin(); it_fl != it_kwset->get_fluent_set().end(); it_fl++) {
			graphviz << domain::get_instance().get_grounder().deground_fluent(*it_fl);
			graphviz << "_";
		}
		graphviz << it_kwset->get_repetition() << "\";\n";
	}

	graphviz << "\n\n";
	graphviz << "//RANKS List:" << std::endl;
	for (unsigned int rank = 0; rank <= get_max_depth(); rank++) {
		graphviz << "	{rank = same; ";
		for (it_kwset = get_worlds().begin(); it_kwset != get_worlds().end(); it_kwset++) {

			if (rank == it_kwset->get_repetition()) {
				graphviz << "\"";
				for (it_fl = it_kwset->get_fluent_set().begin(); it_fl != it_kwset->get_fluent_set().end(); it_fl++) {
					graphviz << domain::get_instance().get_grounder().deground_fluent(*it_fl);
					graphviz << "_";
				}
				graphviz << it_kwset->get_repetition() << "\"; ";
			}

		}
		graphviz << "}\n";
	}

	graphviz << "\n\n";
	graphviz << "//EDGES List:" << std::endl;

	std::map < std::tuple<std::string, std::string>, std::set<std::string> > edges;

	kedge_ptr_set::const_iterator it_keset;
	std::tuple<std::string, std::string> tmp_tuple;
	std::string tmp_string = "";
	for (it_keset = get_edges().begin(); it_keset != get_edges().end(); it_keset++) {
		tmp_string = "";
		for (it_fl = it_keset->get_from().get_fluent_set().begin(); it_fl != it_keset->get_from().get_fluent_set().end(); it_fl++) {
			tmp_string += domain::get_instance().get_grounder().deground_fluent(*it_fl);
			tmp_string += "_";
		}
		tmp_string += std::to_string(it_keset->get_from().get_repetition());
		std::get<0>(tmp_tuple) = tmp_string;

		tmp_string = "";
		for (it_fl = it_keset->get_to().get_fluent_set().begin(); it_fl != it_keset->get_to().get_fluent_set().end(); it_fl++) {
			tmp_string += domain::get_instance().get_grounder().deground_fluent(*it_fl);
			tmp_string += "_";
		}
		tmp_string += std::to_string(it_keset->get_to().get_repetition());
		std::get<1>(tmp_tuple) = tmp_string;

		edges[tmp_tuple].insert(domain::get_instance().get_grounder().deground_agent(it_keset->get_label()));
	}


	std::map < std::tuple<std::string, std::string>, std::set < std::string>>::iterator it_map;
	std::map < std::tuple<std::string, std::string>, std::set < std::string>>::const_iterator it_map_2;

	std::map < std::tuple<std::string, std::string>, std::set < std::string>> to_print_double;
	for (it_map = edges.begin(); it_map != edges.end(); it_map++) {
		for (it_map_2 = it_map; it_map_2 != edges.end(); it_map_2++) {
			if (std::get<0>(it_map->first).compare(std::get<1>(it_map_2->first)) == 0) {
				if (std::get<1>(it_map->first).compare(std::get<0>(it_map_2->first)) == 0) {
					if (it_map->second == it_map_2->second) {
						if (std::get<0>(it_map->first).compare(std::get<1>(it_map->first)) != 0) {
							to_print_double[it_map->first] = it_map->second;
							edges.erase(it_map_2);
							it_map = edges.erase(it_map);
						}
					}
				}
			}
		}
	}



	std::set<std::string>::const_iterator it_stset;
	for (it_map = edges.begin(); it_map != edges.end(); it_map++) {
		graphviz << "	\"";
		graphviz << std::get<0>(it_map->first);
		graphviz << "\" -> \"";
		graphviz << std::get<1>(it_map->first);
		graphviz << "\" ";
		graphviz << "[ label = \"";
		tmp_string = "";
		for (it_stset = it_map->second.begin(); it_stset != it_map->second.end(); it_stset++) {
			tmp_string += *it_stset;
			tmp_string += ",";
		}
		tmp_string.pop_back();
		graphviz << tmp_string;
		graphviz << "\" ];\n";
	}

	for (it_map = to_print_double.begin(); it_map != to_print_double.end(); it_map++) {
		graphviz << "	\"";
		graphviz << std::get<0>(it_map->first);
		graphviz << "\" -> \"";
		graphviz << std::get<1>(it_map->first);
		graphviz << "\" ";
		graphviz << "[ dir=both label = \"";
		tmp_string = "";
		for (it_stset = it_map->second.begin(); it_stset != it_map->second.end(); it_stset++) {
			tmp_string += *it_stset;
			tmp_string += ",";
		}
		tmp_string.pop_back();
		graphviz << tmp_string;
		graphviz << "\" ];\n";
	}
}
/*for (it_keset = get_edges().begin(); it_keset != get_edges().end(); it_keset++) {
	graphviz << "	\"";
	for (it_fl = it_keset->get_from().get_fluent_set().begin(); it_fl != it_keset->get_from().get_fluent_set().end(); it_fl++) {
		graphviz << domain::get_instance().get_grounder().deground_fluent(*it_fl);
		graphviz << "_";
	}
	graphviz << it_keset->get_from().get_repetition() << "\" -> \"";

	for (it_fl = it_keset->get_to().get_fluent_set().begin(); it_fl != it_keset->get_to().get_fluent_set().end(); it_fl++) {
		graphviz << domain::get_instance().get_grounder().deground_fluent(*it_fl);
		graphviz << "_";
	}
	graphviz << it_keset->get_to().get_repetition() << "\" ";

	graphviz << "[ label = \"" << domain::get_instance().get_grounder().deground_agent(it_keset->get_label()) << "\" ];\n";
}
}

void kstate::print_graphviz(std::ostream& graphviz) const
{
fluent_set::const_iterator it_fl;

graphviz << "//WORLDS List:" << std::endl;
graphviz << "	node [shape = circle];\n\n";
kworld_ptr_set::const_iterator it_kwset;
char first_state = 'Q';
std::map <std::string, std::string> names;
std::string description = "";
for (it_kwset = get_worlds().begin(); it_kwset != get_worlds().end(); it_kwset++) {
	description = "";
	for (it_fl = it_kwset->get_fluent_set().begin(); it_fl != it_kwset->get_fluent_set().end(); it_fl++) {
		description += domain::get_instance().get_grounder().deground_fluent(*it_fl);
		description += ",";
	}
	description.pop_back();
	names[description] = description;
	names[description].append("_");
	graphviz << "	\"" << names[description] << it_kwset->get_repetition() << "\";\n";
}


graphviz << "	node [shape = doublecircle]";
description = "";
for (it_fl = get_pointed().get_fluent_set().begin(); it_fl != get_pointed().get_fluent_set().end(); it_fl++) {
	description += domain::get_instance().get_grounder().deground_fluent(*it_fl);
	description += ",";
}
description.pop_back();
graphviz << " \"" << names[description] << get_pointed().get_repetition() << "\";\n";

graphviz << "\n\n";
graphviz << "//RANKS List:" << std::endl;
for (unsigned int rank = 0; rank <= get_max_depth(); rank++) {
	graphviz << "	{rank = same; ";
	for (it_kwset = get_worlds().begin(); it_kwset != get_worlds().end(); it_kwset++) {

		if (rank == it_kwset->get_repetition()) {
			description = "";
			for (it_fl = get_pointed().get_fluent_set().begin(); it_fl != get_pointed().get_fluent_set().end(); it_fl++) {
				description += domain::get_instance().get_grounder().deground_fluent(*it_fl);
				description += ",";
			}
			description.pop_back();
			graphviz << " \"" << names[description] << rank << "\";";
		}

	}
	graphviz << "}\n";
}

graphviz << "\n\n";
graphviz << "//EDGES List:" << std::endl;

std::map < std::tuple<std::string, std::string>, std::set<std::string> > edges;

kedge_ptr_set::const_iterator it_keset;
std::tuple<std::string, std::string> tmp_tuple;
for (it_keset = get_edges().begin(); it_keset != get_edges().end(); it_keset++) {
	description = "";
	for (it_fl = it_keset->get_from().get_fluent_set().begin(); it_fl != it_keset->get_from().get_fluent_set().end(); it_fl++) {
		description += domain::get_instance().get_grounder().deground_fluent(*it_fl);
		description += ",";
	}
	description.pop_back();
	std::get<0>(tmp_tuple) = names[description] + std::to_string(it_keset->get_from().get_repetition());

	description = "";
	for (it_fl = it_keset->get_to().get_fluent_set().begin(); it_fl != it_keset->get_to().get_fluent_set().end(); it_fl++) {
		description += domain::get_instance().get_grounder().deground_fluent(*it_fl);
		description += ",";
	}
	description.pop_back();
	std::get<1>(tmp_tuple) = names[description] + std::to_string(it_keset->get_to().get_repetition());

	edges[tmp_tuple].insert(domain::get_instance().get_grounder().deground_agent(it_keset->get_label()));
}


std::map < std::tuple<std::string, std::string>, std::set < std::string>>::iterator it_map;
std::map < std::tuple<std::string, std::string>, std::set < std::string>>::const_iterator it_map_2;

std::map < std::tuple<std::string, std::string>, std::set < std::string>> to_print_double;
for (it_map = edges.begin(); it_map != edges.end(); it_map++) {
	for (it_map_2 = it_map; it_map_2 != edges.end(); it_map_2++) {
		if (std::get<0>(it_map->first).compare(std::get<1>(it_map_2->first)) == 0) {
			if (std::get<1>(it_map->first).compare(std::get<0>(it_map_2->first)) == 0) {
				if (it_map->second == it_map_2->second) {
					if (std::get<0>(it_map->first).compare(std::get<1>(it_map->first)) != 0) {
						to_print_double[it_map->first] = it_map->second;
						edges.erase(it_map_2);
						it_map = edges.erase(it_map);
					}
				}
			}
		}
	}
}



std::set<std::string>::const_iterator it_stset;
for (it_map = edges.begin(); it_map != edges.end(); it_map++) {
	graphviz << "	\"";
	graphviz << std::get<0>(it_map->first);
	graphviz << "\" -> \"";
	graphviz << std::get<1>(it_map->first);
	graphviz << "\" ";
	graphviz << "[ label = \"";
	description = "";
	for (it_stset = it_map->second.begin(); it_stset != it_map->second.end(); it_stset++) {
		description += *it_stset;
		description += ",";
	}
	description.pop_back();
	graphviz << description;
	graphviz << "\" ];\n";
}

for (it_map = to_print_double.begin(); it_map != to_print_double.end(); it_map++) {
	graphviz << "	\"";
	graphviz << std::get<0>(it_map->first);
	graphviz << "\" -> \"";
	graphviz << std::get<1>(it_map->first);
	graphviz << "\" ";
	graphviz << "[ dir=both label = \"";
	description = "";
	for (it_stset = it_map->second.begin(); it_stset != it_map->second.end(); it_stset++) {
		description += *it_stset;
		description += ",";
	}
	description.pop_back();
	graphviz << description;
	graphviz << "\" ];\n";
}

}*/

/******************************MOVE TO HELPER*********************************/

template <class T>
void kstate::sum_set(std::set<T> & to_modify, const std::set<T> & factor2) const
{
	/**\todo move to helper*/
	typename std::set<T>::const_iterator it_kwset;
	for (it_kwset = factor2.begin(); it_kwset != factor2.end(); it_kwset++) {

		to_modify.insert(*it_kwset);
	}
}

template <class T>
void kstate::minus_set(std::set<T> & to_modify, const std::set<T> & factor2) const
{ /**\todo move to helper*/

	typename std::set<T>::const_iterator it_kwset;
	for (it_kwset = factor2.begin(); it_kwset != factor2.end(); it_kwset++) {

		to_modify.erase(*it_kwset);
	}
}

agent_set kstate::get_agents_if_entailed(const observability_map& map, const kworld_ptr & start) const
{ /**\todo move to helper*/

	agent_set ret;
	observability_map::const_iterator it_map;
	for (it_map = map.begin(); it_map != map.end(); it_map++) {
		if (entails(it_map->second, start)) {

			ret.insert(it_map->first);
		}
	}
	return ret;
}

fluent_formula kstate::get_effects_if_entailed(const effects_map & map, const kworld_ptr & start) const
{ /**\todo move to helper*/

	fluent_formula ret;
	effects_map::const_iterator it_map;
	for (it_map = map.begin(); it_map != map.end(); it_map++) {
		if (entails(it_map->second, start)) {
			ret = helper::and_ff(ret, it_map->first);
		}
	}
	if (ret.size() > 1) {

		std::cerr << "\nNon determinism in action effect is not supported-1.\n";
		exit(1);
	}
	return ret;
}

/*fluent_formula kstate::get_sensing_effects_if_entailed(const effects_map & map, const kworld_ptr & start) const
{ 	fluent_formula ret;
	fluent_formula retret;
	fluent_set ret_tmp;

	effects_map::const_iterator it_map;
	for (it_map = map.begin(); it_map != map.end(); it_map++) {
		if (entails(it_map->second, start)) {
			if (start.get_ptr()->entails(it_map->first))
				ret = helper::and_ff(ret, it_map->first);
			else {
				fluent_formula tmpp = it_map->first;
				if (tmpp.size() == 1) {
					fluent_set tmp = *(tmpp.begin());
					if (tmp.size() == 1) {
						fluent temp = *(tmp.begin());
						if (temp % 2 == 0) {
							temp = temp + 1;
						} else {
							temp = temp - 1;
						}
						ret_tmp.insert(temp);
						retret.insert(ret_tmp);

					} else {
						std::cerr << "DEBUG--ERROR in sensing;";
						exit(1);
					}
				} else {
					std::cerr << "DEBUG--ERROR in sensing;";
					exit(1);
				}
				ret = helper::and_ff(ret, retret);
				retret.erase(ret_tmp);
			}

		}

	}
	if (ret.size() > 1) {

		std::cerr << "\nNon determinism in action effect is not supported-2.\n";
		exit(1);
	}
	return ret;
}*/