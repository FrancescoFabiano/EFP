/*
 * \brief Implementation of \ref pstate.h
 *
 * \copyright GNU Public License.
 *
 * \author Francesco Fabiano.
 * \date September 16, 2019
 */

#include <iostream>
#include <tuple>


#include "pstate.h"
#include "../../domain/domain.h"
#include "../../utilities/helper.h"

void pstate::set_worlds(const pworld_ptr_set & to_set)
{
	m_worlds = to_set;
}

void pstate::set_pointed(const pworld_ptr & to_set)
{
	m_pointed = to_set;
}

void pstate::set_beliefs(const pworld_transitive_map & to_set)
{
	m_beliefs = to_set;
}

void pstate::set_max_depth(unsigned int to_set)
{
	if (m_max_depth < to_set) m_max_depth = to_set;
}

const pworld_ptr_set & pstate::get_worlds() const
{
	return m_worlds;
}

const pworld_ptr & pstate::get_pointed() const
{
	return m_pointed;
}

const pworld_transitive_map & pstate::get_beliefs() const
{
	return m_beliefs;
}

unsigned int pstate::get_max_depth() const
{
	return m_max_depth;
}

bool pstate::operator=(const pstate & to_copy)
{
	set_worlds(to_copy.get_worlds());
	set_beliefs(to_copy.get_beliefs());
	m_max_depth = to_copy.get_max_depth();
	set_pointed(to_copy.get_pointed());
	//std::cerr << "\nDEBUG: eq ";
	return true;
}

bool pstate::operator<(const pstate & to_compare) const
{

	/*if (m_max_depth < to_compare.get_max_depth()) {
		return true;
	} else if (m_max_depth > to_compare.get_max_depth()) {
		return false;
	}*/

	if (m_pointed < to_compare.get_pointed()) {
		return true;
	} else if (m_pointed > to_compare.get_pointed()) {
		return false;
	}

	if (m_worlds < to_compare.get_worlds()) {
		return true;
	} else if (m_worlds > to_compare.get_worlds()) {
		return false;
	}

	pworld_transitive_map::const_iterator it_tramap1;
	pworld_transitive_map::const_iterator it_tramap2 = to_compare.get_beliefs().begin();

	pworld_map tmp_pwmap1, tmp_pwmap2;
	pworld_map::const_iterator it_pwmap1, it_pwmap2;
	//The same size is assured by the same size of m_worlds
	for (it_tramap1 = m_beliefs.begin(); it_tramap1 != m_beliefs.end(); it_tramap1++) {
		if (it_tramap1->first < it_tramap2->first) {
			return true;
		} else if (it_tramap1->first > it_tramap2->first) {
			return false;
		}

		tmp_pwmap1 = it_tramap1->second;
		tmp_pwmap2 = it_tramap2->second;
		if (tmp_pwmap1.size() < tmp_pwmap2.size()) {
			return true;
		} else if (tmp_pwmap1.size() > tmp_pwmap2.size()) {
			return false;
		}
		it_pwmap2 = tmp_pwmap2.begin();
		for (it_pwmap1 = tmp_pwmap1.begin(); it_pwmap1 != tmp_pwmap1.end(); it_pwmap1++) {
			if (it_pwmap1->first < it_pwmap2->first) {
				return true;
			} else if (it_pwmap1->first > it_pwmap2->first) {
				return false;
			}

			if (it_pwmap1->second < it_pwmap2->second) {
				return true;
			} else if (it_pwmap1->second > it_pwmap2->second) {
				return false;
			}
			it_pwmap2++;
		}
		it_tramap2++;
	}
	return false;
}

bool pstate::entails(fluent f) const
{
	return entails(f, m_pointed);
}

bool pstate::entails(fluent f, const pworld_ptr & world) const
{
	return world.get_ptr()->entails(f);
}

bool pstate::entails(const fluent_set & fl) const
{
	return entails(fl, m_pointed);
}

bool pstate::entails(const fluent_set & fl, const pworld_ptr & world) const
{
	return world.get_ptr()->entails(fl);
}

bool pstate::entails(const fluent_formula & ff) const
{
	return entails(ff, m_pointed);
}

bool pstate::entails(const fluent_formula & ff, const pworld_ptr & world) const
{
	return world.get_ptr()->entails(ff);
}

bool pstate::entails(const belief_formula & bf) const
{
	return entails(bf, m_pointed);
}

bool pstate::entails(const belief_formula & to_check, const pworld_ptr_set & reachable) const
{
	pworld_ptr_set::const_iterator it_pws;
	for (it_pws = reachable.begin(); it_pws != reachable.end(); it_pws++) {
		if (!entails(to_check, *it_pws))
			return false;
	}
	return true;
}

bool pstate::entails(const belief_formula & bf, const pworld_ptr & world) const
{
	/*
	 The entailment of a \ref belief_formula just call recursively the entailment on all the reachable world with that formula.
	 */
	pworld_ptr_set D_reachable;
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
		//Check the entails on the D-reachable worlds
	case D_FORMULA:
		D_reachable = get_D_reachable_worlds(bf.get_group_agents(), world);
		if (D_reachable.size() > 0) {
			return entails(bf.get_bf1(), D_reachable);
		} else {
			return false;
		}
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

	return false;
}

//bool pstate::entails(const formula_list & to_check, const pworld_ptr & world) const
//{
//	//formula_list expresses CNF formula
//	formula_list::const_iterator it_fl;
//	for (it_fl = to_check.begin(); it_fl != to_check.end(); it_fl++) {
//		if (!entails(*it_fl, world)) {
//			return false;
//		}
//	}
//	return true;
//}

const pworld_ptr_set pstate::get_B_reachable_worlds(agent ag, const pworld_ptr & world) const
{
	pworld_ptr_set ret;
	auto pw_map = m_beliefs.find(world);

	if (pw_map != m_beliefs.end()) {
		auto pw_set = pw_map->second.find(ag);
		if (pw_set != pw_map->second.end()) {
			sum_set(ret, pw_set->second);
		}
	}
	return ret;
}

