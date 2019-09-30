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

void pstate::set_max_depth(unsigned int to_set)
{
	m_max_depth = to_set;
}

const pworld_ptr_set & pstate::get_worlds() const
{
	return m_worlds;
}

const pworld_ptr & pstate::get_pointed() const
{
	return m_pointed;
}

unsigned int pstate::get_max_depth() const
{
	return m_max_depth;
}

bool pstate::entails(fluent f) const
{
	return entails(f, m_pointed);
}

bool pstate::entails(fluent f, pworld_ptr world) const
{
	return world.get_ptr()->entails(f);
}

bool pstate::entails(const fluent_set & fl) const
{
	return entails(fl, m_pointed);
}

bool pstate::entails(const fluent_set & fl, pworld_ptr world) const
{
	return world.get_ptr()->entails(fl);
}

bool pstate::entails(const fluent_formula & ff) const
{
	return entails(ff, m_pointed);
}

bool pstate::entails(const fluent_formula & ff, pworld_ptr world) const
{
	return world.get_ptr()->entails(ff);
}

bool pstate::entails(const belief_formula & bf) const
{
	return entails(bf, m_pointed);
}

bool pstate::operator=(const pstate & to_copy)
{
	set_worlds(to_copy.get_worlds());
	set_max_depth(to_copy.get_max_depth());
	set_pointed(to_copy.get_pointed());
	return true;
}

bool pstate::entails(const belief_formula & to_check, const pworld_ptr_set & reachable) const
{
	pworld_ptr_set::const_iterator it_kwl;
	for (it_kwl = reachable.begin(); it_kwl != reachable.end(); it_kwl++) {
		/**\todo why setted contary?*/
		if (!entails(to_check, (*it_kwl)))
			return false;
	}
	return true;
}

bool pstate::entails(const belief_formula & bf, pworld_ptr world) const
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

bool pstate::entails(const formula_list & to_check, pworld_ptr world) const
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

const pworld_ptr_set pstate::get_B_reachable_worlds(agent ag, pworld_ptr world) const
{
	pworld_ptr_set ret;
	get_B_reachable_worlds(ag, world, ret);

	return ret;
}

bool pstate::get_B_reachable_worlds(agent ag, pworld_ptr world, pworld_ptr_set& ret) const
{
	/** \todo check: If a--i-->b, b--i-->c then a--i-->c must be there*/
	bool is_fixed_point = true;
	pworld_ptr_set::const_iterator it_pworld_1, it_pworld_2;
	for (it_pworld_1 = m_worlds.begin(); it_pworld_1 != m_worlds.end(); it_pworld_1++) {
	    auto pw = it_pworld_1->get_pworld_map().find(ag);
	    
	    if (pw != world.get_pworld_map().end()) {
            // We use the pair of insert, if we add a new world (true in the set::insert) then is not a fixed point
            for (it_pworld_2 = pw->second.begin(); it_pworld_2 != pw->second.end(); it_pworld_2++) {
                if (std::get<1>(ret.insert(*it_pworld_2))) {
                    is_fixed_point = false;
                }
            }
	    }
	}
	return is_fixed_point;
}

const pworld_ptr_set pstate::get_E_reachable_worlds(const agent_set & ags, pworld_ptr world) const
{
	pworld_ptr_set ret;
	get_E_reachable_worlds(ags, world, ret);

	return ret;
}

bool pstate::get_E_reachable_worlds(const agent_set & ags, pworld_ptr world, pworld_ptr_set& ret) const
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

