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
#include <stack>
#include <stdio.h>


#include "kstate.h"
#include "../../update/union_update.h"
#include "../../domain/domain.h"
#include "../../utilities/helper.h"

/**** GETTERS/SETTERS ****/
void kstate::set_worlds(const kworld_ptr_set & to_set)
{
	m_worlds.clear();
	m_worlds = to_set;
}

void kstate::set_edges(const kedge_map & to_set)
{
	m_edges.clear();
	m_edges = to_set;
}

void kstate::set_pointed(const kworld_ptr & to_set)
{
	m_pointed = to_set;
}

const kworld_ptr_set & kstate::get_worlds() const
{
	return m_worlds;
}

const kedge_map & kstate::get_edges() const
{
	return m_edges;
}

const kworld_ptr & kstate::get_pointed() const
{
	return m_pointed;
}

/**** ADD/REMOVE WORLDS/EDGES ****/
kworld_ptr kstate::add_world(const kworld & world)
{
    kworld_ptr tmp = kstore::get_instance().add_world(world);
    m_worlds.insert(tmp);
    return tmp;
}

void kstate::add_edge(const kworld_ptr & from, const kworld_ptr & to, const agent & ag)
{
    m_edges[from][ag].insert(to);
}

void kstate::remove_kedge(const kworld_ptr & from, const kworld_ptr & to, const agent & ag) {
    auto from_edges = m_edges[from];

    if (!from_edges.empty()) {
        kworld_ptr_set kwset = from_edges[ag];

        if (!kwset.empty()) {
            kwset.erase(to);
        }
    }
}


/**** OPERATORS ****/
bool kstate::operator=(const kstate & to_copy)
{
    set_edges(to_copy.get_edges());
    set_worlds(to_copy.get_worlds());
//	m_max_depth = get_max_depth();
    set_pointed(to_copy.get_pointed());
    return true;
}

bool kstate::operator==(const kstate & to_compare) const
{
    if (m_pointed.get_numerical_id() == to_compare.get_pointed().get_numerical_id()) {
        return true;
    }

    bisimulation b;

    return b.compare_automata_eq(*this, to_compare);
}

bool kstate::operator<(const kstate & to_compare) const
{
//	if (m_max_depth < to_compare.get_max_depth()) {
//		return true;
//	} else if (m_max_depth > to_compare.get_max_depth()) {
//		return false;
//	}

    if (m_pointed.get_numerical_id() < to_compare.get_pointed().get_numerical_id()) {
        return true;
    } else if (m_pointed.get_numerical_id() > to_compare.get_pointed().get_numerical_id()) {
        return false;
    }

//	bisimulation b;
//
//	if (m_edges < to_compare.get_edges()) {
//		return b.compare_automata(*this, to_compare);
//	} else {
//		return b.compare_automata(to_compare, *this);
//	}

    if (m_worlds < to_compare.get_worlds()) {
        return true;
    } else if (m_worlds > to_compare.get_worlds()) {
        return false;
    }

    if (m_edges < to_compare.get_edges()) {
        return true;
    } else if (m_edges > to_compare.get_edges()) {
        return false;
    }

    //These are implemented in std

    //	if (m_worlds.size() < to_compare.get_worlds().size()) {
    //		return true;
    //	} else if (m_worlds.size() > to_compare.get_worlds().size()) {
    //		return false;
    //	}
    //
    //	if (m_edges.size() < to_compare.get_edges().size()) {
    //		return true;
    //	} else if (m_edges.size() > to_compare.get_edges().size()) {
    //		return false;
    //	}

    //	kworld_ptr_set::const_iterator it_kwpts1, it_kwpts2;
    //	it_kwpts2 = to_compare.get_worlds().begin();
    //	for (it_kwpts1 = m_worlds.begin(); it_kwpts1 != m_worlds.end(); it_kwpts1++) {
    //		if ((*it_kwpts1) < (*it_kwpts2)) {
    //			return true;
    //		} else if ((*it_kwpts1) > (*it_kwpts2)) {
    //			return false;
    //		}
    //		//The case when to compare has less world than this
    //		it_kwpts2++;
    //	}

    //	kedge_ptr_set::const_iterator it_kepts1, it_kepts2;
    //	it_kepts2 = to_compare.get_edges().begin();
    //	for (it_kepts1 = m_edges.begin(); it_kepts1 != m_edges.end(); it_kepts1++) {
    //		if ((*it_kepts1) < (*it_kepts2)) {
    //			return true;
    //		} else if ((*it_kepts1) > (*it_kepts2)) {
    //			return false;
    //		}
    //		it_kepts2++;
    //	}

    return false;
}


