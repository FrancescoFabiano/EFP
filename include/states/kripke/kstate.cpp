/*
 * \brief Implementation of \ref kstate.h
 *
 * \copyright GNU Public License.
 *
 * \author Francesco Fabiano.
 * \date April 9, 2019
 */

//int count = 0;

#include "kstate.h"
#include "../../domain/domain.h"

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
}

bool kstate::entails(const belief_formula & bf, const kworld_ptr_set & reachable) const
{
	kworld_ptr_set::const_iterator it_kwl;
	for (it_kwl = reachable.begin(); it_kwl != reachable.end(); it_kwl++) {
		if (entails(bf, (*it_kwl)))
			return true;
	}
	return false;
}

bool kstate::entails(const belief_formula & bf, kworld_ptr world) const
{
	/*
	 The entailment of a \ref belief_formula just call recursively the entailment on all the reachable world with that formula.
	 */
	switch ( bf.get_formula_type() ) {
	case FLUENT_FORMULA:
		/** \todo Make sure its grounded. Maybe add to \ref belief_formula a bool that store if grounded or not or maybe ground
		 * when \ref domain created.
		 * @see belief_formula::ground(const grounder &).*/
		return entails(bf.get_fluent_formula(), world);
		break;

	case BELIEF_FORMULA:
		/** \todo Make sure its grounded. Maybe add to \ref belief_formula a bool that store if grounded or not or maybe ground
		 * when \ref domain created.
		 * @see belief_formula::ground(const grounder &).
		 *
		 * \todo what was the at_lest_one of the previous version?
		 *
		 * \todo self-loop?*/
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
			return entails(bf.get_bf1(), world);
			break;
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

	case BF_TYPE_FAIL:
	default:
		std::cerr << "Something went wrong in checking entailment for Belief formula";
		exit(1);
	}
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

void kstate::add_rep_world(const kworld & world)
{
	kworld_ptr tmp = kstore::get_instance().add_world(world);
	tmp.set_repetition(get_max_depth());
	m_worlds.insert(tmp);
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
					formula_manipulation::check_Bff_Bnotff(tmp.get_bf1(), tmp.get_bf2(), known_ff_ptr);
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
const kstate& kstate::compute_succ(const action & act) const
{


	switch ( act.get_type() ) {
	case ONTIC:
	{
		break;
	}
	case SENSING:
	{
		break;
	}
	case ANNOUNCEMENT:
	{
		break;
	}
	default:
	{
		std::cerr << "Error in executing an action: ";
		std::cerr << " the type of the action is not defined correctly";
		std::cerr << std::endl;
		exit(1);
		break;
	}
	}
}

agent_set kstate::get_agents_if_entailed(const observability_map& map) const
{
	agent_set ret;
	observability_map::const_iterator it_map;
	for (it_map = map.begin(); it_map != map.end(); it_map++) {
		if (entails(it_map->second)) {
			ret.insert(it_map->first);
		}
	}
	return ret;
}

fluent_formula kstate::get_effects_if_entailed(const effects_map & map) const
{
	fluent_formula ret;
	effects_map::const_iterator it_map;
	for (it_map = map.begin(); it_map != map.end(); it_map++) {
		if (entails(it_map->second)) {
			ret = formula_manipulation::and_ff(ret, it_map->first);
		}
	}
	if (ret.size() > 1) {
		std::cerr << "\nNon determinism in action effect is not supported.\n";
		exit(1);
	}
	return ret;

}

void kstate::sum_kworld_ptr_set(kworld_ptr_set & to_modify, const kworld_ptr_set & factor2) const
{
	kworld_ptr_set::const_iterator it_kwset;
	for (it_kwset = factor2.begin(); it_kwset != factor2.end(); it_kwset++) {
		to_modify.insert(*it_kwset);
	}
}

void kstate::minus_kworld_ptr_set(kworld_ptr_set & to_modify, const kworld_ptr_set & factor2) const
{
	kworld_ptr_set::const_iterator it_kwset;
	for (it_kwset = factor2.begin(); it_kwset != factor2.end(); it_kwset++) {
		to_modify.erase(*it_kwset);
	}
}
//add action

void kstate::add_ret_ontic_worlds(agent ag, kworld_ptr start, kworld_ptr_set &reached, const agent_set & fully_obs_agents, kstate& ret, const action & act) const
{

	/**\bug fully_obs = get_value_if_entailed(act.get_fully())---Is the observability actually related to the state?*/
	kworld_ptr_set reachable_by_ag = get_B_reachable_worlds(ag, start);

	/** - Generate all the reachable world from \p ag with \p start as pointed world.*/
	minus_kworld_ptr_set(reachable_by_ag, reached);
	if (reachable_by_ag.size() != 0) {
		unsigned int tmp_depth = ret.get_max_depth() + 1;
		ret.set_max_depth(tmp_depth);

		kworld_ptr_set::const_iterator it_kwset;
		kworld_ptr tmp_kworld_ptr;

		fluent_formula::const_iterator it_eff;
		//Only the real effects of act
		//Maybe change the pointed state here?
		fluent_formula effects = get_effects_if_entailed(act.get_effects());

		for (it_kwset = reachable_by_ag.begin(); it_kwset != reachable_by_ag.end(); it_kwset++) {
			tmp_kworld_ptr = *it_kwset;
			reached.insert(tmp_kworld_ptr);
			//Check if need to make a copy inside this function, no in theory because is copied inside a reached
			//if (entails all the right preconditions){

			/**\bug - this.set_pointed(tmp_kworld)
			 * - if(this.entails(acts.get_executability()))
			 * - effects_map::const_iterator it_eff;
			 * - effects = get_value_if_entailed(act.get_effects())
			 * - for (it_eff = effects.begin(); it_eff != effects.end(); it_eff++) {
			 * -
			 * -this.set_pointed(back);*/
			/** - if the exe_conditions are meet then add the state from the previous passage modified
			 * - use \p depth to distinguish the various worlds generated from oblivious and partial observers*/

			for (it_eff = effects.begin(); it_eff != effects.end(); it_eff++) {
				ret.add_rep_world(kworld(formula_manipulation::ontic_exec(*it_eff, tmp_kworld_ptr.get_fluent_set())));

			}
		}


		agent_set::const_iterator it_agset;
		agent tmp_ag;
		kworld_ptr tmp_start;
		kworld_ptr_set reachable_by_tmp;

		/** - get all the worlds reachable by the other Fully observers.
		 *  - use \p depth to distinguish the various worlds generated from oblivious and partial observers.*/
		for (it_agset = fully_obs_agents.begin(); it_agset != fully_obs_agents.end(); it_agset++) {
			tmp_ag = *it_agset;
			for (it_kwset = reachable_by_ag.begin(); it_kwset != reachable_by_ag.end(); it_kwset++) {
				sum_kworld_ptr_set(reachable_by_tmp, get_B_reachable_worlds(tmp_ag, *it_kwset));
			}
			/** - repeat the function for nested \ref kworld (respect the Kripke configuration).*/
			minus_kworld_ptr_set(reachable_by_tmp, reached);
			while (!reachable_by_tmp.empty()) {
				tmp_start = *(reachable_by_tmp.begin());
				add_ret_ontic_worlds(tmp_ag, tmp_start, reached, fully_obs_agents, ret, act);
				reachable_by_tmp.erase(tmp_start);
				minus_kworld_ptr_set(reachable_by_tmp, reached);
			}
		}
	}

}

const kstate & kstate::execute_ontic(const action & act) const
{

	kstate ret;

	/*function that given a map return what is entailed
	 * the use get_E_rechable and apply the ontic_exec on the fully observant reachable world
	 * then add the pointed world (in case is empty) and copy the old structure if there is at least on
	 * oblivious*/

	//This finds all the worlds that are reachbale from the initial state following
	//the edges labeled with fully observant agents.
	agent_set fully_obs_agents = get_agents_if_entailed(act.get_fully_observants());
	agent_set oblivious_obs_agents;

	agent_set::const_iterator it_ags;
	for (it_ags = domain::get_instance().get_agents().begin(); it_ags != domain::get_instance().get_agents().end(); it_ags++) {
		if (fully_obs_agents.find(*it_ags) != fully_obs_agents.end()) {
			oblivious_obs_agents.insert(*it_ags);
		}
	}

	if (oblivious_obs_agents.size() > 0) {
		ret = (*this);
	}

	//pas tmp for entailment
	kworld_ptr_set reached;
	for (it_ags = fully_obs_agents.begin(); it_ags != fully_obs_agents.end(); it_ags++) {
		//Maybe pointed useless as parameter
		add_ret_ontic_worlds(*it_ags, get_pointed(), reached, fully_obs_agents, ret, act);
	}

	kedge_ptr_set::const_iterator it_kedptr;
	kworld_ptr from_add, to_add;
	for (it_kedptr = get_edges().begin(); it_kedptr != get_edges().end(); it_kedptr++) {
		from_add = (*it_kedptr).get_from();
		from_add.set_repetition(from_add.get_repetition() + ret.get_max_depth() + 1);
		to_add = (*it_kedptr).get_to();
		to_add.set_repetition(to_add.get_repetition() + ret.get_max_depth() + 1);
		if (ret.get_worlds().find(from_add) != ret.get_worlds().end() && ret.get_worlds().find(to_add) != ret.get_worlds().end()) {
			ret.add_edge(kedge(from_add, to_add, it_kedptr->get_label()));
		}
	}

	/** \todo add oblivious edges*/

	//This finds all the worlds that are reachable from the initial state following
	//the edges labeled with fully observant agents.
	/*kworld_ptr_set fully_obs_reachable_kworlds = get_E_reachable_worlds(fully_obs_agents, m_pointed);

	for (it_eff = effects.begin(); it_eff != effects.end(); it_eff++) {
		kworld_ptr_set::const_iterator it_kwptr;
		//Worlds computation, just apply the effect to the already existing world and then add them.
		for (it_kwptr = fully_obs_reachable_kworlds.begin(); it_kwptr != fully_obs_reachable_kworlds.end(); it_kwptr++) {*/
	//Because we can have duplicate previously
	/** \bug What happen if ontic removes ignorance?
	 * for example:
	 * - act-> i,g then (i,g,h),(-i,g,h),(i,-g,h),(-i,-g,-h) are all equal to (i,g,h) in this case is \ref add_world
	 * - act-> i,g then (-i,g,h) in a "different" k (i,-g,h) then two different states -- ontic shouldn't duplicate worlds
	 * because there are not partial but can inherit the duplicates so
	 *
	 * Apply this agent by agent: get all the worlds reachable by ag1 and then get all the world reachable
	 * from {these} with Fully_Observant minus {these} reapply function?.
	 *
	 * What if the action does nothing?*/
	/*	ret.add_copy_world(kworld(formula_manipulation::ontic_exec(*it_eff, it_kwptr->get_fluent_set())), 0);
	}
}*/
	/** \bug Add a function add_oblivious_world that copies the already existing world increasing their counter
	 * in kstore. It creates a new one only if already exist the same world in *this*. Also put an end into the kworld_id so it keeps the lexicographical order. Create two ids and an extra
	 *field unique-id (integer) that allow to distinguish between different copies of the same world.
	 *Move all this to the ptr and make the get id from kworld only accessible to the pointer (friend) and store repetition inside the pointer.
	 */
	/*int oldpId = k.get_id();

	if (!is_executable(k, a)) {
		return NULL;
	}

	//cout << "begin computing fd, pd, od" << endl;
	AgentSet fd = get_fd(a, k);
	AgentSet pd = get_pd(a, k);
	AgentSet od = get_od(a, k);

	//cout << "begin computing compute Res(a,M,s)[i]" << endl;

	Edges eds = m->get_edgeslist();
	Edges::iterator it1;
	for (it1 = eds.begin(); it1 != eds.end(); it1++) {
		if (fd.find(it1->get_label()) == fd.end()) //label not in Fd(a,M,s)
		{
			continue;
		}
		Kedge temp = *it1;
		Kedge* e = new Kedge(&temp);
		int from = e->get_from();
		int to = e->get_to();
		if (inRes.find(from) != inRes.end() && inRes.find(to) != inRes.end()) {
			resI.push_back(*e);
		}
	}
	res.set_edgeslist(resI);
	res.set_pId(oldpId);

	//cout << "begin computing compute final Kripke" << endl;
	map<int, int>* savedResS = new map<int, int>;
	merge_kripkes(m, &res, savedResS);
	Edges edgek = k.get_edgeslist();
	int start = m->get_edgeslist().size();
	for (it1 = edgek.begin(); it1 != edgek.end(); it1++) {
		int idek = it1->get_label();

		if (od.find(idek) == od.end()) {
			//cout << "id edege " << idek << endl;
			continue;
		}
		Kedge* newEd = new Kedge();

		if (savedResS->find(it1->get_from()) == savedResS->end()) {
			continue;
		}
		int fromnew = savedResS->find(it1->get_from())->second;
		newEd->set_from(fromnew);
		int tonew = it1->get_to();
		newEd->set_to(tonew);
		newEd->set_id(start);
		newEd->set_label(it1->get_label());

		m->add_edge2state(fromnew, start, true);
		m->add_edge2state(tonew, start, false);
		m->add_to_edges_list(*newEd);

		start = start + 1;
	}
	//Literals changed =
	//cout << "begin computing update_kripke" << endl;
	Kripke* m1 = update_kripke(*m);
	return m1;*/
}