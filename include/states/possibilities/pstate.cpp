/**
 * \brief Implementation of \ref pstate.h
 *
 * \copyright GNU Public License.
 *
 * \author Francesco Fabiano.
 * \date September 16, 2019
 */

#include <iostream>
#include <tuple>
#include <boost/dynamic_bitset.hpp>

#include "pstate.h"
#include "../../utilities/helper_t.ipp"
#include "../../update/union_update.h"


/**** GETTERS/SETTERS ****/
void pstate::set_worlds(const pworld_ptr_set & to_set)
{
	m_worlds = to_set;
}

void pstate::set_pointed(const pworld_ptr & to_set)
{
	m_pointed = to_set;
}

void pstate::set_beliefs(const pedges & to_set)
{
	m_beliefs = to_set;
}

const pworld_ptr_set & pstate::get_worlds() const
{
	return m_worlds;
}

const pworld_ptr & pstate::get_pointed() const
{
	return m_pointed;
}

const pedges & pstate::get_beliefs() const
{
	return m_beliefs;
}


/**** ADD/REMOVE WORLDS/EDGES ****/
pworld_ptr pstate::add_world(const pworld & world) {
    pworld_ptr tmp = pstore::get_instance().add_world(world);
    m_worlds.insert(tmp);
    return tmp;
}

void pstate::add_edge(const pworld_ptr & from, const pworld_ptr & to, const agent & ag) {
    m_beliefs[from][ag].insert(to);
}

void pstate::remove_edge(const pworld_ptr & from, const pworld_ptr & to, const agent & ag) {
    auto from_beliefs = m_beliefs.find(from);

    if (from_beliefs != m_beliefs.end()) {
        auto ag_beliefs = from_beliefs->second.find(ag);

        if (ag_beliefs != from_beliefs->second.end()) {
            ag_beliefs->second.erase(to);
        }
    }
}


/**** OPERATORS ****/
bool pstate::operator=(const pstate & to_copy)
{
	set_worlds(to_copy.get_worlds());
	set_beliefs(to_copy.get_beliefs());
	set_pointed(to_copy.get_pointed());
	return true;
}

bool pstate::operator<(const pstate & to_compare) const
{
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

	pedges::const_iterator it_tramap1;
	auto it_tramap2 = to_compare.get_beliefs().begin();

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


/**** ENTAILMENT ****/
bool pstate::entails(const fluent & f) const
{
	return entails(f, m_pointed);
}

bool pstate::entails(const fluent & f, const pworld_ptr & world)
{
	return world.get_ptr()->entails(f);
}

bool pstate::entails(const fluent_set & fl) const
{
	return entails(fl, m_pointed);
}

bool pstate::entails(const fluent_set & fl, const pworld_ptr & world)
{
	return world.get_ptr()->entails(fl);
}

bool pstate::entails(const fluent_formula & ff) const
{
	return entails(ff, m_pointed);
}

bool pstate::entails(const fluent_formula & ff, const pworld_ptr & world)
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
	switch ( bf.get_formula_type() ) {
        case FLUENT_FORMULA: {
            /** \todo Make sure its grounded. Maybe add to \ref belief_formula a bool that store if grounded or not or maybe ground
             * when \ref domain created.
             * @see belief_formula::ground(const grounder &).*/
            return entails(bf.get_fluent_formula(), world);
        }

        case BELIEF_FORMULA: {
            /** \todo what was the at_lest_one of the previous version?*/
            return entails(bf.get_bf1(), get_B_reachable_worlds(bf.get_agent(), world));
        }
        case PROPOSITIONAL_FORMULA: {
            switch (bf.get_operator()) {
                case BF_NOT:
                    return !entails(bf.get_bf1(), world);
                case BF_OR:
                    return entails(bf.get_bf1(), world) || entails(bf.get_bf2(), world);
                case BF_AND:
                    return entails(bf.get_bf1(), world) && entails(bf.get_bf2(), world);
                case BF_FAIL:
                default:
                    std::cerr << "Something went wrong in checking entailment for Propositional formula";
                    exit(1);
            }
        }
        case E_FORMULA: {
            return entails(bf.get_bf1(), get_E_reachable_worlds(bf.get_group_agents(), world));
        }
        case D_FORMULA: {
            pworld_ptr_set D_reachable = get_D_reachable_worlds(bf.get_group_agents(), world);
            if (D_reachable.empty()) {
                return false;
            }
            return entails(bf.get_bf1(), D_reachable);
        }
        case C_FORMULA: {
            return entails(bf.get_bf1(), get_C_reachable_worlds(bf.get_group_agents(), world));
        }
        case BF_EMPTY: {
            return true;
        }
        case BF_TYPE_FAIL:
        default: {
            std::cerr << "Something went wrong in checking entailment for Belief formula";
            exit(1);
        }
    }
}

