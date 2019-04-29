/*
 * \brief Implementation of \ref kstate.h
 *
 * \copyright GNU Public License.
 *
 * \author Francesco Fabiano.
 * \date April 9, 2019
 */

int count = 0;

#include "kstate.h"

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
	 The entailment of a \ref beleif_formula just call recursively the entailment on all the reachable world with that formula.
	 */
	switch ( bf.m_formula_type ) {
	case FLUENT_FORMULA:
		/** \todo Make sure its grounded. Maybe add to \ref belief_formula a bool that store if grounded or not or maybe ground
		 * when \ref domain created.
		 * @see belief_formula::ground(const grounder &).*/
		return entails(bf.m_fluent_formula, world);
		break;

	case BELIEF_FORMULA:
		/** \todo Make sure its grounded. Maybe add to \ref belief_formula a bool that store if grounded or not or maybe ground
		 * when \ref domain created.
		 * @see belief_formula::ground(const grounder &).
		 *
		 * \todo what was the at_lest_one of the previous version?
		 *
		 * \todo self-loop?*/
		return entails(*(bf.m_bf1), get_B_reachable_worlds(bf.m_agent_op, world));
		break;

	case PROPOSITIONAL_FORMULA:
		switch ( bf.m_operator ) {
		case BF_NOT:
			return !entails(*(bf.m_bf1), world);
			break;
		case BF_OR:
			return entails(*(bf.m_bf1), world) || entails(*(bf.m_bf2), world);
			break;
		case BF_AND:
			return entails(*(bf.m_bf1), world) && entails(*(bf.m_bf2), world);
			break;
		case BF_NONE:
			return entails(*(bf.m_bf1), world);
			break;
		default:
			std::cerr << "Something went wrong in checking entailment for Propositional formula";
			exit(1);
		}
		break;

	case E_FORMULA:
		//Check the entails on the E-reachable worlds
		return entails(*(bf.m_bf1), get_E_reachable_worlds(bf.m_group_agents, world));
		break;

		//Check the entails on the C-reachable worlds
	case C_FORMULA:
		return entails(*(bf.m_bf1), get_C_reachable_worlds(bf.m_group_agents, world));
		break;

	case EMPTY:
		return true;
		break;

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
	bool is_fixed_point = true;
	kedge_ptr_set::const_iterator it_kedge;
	for (it_kedge = m_edges.begin(); it_kedge != m_edges.end(); it_kedge++) {
		if (((*it_kedge).get_ptr()->get_from().get_ptr()->get_id() == world.get_ptr()->get_id()) && ((*it_kedge).get_ptr()->get_label() == ag)) {
			//We use the pair of insert, if we add a new world (true in the set::insert) then is not a fixed point
			if (std::get<1>(ret.insert((*it_kedge).get_ptr()->get_to()))) {
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

void kstate::add_edge(const kedge & edge)
{
	m_edges.insert(kstore::get_instance().add_edge(edge));
}

void kstate::build_initial(const initially & initials_conditions, int fluent_number, int agent_number)
{
	/**\todo yet to implement.
	 * \todo for now prune building.
	 */
	std::cout << "\nBuilding initial Kripke structure...\n";
	build_initial_prune(initials_conditions, fluent_number, agent_number);

}

void kstate::build_initial_structural(const initially & ini_conditions)
{

}

void kstate::build_initial_prune(const initially & ini_conditions, int fluent_number, int agent_number)
{
	/*Building of all the possible consistent \ref kworld.*/
	//Creation of all the \ref fluent combinations. All the consistent ones are added to \ref kstore.
	fluent_set permutation;
	generate_initial_kworlds(fluent_number, permutation, 0, ini_conditions);

	/*Building of all the consistent \ref kedge.*/
	generate_initial_kedges(ini_conditions, agent_number);


	/* Setting the pointed Kripke Structure
	 * \todo if not all the fluent have been specified then return an error
	 * (should be taken care of in \ref domain).
	 */
}

/*From https://www.geeksforgeeks.org/generate-all-the-binary-strings-of-n-bits/ since is like generating all the binary number.*/
void kstate::generate_initial_kworlds(int fluent_number, fluent_set& permutation, int index, const initially & ini_conditions)
{
	if (index / 2 == fluent_number) {
		kworld to_add(permutation);
		add_initial_kworld(to_add, ini_conditions);
		/*} catch (const std::invalid_argument& ia) {
			std::cout << "\nNot consistent world";
		}*/
		return;
	}
	fluent_set permutation_2 = permutation;
	//Add the \ref fluent in positive version
	permutation.insert(index);
	generate_initial_kworlds(fluent_number, permutation, index + 2, ini_conditions);

	//Add the \ref fluent in negative version
	permutation_2.insert(index + 1);
	generate_initial_kworlds(fluent_number, permutation_2, index + 2, ini_conditions);


}

void kstate::add_initial_kworld(const kworld & possible_add, const initially & ini_conditions)
{
	switch ( ini_conditions.get_ini_restriction() ) {
	case S5:
	{
		/* Since the common knowledge is on all the agent it means that every possible \ref kworld
		 * in the initial state must entail *phi* where C(*phi*) is an initial condition.*/

		//Already setted in \ref domain::build_initially(bool).
		if (possible_add.entails(ini_conditions.get_ff_forS5())) {
			add_world(possible_add);
		} else {
			//Already generated so we save it on kstore
			kstore::get_instance().add_world_no_ret(possible_add);
		}
		break;
	}
	case K45:
	{
		break;
	}
	case NONE:
	{
		break;
	}
	default:
	{
		break;
	}
	}
}

void kstate::generate_initial_kedges(const initially & ini_conditions, int agent_number)
{
	kworld_ptr_set::const_iterator it_kwps_1, it_kwps_2;

	kworld_ptr kwptr_tmp1, kwptr_tmp2;

	/*This for add to *this* all the possible edges.*/
	for (it_kwps_1 = m_worlds.begin(); it_kwps_1 != m_worlds.end(); it_kwps_1++) {
		for (it_kwps_2 = it_kwps_1; it_kwps_2 != m_worlds.end(); it_kwps_2++) {
			for (int i = 0; i < agent_number; i++) {
				/** \todo maybe don't loop twice on the world but exploit using it_kwps_2 = it_kwps_1:
				 * - add (_1, _2).
				 * - add (_2, _1).*/
				kwptr_tmp1 = *it_kwps_1;
				kwptr_tmp2 = *it_kwps_2;

				add_edge(kedge(kwptr_tmp1, kwptr_tmp2, i));
				add_edge(kedge(kwptr_tmp2, kwptr_tmp1, i));
				/** \bug when the line is added problem with the pointers:
				 * add_edge(kedge(*it_kwps_2, *it_kwps_1, i));*/

			}
		}
	}
	formula_list::const_iterator it_fl;
	for (it_fl = ini_conditions.get_initial_conditions().begin(); it_fl != ini_conditions.get_initial_conditions().end(); it_fl++) {
		remove_initial_kedge_bf(*it_fl, ini_conditions.get_ini_restriction());
	}
}

void kstate::remove_kedge(const kedge & to_remove)
{
	/** \bug when the line is added problem with the pointers:*/
	if (m_edges.erase(kstore::get_instance().add_edge(to_remove)) > 0)
		count++;
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

void kstate::remove_initial_kedge_bf(const belief_formula & to_check, domain_restriction ini_restriction)
{
	switch ( ini_restriction ) {
	case S5:
	{
		/* Just check whenever is B(--) \/ B(--) and remove that edge*/
		if (to_check.m_formula_type == C_FORMULA) {

			belief_formula tmp = *to_check.m_bf1;

			switch ( tmp.m_formula_type ) {

				//Only one for edges -- expresses that someone is ignorant.
			case PROPOSITIONAL_FORMULA:
			{
				//We remove all the check on the formula since they have already been controlled when ini_conditions has been created
				if (tmp.m_operator == BF_OR) {

					//fluent_formula known_ff;
					auto known_ff_ptr = std::make_shared<fluent_formula>();
					formula_manipulation::check_Bff_Bnotff(*tmp.m_bf1, *tmp.m_bf2, known_ff_ptr);
					if (known_ff_ptr != nullptr) {
						//printer::print_list(*known_ff_ptr);
						remove_initial_kedge(*known_ff_ptr, tmp.m_bf2->m_agent_op);
					}
					return;

				} else if (tmp.m_operator == BF_AND) {
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
		break;
	}
	case NONE:
	{
		break;
	}
	default:
	{
		break;
	}
	}
}

bool kstate::is_executable(const action & act)const
{
	/** \todo yet to implement.*/
	return false;
}