/**** ENTAILMENT ****/
bool kstate::entails(fluent f) const
{
	return entails(f, m_pointed);
}

bool kstate::entails(fluent f, const kworld_ptr & world) const
{
	return world.get_ptr()->entails(f);
}

bool kstate::entails(const fluent_set & fl) const
{
	return entails(fl, m_pointed);
}

bool kstate::entails(const fluent_set & fl, const kworld_ptr & world) const
{
	return world.get_ptr()->entails(fl);
}

bool kstate::entails(const fluent_formula & ff) const
{
	return entails(ff, m_pointed);
}

bool kstate::entails(const fluent_formula & ff, const kworld_ptr & world) const
{
	return world.get_ptr()->entails(ff);
}

bool kstate::entails(const belief_formula & bf) const
{
	return entails(bf, m_pointed);
}

bool kstate::entails(const belief_formula & bf, const kworld_ptr & world) const
{
	/*
	 The entailment of a \ref belief_formula just call recursively the entailment on all the reachable world with that formula.
	 */
	kworld_ptr_set D_reachable;
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

bool kstate::entails(const formula_list & to_check, const kworld_ptr & world) const
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


/**** REACHABILITY ****/
kworld_ptr_set kstate::get_B_reachable_worlds(const agent & ag, const kworld_ptr & world) const
{
	return m_edges.at(world).at(ag);
}

//bool kstate::get_B_reachable_worlds_recoursive(const agent & ag, const kworld_ptr & world, kworld_ptr_set & ret) const
//{
//	/** \todo check: If a--i-->b, b--i-->c then a--i-->c must be there*/
//	bool is_fixed_point = true;
//	kedge_ptr_set::const_iterator it_kedge;
//	for (it_kedge = m_edges.begin(); it_kedge != m_edges.end(); it_kedge++) {
//		if (((*it_kedge).get_from() == world) && ((*it_kedge).get_label() == ag)) {
//			//We use the pair of insert, if we add a new world (true in the set::insert) then is not a fixed point
//			//if (std::get<1>(ret.insert((*it_kedge).get_to()))) {
//			if ((ret.insert((*it_kedge).get_to())).second) {
//				is_fixed_point = false;
//			}
//		}
//	}
//	return is_fixed_point;
//}

kworld_ptr_set kstate::get_E_reachable_worlds(const agent_set & ags, const kworld_ptr & world) const
{
    /*Optimized, the K^0 call of this function
	 *
	 * Not calling B_reachability iteratively for optimization
	 */
    kworld_ptr_set ret, tmp;
    agent_set::const_iterator it_ags;
    auto world_edges = m_edges.at(world);

    for (it_ags = ags.begin(); it_ags != ags.end(); it_ags++) {
        tmp = world_edges.at(*it_ags);
        std::merge(ret.begin(), ret.end(),
                   tmp.begin(), tmp.end(),
                   std::inserter(ret, ret.begin()));
    }
	return ret;
}

bool kstate::get_E_reachable_worlds_recursive(const agent_set & ags, const kworld_ptr_set & worlds, kworld_ptr_set & reached) const
{
	/*Optimized, the K^i (recursive) call of this function*/

	bool is_fixed_point = true;
	kedge_ptr_set::const_iterator it_kedge;

	kworld_ptr_set::const_iterator it_kws1, it_kws2;
	agent_set::const_iterator it_ags;

	std::map<agent, kworld_ptr_set> world_edges;
	kworld_ptr_set kw_set;
    /*\bug What if the pointed is not reachable, correct this*/
    for (it_kws1 = worlds.begin(); it_kws1 != worlds.end(); it_kws1++) {
	    world_edges = m_edges.at(*it_kws1);

        for (it_ags = ags.begin(); it_ags != ags.end(); it_ags++) {
            kw_set = world_edges.at(*it_ags);

            for (it_kws2 = kw_set.begin(); it_kws2 != kw_set.end(); it_kws2++) {
                if (reached.insert(*it_kws2).second) {
                    is_fixed_point = false;
                }
            }
        }
	}
	return is_fixed_point;
}

kworld_ptr_set kstate::get_C_reachable_worlds(const agent_set & ags, const kworld_ptr & world) const
{
	//Use of fixed point to stop.
	bool is_fixed_point = false;
	//THIS IS K^0
	kworld_ptr_set newly_reached = get_E_reachable_worlds(ags, world);
	kworld_ptr_set already_reached;
	kworld_ptr_set ret;
	//FROM HERE K^i UNTIL FIXED_POINT
	while (!is_fixed_point) {
		sum_set(newly_reached, ret);
		minus_set(newly_reached, already_reached);
		is_fixed_point = get_E_reachable_worlds_recursive(ags, newly_reached, ret);
		already_reached = newly_reached;
	}
	return ret;
}

kworld_ptr_set kstate::get_D_reachable_worlds(const agent_set & ags, const kworld_ptr & world) const
{
	/**@bug: Notion of D-Reachable is correct (page 24 of Reasoning about Knowledge)*/
	auto it_agset = ags.begin();
	kworld_ptr_set ret = get_B_reachable_worlds((*it_agset), world);
	it_agset++;

	for (; it_agset != ags.end(); it_agset++) {

		auto it_pwset1 = ret.begin();
		kworld_ptr_set to_intersect = get_B_reachable_worlds((*it_agset), world);
		auto it_pwset2 = to_intersect.begin();
		while ((it_pwset1 != ret.end()) && (it_pwset2 != to_intersect.end())) {

			if ((*it_pwset1 < *it_pwset2) && (it_pwset1->get_fluent_based_id() != it_pwset2->get_fluent_based_id())) {
				ret.erase(it_pwset1++);
			} else if ((*it_pwset2 < *it_pwset1) && (it_pwset1->get_fluent_based_id() != it_pwset2->get_fluent_based_id())) {
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

void kstate::get_all_reachable_worlds_edges(const kworld_ptr & world, kworld_ptr_set & reached_worlds, kedge_map & reached_edges) const
{
	auto world_edges = m_edges.at(world);
	std::map<agent, kworld_ptr_set>::const_iterator it_agkw;
	kworld_ptr_set::const_iterator it_kws;

	for (it_agkw = world_edges.begin(); it_agkw != world_edges.end(); it_agkw++) {
        for (it_kws = it_agkw->second.begin(); it_kws != it_agkw->second.end(); it_kws++) {
            reached_edges[world][it_agkw->first].insert(*it_kws);

            if (reached_worlds.insert(*it_kws).second) {
                get_all_reachable_worlds_edges(*it_kws, reached_worlds, reached_edges);
            }
        }
	}
}

void kstate::clean_unreachable_kworlds()
{
	kworld_ptr_set reached_worlds;
	kedge_map reached_edges;

	kedge_ptr_set::const_iterator it_keps;

	reached_worlds.insert(get_pointed());
    get_all_reachable_worlds_edges(get_pointed(), reached_worlds, reached_edges);

	set_worlds(reached_worlds);
	set_edges(reached_edges);
}


/**** BISIMULATION ****/
automaton kstate::kstate_to_automaton(/*const std::map<kworld_ptr, kworld_ptr_set> & adj_list,*/ std::vector<kworld_ptr> & kworld_vec, const std::map<agent, bis_label> & agent_to_label) const
{

	std::map<int, int> compact_indices;
	std::map<kworld_ptr, int> index_map;
	kbislabel_map label_map; // Map: from -> (to -> ag_set)

	automaton *a;
	unsigned long Nvertex = get_worlds().size();
    unsigned long ag_set_size = domain::get_instance().get_agents().size();
	//BIS_ADAPTATION For the loop that identifies the id (We add one edge for each node)
	v_elem *Vertex;

	Vertex = (v_elem *) malloc(sizeof(v_elem) * Nvertex);

	// Initializating vertices
	kworld_ptr_set::const_iterator it_kwps;
	kedge_ptr_set::const_iterator it_keps;
	kbislabel_map::const_iterator it_klm;
	bis_label_set::const_iterator it_bislab;
	std::map<kworld_ptr, bis_label_set>::const_iterator it_kw_bislab;

	//std::cerr << "\nDEBUG: Inizializzazione Edges\n";

	// The pointed world is set to the index 0. This ensures that, when deleting the bisimilar nodes, the pointed kworld
	// is always chosen as the first of its block. Therefore, we do not need to update it when converting back to a kstate
	index_map[get_pointed()] = 0;
	kworld_vec.push_back(get_pointed());
	compact_indices[get_pointed().get_numerical_id()] = 0;

	//For the loop that identifies the id
	//BIS_ADAPTATION For the loop that identifies the id (+1)
	///@bug: If the pointed has no self-loop to add
	//kworld_ptr_set pointed_adj = adj_list.at(get_pointed());

	Vertex[0].ne = 0; // pointed_adj.size(); // edge_counter[get_pointed()];
	//	if (pointed_adj.find(get_pointed()) == pointed_adj.end()) {
	//		Vertex[0].ne++;
	//	}
	//	Vertex[0].e = (e_elem *) malloc(sizeof(e_elem) * Vertex[0].ne);

	int i = 1, c = 1;

	//std::cerr << "\nDEBUG: Inizializzazione Vertex\n";

	for (it_kwps = m_worlds.begin(); it_kwps != m_worlds.end(); it_kwps++) {
		if (!(*it_kwps == get_pointed())) {
			index_map[*it_kwps] = i;
			kworld_vec.push_back(*it_kwps);

			// if (compact_indices.find(it_kwps->get_numerical_id()) == compact_indices.end()) {
			if (compact_indices.insert({it_kwps->get_numerical_id(), c}).second) {
				// compact_indices[it_kwps->get_numerical_id()] = c;
				c++;
				//std::cerr << "\nDEBUG: Added:" << it_kwps->get_id() << "\n";
			}

			//BIS_ADAPTATION For the loop that identifies the id (+1)
			Vertex[i].ne = 0; //adj_list.at(*it_kwps).size(); // edge_counter[*it_kwps];
			//			Vertex[i].e = (e_elem *) malloc(sizeof(e_elem) * Vertex[i].ne);
			i++;
		}
		//BIS_ADAPTATION (Added self-loop)
		label_map[*it_kwps][*it_kwps].insert(compact_indices[it_kwps->get_numerical_id()] + ag_set_size);
		//std::cerr << "\nDEBUG: Added to " << it_kwps->get_numerical_id() << " the label " << compact_indices[it_kwps->get_numerical_id()] + ag_set_size << std::endl;
	}


	//BIS_ADAPTATION For the loop that identifies the id (We add one potential label for each node)
    unsigned long bhtabSize = ag_set_size + c;

	//std::cerr << "\nDEBUG: Inizializzazione Behavs\n";

	//BIS_ADAPTATION (Moved down here)
	kedge_map::const_iterator it_kem;
	std::map<agent, kworld_ptr_set>::const_iterator it_agkw;

	for (it_kem = m_edges.begin(); it_kem != m_edges.end(); it_kem++) {
        for (it_agkw = it_kem->second.begin(); it_agkw != it_kem->second.end(); it_agkw++) {
            for (it_kwps = it_agkw->second.begin(); it_kwps != it_agkw->second.end(); it_kwps++) {
                label_map[it_kem->first][*it_kwps].insert(agent_to_label.at(it_agkw->first));
                Vertex[index_map[it_kem->first]].ne++;
            }
        }
	}

	/*for (it_keps = m_edges.begin(); it_keps != m_edges.end(); it_keps++) {

		//DEBUG:Change this
		// if (adj_list[it_keps->get_from()][it_keps->get_to()].empty())
		// {
		// 	edge_counter[it_keps->get_from()]++;
		// }

		label_map[it_keps->get_from()][it_keps->get_to()].insert(agent_to_label.at(it_keps->get_label()));
		Vertex[index_map[it_keps->get_from()]].ne++;
	}*/

	i = 0;
	for (it_kwps = m_worlds.begin(); it_kwps != m_worlds.end(); it_kwps++) {
		Vertex[i].ne++; //Self loop bisimulation
		Vertex[i].e = (e_elem *) malloc(sizeof(e_elem) * Vertex[i].ne);
		i++;
	}

	//std::cerr << "\nDEBUG: Fine Inizializzazione Vertex\n";


	int from, to, j = 0; //, k = 0, nbh;

	//std::cerr << "\nDEBUG: Inizializzazione Mappa\n";
	for (it_klm = label_map.begin(); it_klm != label_map.end(); it_klm++) {
		from = index_map[it_klm->first]; // For each kworld 'from'

		//std::cerr << "\nDEBUG: Inizializzazione K\n";

		for (it_kw_bislab = it_klm->second.begin(); it_kw_bislab != it_klm->second.end(); it_kw_bislab++) { // For each edge that reaches the kworld 'to'
			to = index_map[it_kw_bislab->first];
			//nbh = it_kw_bislab->second.size();

			for (it_bislab = it_kw_bislab->second.begin(); it_bislab != it_kw_bislab->second.end(); it_bislab++) { // For each agent 'ag' in the label of the kedge
				//std::cerr << "\nDEBUG: j is: " << j << " and k is: " << k << "\n";
				//nbh = 1;
				Vertex[from].e[j].nbh = 1; // Let j be the index of the adjacency list of from that stores the kedge (from, to)
				Vertex[from].e[j].bh = (int *) malloc(sizeof(int)); // Let nbh be the number of agents in such kedge
				Vertex[from].e[j].tv = to; // Update the value of the reache kworld
				Vertex[from].e[j].bh[0] = *it_bislab; // Update the value of the label at index k to 'ag'
				//std::cerr << "\nDEBUG: j is: " << j << " and k is: " << k << "\n";

				j++; // Update the value of the index j
			}

			//std::cerr << "\nDEBUG J: " << j-1 << "\n";

			//k = 0; // Reset k
		}
		//std::cerr << "\nDEBUG: Fine Inizializzazione K\n";

		j = 0; // Reset j
	}

	//std::cerr << "\nDEBUG: Fine Inizializzazione Mappa\n";

	//		std::vector<kworld_ptr>::const_iterator it_kwp;
	//	int temp_counter = 0;
	//	for (it_kwp = kworld_vec.begin(); it_kwp != kworld_vec.end(); it_kwp++) {
	//		//std::cerr << "DEBUG: World " << temp_counter << " has ID: " << it_kwp->get_numerical_id() << std::endl;
	//		temp_counter++;
	//	}
	//
	// Building the automaton
	unsigned long Nbehavs = bhtabSize;
	a = (automaton *) malloc(sizeof(automaton));
	a->Nvertex = Nvertex;
	a->Nbehavs = Nbehavs;
	a->Vertex = Vertex;

	//	std::cerr << "\nDEBUG: \n\tNvertex = " << Nvertex << std::endl;
	//	std::cerr << "\tNbehavs = " << Nbehavs << std::endl;

	return *a;
}

void kstate::automaton_to_kstate(const automaton & a, const std::vector<kworld_ptr> & kworld_vec, const std::map<bis_label, agent> & label_to_agent)
{
	kworld_ptr_set worlds;
	kedge_map edges;
	// The pointed world does not change when we calculate the minimum bisimilar state
	// Hence we do not need to update it

	unsigned long agents_size = domain::get_instance().get_agents().size();
	int i, j, k, label;

	for (i = 0; i < a.Nvertex; i++) {
		if (a.Vertex[i].ne > 0) {
			worlds.insert(kworld_vec[i]);

			for (j = 0; j < a.Vertex[i].ne; j++) {
				for (k = 0; k < a.Vertex[i].e[j].nbh; k++) {
					label = a.Vertex[i].e[j].bh[k];
					if (label < agents_size) {
						edges[kworld_vec[i]][label_to_agent.at(label)].insert(kworld_vec[a.Vertex[i].e[j].tv]);
//						edges.insert(kstore::get_instance().add_edge(kedge(kworld_vec[i], kworld_vec[a.Vertex[i].e[j].tv], label_to_agent.at(label))));
					}
				}
			}
		}
	}

	set_worlds(worlds);
	set_edges(edges);
}

void kstate::calc_min_bisimilar()
{

	// ************* Cleaning unreachable kworlds *************
	//DEBUG_add_extra_world();

	
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

	clean_unreachable_kworlds();

	//std::cerr << "\nDEBUG: INIZIO BISIMULATION IN KSTATE\n" << std::flush;
	std::vector<kworld_ptr> kworld_vec; // Vector of all kworld_ptr
	//std::cerr << "\nDEBUG: PRE-ALLOCAZIONE AUTOMA\n" << std::flush;


	//	std::cerr << "\nDEBUG: \n\tNvertex_before = " << m_worlds.size() << std::endl;
	//	std::cerr << "\tNbehavs_before = " << m_edges.size() << std::endl;

	automaton a;
	kworld_vec.reserve(get_worlds().size());

	a = kstate_to_automaton(/*adj_list,*/ kworld_vec, agent_to_label);

	bisimulation b;
	//std::cout << "\nDEBUG: Printing automaton pre-Bisimulation\n";
	//b.VisAutoma(&a);



	if (domain::get_instance().get_bisimulation() == PaigeTarjan) {
		if (b.MinimizeAutomaPT(&a)) {
			//VisAutoma(a);

			//std::cout << "\nDEBUG: Printing automaton post-Bisimulation\n";
			//b.VisAutoma(&a);
			//std::cout << "Done\n";
			automaton_to_kstate(a, kworld_vec,label_to_agent);

			//b.DisposeAutoma(&a);
		}
	} else {
		if (b.MinimizeAutomaFB(&a)) {

			//std::cerr << "\nDEBUG: Printing automaton post-Bisimulation\n";
			//b.VisAutoma(&a);
			//std::cerr << "Done\n";
			automaton_to_kstate(a, kworld_vec,label_to_agent);
			//b.DisposeAutoma(&a);
		}
	}

	//std::cerr << "\nDEBUG: PRe Clean" << std::endl;

}


/**** INITIAL STATE ****/
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
		initially ini_conditions = domain::get_instance().get_initial_description();

	generate_initial_kworlds(permutation, 0, ini_conditions.get_initially_known_fluents());


	/*Building of all the consistent \ref kedge.*/
	generate_initial_kedges();
}

/*From https://www.geeksforgeeks.org/generate-all-the-binary-strings-of-n-bits/ since is like generating all the binary numbers.*/
void kstate::generate_initial_kworlds(fluent_set& permutation, int index, const fluent_set & initially_known)
{
		int fluent_number = domain::get_instance().get_fluent_number();

	int bit_size = (domain::get_instance().get_size_fluent());

	if (index == fluent_number) {
		kworld to_add(permutation);
		add_initial_kworld(to_add);

		return;
	}

	fluent_set permutation_2 = permutation;
	//Add the \ref fluent in positive version
	boost::dynamic_bitset<> bitSetToFindPositve(bit_size, index);
	boost::dynamic_bitset<> bitSetToFindNegative(bit_size, index);
	bitSetToFindNegative.set(bitSetToFindPositve.size() - 1, 1);
	bitSetToFindPositve.set(bitSetToFindPositve.size() - 1, 0);


	if (initially_known.find(bitSetToFindNegative) == initially_known.end()) {
		permutation.insert(bitSetToFindPositve);
		generate_initial_kworlds(permutation, index + 1, initially_known);
	}
	if (initially_known.find(bitSetToFindPositve) == initially_known.end()) {
		permutation_2.insert(bitSetToFindNegative);
		generate_initial_kworlds(permutation_2, index + 1, initially_known);
	}

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
		std::cerr << "\nNot implemented yet - 1\n";
		exit(1);
		break;
	}
	case NONE:
	{
		std::cerr << "\nNot implemented yet - 2\n";
		exit(1);
		break;
	}
	default:
	{
		std::cerr << "\nNot implemented yet - 3\n";
		exit(1);

		break;
	}
	}
}

void kstate::generate_initial_kedges()
{
	kworld_ptr_set::const_iterator it_kwps_1, it_kwps_2;
	auto agents = domain::get_instance().get_agents();
	kworld_ptr kwptr_tmp1, kwptr_tmp2;

	/*This for add to *this* all the possible edges.*/
	for (it_kwps_1 = m_worlds.begin(); it_kwps_1 != m_worlds.end(); it_kwps_1++) {
		for (it_kwps_2 = it_kwps_1; it_kwps_2 != m_worlds.end(); it_kwps_2++) {
			for (auto i = agents.begin(); i != agents.end(); i++) {
				kwptr_tmp1 = *it_kwps_1;
				kwptr_tmp2 = *it_kwps_2;

				add_edge(kwptr_tmp1, kwptr_tmp2, *i);
				add_edge(kwptr_tmp2, kwptr_tmp1, *i);
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
				remove_kedge(kwptr_tmp1, kwptr_tmp2, ag);
				remove_kedge(kwptr_tmp2, kwptr_tmp1, ag);
			} else if (kwptr_tmp2.get_ptr()->entails(known_ff) && !kwptr_tmp1.get_ptr()->entails(known_ff)) {

				remove_kedge(kwptr_tmp2, kwptr_tmp1, ag);
				remove_kedge(kwptr_tmp1, kwptr_tmp2, ag);
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
					helper::check_Bff_notBff(tmp.get_bf1(), tmp.get_bf2(), known_ff_ptr);
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
		std::cerr << "\nNot implemented yet - 4\n";
		exit(1);
		break;
	}
	case NONE:
	{
		std::cerr << "\nNot implemented yet - 5\n";
		exit(1);
		break;
	}
	default:
	{
		std::cerr << "\nNot implemented yet - 6\n";
		exit(1);

		break;
	}
	}
}


/**** TRANSITION FUNCTION ****/
kstate kstate::compute_succ(const action & act) const
{
    /** \warning executability should be check in \ref state (or \ref planner).*/
	return union_update::u_update(*this, act);
}


/**** UTILITIES ****/
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

//	kedge_ptr_set::const_iterator it_keset;
	kedge_map::const_iterator it_kem;
	std::map<agent, kworld_ptr_set>::const_iterator it_agkw;

	std::cout << "Edge List:" << std::endl;

    for (it_kem = m_edges.begin(); it_kem != m_edges.end(); it_kem++) {
        for (it_agkw = it_kem->second.begin(); it_agkw != it_kem->second.end(); it_agkw++) {
            for (it_kwset = it_agkw->second.begin(); it_kwset != it_agkw->second.end(); it_kwset++) {
                std::cout << "E-" << counter << ": (";
                printer::get_instance().print_list(it_kem->first.get_fluent_set());
                std::cout << "," << it_kem->first.get_repetition();
                std::cout << ") - (";
                printer::get_instance().print_list(it_kwset->get_fluent_set());
                std::cout << "," << it_kwset->get_repetition();
                std::cout << ") ag:" << domain::get_instance().get_grounder().deground_agent(it_agkw->first);
                std::cout << std::endl;
                counter++;
            }
        }
    }

	/*for (it_keset = get_edges().begin(); it_keset != get_edges().end(); it_keset++) {

		std::cout << "E-" << counter << ": (";
		printer::get_instance().print_list(it_keset->get_from().get_fluent_set());
		std::cout << "," << it_keset->get_from().get_repetition();
		std::cout << ") - (";
		printer::get_instance().print_list(it_keset->get_to().get_fluent_set());
		std::cout << "," << it_keset->get_to().get_repetition();
		std::cout << ") ag:" << domain::get_instance().get_grounder().deground_agent(it_keset->get_label());
		std::cout << std::endl;
		counter++;

	}*/
	std::cout << "*******************************************************************" << std::endl;
}

void kstate::print_graphviz(std::ostream & graphviz) const
{
	string_set::const_iterator it_st_set;
	fluent_set::const_iterator it_fs;


	graphviz << "//WORLDS List:" << std::endl;

	kworld_ptr_set::const_iterator it_kpset;
	std::map<fluent_set, int> map_world_to_index;
	std::map<unsigned short, char> map_rep_to_name;
	char found_rep = (char) ((char) domain::get_instance().get_agents().size() + 'A');
	int found_fs = 0;
	fluent_set tmp_fs;
	char tmp_unsh;
	string_set tmp_stset;
	bool print_first;
	kworld_ptr_set::const_iterator it_kwset;
	for (it_kwset = get_worlds().begin(); it_kwset != get_worlds().end(); it_kwset++) {
		if (*it_kwset == get_pointed())
			graphviz << "	node [shape = doublecircle] ";
		else
			graphviz << "	node [shape = circle] ";

		print_first = false;
		tmp_fs = it_kwset->get_fluent_set();
		if (map_world_to_index.count(tmp_fs) == 0) {
			map_world_to_index[tmp_fs] = found_fs;
			found_fs++;
		}
		tmp_unsh = it_kwset->get_repetition();
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

	std::map<int, kworld_ptr_set> for_rank_print;
	for (it_kwset = get_worlds().begin(); it_kwset != get_worlds().end(); it_kwset++) {
		for_rank_print[it_kwset->get_repetition()].insert(*it_kwset);
	}

	std::map<int, kworld_ptr_set>::const_iterator it_map_rank;
	for (it_map_rank = for_rank_print.begin(); it_map_rank != for_rank_print.end(); it_map_rank++) {
		graphviz << "	{rank = same; ";
		for (it_kwset = it_map_rank->second.begin(); it_kwset != it_map_rank->second.end(); it_kwset++) {
			graphviz << "\"" << map_rep_to_name[it_kwset->get_repetition()] << "_" << map_world_to_index[it_kwset->get_fluent_set()] << "\"; ";
		}
		graphviz << "}\n";
	}


	graphviz << "\n\n";
	graphviz << "//EDGES List:" << std::endl;

	std::map < std::tuple<std::string, std::string>, std::set<std::string> > edges;

//	kedge_ptr_set::const_iterator it_keset;
	std::tuple<std::string, std::string> tmp_tuple;
	std::string tmp_string;

    kedge_map::const_iterator it_kem;
    std::map<agent, kworld_ptr_set>::const_iterator it_agkw;

    for (it_kem = m_edges.begin(); it_kem != m_edges.end(); it_kem++) {
        for (it_agkw = it_kem->second.begin(); it_agkw != it_kem->second.end(); it_agkw++) {
            for (it_kwset = it_agkw->second.begin(); it_kwset != it_agkw->second.end(); it_kwset++) {
                tmp_string = "_" + std::to_string(map_world_to_index[it_kem->first.get_fluent_set()]);
                tmp_string.insert(0, 1, map_rep_to_name[it_kem->first.get_repetition()]);
                std::get<0>(tmp_tuple) = tmp_string;

                tmp_string = "_" + std::to_string(map_world_to_index[it_kwset->get_fluent_set()]);
                tmp_string.insert(0, 1, map_rep_to_name[it_kwset->get_repetition()]);
                std::get<1>(tmp_tuple) = tmp_string;

                edges[tmp_tuple].insert(domain::get_instance().get_grounder().deground_agent(it_agkw->first));
            }
        }
    }

	/*for (it_keset = get_edges().begin(); it_keset != get_edges().end(); it_keset++) {
		tmp_string = "_" + std::to_string(map_world_to_index[it_keset->get_from().get_fluent_set()]);
		tmp_string.insert(0, 1, map_rep_to_name[it_keset->get_from().get_repetition()]);
		std::get<0>(tmp_tuple) = tmp_string;

		tmp_string = "_" + std::to_string(map_world_to_index[it_keset->get_to().get_fluent_set()]);
		tmp_string.insert(0, 1, map_rep_to_name[it_keset->get_to().get_repetition()]);
		std::get<1>(tmp_tuple) = tmp_string;

		edges[tmp_tuple].insert(domain::get_instance().get_grounder().deground_agent(it_keset->get_label()));
	}*/


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
	for (it_kwset = get_worlds().begin(); it_kwset != get_worlds().end(); it_kwset++) {
		tmp_fs = it_kwset->get_fluent_set();
		print_first = false;
		graphviz << "		<tr><td>" << map_rep_to_name[it_kwset->get_repetition()] << "_" << map_world_to_index[tmp_fs] << "</td> <td>";
		for (it_fs = tmp_fs.begin(); it_fs != tmp_fs.end(); it_fs++) {
			if (print_first) {
				graphviz << ", ";
			}
			print_first = true;
			if(!helper::is_negate(*it_fs)) color = "<font color=\"#0000ff\"> ";
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


//void kstate::DEBUG_add_extra_world()
//{
//
//	kworld_ptr tmp_ptr = *m_worlds.begin();
//	kworld extra(tmp_ptr.get_fluent_set());
//	unsigned short depth = tmp_ptr.get_repetition() + 4;
//	//std::cout << "\n\nDEBUG: Depth: " << depth << "\n\n";
//	kworld_ptr extra_ptr = add_rep_world(extra, depth);
//	set_max_depth(get_max_depth() + depth);
//
//
//
//
//	kedge_ptr_set::const_iterator it_kep;
//	for (it_kep = m_edges.begin(); it_kep != m_edges.end(); it_kep++) {
//		if (it_kep->get_from() == tmp_ptr && !(it_kep->get_to() == tmp_ptr)) {
//			kedge extra_ed(extra_ptr, it_kep->get_to(), it_kep->get_label());
//			add_edge(extra_ed);
//		} else if (it_kep->get_to() == tmp_ptr && !(it_kep->get_from() == tmp_ptr)) {
//			kedge extra_ed(it_kep->get_from(), extra_ptr, it_kep->get_label());
//			add_edge(extra_ed);
//		} else if (it_kep->get_to() == tmp_ptr && it_kep->get_from() == tmp_ptr) {
//			kedge extra_ed(extra_ptr, extra_ptr, it_kep->get_label());
//			add_edge(extra_ed);
//		}
//	}
//
//}








//DEBUG

//void kstate::debug_print(const kstate & to_compare)
//{
//
//	bisimulation b;
//	std::vector<kworld_ptr> kworld_vec;
//	automaton a;
//
//	//std::cerr << "\nDEBUG:HERE\n";
//
//
//	if (m_edges < to_compare.get_edges()) {
//		//std::cerr << "\nDEBUG:HERE 1\n";
//
//		a = b.compare_automata_debug(*this, to_compare, kworld_vec);
//	} else {
//		//std::cerr << "\nDEBUG:HERE 1\n";
//
//		a = b.compare_automata_debug(to_compare, *this, kworld_vec);
//	}
//	automaton_to_kstate_debug(a, kworld_vec);
//}
//
//void kstate::automaton_to_kstate_debug(const automaton & a, std::vector<kworld_ptr> & kworld_vec)
//{
//	kworld_ptr_set worlds;
//	kedge_ptr_set edges;
//	// The pointed world does not change when we calculate the minimum bisimilar state
//	// Hence we do not need to update it
//
//	int i, j, k, label, agents_size = domain::get_instance().get_agents().size();
//
//	for (i = 0; i < a.Nvertex; i++) {
//		if (a.Vertex[i].ne > 0) {
//			kworld_vec[i].set_repetition(i);
//			std::cerr << "\nDEBUG: Analyzing the " << i << "^th world with num_id = " << kworld_vec[i].get_numerical_id() << " and repetition = " << kworld_vec[i].get_repetition() << std::endl;
//			worlds.insert(kworld_vec[i]);
//
//			for (j = 0; j < a.Vertex[i].ne; j++) {
//				for (k = 0; k < a.Vertex[i].e[j].nbh; k++) {
//					label = a.Vertex[i].e[j].bh[k];
//					if (label < agents_size) {
//						
//							kedge_ptr tmp_edge = kstore::get_instance().add_edge(kedge(kworld_vec[i], kworld_vec[a.Vertex[i].e[j].tv], label));
//							edges.insert(tmp_edge);
//							//tmp_edge.print();
//							//std::cerr << "\n";
//						
//					}
//				}
//			}
//		}
//
//	}
//
//	set_pointed(*worlds.begin());
//	
//	set_worlds(worlds);
//	set_edges(edges);
//}