bool pstate::get_B_reachable_worlds_recoursive(agent ag, const pworld_ptr & world, pworld_ptr_set& ret) const
{
	/** \todo check: If a--i-->b, b--i-->c then a--i-->c must be there*/
	auto pw_map = m_beliefs.find(world);

	if (pw_map != m_beliefs.end()) {
		auto pw_set = pw_map->second.find(ag);
		if (pw_set != pw_map->second.end()) {
			unsigned long previous_size = ret.size();
			sum_set(ret, pw_set->second);
			unsigned long current_size = ret.size();

			return previous_size == current_size;
		}
		/**@bug: We don't know why sometimes is outside the two if cases.*/
	}
	return true;
}

const pworld_ptr_set pstate::get_E_reachable_worlds(const agent_set & ags, const pworld_ptr & world) const
{
	/*The K^0 call of this function*/
	pworld_ptr_set ret;
	agent_set::const_iterator it_agset;
	for (it_agset = ags.begin(); it_agset != ags.end(); it_agset++) {
		sum_set(ret, get_B_reachable_worlds(*it_agset, world));
	}

	return ret;
}

bool pstate::get_E_reachable_worlds_recoursive(const agent_set & ags, const pworld_ptr_set & worlds, pworld_ptr_set & ret) const
{
	/*The K^i (recoursive) call of this function*/

	bool is_fixed_point = true;
	pworld_ptr_set::const_iterator it_pwptr;
	agent_set::const_iterator it_agset;
	for (it_pwptr = worlds.begin(); it_pwptr != worlds.end(); it_pwptr++) {
		for (it_agset = ags.begin(); it_agset != ags.end(); it_agset++) {
			if (!get_B_reachable_worlds_recoursive(*it_agset, *it_pwptr, ret)) {
				is_fixed_point = false;
			}
		}
	}
	return is_fixed_point;

}

const pworld_ptr_set pstate::get_C_reachable_worlds(const agent_set & ags, const pworld_ptr & world) const
{
	//Use of fixed point to stop.
	bool is_fixed_point = false;
	pworld_ptr_set newly_reached = get_E_reachable_worlds(ags, world);
	pworld_ptr_set already_reached;
	pworld_ptr_set ret;
	while (!is_fixed_point) {
		sum_set(newly_reached, ret);
		minus_set(newly_reached, already_reached);
		is_fixed_point = get_E_reachable_worlds_recoursive(ags, newly_reached, ret);
		already_reached = newly_reached;
	}
	return ret;
}

const pworld_ptr_set pstate::get_D_reachable_worlds(const agent_set & ags, const pworld_ptr & world) const
{
	/**@bug: Notion of D-Reachable is correct (page 24 of Reasoning about Knowledge)*/
	agent_set::const_iterator it_agset = ags.begin();
	pworld_ptr_set ret = get_B_reachable_worlds((*it_agset), world);
	it_agset++;

	for (; it_agset != ags.end(); it_agset++) {

		pworld_ptr_set::iterator it_pwset1 = ret.begin();
		pworld_ptr_set to_intersect = get_B_reachable_worlds((*it_agset), world);
		pworld_ptr_set::const_iterator it_pwset2 = to_intersect.begin();
		while ((it_pwset1 != ret.end()) && (it_pwset2 != to_intersect.end())) {

			if ((*it_pwset1 < *it_pwset2) && ((*it_pwset1).get_fluent_based_id().compare((*it_pwset2).get_fluent_based_id()) != 0)) {
				ret.erase(it_pwset1++);
			} else if ((*it_pwset2 < *it_pwset1) && ((*it_pwset1).get_fluent_based_id().compare((*it_pwset2).get_fluent_based_id()) != 0)) {
				++it_pwset2;
			} else { // *it_pwset1 == *it_pwset2
				++it_pwset1;
				++it_pwset2;
			}
		}

		// Anything left in ret from here on did not appear in to_intersect,
		// so we remove it.
		ret.erase(it_pwset1, ret.end());

	}
	std::cerr << "\nERROR: D_REACHABLILITY not yet Implemented correctly\n";
	exit(1);
	return ret;
}

void pstate::add_world(const pworld & world)
{

	m_worlds.insert(pstore::get_instance().add_world(world));
}

pworld_ptr pstate::add_rep_world(const pworld & world, unsigned short repetition, bool& is_new)
{
	pworld_ptr tmp = pstore::get_instance().add_world(world);
	tmp.set_repetition(repetition);
	is_new = std::get<1>(m_worlds.insert(tmp));

	return tmp;
}

pworld_ptr pstate::add_rep_world(const pworld & world, unsigned short old_repetition)
{
	bool tmp = false;

	return add_rep_world(world, get_max_depth() + old_repetition, tmp);
}

pworld_ptr pstate::add_rep_world(const pworld & world)
{
	bool tmp = false;

	return add_rep_world(world, get_max_depth(), tmp);
}

void pstate::add_edge(const pworld_ptr &from, const pworld_ptr &to, agent ag)
{
	pworld_transitive_map::iterator from_beliefs = m_beliefs.find(from);

	if (from_beliefs != m_beliefs.end()) {
		pworld_map::iterator ag_beliefs = from_beliefs->second.find(ag);

		if (ag_beliefs != from_beliefs->second.end()) {
			ag_beliefs->second.insert(to);
		} else {
			from_beliefs->second.insert(pworld_map::value_type(ag,{to}));
		}
	} else {

		pworld_map pwm;
		pwm.insert(pworld_map::value_type(ag,{to}));
		m_beliefs.insert(pworld_transitive_map::value_type(from, pwm));
	}
}

void pstate::add_pworld_beliefs(const pworld_ptr & world, const pworld_map & beliefs)
{

	m_beliefs[world] = beliefs;
}

void pstate::build_initial()
{

	/** \todo for now prune building.*/
	std::cout << "\nBuilding initial possibility...\n";
	build_initial_prune();
}

void pstate::build_initial_structural()
{

}