bool pstate::entails(const formula_list & to_check, const pworld_ptr & world) const
{
	formula_list::const_iterator it_fl;
	for (it_fl = to_check.begin(); it_fl != to_check.end(); it_fl++) {
		if (!entails(*it_fl, world)) {
			return false;
		}
	}
	return true;
}


/**** REACHABILITY ****/
pworld_ptr_set pstate::get_B_reachable_worlds(const agent & ag, const pworld_ptr & world) const
{
	pworld_ptr_set ret;
	auto pw_map = m_beliefs.find(world);

	if (pw_map != m_beliefs.end()) {
		auto pw_set = pw_map->second.find(ag);
		if (pw_set != pw_map->second.end()) {
			helper_t::sum_set<pworld_ptr>(ret, pw_set->second);
		}
	}
	return ret;
}

bool pstate::get_B_reachable_worlds_recursive(const agent & ag, const pworld_ptr & world, pworld_ptr_set& ret) const
{
	/** \todo check: If a--i-->b, b--i-->c then a--i-->c must be there*/
	auto pw_map = m_beliefs.find(world);

	if (pw_map != m_beliefs.end()) {
		auto pw_set = pw_map->second.find(ag);
		if (pw_set != pw_map->second.end()) {
			unsigned long previous_size = ret.size();
			helper_t::sum_set<pworld_ptr>(ret, pw_set->second);
			unsigned long current_size = ret.size();

			return previous_size == current_size;
		}
		/**@bug: We don't know why sometimes is outside the two if cases.*/
	}
	return true;
}

pworld_ptr_set pstate::get_E_reachable_worlds(const agent_set & ags, const pworld_ptr & world) const
{
	/*The K^0 call of this function*/
	pworld_ptr_set ret;
	agent_set::const_iterator it_agset;
	for (it_agset = ags.begin(); it_agset != ags.end(); it_agset++) {
		helper_t::sum_set<pworld_ptr>(ret, get_B_reachable_worlds(*it_agset, world));
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
			if (!get_B_reachable_worlds_recursive(*it_agset, *it_pwptr, ret)) {
				is_fixed_point = false;
			}
		}
	}
	return is_fixed_point;
}

pworld_ptr_set pstate::get_C_reachable_worlds(const agent_set & ags, const pworld_ptr & world) const
{
	//Use of fixed point to stop.
	bool is_fixed_point = false;
	pworld_ptr_set newly_reached = get_E_reachable_worlds(ags, world);
	pworld_ptr_set already_reached;
	pworld_ptr_set ret;
	while (!is_fixed_point) {
		helper_t::sum_set<pworld_ptr>(newly_reached, ret);
		helper_t::minus_set<pworld_ptr>(newly_reached, already_reached);
		is_fixed_point = get_E_reachable_worlds_recoursive(ags, newly_reached, ret);
		already_reached = newly_reached;
	}
	return ret;
}

