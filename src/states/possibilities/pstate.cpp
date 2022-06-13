/**
 * \brief Implementation of \ref pstate.h
 *
 * \copyright GNU Public License.
 *
 * \author Francesco Fabiano, Alessandro Burigana.
 * \date September 16, 2019
 */

#include <iostream>

#include "pstate.h"
#include "../../domain/domain.h"

pstate::pstate() = default;

pstate::pstate(const domain &domain, const finitary_theory &theory) {
    generate_from_theory(domain, theory);
}

pstate::pstate(const pstate *previous, const action *action) {
    update(previous, action);
}

/**** GETTERS ****/
const pworld_set &pstate::get_worlds() const {
    return m_worlds;
}

const pedge_map &pstate::get_edges() const {
    return m_edges;
}

const pworld *pstate::get_pointed() const {
    return m_pointed;
}

bool pstate::operator<(const pstate &to_compare) const {
    return false;
}

bool pstate::entails(const formula &formula, const pworld &world) const {
    return false;
//    return formula.is_entailed(this, world);
}

void pstate::get_all_reachable_worlds_edges(const pworld *world, pworld_set &reached_worlds, pedge_map &reached_edges) const {
    // todo: TEST
    auto adjacent_worlds = m_edges.at(world);
    pworld_set kw_list;

    std::map<const agent*, pworld_set>::const_iterator it_kwmap;
    pworld_set::const_iterator it_kws;

    // todo: SISTEMA
//    for (it_kwmap = adjacent_worlds.begin(); it_kwmap != adjacent_worlds.end(); it_kwmap++) {
//        kw_list = it_kwmap->second;
//
//        for (it_kws = kw_list.begin(); it_kws != kw_list.end(); it_kws++) {
//            if (reached_worlds.insert(*it_kws).second) {
//                reached_edges[*it_kws] = m_edges.at(*it_kws);
//                get_all_reachable_worlds_edges(*it_kws, reached_worlds, reached_edges);
//            }
//        }
//    }
}

void pstate::clean_unreachable_pworlds() {
    pworld_set reached_worlds;
    pedge_map reached_edges;

    reached_worlds.insert(m_pointed);
    reached_edges[m_pointed] = m_edges.at(m_pointed);

    get_all_reachable_worlds_edges(m_pointed, reached_worlds, reached_edges);

//    set_worlds(reached_worlds);
//    set_edges(reached_edges);
}

/**** INITIAL STATE ****/
void pstate::generate_from_theory(const domain &domain, const finitary_theory &theory) {
    generate_initial_pointed(theory);
    generate_initial_worlds(domain, theory);
    generate_initial_edges(domain, theory);
}

void pstate::generate_initial_pointed(const finitary_theory &theory) {
    fluent_ptr_set pointed_fluent_set;

    for (const auto f : theory.get_pointed_fluents()) {
        pointed_fluent_set.insert(f);
    }

    pworld tmp = pworld(&pointed_fluent_set, nullptr, 0);
    // todo: utilizza store per ottenere il puntatore
    m_pointed = &tmp;
    m_worlds.insert(m_pointed);
}

// Power set generation: https://rosettacode.org/wiki/Power_set#C.2B.2B
template<class S>
void pstate::power_set(S &set, std::set<S> &power_set) {
    power_set.emplace();

    for (auto&& item : set) {
        std::set<S> subset;

        for (auto s : power_set) {
            s.insert(item);
            subset.insert(s);
        }
        power_set.insert(subset.begin(), subset.end());
    }
}

void pstate::generate_initial_fluent_sets(const finitary_theory &theory, std::set<fluent_ptr_set> &initial_fluent_sets) {
    auto form_it = theory.get_ck_formulae().begin();
    bool is_consistent = true;

    // We add the set of initially known fluents to each combination of initially unknown fluents
    // And we create a world for each resulting fluent set
    for (auto fps : initial_fluent_sets) {
        fps.insert(theory.get_ck_fluents().begin(), theory.get_ck_fluents().end());
        // todo: implementa set di fluent set (store); probabilmente va messo in domain

        // We check that the current fluent set is consistent with the fluent formulae that are
        // Commonly known by all agents according to the theory
        while (is_consistent && form_it != theory.get_ck_formulae().end()) {
            if (!(*form_it)->is_entailed(&fps)) {
                is_consistent = false;
            }
            ++form_it;
        }

        if (is_consistent) {
            pworld tmp(&fps, nullptr);
            // todo: utilizza store per ottenere il puntatore
            m_worlds.insert(&tmp);
            // todo: inserisco due volte il pointed?
        }

        form_it = theory.get_ck_formulae().begin();
        is_consistent = true;
    }
}