void pstate::build_initial_prune()
{

	/*Building of all the possible consistent \ref pworld and setting the pointed world.
	 * Creation of all the \ref fluent combinations. All the consistent ones are added to \ref pstore.*/
	fluent_set permutation;
	initially ini_conditions = domain::get_instance().get_initial_description();
	//	std::cerr << "\nDEBUG: Initially known fluents: ";
	//	printer::get_instance().print_list(domain::get_instance().get_grounder().deground_fluent(ini_conditions.get_initially_known_fluents()));
	generate_initial_pworlds(permutation, 0, ini_conditions.get_initially_known_fluents());


	/*Building of all the consistent edges.*/
	generate_initial_pedges();
}

/*From https://www.geeksforgeeks.org/generate-all-the-binary-strings-of-n-bits/ since is like generating all the binary numbers.*/
void pstate::generate_initial_pworlds(fluent_set& permutation, int index, const fluent_set & initially_known)
{
	int fluent_number = domain::get_instance().get_fluent_number();
	if (index / 2 == fluent_number) {
		pworld to_add(permutation);
		add_initial_pworld(to_add);

		return;
	}
	fluent_set permutation_2 = permutation;
	//Add the \ref fluent in positive version
	if (initially_known.find(index + 1) == initially_known.end()) {
		permutation.insert(index);
		generate_initial_pworlds(permutation, index + 2, initially_known);
	}
	if (initially_known.find(index) == initially_known.end()) {
		permutation_2.insert(index + 1);
		generate_initial_pworlds(permutation_2, index + 2, initially_known);
	}
}