pworld_ptr_set pstate::get_D_reachable_worlds(const agent_set & ags, const pworld_ptr & world) const
{
	/**@bug: Notion of D-Reachable is correct (page 24 of Reasoning about Knowledge)*/
	auto it_agset = ags.begin();
	pworld_ptr_set ret = get_B_reachable_worlds((*it_agset), world);
	it_agset++;

	for (; it_agset != ags.end(); it_agset++) {
		auto it_pwset1 = ret.begin();
		pworld_ptr_set to_intersect = get_B_reachable_worlds((*it_agset), world);
		auto it_pwset2 = to_intersect.begin();
		while ((it_pwset1 != ret.end()) && (it_pwset2 != to_intersect.end())) {

			if ((*it_pwset1 < *it_pwset2) && (((*it_pwset1).get_fluent_based_id()) == ((*it_pwset2).get_fluent_based_id()))) {
				ret.erase(it_pwset1++);
			} else if ((*it_pwset2 < *it_pwset1) && ((((*it_pwset1).get_fluent_based_id()) == ((*it_pwset2).get_fluent_based_id())))) {
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

void pstate::get_all_reachable_worlds_edges(const pworld_ptr & world, pworld_ptr_set & reached_worlds, pedges & reached_edges) const
{
    pworld_ptr_set::const_iterator it_pws;
    pworld_ptr_set pw_list;

    auto ag_set = domain::get_instance().get_agents();
    auto ag_it = ag_set.begin();

    for (; ag_it != ag_set.end(); ag_it++) {
        pw_list = m_beliefs.at(world).at(*ag_it);

        for (it_pws = pw_list.begin(); it_pws != pw_list.end(); it_pws++) {
            if (reached_worlds.insert(*it_pws).second) {
                reached_edges[*it_pws] = m_beliefs.at(*it_pws);
                get_all_reachable_worlds_edges(*it_pws, reached_worlds, reached_edges);
            }
        }
    }
}

void pstate::clean_unreachable_pworlds()
{
    pworld_ptr_set reached_worlds;
    pedges reached_edges;

    reached_worlds.insert(get_pointed());
    reached_edges[get_pointed()] = m_beliefs.at(get_pointed());

    get_all_reachable_worlds_edges(get_pointed(), reached_worlds, reached_edges);

    set_worlds(reached_worlds);
    set_beliefs(reached_edges);
}


/**** INITIAL STATE ****/
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
	//todo non usare indici ma bitset prima positivo fluent poi negativo
	unsigned int fluent_number = domain::get_instance().get_fluent_number();
	unsigned int bit_size = domain::get_instance().get_size_fluent();

	if (index == fluent_number) {
		pworld to_add(permutation);
		add_initial_pworld(to_add);

		return;
	}

	fluent_set permutation_2 = permutation;
	//Add the \ref fluent in positive version
	boost::dynamic_bitset<> bitSetToFindPositve(bit_size, index);
	boost::dynamic_bitset<> bitSetToFindNegative(bit_size, index);
	bitSetToFindNegative.set(bitSetToFindPositve.size() - 1, true);
	bitSetToFindPositve.set(bitSetToFindPositve.size() - 1, false);


	if (initially_known.find(bitSetToFindNegative) == initially_known.end()) {
		permutation.insert(bitSetToFindPositve);
		generate_initial_pworlds(permutation, index + 1, initially_known);
	}
	if (initially_known.find(bitSetToFindPositve) == initially_known.end()) {
		permutation_2.insert(bitSetToFindNegative);
		generate_initial_pworlds(permutation_2, index + 1, initially_known);
	}

}

void pstate::add_initial_pworld(const pworld & possible_add)
{
	initially ini_conditions = domain::get_instance().get_initial_description();

	switch (ini_conditions.get_ini_restriction()) {
        case S5: {
            /* Since the common knowledge is on all the agent it means that every possible \ref pworld
             * in the initial state must entail *phi* where C(*phi*) is an initial condition.*/

            //Already setted in \ref domain::build_initially(bool).
            if (possible_add.entails(ini_conditions.get_ff_forS5())) {
                add_world(possible_add);
                if (possible_add.entails(ini_conditions.get_pointed_world_conditions())) {
                    m_pointed = pworld_ptr(possible_add);
                }
            } else {
                //Already generated so we save it on pstore
                pstore::get_instance().add_world(possible_add);
            }
            break;
        }
        case K45:
        case NONE:
        default: {
            std::cerr << "\nNot implemented yet\n";
            exit(1);
        }
	}
}

void pstate::generate_initial_pedges()
{
    agent_set agents = domain::get_instance().get_agents();
	pworld_ptr pwptr_tmp1, pwptr_tmp2;

	pworld_ptr_set::const_iterator it_pwps_1, it_pwps_2;
    agent_set::const_iterator it_ags;

	/*This for add to *this* all the possible edges.*/
	for (it_pwps_1 = m_worlds.begin(); it_pwps_1 != m_worlds.end(); it_pwps_1++) {
		for (it_pwps_2 = it_pwps_1; it_pwps_2 != m_worlds.end(); it_pwps_2++) {
			for (it_ags = agents.begin(); it_ags != agents.end(); it_ags++) {
				pwptr_tmp1 = *it_pwps_1;
				pwptr_tmp2 = *it_pwps_2;

				add_edge(pwptr_tmp1, pwptr_tmp2, *it_ags);
				add_edge(pwptr_tmp2, pwptr_tmp1, *it_ags);

			}
		}
	}

	initially ini_conditions = domain::get_instance().get_initial_description();
	formula_list::const_iterator it_fl;

	for (it_fl = ini_conditions.get_initial_conditions().begin(); it_fl != ini_conditions.get_initial_conditions().end(); it_fl++) {
		remove_initial_pedge_bf(*it_fl);
	}
}

void pstate::remove_initial_pedge(const fluent_formula &known_ff, const agent & ag)
{
	pworld_ptr_set::const_iterator it_pwps_1, it_pwps_2;

	pworld_ptr pwptr_tmp1, pwptr_tmp2;

	/** \todo maybe don't loop twice on the world but exploit using it_pwps_2 = it_pwps_1:
	 * - remove (_1, _2).
	 * - remove (_2, _1).*/
	for (it_pwps_1 = m_worlds.begin(); it_pwps_1 != m_worlds.end(); it_pwps_1++) {
		for (it_pwps_2 = it_pwps_1; it_pwps_2 != m_worlds.end(); it_pwps_2++) {
			/** \todo or entails(-known_ff)?*/
			pwptr_tmp1 = *it_pwps_1;
			pwptr_tmp2 = *it_pwps_2;
			if (pwptr_tmp1.get_ptr()->entails(known_ff) && !pwptr_tmp2.get_ptr()->entails(known_ff)) {
				remove_edge(pwptr_tmp1, pwptr_tmp2, ag);
				remove_edge(pwptr_tmp2, pwptr_tmp1, ag);
			} else if (pwptr_tmp2.get_ptr()->entails(known_ff) && !pwptr_tmp1.get_ptr()->entails(known_ff)) {

				remove_edge(pwptr_tmp2, pwptr_tmp1, ag);
				remove_edge(pwptr_tmp1, pwptr_tmp2, ag);
			}
		}
	}
}

void pstate::remove_initial_pedge_bf(const belief_formula & to_check)
{
	switch (domain::get_instance().get_initial_description().get_ini_restriction()) {
        case S5: {
            /* Just check whenever is B(--) \/ B(--) and remove that edge*/
            if (to_check.get_formula_type() == C_FORMULA) {
                const belief_formula& tmp = to_check.get_bf1();

                switch (tmp.get_formula_type()) {
                    //Only one for edges -- expresses that someone is ignorant.
                    case PROPOSITIONAL_FORMULA: {
                        //We remove all the check on the formula since they have already been controlled when ini_conditions has been created
                        if (tmp.get_operator() == BF_OR) {
                            //fluent_formula known_ff;
                            auto known_ff_ptr = std::make_shared<fluent_formula>();
                            helper::check_Bff_notBff(tmp.get_bf1(), tmp.get_bf2(), known_ff_ptr);
                            if (known_ff_ptr != nullptr) {
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
                    }
                    case FLUENT_FORMULA:
                    case BELIEF_FORMULA:
                    case BF_EMPTY: {
                        return;
                    }
                    default: {
                        std::cerr << "\nError in the type of initial formulae (SIXTH).\n";
                        exit(1);
                    }
                }
            } else {
                std::cerr << "\nError in the type of initial formulae (SEVENTH).\n";
                exit(1);
            }
            return;
        }
        case K45:
        case NONE:
        default: {
            std::cerr << "\nNot implemented yet\n";
            exit(1);
        }
	}
}


/**** TRANSITION FUNCTION ****/
pstate pstate::compute_succ(const action & act) const
{
    /** \warning executability should be check in \ref state (or \ref planner).*/
    return union_update::u_update(*this, act);
}


/**** BISIMULATION ****/
automaton pstate::pstate_to_automaton(std::vector<pworld_ptr> & pworld_vec, const std::map<agent, bis_label> & agent_to_label) const
{

    std::map<int, int> compact_indices;
    std::map<pworld_ptr, int> index_map;
    pbislabel_map label_map; // Map: from -> (to -> ag_set)

    automaton *a;
    unsigned long Nvertex = get_worlds().size();
    unsigned long  ag_set_size = domain::get_instance().get_agents().size();
    //BIS_ADAPTATION For the loop that identifies the id (We add one edge for each node)
    v_elem *Vertex;

    Vertex = (v_elem *) malloc(sizeof(v_elem) * Nvertex);

    // Initializating vertices
    pworld_ptr_set::const_iterator it_pwps;
    pedges::const_iterator it_peps;
    pbislabel_map::const_iterator it_plm;
    bis_label_set::const_iterator it_bislab;
    std::map<pworld_ptr, bis_label_set>::const_iterator it_pw_bislab;

    //std::cerr << "\nDEBUG: Inizializzazione Edges\n";

    // The pointed world is set to the index 0. This ensures that, when deleting the bisimilar nodes, the pointed pworld
    // is always chosen as the first of its block. Therefore, we do not need to update it when converting back to a kstate
    index_map[get_pointed()] = 0;
    pworld_vec.push_back(get_pointed());
    compact_indices[static_cast<int>(get_pointed().get_numerical_id())] = 0;

    //For the loop that identifies the id
    //BIS_ADAPTATION For the loop that identifies the id (+1)
    ///@bug: If the pointed has no self-loop to add
    //pworld_ptr_set pointed_adj = adj_list.at(get_pointed());

    Vertex[0].ne = 0; // pointed_adj.size(); // edge_counter[get_pointed()];
    //	if (pointed_adj.find(get_pointed()) == pointed_adj.end()) {
    //		Vertex[0].ne++;
    //	}
    //	Vertex[0].e = (e_elem *) malloc(sizeof(e_elem) * Vertex[0].ne);

    int i = 1, c = 1;

    //std::cerr << "\nDEBUG: Inizializzazione Vertex\n";

    for (it_pwps = m_worlds.begin(); it_pwps != m_worlds.end(); it_pwps++) {
        if (!(*it_pwps == get_pointed())) {
            index_map[*it_pwps] = i;
            pworld_vec.push_back(*it_pwps);

            // if (compact_indices.find(it_pwps->get_numerical_id()) == compact_indices.end()) {
            if (compact_indices.insert({it_pwps->get_numerical_id(), c}).second) {
                // compact_indices[it_pwps->get_numerical_id()] = c;
                c++;
                //std::cerr << "\nDEBUG: Added:" << it_pwps->get_id() << "\n";
            }
            Vertex[i].ne = 0;
            i++;
        }
        //BIS_ADAPTATION (Added self-loop)
        label_map[*it_pwps][*it_pwps].insert(compact_indices[static_cast<int>(it_pwps->get_numerical_id())] + ag_set_size);
        //std::cerr << "\nDEBUG: Added to " << it_pwps->get_numerical_id() << " the label " << compact_indices[it_pwps->get_numerical_id()] + ag_set_size << std::endl;
    }


    //BIS_ADAPTATION For the loop that identifies the id (We add one potential label for each node)
    unsigned long bhtabSize = ag_set_size + c;

    //std::cerr << "\nDEBUG: Inizializzazione Behavs\n";

    //BIS_ADAPTATION (Moved down here)

    for (it_peps = m_beliefs.begin(); it_peps != m_beliefs.end(); it_peps++) {

        for (auto it_mid_bel = it_peps->second.begin(); it_mid_bel != it_peps->second.end(); it_mid_bel++) {
            for (auto it_int_ed = it_mid_bel->second.begin(); it_int_ed != it_mid_bel->second.end(); it_int_ed++) {
                label_map[it_peps->first][*it_int_ed].insert(agent_to_label.at(it_mid_bel->first));
                Vertex[index_map[it_peps->first]].ne++;
            }
        }
    }

    i = 0;
    for (it_pwps = m_worlds.begin(); it_pwps != m_worlds.end(); it_pwps++) {
        Vertex[i].ne++; //Self loop bisimulation
        Vertex[i].e = (e_elem *) malloc(sizeof(e_elem) * Vertex[i].ne);
        i++;
    }

    //std::cerr << "\nDEBUG: Fine Inizializzazione Vertex\n";


    int from, to, j = 0; //, k = 0, nbh;

    //std::cerr << "\nDEBUG: Inizializzazione Mappa\n";
    for (it_plm = label_map.begin(); it_plm != label_map.end(); it_plm++) {
        from = index_map[it_plm->first]; // For each pworld 'from'

        //std::cerr << "\nDEBUG: Inizializzazione K\n";

        for (it_pw_bislab = it_plm->second.begin(); it_pw_bislab != it_plm->second.end(); it_pw_bislab++) { // For each edge that reaches the pworld 'to'
            to = index_map[it_pw_bislab->first];
            //nbh = it_pw_bislab->second.size();

            for (it_bislab = it_pw_bislab->second.begin(); it_bislab != it_pw_bislab->second.end(); it_bislab++) { // For each agent 'ag' in the label of the kedge
                //std::cerr << "\nDEBUG: j is: " << j << " and k is: " << k << "\n";
                //nbh = 1;
                Vertex[from].e[j].nbh = 1; // Let j be the index of the adjacency list of from that stores the kedge (from, to)
                Vertex[from].e[j].bh = (int *) malloc(sizeof(int)); // Let nbh be the number of agents in such kedge
                Vertex[from].e[j].tv = to; // Update the value of the reache pworld
                Vertex[from].e[j].bh[0] = *it_bislab; // Update the value of the label at index k to 'ag'
                //std::cerr << "\nDEBUG: j is: " << j << " and k is: " << k << "\n";

                j++; // Update the value of the index j
            }


        }

        j = 0; // Reset j
    }

    unsigned long Nbehavs = bhtabSize;
    a = (automaton *) malloc(sizeof(automaton));
    a->Nvertex = Nvertex;
    a->Nbehavs = Nbehavs;
    a->Vertex = Vertex;

    //	std::cerr << "\nDEBUG: \n\tNvertex = " << Nvertex << std::endl;
    //	std::cerr << "\tNbehavs = " << Nbehavs << std::endl;

    return *a;
}

void pstate::automaton_to_pstate(const automaton & a, const std::vector<pworld_ptr> & pworld_vec, const std::map<bis_label, agent> & label_to_agent)
{
    pworld_ptr_set worlds;
    m_beliefs.clear();
    // The pointed world does not change when we calculate the minimum bisimilar state
    // Hence we do not need to update it

    int i, j, k, label;
    unsigned long agents_size = domain::get_instance().get_agents().size();

    for (i = 0; i < a.Nvertex; i++) {
        if (a.Vertex[i].ne > 0) {
            worlds.insert(pworld_vec[i]);
            for (j = 0; j < a.Vertex[i].ne; j++) {
                for (k = 0; k < a.Vertex[i].e[j].nbh; k++) {
                    label = a.Vertex[i].e[j].bh[k];
                    if (label < agents_size) {
                        add_edge(pworld_vec[i], pworld_vec[a.Vertex[i].e[j].tv], label_to_agent.at(label));
                    }
                }
            }
        }
    }

    set_worlds(worlds);
}

void pstate::calc_min_bisimilar()
{
    //std::cerr << "\nDEBUG: INIZIO BISIMULATION IN PSTATE\n" << std::flush;

    // ************* Cleaning unreachable pworlds *************

    clean_unreachable_pworlds();

    std::vector<pworld_ptr> pworld_vec; // Vector of all pworld_ptr
    //std::cerr << "\nDEBUG: PRE-ALLOCAZIONE AUTOMA\n" << std::flush;


    //	std::cerr << "\nDEBUG: \n\tNvertex_before = " << m_worlds.size() << std::endl;
    //	std::cerr << "\tNbehavs_before = " << m_edges.size() << std::endl;

    automaton a;
    pworld_vec.reserve(get_worlds().size());

    std::map<bis_label, agent> label_to_agent;
    std::map<agent, bis_label> agent_to_label;


    auto agents = domain::get_instance().get_agents();
    auto it_ag = agents.begin();
    bis_label ag_label = 0;
    agent lab_agent;
    for (; it_ag != agents.end(); it_ag++) {
        lab_agent = *it_ag;
        label_to_agent.insert(std::make_pair(ag_label, lab_agent));
        agent_to_label.insert(std::make_pair(lab_agent, ag_label));
        ag_label++;

    }

    a = pstate_to_automaton(pworld_vec, agent_to_label);

    bisimulation b;
    //std::cout << "\nDEBUG: Printing automaton pre-Bisimulation\n";
    //b.VisAutoma(&a);

    if (domain::get_instance().get_bisimulation() == PaigeTarjan) {
        if (b.MinimizeAutomaPT(&a)) {
            //VisAutoma(a);

            //std::cout << "\nDEBUG: Printing automaton post-Bisimulation\n";
            //b.VisAutoma(&a);
            //std::cout << "Done\n";
            automaton_to_pstate(a, pworld_vec, label_to_agent);

            //b.DisposeAutoma(&a);
        }
    } else {
        if (b.MinimizeAutomaFB(&a)) {

            //std::cerr << "\nDEBUG: Printing automaton post-Bisimulation\n";
            //b.VisAutoma(&a);
            //std::cerr << "Done\n";
            automaton_to_pstate(a, pworld_vec, label_to_agent);
            //b.DisposeAutoma(&a);
        }
    }

    //std::cerr << "\nDEBUG: PRe Clean" << std::endl;

}


/**** UTILITIES ****/
bool pstate::check_properties(const agent_set & fully, const agent_set & partially, const fluent_formula & effects, const pstate & updated) const
{
	if (!fully.empty()) {
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

		if (!partially.empty()) {
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

void pstate::print() const
{
	int counter = 1;
	std::cout << std::endl;
	std::cout << "The Pointed World has id ";
	printer::get_instance().print_list(get_pointed().get_fluent_set());
	std::cout << "-" << get_pointed().get_repetition();
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
	pedges::const_iterator it_pwtm;
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
		tmp_unsh = static_cast<char>(it_pwset->get_repetition());
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

	pedges::const_iterator it_pwtm;
	pworld_map::const_iterator it_pwm;
	std::tuple<std::string, std::string> tmp_tuple;
	std::string tmp_string;

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
			if (std::get<0>(it_map->first) == std::get<1>(it_map_2->first)) {
				if (std::get<1>(it_map->first) == std::get<0>(it_map_2->first)) {
					if (it_map->second == it_map_2->second) {
						if (std::get<0>(it_map->first) != std::get<1>(it_map->first)) {
							to_print_double[it_map->first] = it_map->second;
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
			if (helper::is_negate(*it_fs)) color = "<font color=\"#0000ff\"> ";
			else color = "<font color=\"#ff1020\">";
			graphviz << color << domain::get_instance().get_grounder().deground_fluent(*it_fs) << "</font>";
		}
		graphviz << "</td></tr>\n";
	}
	graphviz << "	</table>>]\n";
	graphviz << "	{rank = max; description};\n";

}