void pstate::generate_initial_worlds(const domain &domain, const finitary_theory &theory) {
    fluent_ptr_set initially_unknown_fluents;
    std::set<fluent_ptr_set> initial_fluent_sets;

    /// The field \ref m_ck_fluents represents the set of fluents that are common knowledge in the initial state
    std::set_difference(domain.get_fluent_set().begin(), domain.get_fluent_set().end(),
                        domain.get_finitary_theory().get_ck_fluents().begin(), domain.get_finitary_theory().get_ck_fluents().end(),
                        std::inserter(initially_unknown_fluents, initially_unknown_fluents.end()));

    // We generate all the combinations of fluents that are not known initially
    // This is equivalent to generating the power set of initially_unknown_fluents
    power_set(initially_unknown_fluents, initial_fluent_sets);
    generate_initial_fluent_sets(theory, initial_fluent_sets);
}

void pstate::calculate_no_good_edges(const finitary_theory &theory, pedge_map &no_goods) {
    std::map<const agent*, pworld_set> ng_ag_worlds;
    pworld_set ng_ws;

    for (const auto w1 : m_worlds) {
        ng_ag_worlds = no_goods[w1];

        for (const auto knows_whether : theory.get_knows_whether()) {
            assert(knows_whether->get_formula()->is_propositional());
            ng_ws = ng_ag_worlds[knows_whether->get_agent()];

            for (const auto w2 : m_worlds) {
                // We insert an edge (w1 --ag--> w2) in the map of no-goods iff w1 and w2 disagree on the formula
                if (knows_whether->get_formula()->is_entailed(w1->get_fluent_set()) !=
                    knows_whether->get_formula()->is_entailed(w2->get_fluent_set())) {
                    ng_ws.insert(w2);
                }
            }
        }
    }
}

void pstate::filter_no_good_edges(const domain &domain, pedge_map &no_goods) {
    std::map<const agent*, pworld_set> ag_worlds, ng_ag_worlds;
    pworld_set ws, ng_ws;

    for (const auto w1 : m_worlds) {
        ag_worlds = m_edges[w1];
        ng_ag_worlds = no_goods[w1];

        for (const auto& ag : domain.get_agent_set()) {
//            ws = ag_worlds[ag];
            ng_ws = ng_ag_worlds[ag];

            // We store only edges that are *not* in the no-goods map
//            std::set_difference(m_worlds.begin(), m_worlds.end(), ng_ws.begin(), ng_ws.end(), ag_worlds[ag].begin());
            // todo: SISTEMA
        }
    }
}

void pstate::generate_initial_edges(const domain &domain, const finitary_theory &theory) {
    // todo: implementa classe template 'store' per collezionare elementi in un set (world, fluent_set, formule...)
    pedge_map no_goods;

    // In 'no_good' we keep track of what edges we don't want to include
    calculate_no_good_edges(theory, no_goods);
    filter_no_good_edges(domain, no_goods);
}

/**** TRANSITION FUNCTION ****/
void pstate::update(const pstate *previous, const action *action) {
    // todo: implement
}

//pstate pstate::compute_succ(const action & act) const {
//    /** \warning executability should be check in \ref state (or \ref planner).*/
//    pstate tmp;
//    return tmp;
//    //return product_update::update(*this, act);
//}

