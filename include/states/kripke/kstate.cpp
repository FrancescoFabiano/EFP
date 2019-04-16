/*
 * \brief Implementation of \ref kstate.h
 *
 * \copyright GNU Public License.
 *
 * \author Francesco Fabiano.
 * \date April 9, 2019
 */

#include "kstate.h"

bool kstate::entails(fluent f) const
{
	return entails(f, m_pointed);
}

bool kstate::entails(fluent f, kworld_ptr world) const
{
	return world->entails(f);
}

bool kstate::entails(const fluent_set & fl) const
{
	return entails(fl, m_pointed);
}

bool kstate::entails(const fluent_set & fl, kworld_ptr world) const
{
	return world->entails(fl);
}

bool kstate::entails(const fluent_formula & ff) const
{
	return entails(ff, m_pointed);
}

bool kstate::entails(const fluent_formula & ff, kworld_ptr world) const
{
	return world->entails(ff);
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
	switch (bf.m_formula_type) {
	case FLUENT_FORMULA:
		/** \todo Make sure its grounded. Maybe add to beleif_formula a bool that store if grounded or not or maybe ground
		 * when \ref domain created.
		 * @see belief_formula::ground(const grounder &)
		 */
		return entails(bf.m_fluent_formula, world);
		break;

	case BELIEF_FORMULA:
		/** \todo Make sure its grounded. Maybe add to beleif_formula a bool that store if grounded or not or maybe ground
		 * when \ref domain created.
		 * @see belief_formula::ground(const grounder &)
		 * 
		 * \todo what was the at_lest_one of the previous version?
		 * 
		 * \todo self-loop?
		 */
		return entails(*(bf.m_bf1), get_B_reachable_worlds(bf.m_agent_op, world));
		break;

	case PROPOSITIONAL_FORMULA:
		switch (bf.m_operator) {
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

const kworld_ptr_set kstate::get_B_reachable_worlds(agent ag, kworld_ptr world)
{
	kworld_ptr_set ret;
	get_B_reachable_worlds(ag, world, ret);
	return ret;
}

bool kstate::get_B_reachable_worlds(agent ag, kworld_ptr world, kworld_ptr_set& ret)
{
	bool is_fixed_point = true;
	kedge_ptr_set::const_iterator it_kedge;
	for (it_kedge = m_edges.begin(); it_kedge != m_edges.end(); it_kedge++) {
		if (((*it_kedge)->get_from()->get_id() == world->get_id()) && ((*it_kedge)->get_label() == ag)) {
			//We use the pair of insert, if we add a new world (true in the set::insert) then is not a fixed point
			if (std::get<1>(ret.insert((*it_kedge)->get_to()))) {
				is_fixed_point = false;
			}
		}
	}

	return is_fixed_point;
}

const kworld_ptr_set kstate::get_E_reachable_worlds(const agent_set & ags, kworld_ptr world)
{
	kworld_ptr_set ret;
	get_E_reachable_worlds(ags, world, ret);
	return ret;
}

bool kstate::get_E_reachable_worlds(const agent_set & ags, kworld_ptr world, kworld_ptr_set& ret)
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

const kworld_ptr_set kstate::get_C_reachable_worlds(const agent_set & ags, kworld_ptr world)
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

void kstate::build_initial(const initially & initials_conditions)
{
	/**
	 * \todo yet to implement.
	*/
	std::cout << "\nBuilding initial Kripke structure...\n";
}

 bool kstate::is_executable(const action & act)const
 {
	/**
	 * \todo yet to implement.
	*/
}