void pstate::add_initial_pworld(const pworld & possible_add)
{
	initially ini_conditions = domain::get_instance().get_initial_description();

	switch ( ini_conditions.get_ini_restriction() ) {
	case S5:
	{
		/* Since the common knowledge is on all the agent it means that every possible \ref pworld
		 * in the initial state must entail *phi* where C(*phi*) is an initial condition.*/

		//Already setted in \ref domain::build_initially(bool).
		if (possible_add.entails(ini_conditions.get_ff_forS5())) {
			add_world(possible_add);
			if (possible_add.entails(ini_conditions.get_pointed_world_conditions())) {
				m_pointed = pworld_ptr(possible_add);
				/*std::cout << "pointed world: ";
				printer::get_instance().print_list(possible_add.get_fluent_set());
				std::cout << std::endl;*/
			}
		} else {
			//Already generated so we save it on pstore
			pstore::get_instance().add_world(possible_add);
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

void pstate::generate_initial_pedges()
{
	pworld_ptr_set::const_iterator it_pwps_1, it_pwps_2;

	pworld_ptr pwptr_tmp1, pwptr_tmp2;

	/*This for add to *this* all the possible edges.*/
	for (it_pwps_1 = m_worlds.begin(); it_pwps_1 != m_worlds.end(); it_pwps_1++) {
		for (it_pwps_2 = it_pwps_1; it_pwps_2 != m_worlds.end(); it_pwps_2++) {
			for (unsigned int i = 0; i < domain::get_instance().get_agents().size(); i++) {
				pwptr_tmp1 = *it_pwps_1;
				pwptr_tmp2 = *it_pwps_2;

				add_edge(pwptr_tmp1, pwptr_tmp2, i);
				add_edge(pwptr_tmp2, pwptr_tmp1, i);
			}
		}
	}

	//std::cout << "Tot edges: " << m_edges.size() << std::endl;

	initially ini_conditions = domain::get_instance().get_initial_description();

	formula_list::const_iterator it_fl;
	for (it_fl = ini_conditions.get_initial_conditions().begin(); it_fl != ini_conditions.get_initial_conditions().end(); it_fl++) {

		remove_initial_pedge_bf(*it_fl);
	}
	//std::cout << "Removed edges: " << count << std::endl;

	//std::cout << "Final edges: " << m_edges.size() << std::endl;

}

void pstate::remove_edge(pworld_ptr &from, const pworld &to, const agent ag)
{
	auto from_beliefs = m_beliefs.find(from);

	if (from_beliefs != m_beliefs.end()) {
		auto ag_beliefs = from_beliefs->second.find(ag);

		if (ag_beliefs != from_beliefs->second.end()) {

			ag_beliefs->second.erase(to);
		}
	}
}

void pstate::remove_initial_pedge(const fluent_formula &known_ff, agent ag)
{
	pworld_ptr_set::const_iterator it_pwps_1, it_pwps_2;

	pworld_ptr pwptr_tmp1, pwptr_tmp2;

	/** \todo maybe don't loop twice on the world but exploit using it_kwps_2 = it_kwps_1:
	 * - remove (_1, _2).
	 * - remove (_2, _1).*/
	for (it_pwps_1 = m_worlds.begin(); it_pwps_1 != m_worlds.end(); it_pwps_1++) {
		for (it_pwps_2 = it_pwps_1; it_pwps_2 != m_worlds.end(); it_pwps_2++) {
			/** \todo or entails(-known_ff)?*/
			pwptr_tmp1 = *it_pwps_1;
			pwptr_tmp2 = *it_pwps_2;
			if (pwptr_tmp1.get_ptr()->entails(known_ff) && !pwptr_tmp2.get_ptr()->entails(known_ff)) {
				remove_edge(pwptr_tmp1, *pwptr_tmp2.get_ptr(), ag);
				remove_edge(pwptr_tmp2, *pwptr_tmp1.get_ptr(), ag);
			} else if (pwptr_tmp2.get_ptr()->entails(known_ff) && !pwptr_tmp1.get_ptr()->entails(known_ff)) {

				remove_edge(pwptr_tmp2, *pwptr_tmp1.get_ptr(), ag);
				remove_edge(pwptr_tmp1, *pwptr_tmp2.get_ptr(), ag);
			}
		}
	}
}

void pstate::remove_initial_pedge_bf(const belief_formula & to_check)
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
					helper::check_Bff_notBff(tmp.get_bf1(), tmp.get_bf2(), known_ff_ptr);
					if (known_ff_ptr != nullptr) {
						//printer::get_instance().print_list(*known_ff_ptr);
						remove_initial_pedge(*known_ff_ptr, tmp.get_bf2().get_agent());
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
pstate pstate::compute_succ(const action & act) const
{
	//std::cerr << "\nDEBUG: Executing " << act.get_name();
	switch ( act.get_type() ) {
	case ONTIC:
	{
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
	case LIES:
	{
		return execute_announcement_dox(act);
		break;
	}
	default:
	{
		std::cerr << "Error in executing an action: " << act.get_name();
		std::cerr << " the type of the action is not defined correctly1";
		std::cerr << std::endl;
		exit(1);

		break;
	}
	}
}

void pstate::maintain_oblivious_believed_pworlds(pstate &ret, const agent_set & oblivious_obs_agents) const
{
	agent_set::const_iterator it_agset;
	pworld_ptr_set world_oblivious;
	pworld_ptr_set tmp_world_set;

	pworld_ptr_set::const_iterator it_wo_ob;

	if (!oblivious_obs_agents.empty()) {
		tmp_world_set = get_E_reachable_worlds(oblivious_obs_agents, get_pointed());
		for (it_agset = domain::get_instance().get_agents().begin(); it_agset != domain::get_instance().get_agents().end(); it_agset++) {
			for (it_wo_ob = tmp_world_set.begin(); it_wo_ob != tmp_world_set.end(); it_wo_ob++) {
				sum_set(world_oblivious, get_B_reachable_worlds(*it_agset, *it_wo_ob));
			}
		}
		sum_set(world_oblivious, tmp_world_set);
		ret.set_max_depth(get_max_depth() + 1);
		ret.set_worlds(world_oblivious);

		for (it_wo_ob = world_oblivious.begin(); it_wo_ob != world_oblivious.end(); it_wo_ob++) {
			auto it_pwmap = m_beliefs.find(*it_wo_ob);

			if (it_pwmap != m_beliefs.end()) {

				ret.add_pworld_beliefs(*it_wo_ob, it_pwmap->second);
			}
		}
	}
}

pworld_ptr pstate::execute_ontic_helper(const action &act, pstate &ret, const pworld_ptr &current_pw, transition_map &calculated, agent_set & oblivious_obs_agents) const
{
	// Execute the all the effects
	fluent_formula current_pw_effects = get_effects_if_entailed(act.get_effects(), current_pw);
	fluent_set world_description = current_pw.get_fluent_set();
	fluent_formula::const_iterator it_eff;

	for (it_eff = current_pw_effects.begin(); it_eff != current_pw_effects.end(); it_eff++) {
		world_description = helper::ontic_exec(*it_eff, world_description);
		//		if (act.get_name().compare("distract_c_a") == 0) {
		//			std::cerr << "\nDEBUG: Inside the first ONTIC loop " << act.get_name();
		//		}
	}

	//	if (act.get_name().compare("distract_c_a") == 0) {
	//		std::cerr << "\nDEBUG: Out the first ONTIC loop " << act.get_name();
	//	}

	pworld_ptr new_pw = ret.add_rep_world(pworld(world_description), current_pw.get_repetition()); // We add the corresponding pworld in ret
	calculated.insert(transition_map::value_type(current_pw, new_pw)); // And we update the calculated map

	auto it_pwtm = get_beliefs().find(current_pw);

	if (it_pwtm != get_beliefs().end()) {
		pworld_map::const_iterator it_pwm;
		pworld_ptr_set::const_iterator it_pws;

		for (it_pwm = it_pwtm->second.begin(); it_pwm != it_pwtm->second.end(); it_pwm++) {
			agent ag = it_pwm->first;

			//			if (act.get_name().compare("distract_c_a") == 0) {
			//				std::cerr << "\nDEBUG: Inside the SECOND ONTIC loop " << act.get_name();
			//			}

			bool is_oblivious_obs = oblivious_obs_agents.find(ag) != oblivious_obs_agents.end();

			for (it_pws = it_pwm->second.begin(); it_pws != it_pwm->second.end(); it_pws++) {

				//				if (act.get_name().compare("distract_c_a") == 0) {
				//					std::cerr << "\nDEBUG: Inside the Third ONTIC loop " << act.get_name();
				//				}

				if (is_oblivious_obs) { // If we are dealing with an OBLIVIOUS agent we maintain its beliefs as they were
					auto maintained_pworld = ret.get_worlds().find(*it_pws);

					if (maintained_pworld != ret.get_worlds().end()) {
						ret.add_edge(new_pw, *it_pws, ag);
					}
				} else { // Otherwise, if we have a FULLY OBS agent
					auto calculated_pworld = calculated.find(*it_pws);

					if (calculated_pworld != calculated.end()) { // If we already calculated the transition function for this pworld
						ret.add_edge(new_pw, calculated_pworld->second, ag); // Then we update agents' beliefs
					} else {

						pworld_ptr believed_pw = execute_ontic_helper(act, ret, *it_pws, calculated, oblivious_obs_agents);
						ret.add_edge(new_pw, believed_pw, ag);

						ret.set_max_depth(ret.get_max_depth() + 1 + current_pw.get_repetition());
					}
				}
			}
			//			if (act.get_name().compare("distract_c_a") == 0) {
			//				std::cerr << "\nDEBUG: Out the Second ONTIC loop " << act.get_name();
			//			}
		}
		//std::cerr << "\nDEBUG: Out the THIRD ONTIC loop " << act.get_name();

	}
	//std::cerr << "\nDEBUG: RETURN TO " << act.get_name();

	return new_pw;
}

pworld_ptr pstate::execute_sensing_announcement_helper(const fluent_formula &effects, pstate &ret, const pworld_ptr &current_pw, transition_map &calculated, agent_set &partially_obs_agents, agent_set &oblivious_obs_agents, bool previous_entailment) const
{
	pworld_ptr new_pw = ret.add_rep_world(pworld(current_pw.get_fluent_set()), current_pw.get_repetition()); // We add the corresponding pworld in ret
	calculated.insert(transition_map::value_type(current_pw, new_pw)); // And we update the calculated map

	auto it_pwtm = get_beliefs().find(current_pw);

	if (it_pwtm != get_beliefs().end()) {
		pworld_map::const_iterator it_pwm;
		pworld_ptr_set::const_iterator it_pws;

		for (it_pwm = it_pwtm->second.begin(); it_pwm != it_pwtm->second.end(); it_pwm++) {
			agent ag = it_pwm->first;

			bool is_oblivious_obs = oblivious_obs_agents.find(ag) != oblivious_obs_agents.end();
			bool is_partially_obs = partially_obs_agents.find(ag) != partially_obs_agents.end();
			bool is_fully_obs = !is_oblivious_obs && !is_partially_obs;

			for (it_pws = it_pwm->second.begin(); it_pws != it_pwm->second.end(); it_pws++) {
				if (is_oblivious_obs) { // If we are dealing with an OBLIVIOUS agent we maintain its beliefs as they were
					auto maintained_pworld = ret.get_worlds().find(*it_pws);

					if (maintained_pworld != ret.get_worlds().end()) {
						ret.add_edge(new_pw, *it_pws, ag);
					}
				} else { // Otherwise, if we have a FULLY/PARTIALLY observant agent
					auto calculated_pworld = calculated.find(*it_pws);
					//fluent_formula effects = get_effects_if_entailed(act.get_effects(), get_pointed());
					//bool ent = act.get_type() == SENSING ? entails(effects, *it_pws) == entails(effects, get_pointed()) : entails(effects, *it_pws);
					bool ent = entails(effects, *it_pws); // == entails(effects, get_pointed());


					bool is_consistent_belief = is_partially_obs || // If "ag" is PARTIALLY OBS, we always add an edge; If "ag" is FULLY OBS, we add an edge if he believes that "calculated" may be true (i.e., when "ent" holds) XOR
						(is_fully_obs && (ent == previous_entailment)); // if a PARTIALLY OBS agent believes that "ag" thinks that "calculated" may be true (i.e., when "previous_entailment" holds)

					if (calculated_pworld != calculated.end()) { // If we already calculated the transition function for this pworld
						if (is_consistent_belief) {
							ret.add_edge(new_pw, calculated_pworld->second, ag);
						}
					} else { // If we did not already calculate the transition function
						if (is_consistent_belief) { // We calculate it if it would result in a consistent belief...

							pworld_ptr believed_pw = execute_sensing_announcement_helper(effects, ret, *it_pws, calculated, partially_obs_agents, oblivious_obs_agents, ent);
							ret.add_edge(new_pw, believed_pw, ag);
						}
						//						else if (is_partially_obs && (ent != previous_entailment)) { // ...and when it does not entail the action effects, but a PARTIALLY OBS agent believes that it may be true
						//							pworld_ptr believed_pw_neg = execute_sensing_announcement_helper(act, ret, *it_pws, calculated, partially_obs_agents, oblivious_obs_agents, ent);
						//							ret.add_edge(new_pw, believed_pw_neg, ag);
						//						}
					}
				}
			}
		}
	}
	return new_pw;
}

pstate pstate::execute_ontic(const action & act) const
{
	pstate ret;

	//This finds all the worlds that are reachable from the initial state following
	//the edges labeled with fully observant agents.
	agent_set agents = domain::get_instance().get_agents();
	agent_set fully_obs_agents = get_agents_if_entailed(act.get_fully_observants(), get_pointed());

	agent_set oblivious_obs_agents = agents;
	minus_set(oblivious_obs_agents, fully_obs_agents);

	transition_map calculated; // A map that links the pworlds of *this* to the corresponding ones of ret
	maintain_oblivious_believed_pworlds(ret, oblivious_obs_agents);

	pworld_ptr new_pointed = execute_ontic_helper(act, ret, get_pointed(), calculated, oblivious_obs_agents);
	ret.set_pointed(new_pointed); // Updating the pointed world
	//std::cerr << "\nDEBUG: Out ONTIC " << act.get_name();

	return ret;
}

pstate pstate::execute_sensing(const action & act) const
{
	pstate ret;

	//This finds all the worlds that are reachable from the initial state following
	//the edges labeled with fully observant agents.
	agent_set agents = domain::get_instance().get_agents();
	agent_set fully_obs_agents = get_agents_if_entailed(act.get_fully_observants(), get_pointed());
	agent_set partially_obs_agents = get_agents_if_entailed(act.get_partially_observants(), get_pointed());

	agent_set oblivious_obs_agents = agents;
	minus_set(oblivious_obs_agents, fully_obs_agents);
	minus_set(oblivious_obs_agents, partially_obs_agents);

	if (!oblivious_obs_agents.empty()) {
		ret.set_max_depth(get_max_depth() + 1);
	}

	transition_map calculated; // A map that links the pworlds of *this* to the corresponding ones of ret
	maintain_oblivious_believed_pworlds(ret, oblivious_obs_agents);

	fluent_formula effects = get_effects_if_entailed(act.get_effects(), get_pointed());

	pworld_ptr new_pointed = execute_sensing_announcement_helper(effects, ret, get_pointed(), calculated, partially_obs_agents, oblivious_obs_agents, entails(effects));
	ret.set_pointed(new_pointed); // Updating the pointed world

	if (!check_properties(fully_obs_agents, partially_obs_agents, effects, ret)) {
		std::cerr << "\nDEBUG: Some properties are not respected\n\n";
		exit(1);
	}

	return ret;
}

bool pstate::check_properties(const agent_set & fully, const agent_set & partially, const fluent_formula & effects, const pstate & updated) const
{

	if (fully.size() > 0) {
		/************Formulae Building************/
		/******Formulae containing the effects of the sensing******/
		belief_formula effects_formula;
		effects_formula.set_formula_type(FLUENT_FORMULA);
		effects_formula.set_fluent_formula(effects);
		effects_formula.set_is_grounded(true);
		effects_formula.deground();


		/******First Property C(Fully, eff)******/
		belief_formula property1;
		property1.set_group_agents(fully);
		property1.set_formula_type(C_FORMULA);
		property1.set_bf1(effects_formula);
		property1.set_is_grounded(true);
		property1.deground();



		if (!updated.entails(property1)) {
			std::cerr << "\nDEBUG: First property not respected";
			return false;
		}



		if (partially.size() > 0) {
			/******Second Property C(P, (C(Fully,eff) | C(Fully, -eff)))******/
			belief_formula inner_nested2, nested2, disjunction, property2;
			//First nested formula is equal to First Property C(Fully, eff)
			//Second nested formula C(Fully, -f) is comprised of two layer (the NOT and C)
			inner_nested2.set_group_agents(fully);
			inner_nested2.set_formula_type(C_FORMULA);
			inner_nested2.set_bf1(effects_formula);
			inner_nested2.set_is_grounded(true);
			inner_nested2.deground();


			nested2.set_formula_type(PROPOSITIONAL_FORMULA);
			nested2.set_operator(BF_NOT);
			nested2.set_bf1(inner_nested2);
			nested2.set_is_grounded(true);
			nested2.deground();


			//The disjunction (C(Fully,eff) | C(Fully, -eff)) is made with property1 and nested2
			disjunction.set_formula_type(PROPOSITIONAL_FORMULA);
			disjunction.set_operator(BF_OR);
			disjunction.set_bf1(property1);
			disjunction.set_bf2(nested2);
			disjunction.set_is_grounded(true);
			disjunction.deground();


			//Finally we can construct the second property
			property2.set_group_agents(partially);
			property2.set_formula_type(C_FORMULA);
			property2.set_bf1(disjunction);
			property2.set_is_grounded(true);
			property2.deground();


			

			/******Third Property C(F, C(P, (C(Fully,eff) | C(Fully, -eff))))******/
			//This formula is just C(Fully, property2)
			belief_formula property3;
			property3.set_group_agents(fully);
			property3.set_formula_type(C_FORMULA);
			property3.set_bf1(property2);
			
			property3.set_is_grounded(true);
			
			property3.deground();


			if (!updated.entails(property2)) {
				std::cerr << "\nDEBUG: Second property not respected in the formula: ";
				property2.print_deground();
				return false;
			}

			if (!updated.entails(property3)) {
				std::cerr << "\nDEBUG: Third property not respected in the formula: ";
				property3.print_deground();
				return false;
			}
		}
	}

	return true;
}

pstate pstate::execute_announcement(const action & act) const
{
	//	pstate ret;
	//
	//	//This finds all the worlds that are reachable from the initial state following
	//	//the edges labeled with fully observant agents.
	//	agent_set agents = domain::get_instance().get_agents();
	//	agent_set fully_obs_agents = get_agents_if_entailed(act.get_fully_observants(), get_pointed());
	//	agent_set partially_obs_agents = get_agents_if_entailed(act.get_partially_observants(), get_pointed());
	//
	//	agent_set oblivious_obs_agents = agents;
	//	minus_set(oblivious_obs_agents, fully_obs_agents);
	//	minus_set(oblivious_obs_agents, partially_obs_agents);
	//
	//	if (!oblivious_obs_agents.empty()) {
	//		ret.set_max_depth(get_max_depth() + 1);
	//	}
	//
	//	transition_map calculated; // A map that links the pworlds of *this* to the corresponding ones of ret
	//	maintain_oblivious_believed_pworlds(ret, oblivious_obs_agents);
	//
	//	fluent_formula effects = get_effects_if_entailed(act.get_effects(), get_pointed());
	//
	//	pworld_ptr new_pointed = execute_sensing_announcement_helper(act, ret, get_pointed(), calculated, partially_obs_agents, oblivious_obs_agents, entails(effects));
	//	ret.set_pointed(new_pointed); // Updating the pointed world
	//
	//	return ret;

	return execute_sensing(act);
}

void pstate::calc_min_bisimilar()
{
	std::cerr << "\nMin bisimilar is not supported for possibilities yet.\n";
	exit(1);
}

void pstate::print() const
{
	int counter = 1;
	std::cout << std::endl;
	std::cout << "The Pointed World has id ";
	printer::get_instance().print_list(get_pointed().get_fluent_set());
	std::cout << std::endl;
	std::cout << "*******************************************************************" << std::endl;

	pworld_ptr_set::const_iterator it_pwset;
	std::cout << "World List:" << std::endl;

	for (it_pwset = get_worlds().begin(); it_pwset != get_worlds().end(); it_pwset++) {
		std::cout << "W-" << counter << ": ";
		printer::get_instance().print_list(it_pwset->get_fluent_set());
		std::cout << " rep:" << it_pwset->get_repetition();
		std::cout << std::endl;
		counter++;
	}
	counter = 1;
	std::cout << std::endl;
	std::cout << "*******************************************************************" << std::endl;
	pworld_transitive_map::const_iterator it_pwtm;
	pworld_map::const_iterator it_pwm;
	std::cout << "Edge List:" << std::endl;
	for (it_pwtm = get_beliefs().begin(); it_pwtm != get_beliefs().end(); it_pwtm++) {
		pworld_ptr from = it_pwtm->first;
		pworld_map from_map = it_pwtm->second;

		for (it_pwm = from_map.begin(); it_pwm != from_map.end(); it_pwm++) {
			agent ag = it_pwm->first;
			pworld_ptr_set to_set = it_pwm->second;

			for (it_pwset = to_set.begin(); it_pwset != to_set.end(); it_pwset++) {

				pworld_ptr to = *it_pwset;

				std::cout << "E-" << counter << ": (";
				printer::get_instance().print_list(from.get_fluent_set());
				std::cout << "," << from.get_repetition();
				std::cout << ") - (";
				printer::get_instance().print_list(to.get_fluent_set());
				std::cout << "," << to.get_repetition();
				std::cout << ") ag:" << domain::get_instance().get_grounder().deground_agent(ag);
				std::cout << std::endl;
				counter++;
			}
		}
	}
	std::cout << "*******************************************************************" << std::endl;
}

void pstate::print_graphviz(std::ostream & graphviz) const
{
	string_set::const_iterator it_st_set;
	fluent_set::const_iterator it_fs;


	graphviz << "//WORLDS List:" << std::endl;
	std::map<fluent_set, int> map_world_to_index;
	std::map<unsigned short, char> map_rep_to_name;
	char found_rep = (char) ((char) domain::get_instance().get_agents().size() + 'A');
	int found_fs = 0;
	fluent_set tmp_fs;
	char tmp_unsh;
	string_set tmp_stset;
	bool print_first;
	pworld_ptr_set::const_iterator it_pwset;
	for (it_pwset = get_worlds().begin(); it_pwset != get_worlds().end(); it_pwset++) {
		if (*it_pwset == get_pointed())
			graphviz << "	node [shape = doublecircle] ";
		else
			graphviz << "	node [shape = circle] ";

		print_first = false;
		tmp_fs = it_pwset->get_fluent_set();
		if (map_world_to_index.count(tmp_fs) == 0) {
			map_world_to_index[tmp_fs] = found_fs;
			found_fs++;
		}
		tmp_unsh = it_pwset->get_repetition();
		if (map_rep_to_name.count(tmp_unsh) == 0) {
			map_rep_to_name[tmp_unsh] = found_rep;
			found_rep++;
		}
		graphviz << "\"" << map_rep_to_name[tmp_unsh] << "_" << map_world_to_index[tmp_fs] << "\";";
		graphviz << "// (";
		tmp_stset = domain::get_instance().get_grounder().deground_fluent(tmp_fs);
		for (it_st_set = tmp_stset.begin(); it_st_set != tmp_stset.end(); it_st_set++) {
			if (print_first) {
				graphviz << ",";
			}
			print_first = true;
			graphviz << *it_st_set;
		}
		graphviz << ")\n";
	}

	graphviz << "\n\n";
	graphviz << "//RANKS List:" << std::endl;

	std::map<int, pworld_ptr_set> for_rank_print;
	for (it_pwset = get_worlds().begin(); it_pwset != get_worlds().end(); it_pwset++) {
		for_rank_print[it_pwset->get_repetition()].insert(*it_pwset);
	}

	std::map<int, pworld_ptr_set>::const_iterator it_map_rank;
	for (it_map_rank = for_rank_print.begin(); it_map_rank != for_rank_print.end(); it_map_rank++) {
		graphviz << "	{rank = same; ";
		for (it_pwset = it_map_rank->second.begin(); it_pwset != it_map_rank->second.end(); it_pwset++) {
			graphviz << "\"" << map_rep_to_name[it_pwset->get_repetition()] << "_" << map_world_to_index[it_pwset->get_fluent_set()] << "\"; ";
		}
		graphviz << "}\n";
	}


	graphviz << "\n\n";
	graphviz << "//EDGES List:" << std::endl;

	std::map < std::tuple<std::string, std::string>, std::set<std::string> > edges;

	pworld_transitive_map::const_iterator it_pwtm;
	pworld_map::const_iterator it_pwm;
	std::tuple<std::string, std::string> tmp_tuple;
	std::string tmp_string = "";

	for (it_pwtm = get_beliefs().begin(); it_pwtm != get_beliefs().end(); it_pwtm++) {
		pworld_ptr from = it_pwtm->first;
		pworld_map from_map = it_pwtm->second;

		for (it_pwm = from_map.begin(); it_pwm != from_map.end(); it_pwm++) {
			agent ag = it_pwm->first;
			pworld_ptr_set to_set = it_pwm->second;

			for (it_pwset = to_set.begin(); it_pwset != to_set.end(); it_pwset++) {
				pworld_ptr to = *it_pwset;

				tmp_string = "_" + std::to_string(map_world_to_index[from.get_fluent_set()]);
				tmp_string.insert(0, 1, map_rep_to_name[from.get_repetition()]);
				std::get<0>(tmp_tuple) = tmp_string;

				tmp_string = "_" + std::to_string(map_world_to_index[to.get_fluent_set()]);
				tmp_string.insert(0, 1, map_rep_to_name[to.get_repetition()]);
				std::get<1>(tmp_tuple) = tmp_string;

				edges[tmp_tuple].insert(domain::get_instance().get_grounder().deground_agent(ag));

			}
		}
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
							//std::cerr << std::get<0>(it_map->first) << " " << std::get<0>(it_map_2->first) << "\n";
							it_map_2 = edges.erase(it_map_2);
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

	std::string color = "<font color=\"#ffffff\">";
	graphviz << "\n\n//WORLDS description Table:" << std::endl;
	graphviz << "	node [shape = plain]\n\n";
	graphviz << "	description[label=<\n";
	graphviz << "	<table border = \"0\" cellborder = \"1\" cellspacing = \"0\" >\n";
	for (it_pwset = get_worlds().begin(); it_pwset != get_worlds().end(); it_pwset++) {
		tmp_fs = it_pwset->get_fluent_set();
		print_first = false;
		graphviz << "		<tr><td>" << map_rep_to_name[it_pwset->get_repetition()] << "_" << map_world_to_index[tmp_fs] << "</td> <td>";
		for (it_fs = tmp_fs.begin(); it_fs != tmp_fs.end(); it_fs++) {
			if (print_first) {

				graphviz << ", ";
			}
			print_first = true;
			if ((*it_fs) % 2 == 0) color = "<font color=\"#0000ff\"> ";
			else color = "<font color=\"#ff1020\">";
			graphviz << color << domain::get_instance().get_grounder().deground_fluent(*it_fs) << "</font>";
		}
		graphviz << "</td></tr>\n";
	}
	graphviz << "	</table>>]\n";
	graphviz << "	{rank = max; description};\n";

}

/******************************MOVE TO HELPER*********************************/

template <class T>
void pstate::sum_set(std::set<T> & to_modify, const std::set<T> & factor2) const
{
	/**\todo move to helper*/
	typename std::set<T>::const_iterator it_kwset;
	for (it_kwset = factor2.begin(); it_kwset != factor2.end(); it_kwset++) {

		to_modify.insert(*it_kwset);
	}
}

template <class T>
void pstate::minus_set(std::set<T> & to_modify, const std::set<T> & factor2) const
{ /**\todo move to helper*/

	typename std::set<T>::const_iterator it_kwset;
	for (it_kwset = factor2.begin(); it_kwset != factor2.end(); it_kwset++) {

		to_modify.erase(*it_kwset);
	}
}

agent_set pstate::get_agents_if_entailed(const observability_map& map, const pworld_ptr & start) const
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

fluent_formula pstate::get_effects_if_entailed(const effects_map & map, const pworld_ptr & start) const
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

/***************DOXASTIC REASONING***************/

pworld_ptr pstate::execute_announcement_helper_dox(const fluent_formula &effects, pstate &ret, const pworld_ptr &current_pw, transition_map &calculated, agent_set &partially_obs_agents, agent_set &oblivious_obs_agents, bool reached_by_fully, bool & implications) const
{
	pworld_ptr new_pw = ret.add_rep_world(pworld(current_pw.get_fluent_set()), current_pw.get_repetition()); // We add the corresponding pworld in ret
	calculated.insert(transition_map::value_type(current_pw, new_pw)); // And we update the calculated map

	auto it_pwtm = get_beliefs().find(current_pw);

	if (it_pwtm != get_beliefs().end()) {
		pworld_map::const_iterator it_pwm;
		pworld_ptr_set::const_iterator it_pws;

		for (it_pwm = it_pwtm->second.begin(); it_pwm != it_pwtm->second.end(); it_pwm++) {
			agent ag = it_pwm->first;

			bool is_oblivious_obs = oblivious_obs_agents.find(ag) != oblivious_obs_agents.end();
			bool is_partially_obs = partially_obs_agents.find(ag) != partially_obs_agents.end();
			bool is_fully_obs = !is_oblivious_obs && !is_partially_obs;

			for (it_pws = it_pwm->second.begin(); it_pws != it_pwm->second.end(); it_pws++) {

				if (is_partially_obs) {
					pworld_ptr_set tmp_set = get_B_reachable_worlds(ag, *it_pws);
					pworld_ptr_set::const_iterator it_tmp;
					bool tmp_ent1 = false;
					bool tmp_ent2 = false;

					for (it_tmp = tmp_set.begin(); it_tmp != tmp_set.end();) {
						if (entails(effects, *it_tmp)) {
							tmp_ent1 = true;
						} else {
							tmp_ent2 = true;
						}

						if (tmp_ent1 && tmp_ent2) {
							is_fully_obs = true;
							implications = true;
							is_partially_obs = false;
							it_tmp = tmp_set.end();
						} else {
							it_tmp++;
						}
					}
				}

				if (is_oblivious_obs) { // If we are dealing with an OBLIVIOUS agent we maintain its beliefs as they were
					auto maintained_pworld = ret.get_worlds().find(*it_pws);

					if (maintained_pworld != ret.get_worlds().end()) {
						ret.add_edge(new_pw, *it_pws, ag);
					}
				} else { // Otherwise, if we have a FULLY/PARTIALLY observant agent
					auto calculated_pworld = calculated.find(*it_pws);
					//fluent_formula effects = get_effects_if_entailed(act.get_effects(), get_pointed());
					//bool ent = act.get_type() == SENSING ? entails(effects, *it_pws) == entails(effects, get_pointed()) : entails(effects, *it_pws);


					bool is_consistent_belief = ((!reached_by_fully && is_partially_obs) ||
						(reached_by_fully && is_partially_obs && (entails(effects, *it_pws))) ||
						(is_fully_obs && (entails(effects, *it_pws)))); // if a PARTIALLY OBS agent believes that "ag" thinks that "calculated" may be true (i.e., when "previous_entailment" holds)

					if (calculated_pworld != calculated.end()) { // If we already calculated the transition function for this pworld
						if (is_consistent_belief) {
							ret.add_edge(new_pw, calculated_pworld->second, ag);
						}
					} else { // If we did not already calculate the transition function
						if (is_consistent_belief) { // We calculate it if it would result in a consistent belief...
							pworld_ptr believed_pw = execute_announcement_helper_dox(effects, ret, *it_pws, calculated, partially_obs_agents, oblivious_obs_agents, (reached_by_fully || is_fully_obs), implications);
							ret.add_edge(new_pw, believed_pw, ag);
						}
					}
				}
			}
		}
	}
	return new_pw;
}

pstate pstate::execute_announcement_dox(const action & act) const
{
	pstate ret;

	//This finds all the worlds that are reachable from the initial state following
	//the edges labeled with fully observant agents.
	agent_set agents = domain::get_instance().get_agents();
	agent_set fully_obs_agents = get_agents_if_entailed(act.get_fully_observants(), get_pointed());
	agent_set partially_obs_agents = get_agents_if_entailed(act.get_partially_observants(), get_pointed());

	agent_set oblivious_obs_agents = agents;
	minus_set(oblivious_obs_agents, fully_obs_agents);
	minus_set(oblivious_obs_agents, partially_obs_agents);

	if (!oblivious_obs_agents.empty()) {
		ret.set_max_depth(get_max_depth() + 1);
	}

	transition_map calculated; // A map that links the pworlds of *this* to the corresponding ones of ret
	maintain_oblivious_believed_pworlds(ret, oblivious_obs_agents);

	fluent_formula effects = get_effects_if_entailed(act.get_effects(), get_pointed());

	bool implications = true;
	if (fully_obs_agents.size() == 0) {
		implications = false;
	}

	pworld_ptr new_pointed = execute_announcement_helper_dox(effects, ret, get_pointed(), calculated, partially_obs_agents, oblivious_obs_agents, false, implications);
	ret.set_pointed(new_pointed); // Updating the pointed world

	if (!implications) {
		ret = *this;
	}

	return ret;
}

/***************END DOXASTIC***************/
//DEBUG

int pstate::get_edges() const
{
	std::cerr << "\nYou are playing with debug only options-1.\n";
	exit(1);
}

void pstate::debug_print(pstate tmp)
{
	std::cerr << "\nYou are playing with debug only options-2.\n";
	exit(1);
}