/**** BISIMULATION ****/
automaton pstate::pstate_to_automaton(std::vector<pworld *> &pworld_vec, const std::map<agent, bis_label> & agent_to_label) const {
//	std::map<int, int> compact_indices;
//	std::map<pworld_ptr, int> index_map;
//	pbislabel_map label_map; // Map: from -> (to -> ag_set)

	automaton *a;
//	unsigned long Nvertex = get_worlds().size();
//    // \todo: SISTEMA
//	unsigned long ag_set_size = 0; // domain.get_agents().size();
//	//BIS_ADAPTATION For the loop that identifies the id (We add one edge for each node)
//	v_elem *Vertex;
//
//	Vertex = (v_elem *) malloc(sizeof(v_elem) * Nvertex);
//
//	// Initializating vertices
//	pworld_set::const_iterator it_pwps;
//	pedge_map::const_iterator it_peps;
//	pbislabel_map::const_iterator it_plm;
//	bis_label_set::const_iterator it_bislab;
//	std::map<pworld_ptr, bis_label_set>::const_iterator it_pw_bislab;
//
//	//std::cerr << "\nDEBUG: Inizializzazione Edges\n";
//
//	// The pointed world is set to the index 0. This ensures that, when deleting the bisimilar nodes, the pointed pworld
//	// is always chosen as the first of its block. Therefore, we do not need to update it when converting back to a pstate
//	index_map[get_pointed()] = 0;
//	pworld_vec.push_back(get_pointed());
//	compact_indices[static_cast<int> (get_pointed().get_numerical_id())] = 0;
//
//	//For the loop that identifies the id
//	//BIS_ADAPTATION For the loop that identifies the id (+1)
//	///@bug: If the pointed has no self-loop to add
//	//pworld_ptr_set pointed_adj = adj_list.at(get_pointed());
//
//	Vertex[0].ne = 0; // pointed_adj.size(); // edge_counter[get_pointed()];
//	//	if (pointed_adj.find(get_pointed()) == pointed_adj.end()) {
//	//		Vertex[0].ne++;
//	//	}
//	//	Vertex[0].e = (e_elem *) malloc(sizeof(e_elem) * Vertex[0].ne);
//
//	int i = 1, c = 1;
//
//	//std::cerr << "\nDEBUG: Inizializzazione Vertex\n";
//
//	for (it_pwps = m_worlds.begin(); it_pwps != m_worlds.end(); it_pwps++) {
//		if (!(*it_pwps == get_pointed())) {
//			index_map[*it_pwps] = i;
//			pworld_vec.push_back(*it_pwps);
//
//			// if (compact_indices.find(it_pwps->get_numerical_id()) == compact_indices.end()) {
//			if (compact_indices.insert({it_pwps->get_numerical_id(), c}).second) {
//				// compact_indices[it_pwps->get_numerical_id()] = c;
//				c++;
//				//std::cerr << "\nDEBUG: Added:" << it_pwps->get_id() << "\n";
//			}
//			Vertex[i].ne = 0;
//			i++;
//		}
//		//BIS_ADAPTATION (Added self-loop)
//		label_map[*it_pwps][*it_pwps].insert(compact_indices[static_cast<int> (it_pwps->get_numerical_id())] + ag_set_size);
//		//std::cerr << "\nDEBUG: Added to " << it_pwps->get_numerical_id() << " the label " << compact_indices[it_pwps->get_numerical_id()] + ag_set_size << std::endl;
//	}
//
//
//	//BIS_ADAPTATION For the loop that identifies the id (We add one potential label for each node)
//	unsigned long bhtabSize = ag_set_size + c;
//
//	//std::cerr << "\nDEBUG: Inizializzazione Behavs\n";
//
//	//BIS_ADAPTATION (Moved down here)
//
//	for (it_peps = m_beliefs.begin(); it_peps != m_beliefs.end(); it_peps++) {
//
//		for (auto it_mid_bel = it_peps->second.begin(); it_mid_bel != it_peps->second.end(); it_mid_bel++) {
//			for (auto it_int_ed = it_mid_bel->second.begin(); it_int_ed != it_mid_bel->second.end(); it_int_ed++) {
//				label_map[it_peps->first][*it_int_ed].insert(agent_to_label.at(it_mid_bel->first));
//				Vertex[index_map[it_peps->first]].ne++;
//			}
//		}
//	}
//
//	i = 0;
//	for (it_pwps = m_worlds.begin(); it_pwps != m_worlds.end(); it_pwps++) {
//		Vertex[i].ne++; //Self loop bisimulation
//		Vertex[i].e = (e_elem *) malloc(sizeof(e_elem) * Vertex[i].ne);
//		i++;
//	}
//
//	//std::cerr << "\nDEBUG: Fine Inizializzazione Vertex\n";
//
//
//	int from, to, j = 0; //, k = 0, nbh;
//
//	//std::cerr << "\nDEBUG: Inizializzazione Mappa\n";
//	for (it_plm = label_map.begin(); it_plm != label_map.end(); it_plm++) {
//		from = index_map[it_plm->first]; // For each pworld 'from'
//
//		//std::cerr << "\nDEBUG: Inizializzazione K\n";
//
//		for (it_pw_bislab = it_plm->second.begin(); it_pw_bislab != it_plm->second.end(); it_pw_bislab++) { // For each edge that reaches the pworld 'to'
//			to = index_map[it_pw_bislab->first];
//			//nbh = it_pw_bislab->second.size();
//
//			for (it_bislab = it_pw_bislab->second.begin(); it_bislab != it_pw_bislab->second.end(); it_bislab++) { // For each agent 'ag' in the label of the pedge
//				//std::cerr << "\nDEBUG: j is: " << j << " and k is: " << k << "\n";
//				//nbh = 1;
//				Vertex[from].e[j].nbh = 1; // Let j be the index of the adjacency list of from that stores the pedge (from, to)
//				Vertex[from].e[j].bh = (int *) malloc(sizeof(int)); // Let nbh be the number of agents in such pedge
//				Vertex[from].e[j].tv = to; // Update the value of the reache pworld
//				Vertex[from].e[j].bh[0] = *it_bislab; // Update the value of the label at index k to 'ag'
//				//std::cerr << "\nDEBUG: j is: " << j << " and k is: " << k << "\n";
//
//				j++; // Update the value of the index j
//			}
//
//
//		}
//
//		j = 0; // Reset j
//	}
//
//	unsigned long Nbehavs = bhtabSize;
//	a = (automaton *) malloc(sizeof(automaton));
//	a->Nvertex = Nvertex;
//	a->Nbehavs = Nbehavs;
//	a->Vertex = Vertex;

	//	std::cerr << "\nDEBUG: \n\tNvertex = " << Nvertex << std::endl;
	//	std::cerr << "\tNbehavs = " << Nbehavs << std::endl;

	return *a;
}