const pworld_ptr_set pstate::get_C_reachable_worlds(const agent_set & ags, pworld_ptr world) const
{
	//Use of fixed point to stop.
	bool is_fixed_point = false;
	pworld_ptr_set ret;
	while (!is_fixed_point) {
		is_fixed_point = get_E_reachable_worlds(ags, world, ret);
	}
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

/*void pstate::add_copy_world(const pworld & world, unsigned short repetition)
{
	pworld_ptr tmp = pstore::get_instance().add_world(world);
	tmp.set_repetition(repetition);

	//If this pworld_ptr exist then add another, we need to add a copy
	if (!(std::get<1>(m_worlds.insert(tmp)))) {
		///arg = 0 in pworld constructor by default but when put to x it set the extra id to x
		add_copy_world(world, repetition + 1);
	}
}*/

void pstate::add_edge(pworld_ptr & pw1, const pworld & pw2, const agent ag) const
{
    auto it_pwm_1 = pw1.get_pworld_map().find(ag);

    if (it_pwm_1 != pw1.get_pworld_map().end()) {
        pworld_ptr_set pwps = it_pwm_1->second;
        pwps.insert(pstore::get_instance().add_world(pw2));
    } else {
        pworld_map pmap = pw1.get_pworld_map();
        pmap.insert(pworld_map::value_type(ag, {pstore::get_instance().add_world(pw2)}));
    }
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
	generate_initial_pworlds(permutation, 0);


	/*Building of all the consistent edges.*/
    generate_initial_pedges();
}

/*From https://www.geeksforgeeks.org/generate-all-the-binary-strings-of-n-bits/ since is like generating all the binary numbers.*/
void pstate::generate_initial_pworlds(fluent_set& permutation, int index)
{
	int fluent_number = domain::get_instance().get_fluent_number();
	if (index / 2 == fluent_number) {
	    pworld_map tmp;     /** \todo: is it ok to use a tmp like this? */
		pworld to_add(permutation, tmp);
		add_initial_pworld(to_add);

		return;
	}
	fluent_set permutation_2 = permutation;
	//Add the \ref fluent in positive version
	permutation.insert(index);
	generate_initial_pworlds(permutation, index + 2);

	//Add the \ref fluent in negative version
	permutation_2.insert(index + 1);
	generate_initial_pworlds(permutation_2, index + 2);
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

				add_edge(pwptr_tmp1, *pwptr_tmp2.get_ptr(), i);
				add_edge(pwptr_tmp2, *pwptr_tmp1.get_ptr(), i);
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

void pstate::remove_edge(pworld_ptr & pw1, const pworld & pw2, const agent ag)
{
    auto it_pwm_1 = pw1.get_pworld_map().find(ag);

    if (it_pwm_1 != pw1.get_pworld_map().end()) {
        pworld_ptr_set pwps = it_pwm_1->second;
        pwps.erase(pstore::get_instance().add_world(pw2));
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

void pstate::remove_initial_pedge_bf(const belief_formula &to_check)
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
	switch ( act.get_type() ) {
	case ONTIC:
	{
		/*pstate tmp = execute_ontic(act);
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

void pstate::execute_ontic_effects(const action &act, pworld_ptr &world) const {
    fluent_formula::const_iterator it_eff;

    fluent_formula effects = get_effects_if_entailed(act.get_effects(), world);
    fluent_set world_description = world.get_fluent_set();

    for (it_eff = effects.begin(); it_eff != effects.end(); it_eff++) {
        world_description = helper::ontic_exec(*it_eff, world_description);
    }
}

void pstate::explore_unvisited_pworlds(const action &act, pworld_ptr &world, pworld_map &current_pmap, pworld_queue &to_visit, transition_map &calculated, beliefs_vector &to_backtrack, agent_set &fully_obs_agents, agent_set &oblivious_obs_agents) const {
    pworld_ptr_set::const_iterator it_pws;
    pworld_map::const_iterator it_pwm;

    for (it_pwm = current_pmap.begin(); it_pwm != current_pmap.end(); it_pwm++) {
        agent ag = it_pwm->first;
        if (fully_obs_agents.find(ag) != fully_obs_agents.end()) {
            for (it_pws = it_pwm->second.begin(); it_pws != it_pwm->second.end(); it_pws++) {
                if (calculated.find(*it_pws) != calculated.end()) {     // If we already calculated the transition function for this pworld
                    add_edge(world, *it_pws->get_ptr(), ag);            // Then we update agents' beliefs
                } else {
                    to_visit.push(*it_pws);                             // Otherwise we push the pworld in the to_visit queue
                    to_backtrack.emplace_back(world, *it_pws, ag);      // And we keep track of the "edge" that we must insert later
                }
            }
        } else if (oblivious_obs_agents.find(ag) != oblivious_obs_agents.end()) {   // If we reach a pworld believed to be true by some oblivious agent
            if (calculated.find(*it_pws) == calculated.end()) {                     // Then we do nothing and we update the calculated map
                calculated.insert(transition_map::value_type(*it_pws, *it_pws));
            }
        } else {
            switch (act.get_type()) {
                case ONTIC: {
                    std::cerr << "Error in executing an action: ";
                    std::cerr << "ontic actions do not admit partially observant agents";
                    std::cerr << std::endl;
                    exit(1);
                } case SENSING: {
                    /** \todo: implement the sensing case.*/
                    break;
                } case ANNOUNCEMENT: {
                    /** \todo: implement the announcement case.*/
                    break;
                } default: {
                    std::cerr << "Error in executing an action: ";
                    std::cerr << "the type of the action is not defined correctly";
                    std::cerr << std::endl;
                    exit(1);
                }
            }
        }
    }
}

void pstate::backtrack_remaining_beliefs(transition_map &calculated, beliefs_vector &to_backtrack) const {
    beliefs_vector::const_iterator it_bv;

    for (it_bv = to_backtrack.begin(); it_bv != to_backtrack.end(); it_bv++) {
        auto tmp = calculated.find(std::get<1>(*it_bv));

        if (tmp != calculated.end()) {
            pworld_ptr from = std::get<0>(*it_bv);
            add_edge(from, *tmp->second.get_ptr(), std::get<2>(*it_bv));
        } else {
            std::cerr << "\nSomething went wrong while updating the agents' beliefs\n";
            exit(1);
        }
    }
}

void pstate::execute_action(const action &act, pstate &ret, agent_set &fully_obs_agents, agent_set &oblivious_obs_agents) const {
    pworld_queue to_visit;          // The pworlds yet to visit
    transition_map calculated;      // A map that links the pworlds of *this* to the corresponding ones of ret
    beliefs_vector to_backtrack;    // A vector of tuples that contains the "edges" (u,v) where the pworld v still has to be calculated

    pworld_ptr current_pw;          // The current pworld being considered
    pworld_map current_pmap;        // And its current pworld_map

    to_visit.push(get_pointed());   // We always visit the pointed pworld

    while (!to_visit.empty()) {
        current_pw = to_visit.front();
        current_pmap = current_pw.get_pworld_map();

        pworld_ptr pw_new = ret.add_rep_world(*current_pw.get_ptr(), current_pw.get_repetition() + 1);  // We add the corresponding pworld in ret
        calculated.insert(transition_map::value_type(current_pw, pw_new));                              // And we update the calculated map

        // Execute the all the effects
        switch (act.get_type()) {
            case ONTIC: {
                execute_ontic_effects(act, pw_new);
                break;
            } case SENSING: {
                /** \todo: implement the sensing case.*/
                break;
            } case ANNOUNCEMENT: {
                /** \todo: implement the announcement case.*/
                break;
            } default: {
                std::cerr << "Error in executing an action: ";
                std::cerr << "the type of the action is not defined correctly";
                std::cerr << std::endl;
                exit(1);
            }
        }

        // We visit only those unvisited pworlds that are believed to be true by some fully observant agent
        explore_unvisited_pworlds(act, pw_new, current_pmap, to_visit, calculated, to_backtrack, fully_obs_agents, oblivious_obs_agents);
        to_visit.pop();
    }

    // Updating the remaining agents' beliefs
    backtrack_remaining_beliefs(calculated, to_backtrack);

    // Updating the pointed world
    auto new_pointed = calculated.find(get_pointed());

    if (new_pointed != calculated.end()) {
        ret.set_pointed(new_pointed->second);
    } else {
        std::cerr << "\nInvestigate the case when pointed is not update\n";
        exit(1);
    }
}

pstate pstate::execute_ontic(const action &act) const {
    pstate ret;

    //This finds all the worlds that are reachable from the initial state following
    //the edges labeled with fully observant agents.
    agent_set agents = domain::get_instance().get_agents();
    agent_set fully_obs_agents = get_agents_if_entailed(act.get_fully_observants(), get_pointed());

    agent_set oblivious_obs_agents = agents;
    minus_set(oblivious_obs_agents, fully_obs_agents);

    execute_action(act, ret, fully_obs_agents, oblivious_obs_agents);
    return ret;
}

pstate pstate::execute_sensing(const action &act) const {
    pstate ret;

    //This finds all the worlds that are reachable from the initial state following
    //the edges labeled with fully observant agents.
    agent_set agents = domain::get_instance().get_agents();
    agent_set fully_obs_agents = get_agents_if_entailed(act.get_fully_observants(), get_pointed());
    agent_set partially_obs_agents = get_agents_if_entailed(act.get_partially_observants(), get_pointed());

    agent_set oblivious_obs_agents = agents;
    minus_set(oblivious_obs_agents, fully_obs_agents);
    minus_set(oblivious_obs_agents, partially_obs_agents);

    execute_action(act, ret, fully_obs_agents, oblivious_obs_agents);
    return ret;
}

pstate pstate::execute_announcement(const action &act) const {
    pstate ret;

    //This finds all the worlds that are reachable from the initial state following
    //the edges labeled with fully observant agents.
    agent_set agents = domain::get_instance().get_agents();
    agent_set fully_obs_agents = get_agents_if_entailed(act.get_fully_observants(), get_pointed());
    agent_set partially_obs_agents = get_agents_if_entailed(act.get_partially_observants(), get_pointed());

    agent_set oblivious_obs_agents = agents;
    minus_set(oblivious_obs_agents, fully_obs_agents);
    minus_set(oblivious_obs_agents, partially_obs_agents);

    execute_action(act, ret, fully_obs_agents, oblivious_obs_agents);
    return ret;
}

void pstate::print() const
{
//	int counter = 1;
//	std::cout << std::endl;
//	std::cout << "The Pointed World has id ";
//	printer::get_instance().print_list(get_pointed().get_fluent_set());
//	std::cout << std::endl;
//	std::cout << "*******************************************************************" << std::endl;
//	;
//	pworld_ptr_set::const_iterator it_kwset;
//	std::cout << "World List:" << std::endl;
//
//	for (it_kwset = get_worlds().begin(); it_kwset != get_worlds().end(); it_kwset++) {
//		std::cout << "W-" << counter << ": ";
//		printer::get_instance().print_list(it_kwset->get_fluent_set());
//		std::cout << " rep:" << it_kwset->get_repetition();
//		std::cout << std::endl;
//		counter++;
//	}
//	counter = 1;
//	std::cout << std::endl;
//	std::cout << "*******************************************************************" << std::endl;
//	kedge_ptr_set::const_iterator it_keset;
//	std::cout << "Edge List:" << std::endl;
//	for (it_keset = get_edges().begin(); it_keset != get_edges().end(); it_keset++) {
//
//		std::cout << "E-" << counter << ": (";
//		printer::get_instance().print_list(it_keset->get_from().get_fluent_set());
//		std::cout << "," << it_keset->get_from().get_repetition();
//		std::cout << ") - (";
//		printer::get_instance().print_list(it_keset->get_to().get_fluent_set());
//		std::cout << "," << it_keset->get_to().get_repetition();
//		std::cout << ") ag:" << domain::get_instance().get_grounder().deground_agent(it_keset->get_label());
//		std::cout << std::endl;
//		counter++;
//
//	}
//	std::cout << "*******************************************************************" << std::endl;
}

void pstate::print_graphviz(std::ostream & graphviz) const
{
//	string_set::const_iterator it_st_set;
//	fluent_set::const_iterator it_fs;
//
//
//	graphviz << "//WORLDS List:" << std::endl;
//	std::map<fluent_set, int> map_world_to_index;
//	std::map<unsigned short, char> map_rep_to_name;
//	char found_rep = (char) ((char) domain::get_instance().get_agents().size() + 'A');
//	int found_fs = 0;
//	fluent_set tmp_fs;
//	char tmp_unsh;
//	string_set tmp_stset;
//	bool print_first;
//	pworld_ptr_set::const_iterator it_kwset;
//	for (it_kwset = get_worlds().begin(); it_kwset != get_worlds().end(); it_kwset++) {
//		if (*it_kwset == get_pointed())
//			graphviz << "	node [shape = doublecircle] ";
//		else
//			graphviz << "	node [shape = circle] ";
//
//		print_first = false;
//		tmp_fs = it_kwset->get_fluent_set();
//		if (map_world_to_index.count(tmp_fs) == 0) {
//			map_world_to_index[tmp_fs] = found_fs;
//			found_fs++;
//		}
//		tmp_unsh = it_kwset->get_repetition();
//		if (map_rep_to_name.count(tmp_unsh) == 0) {
//			map_rep_to_name[tmp_unsh] = found_rep;
//			found_rep++;
//		}
//		graphviz << "\"" << map_rep_to_name[tmp_unsh] << "_" << map_world_to_index[tmp_fs] << "\";";
//		graphviz << "// (";
//		tmp_stset = domain::get_instance().get_grounder().deground_fluent(tmp_fs);
//		for (it_st_set = tmp_stset.begin(); it_st_set != tmp_stset.end(); it_st_set++) {
//			if (print_first) {
//				graphviz << ",";
//			}
//			print_first = true;
//			graphviz << *it_st_set;
//		}
//		graphviz << ")\n";
//	}
//
//	graphviz << "\n\n";
//	graphviz << "//RANKS List:" << std::endl;
//
//	std::map<int, pworld_ptr_set> for_rank_print;
//	for (it_kwset = get_worlds().begin(); it_kwset != get_worlds().end(); it_kwset++) {
//		for_rank_print[it_kwset->get_repetition()].insert(*it_kwset);
//	}
//
//	std::map<int, pworld_ptr_set>::const_iterator it_map_rank;
//	for (it_map_rank = for_rank_print.begin(); it_map_rank != for_rank_print.end(); it_map_rank++) {
//		graphviz << "	{rank = same; ";
//		for (it_kwset = it_map_rank->second.begin(); it_kwset != it_map_rank->second.end(); it_kwset++) {
//			graphviz << "\"" << map_rep_to_name[it_kwset->get_repetition()] << "_" << map_world_to_index[it_kwset->get_fluent_set()] << "\"; ";
//		}
//		graphviz << "}\n";
//	}
//
//
//	graphviz << "\n\n";
//	graphviz << "//EDGES List:" << std::endl;
//
//	std::map < std::tuple<std::string, std::string>, std::set<std::string> > edges;
//
//	kedge_ptr_set::const_iterator it_keset;
//	std::tuple<std::string, std::string> tmp_tuple;
//	std::string tmp_string = "";
//	for (it_keset = get_edges().begin(); it_keset != get_edges().end(); it_keset++) {
//		tmp_string = "_" + std::to_string(map_world_to_index[it_keset->get_from().get_fluent_set()]);
//		tmp_string.insert(0, 1, map_rep_to_name[it_keset->get_from().get_repetition()]);
//		std::get<0>(tmp_tuple) = tmp_string;
//
//		tmp_string = "_" + std::to_string(map_world_to_index[it_keset->get_to().get_fluent_set()]);
//		tmp_string.insert(0, 1, map_rep_to_name[it_keset->get_to().get_repetition()]);
//		std::get<1>(tmp_tuple) = tmp_string;
//
//		edges[tmp_tuple].insert(domain::get_instance().get_grounder().deground_agent(it_keset->get_label()));
//	}
//
//
//	std::map < std::tuple<std::string, std::string>, std::set < std::string>>::iterator it_map;
//	std::map < std::tuple<std::string, std::string>, std::set < std::string>>::const_iterator it_map_2;
//
//	std::map < std::tuple<std::string, std::string>, std::set < std::string>> to_print_double;
//	for (it_map = edges.begin(); it_map != edges.end(); it_map++) {
//		for (it_map_2 = it_map; it_map_2 != edges.end(); it_map_2++) {
//			if (std::get<0>(it_map->first).compare(std::get<1>(it_map_2->first)) == 0) {
//				if (std::get<1>(it_map->first).compare(std::get<0>(it_map_2->first)) == 0) {
//					if (it_map->second == it_map_2->second) {
//						if (std::get<0>(it_map->first).compare(std::get<1>(it_map->first)) != 0) {
//							to_print_double[it_map->first] = it_map->second;
//							edges.erase(it_map_2);
//							it_map = edges.erase(it_map);
//						}
//					}
//				}
//			}
//		}
//	}
//
//	std::set<std::string>::const_iterator it_stset;
//	for (it_map = edges.begin(); it_map != edges.end(); it_map++) {
//		graphviz << "	\"";
//		graphviz << std::get<0>(it_map->first);
//		graphviz << "\" -> \"";
//		graphviz << std::get<1>(it_map->first);
//		graphviz << "\" ";
//		graphviz << "[ label = \"";
//		tmp_string = "";
//		for (it_stset = it_map->second.begin(); it_stset != it_map->second.end(); it_stset++) {
//			tmp_string += *it_stset;
//			tmp_string += ",";
//		}
//		tmp_string.pop_back();
//		graphviz << tmp_string;
//		graphviz << "\" ];\n";
//	}
//
//	for (it_map = to_print_double.begin(); it_map != to_print_double.end(); it_map++) {
//		graphviz << "	\"";
//		graphviz << std::get<0>(it_map->first);
//		graphviz << "\" -> \"";
//		graphviz << std::get<1>(it_map->first);
//		graphviz << "\" ";
//		graphviz << "[ dir=both label = \"";
//		tmp_string = "";
//		for (it_stset = it_map->second.begin(); it_stset != it_map->second.end(); it_stset++) {
//
//			tmp_string += *it_stset;
//			tmp_string += ",";
//		}
//		tmp_string.pop_back();
//		graphviz << tmp_string;
//		graphviz << "\" ];\n";
//	}
//
//	graphviz << "\n\n//WORLDS description Table:" << std::endl;
//	graphviz << "	node [shape = plain]\n\n";
//	graphviz << "	description[label=<\n";
//	graphviz << "	<table border = \"0\" cellborder = \"1\" cellspacing = \"0\" >\n";
//	for (it_kwset = get_worlds().begin(); it_kwset != get_worlds().end(); it_kwset++) {
//		tmp_fs = it_kwset->get_fluent_set();
//		print_first = false;
//		graphviz << "		<tr><td>" << map_rep_to_name[it_kwset->get_repetition()] << "_" << map_world_to_index[tmp_fs] << "</td> <td>";
//		for (it_fs = tmp_fs.begin(); it_fs != tmp_fs.end(); it_fs++) {
//			if (print_first) {
//				graphviz << ", ";
//			}
//			print_first = true;
//			graphviz << domain::get_instance().get_grounder().deground_fluent(*it_fs);
//		}
//		graphviz << "</td></tr>\n";
//	}
//	graphviz << "	</table>>]\n";
//	graphviz << "	{rank = max; description};\n";

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

/*fluent_formula pstate::get_sensing_effects_if_entailed(const effects_map & map, const pworld_ptr & start) const
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