void pstate::automaton_to_pstate(const automaton & a, const std::vector<pworld *> &pworld_vec, const std::map<bis_label, agent> & label_to_agent) {
//	pworld_set worlds;
//	m_beliefs.clear();
//	// The pointed world does not change when we calculate the minimum bisimilar state
//	// Hence we do not need to update it
//
//	int i, j, k, label;
//    // \todo: SISTEMA
//	unsigned long agents_size = 0; // domain.get_agents().size();
//
//	for (i = 0; i < a.Nvertex; i++) {
//		if (a.Vertex[i].ne > 0) {
//			worlds.insert(pworld_vec[i]);
//			for (j = 0; j < a.Vertex[i].ne; j++) {
//				for (k = 0; k < a.Vertex[i].e[j].nbh; k++) {
//					label = a.Vertex[i].e[j].bh[k];
//					if (label < agents_size) {
//						add_edge(pworld_vec[i], pworld_vec[a.Vertex[i].e[j].tv], label_to_agent.at(label));
//					}
//				}
//			}
//		}
//	}
//
//	set_worlds(worlds);
}

void pstate::calc_min_bisimilar(Bisimulation_Algorithm algorithm) {
	//std::cerr << "\nDEBUG: INIZIO BISIMULATION IN PSTATE\n" << std::flush;

	// ************* Cleaning unreachable pworlds *************

	clean_unreachable_pworlds();

	std::vector<pworld*> pworld_vec; // Vector of all pworld_ptr
	//std::cerr << "\nDEBUG: PRE-ALLOCAZIONE AUTOMA\n" << std::flush;


	//	std::cerr << "\nDEBUG: \n\tNvertex_before = " << m_worlds.size() << std::endl;
	//	std::cerr << "\tNbehavs_before = " << m_edges.size() << std::endl;

	automaton a;
	pworld_vec.reserve(get_worlds().size());

	std::map<bis_label, agent> label_to_agent;
	std::map<agent, bis_label> agent_to_label;

    /**
     * \todo: SISTEMARE QUESTO BLOCCO DI CODICE COMMENTATO!!!
     *        SENZA DI QUESTO BISIMULATION _NON_ FUNZIONA!!!
     */
//	auto agents = domain.get_agents();
//	auto it_ag = agents.begin();
//	bis_label ag_label = 0;
//	agent lab_agent;
//	for (; it_ag != agents.end(); it_ag++) {
//		lab_agent = *it_ag;
//		label_to_agent.insert(std::make_pair(ag_label, lab_agent));
//		agent_to_label.insert(std::make_pair(lab_agent, ag_label));
//		ag_label++;
//	}

	a = pstate_to_automaton(pworld_vec, agent_to_label);

	bisimulation b;
	//std::cout << "\nDEBUG: Printing automaton pre-Bisimulation\n";
	//b.VisAutoma(&a);

	if (algorithm == Bisimulation_Algorithm::PAIGE_TARJAN) {
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
void pstate::print() const {
//	int counter = 1;
//	std::cout << std::endl;
//	std::cout << "The Pointed World has id ";
//	printer::get_instance().print_list(get_pointed().get_fluent_set());
//	std::cout << "-" << get_pointed().get_repetition();
//	std::cout << std::endl;
//	std::cout << "*******************************************************************" << std::endl;
//
//	pworld_ptr_set::const_iterator it_pwset;
//	std::cout << "World List:" << std::endl;
//
//	for (it_pwset = get_worlds().begin(); it_pwset != get_worlds().end(); it_pwset++) {
//		std::cout << "W-" << counter << ": ";
//		printer::get_instance().print_list(it_pwset->get_fluent_set());
//		std::cout << " rep:" << it_pwset->get_repetition();
//		std::cout << std::endl;
//		counter++;
//	}
//	counter = 1;
//	std::cout << std::endl;
//	std::cout << "*******************************************************************" << std::endl;
//	pedges::const_iterator it_pwtm;
//	pworld_map::const_iterator it_pwm;
//	std::cout << "Edge List:" << std::endl;
//	for (it_pwtm = get_beliefs().begin(); it_pwtm != get_beliefs().end(); it_pwtm++) {
//		pworld_ptr from = it_pwtm->first;
//		pworld_map from_map = it_pwtm->second;
//
//		for (it_pwm = from_map.begin(); it_pwm != from_map.end(); it_pwm++) {
//			agent ag = it_pwm->first;
//			pworld_ptr_set to_set = it_pwm->second;
//
//			for (it_pwset = to_set.begin(); it_pwset != to_set.end(); it_pwset++) {
//
//				pworld_ptr to = *it_pwset;
//
//				std::cout << "E-" << counter << ": (";
//				printer::get_instance().print_list(from.get_fluent_set());
//				std::cout << "," << from.get_repetition();
//				std::cout << ") - (";
//				printer::get_instance().print_list(to.get_fluent_set());
//				std::cout << "," << to.get_repetition();
//				std::cout << ") ag:" << domain.get_grounder().deground_agent(ag);
//				std::cout << std::endl;
//				counter++;
//			}
//		}
//	}
//	std::cout << "*******************************************************************" << std::endl;
}

void pstate::print_graphviz(std::ostream & graphviz) const {
//	string_set::const_iterator it_st_set;
//	fluent_set::const_iterator it_fs;
//
//
//	graphviz << "//WORLDS List:" << std::endl;
//	std::map<fluent_set, int> map_world_to_index;
//	std::map<unsigned short, char> map_rep_to_name;
//	char found_rep = (char) ((char) domain.get_agents().size() + 'A');
//	int found_fs = 0;
//	fluent_set tmp_fs;
//	char tmp_unsh;
//	string_set tmp_stset;
//	bool print_first;
//	pworld_ptr_set::const_iterator it_pwset;
//	for (it_pwset = get_worlds().begin(); it_pwset != get_worlds().end(); it_pwset++) {
//		if (*it_pwset == get_pointed())
//			graphviz << "	node [shape = doublecircle] ";
//		else
//			graphviz << "	node [shape = circle] ";
//
//		print_first = false;
//		tmp_fs = it_pwset->get_fluent_set();
//		if (map_world_to_index.count(tmp_fs) == 0) {
//			map_world_to_index[tmp_fs] = found_fs;
//			found_fs++;
//		}
//		tmp_unsh = static_cast<char> (it_pwset->get_repetition());
//		if (map_rep_to_name.count(tmp_unsh) == 0) {
//			map_rep_to_name[tmp_unsh] = found_rep;
//			found_rep++;
//		}
//		graphviz << "\"" << map_rep_to_name[tmp_unsh] << "_" << map_world_to_index[tmp_fs] << "\";";
//		graphviz << "// (";
//		tmp_stset = domain.get_grounder().deground_fluent(tmp_fs);
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
//	for (it_pwset = get_worlds().begin(); it_pwset != get_worlds().end(); it_pwset++) {
//		for_rank_print[it_pwset->get_repetition()].insert(*it_pwset);
//	}
//
//	std::map<int, pworld_ptr_set>::const_iterator it_map_rank;
//	for (it_map_rank = for_rank_print.begin(); it_map_rank != for_rank_print.end(); it_map_rank++) {
//		graphviz << "	{rank = same; ";
//		for (it_pwset = it_map_rank->second.begin(); it_pwset != it_map_rank->second.end(); it_pwset++) {
//			graphviz << "\"" << map_rep_to_name[it_pwset->get_repetition()] << "_" << map_world_to_index[it_pwset->get_fluent_set()] << "\"; ";
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
//	pedges::const_iterator it_pwtm;
//	pworld_map::const_iterator it_pwm;
//	std::tuple<std::string, std::string> tmp_tuple;
//	std::string tmp_string;
//
//	for (it_pwtm = get_beliefs().begin(); it_pwtm != get_beliefs().end(); it_pwtm++) {
//		pworld_ptr from = it_pwtm->first;
//		pworld_map from_map = it_pwtm->second;
//
//		for (it_pwm = from_map.begin(); it_pwm != from_map.end(); it_pwm++) {
//			agent ag = it_pwm->first;
//			pworld_ptr_set to_set = it_pwm->second;
//
//			for (it_pwset = to_set.begin(); it_pwset != to_set.end(); it_pwset++) {
//				pworld_ptr to = *it_pwset;
//
//				tmp_string = "_" + std::to_string(map_world_to_index[from.get_fluent_set()]);
//				tmp_string.insert(0, 1, map_rep_to_name[from.get_repetition()]);
//				std::get<0>(tmp_tuple) = tmp_string;
//
//				tmp_string = "_" + std::to_string(map_world_to_index[to.get_fluent_set()]);
//				tmp_string.insert(0, 1, map_rep_to_name[to.get_repetition()]);
//				std::get<1>(tmp_tuple) = tmp_string;
//
//				edges[tmp_tuple].insert(domain.get_grounder().deground_agent(ag));
//
//			}
//		}
//	}
//
//	std::map < std::tuple<std::string, std::string>, std::set < std::string>>::iterator it_map;
//	std::map < std::tuple<std::string, std::string>, std::set < std::string>>::const_iterator it_map_2;
//
//	std::map < std::tuple<std::string, std::string>, std::set < std::string>> to_print_double;
//	for (it_map = edges.begin(); it_map != edges.end(); it_map++) {
//		for (it_map_2 = it_map; it_map_2 != edges.end(); it_map_2++) {
//			if (std::get<0>(it_map->first) == std::get<1>(it_map_2->first)) {
//				if (std::get<1>(it_map->first) == std::get<0>(it_map_2->first)) {
//					if (it_map->second == it_map_2->second) {
//						if (std::get<0>(it_map->first) != std::get<1>(it_map->first)) {
//							to_print_double[it_map->first] = it_map->second;
//							it_map_2 = edges.erase(it_map_2);
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
//	std::string color = "<font color=\"#ffffff\">";
//	graphviz << "\n\n//WORLDS description Table:" << std::endl;
//	graphviz << "	node [shape = plain]\n\n";
//	graphviz << "	description[label=<\n";
//	graphviz << "	<table border = \"0\" cellborder = \"1\" cellspacing = \"0\" >\n";
//	for (it_pwset = get_worlds().begin(); it_pwset != get_worlds().end(); it_pwset++) {
//		tmp_fs = it_pwset->get_fluent_set();
//		print_first = false;
//		graphviz << "		<tr><td>" << map_rep_to_name[it_pwset->get_repetition()] << "_" << map_world_to_index[tmp_fs] << "</td> <td>";
//		for (it_fs = tmp_fs.begin(); it_fs != tmp_fs.end(); it_fs++) {
//			if (print_first) {
//
//				graphviz << ", ";
//			}
//			print_first = true;
//			if (helper::is_negate(*it_fs)) color = "<font color=\"#0000ff\"> ";
//			else color = "<font color=\"#ff1020\">";
//			graphviz << color << domain.get_grounder().deground_fluent(*it_fs) << "</font>";
//		}
//		graphviz << "</td></tr>\n";
//	}
//	graphviz << "	</table>>]\n";
//	graphviz << "	{rank = max; description};\n";

}


/*******FOR MAL OPTIMIZATION******/
//
//agent_set pstate::get_agents_if_entailed(const mal_observability_map& map, const pworld_ptr & start) const
//{ /**\todo move to helper*/
//
//    agent_set ret;
//    for (auto it_map = map.begin(); it_map != map.end(); it_map++) {
//        if (entails(it_map->second, start)) {
//
//            ret.insert(it_map->first);
//        }
//    }
//    return ret;
//}
//
//pworld_ptr pstate::execute_ontic_helper(const action &act, pstate &ret, const pworld_ptr &current_pw, transition_map &calculated, agent_set & oblivious_obs_agents) const {
//    // Execute the all the effects
//    fluent_formula current_pw_effects = get_effects_if_entailed(act.get_effects(), current_pw);
//    fluent_set world_description = current_pw.get_fluent_set();
//    fluent_formula::const_iterator it_eff;
//
//    for (it_eff = current_pw_effects.begin(); it_eff != current_pw_effects.end(); it_eff++) {
//        world_description = helper::ontic_exec(*it_eff, world_description);
//    }
//
//    pworld_ptr new_pw = ret.add_rep_world(pworld(world_description), current_pw.get_repetition()); // We add the corresponding pworld in ret
//    calculated.insert(transition_map::value_type(current_pw, new_pw)); // And we update the calculated map
//
//    auto it_pwtm = get_beliefs().find(current_pw);
//
//    if (it_pwtm != get_beliefs().end()) {
//        pworld_map::const_iterator it_pwm;
//        pworld_set::const_iterator it_pws;
//
//        for (it_pwm = it_pwtm->second.begin(); it_pwm != it_pwtm->second.end(); it_pwm++) {
//            agent ag = it_pwm->first;
//            bool is_oblivious_obs = oblivious_obs_agents.find(ag) != oblivious_obs_agents.end();
//
//            for (it_pws = it_pwm->second.begin(); it_pws != it_pwm->second.end(); it_pws++) {
//                if (is_oblivious_obs) { // If we are dealing with an OBLIVIOUS agent we maintain its beliefs as they were
//                    auto maintained_pworld = ret.get_worlds().find(*it_pws);
//
//                    if (maintained_pworld != ret.get_worlds().end()) {
//                        ret.add_edge(new_pw, *it_pws, ag);
//                    }
//                } else { // Otherwise, if we have a FULLY OBS agent
//                    auto calculated_pworld = calculated.find(*it_pws);
//
//                    if (calculated_pworld != calculated.end()) { // If we already calculated the transition function for this pworld
//                        ret.add_edge(new_pw, calculated_pworld->second, ag); // Then we update agents' beliefs
//                    } else {
//                        pworld_ptr believed_pw = execute_ontic_helper(act, ret, *it_pws, calculated, oblivious_obs_agents);
//                        ret.add_edge(new_pw, believed_pw, ag);
//
//                        ret.set_max_depth(ret.get_max_depth() + 1 + current_pw.get_repetition());
//                    }
//                }
//            }
//        }
//    }
//    return new_pw;
//}
//
//pworld_ptr pstate::execute_sensing_announcement_helper(const fluent_formula &effects, pstate &ret, const pworld_ptr &current_pw, transition_map &calculated, agent_set &partially_obs_agents, agent_set &oblivious_obs_agents, bool previous_entailment) const
//{
//    pworld_ptr new_pw = ret.add_rep_world(pworld(current_pw.get_fluent_set()), current_pw.get_repetition()); // We add the corresponding pworld in ret
//    calculated.insert(transition_map::value_type(current_pw, new_pw)); // And we update the calculated map
//
//    auto it_pwtm = get_beliefs().find(current_pw);
//
//    if (it_pwtm != get_beliefs().end()) {
//        pworld_map::const_iterator it_pwm;
//        pworld_set::const_iterator it_pws;
//
//        for (it_pwm = it_pwtm->second.begin(); it_pwm != it_pwtm->second.end(); it_pwm++) {
//            agent ag = it_pwm->first;
//
//            bool is_oblivious_obs = oblivious_obs_agents.find(ag) != oblivious_obs_agents.end();
//            bool is_partially_obs = partially_obs_agents.find(ag) != partially_obs_agents.end();
//            bool is_fully_obs = !is_oblivious_obs && !is_partially_obs;
//
//            for (it_pws = it_pwm->second.begin(); it_pws != it_pwm->second.end(); it_pws++) {
//                if (is_oblivious_obs) { // If we are dealing with an OBLIVIOUS agent we maintain its beliefs as they were
//                    auto maintained_pworld = ret.get_worlds().find(*it_pws);
//
//                    if (maintained_pworld != ret.get_worlds().end()) {
//                        ret.add_edge(new_pw, *it_pws, ag);
//                    }
//                } else { // Otherwise, if we have a FULLY/PARTIALLY observant agent
//                    auto calculated_pworld = calculated.find(*it_pws);
//                    //fluent_formula effects = get_effects_if_entailed(act.get_effects(), get_pointed());
//                    //bool ent = act.get_type() == SENSING ? entails(effects, *it_pws) == entails(effects, get_pointed()) : entails(effects, *it_pws);
//                    bool ent = entails(effects, *it_pws); // == entails(effects, get_pointed());
//
//
//                    bool is_consistent_belief = is_partially_obs || // If "ag" is PARTIALLY OBS, we always add an edge; If "ag" is FULLY OBS, we add an edge if he believes that "calculated" may be true (i.e., when "ent" holds) XOR
//                                                (is_fully_obs && (ent == previous_entailment)); // if a PARTIALLY OBS agent believes that "ag" thinks that "calculated" may be true (i.e., when "previous_entailment" holds)
//
//                    if (calculated_pworld != calculated.end()) { // If we already calculated the transition function for this pworld
//                        if (is_consistent_belief) {
//                            ret.add_edge(new_pw, calculated_pworld->second, ag);
//                        }
//                    } else { // If we did not already calculate the transition function
//                        if (is_consistent_belief) { // We calculate it if it would result in a consistent belief...
//
//                            pworld_ptr believed_pw = execute_sensing_announcement_helper(effects, ret, *it_pws, calculated, partially_obs_agents, oblivious_obs_agents, ent);
//                            ret.add_edge(new_pw, believed_pw, ag);
//                        }
//                        //						else if (is_partially_obs && (ent != previous_entailment)) { // ...and when it does not entail the action effects, but a PARTIALLY OBS agent believes that it may be true
//                        //							pworld_ptr believed_pw_neg = execute_sensing_announcement_helper(act, ret, *it_pws, calculated, partially_obs_agents, oblivious_obs_agents, ent);
//                        //							ret.add_edge(new_pw, believed_pw_neg, ag);
//                        //						}
//                    }
//                }
//            }
//        }
//    }
//    return new_pw;
//}
//
//pstate pstate::execute_ontic(const action & act) const
//{
//    pstate ret;
//
//    //This finds all the worlds that are reachable from the initial state following
//    //the edges labeled with fully observant agents.
//    // \todo: SISTEMA
//    agent_set agents; // = domain.get_agents();
//    agent_set fully_obs_agents = get_agents_if_entailed(act.get_fully_observants(), get_pointed());
//
//    agent_set oblivious_obs_agents = agents;
//    minus_set(oblivious_obs_agents, fully_obs_agents);
//
//    transition_map calculated; // A map that links the pworlds of *this* to the corresponding ones of ret
//    maintain_oblivious_believed_pworlds(ret, oblivious_obs_agents);
//
//    pworld_ptr new_pointed = execute_ontic_helper(act, ret, get_pointed(), calculated, oblivious_obs_agents);
//    ret.set_pointed(new_pointed); // Updating the pointed world
//    //std::cerr << "\nDEBUG: Out ONTIC " << act.get_name();
//
//    return ret;
//}
//
//pstate pstate::execute_sensing(const action & act) const
//{
//    pstate ret;
//
//    //This finds all the worlds that are reachable from the initial state following
//    //the edges labeled with fully observant agents.
//    // \todo: SISTEMA
//    agent_set agents; // = domain.get_agents();
//    agent_set fully_obs_agents = get_agents_if_entailed(act.get_fully_observants(), get_pointed());
//    agent_set partially_obs_agents = get_agents_if_entailed(act.get_partially_observants(), get_pointed());
//
//    agent_set oblivious_obs_agents = agents;
//    minus_set(oblivious_obs_agents, fully_obs_agents);
//    minus_set(oblivious_obs_agents, partially_obs_agents);
//
//    if (!oblivious_obs_agents.empty()) {
//        ret.set_max_depth(get_max_depth() + 1);
//    }
//
//    transition_map calculated; // A map that links the pworlds of *this* to the corresponding ones of ret
//    maintain_oblivious_believed_pworlds(ret, oblivious_obs_agents);
//
//    fluent_formula effects = get_effects_if_entailed(act.get_effects(), get_pointed());
//
//    pworld_ptr new_pointed = execute_sensing_announcement_helper(effects, ret, get_pointed(), calculated, partially_obs_agents, oblivious_obs_agents, entails(effects));
//    ret.set_pointed(new_pointed); // Updating the pointed world
//
////    if (!check_properties(fully_obs_agents, partially_obs_agents, effects, ret)) {
////        std::cerr << "\nDEBUG: Some properties are not respected\n\n";
////        exit(1);
////    }
//
//    return ret;
//}
//
//pstate pstate::execute_announcement(const action &act) const
//{
//    return execute_sensing(act);
//}
