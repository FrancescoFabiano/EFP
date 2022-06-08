/**
 * \brief Implementation of \ref kstate.h
 *
 * \copyright GNU Public License.
 *
 * \author Francesco Fabiano, Alessandro Burigana.
 * \date April 9, 2019
 */

#include <iostream>

#include "kstate.h"
#include "../../update/product_update.h"
#include "../../utilities/helper_t.ipp"


kstate::kstate() = default;

template<class M>
kstate::kstate(const domain &domain, const finitary_theory<M> &theory) {
    generate_from_theory(theory);
}

kstate::kstate(const kstate &previous, const action &action) {
    update(previous, action);
}

/**** GETTERS ****/
const kworld_set &kstate::get_worlds() const {
    return m_worlds;
}

const kedge_map &kstate::get_edges() const {
    return m_edges;
}

const kworld *kstate::get_pointed() const {
    return m_pointed;
}

/**** OPERATORS ****/
//bool kstate::operator=(const kstate & to_copy) {
//    set_edges(to_copy.get_edges());
//    set_worlds(to_copy.get_worlds());
//    set_pointed(to_copy.get_pointed());
//    return true;
//}
//
//bool kstate::operator==(const kstate & to_compare) const {
//    if (m_pointed->get_id() == to_compare.get_pointed()->get_id()) {
//        return true;
//    }
//
//    bisimulation b;
//    // \todo: SISTEMA
//    agent_set to_fix;
//    return b.compare_automata_eq(to_compare, *this, to_fix); // domain.get_agents());
//}
//
//bool kstate::operator<(const kstate & to_compare) const {
//    if (m_pointed->get_id() < to_compare.get_pointed()->get_id()) {
//        return true;
//    } else if (m_pointed->get_id() > to_compare.get_pointed()->get_id()) {
//        return false;
//    }
//
//    if (m_worlds < to_compare.get_worlds()) {
//        return true;
//    } else if (m_worlds > to_compare.get_worlds()) {
//        return false;
//    }
//
//    if (m_edges < to_compare.get_edges()) {
//        return true;
//    } else if (m_edges > to_compare.get_edges()) {
//        return false;
//    }
//
//    return false;
//}

bool kstate::entails(const formula &formula, const kworld &world) const {
    return false;
//    return formula.is_entailed(this, world);
}

void kstate::get_all_reachable_worlds_edges(const kworld *world, kworld_set &reached_worlds, kedge_map &reached_edges) const {
    // todo: TEST
    auto adjacent_worlds = m_edges.at(world);
    kworld_set kw_list;

    std::map<const agent*, kworld_set>::const_iterator it_kwmap;
    kworld_set::const_iterator it_kws;

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

void kstate::clean_unreachable_kworlds() {
    kworld_set reached_worlds;
    kedge_map reached_edges;

    reached_worlds.insert(m_pointed);
    reached_edges[m_pointed] = m_edges.at(m_pointed);

    get_all_reachable_worlds_edges(m_pointed, reached_worlds, reached_edges);

//    set_worlds(reached_worlds);
//    set_edges(reached_edges);
}

/**** INITIAL STATE ****/
template<class M>
void kstate::generate_from_theory(const domain &domain, const finitary_theory<M> &theory) {
    generate_initial_pointed(theory);
    generate_initial_worlds(domain, theory);
    generate_initial_edges(domain, theory);
}

template<class M>
void kstate::generate_initial_pointed(const finitary_theory<M> &theory) {
    fluent_set pointed_fluent_set;
    std::list<literal>::const_iterator it_ls;

    for (it_ls = theory.get_pointed_literals().begin(); it_ls != theory.get_pointed_literals().end(); ++it_ls) {
        if (it_ls->is_positive()) {
            pointed_fluent_set.insert(it_ls->get_fluent());
        }
    }

    kworld tmp = kworld(&pointed_fluent_set, nullptr, 0);
    // todo: utilizza store per ottenere il puntatore
    m_pointed = &tmp;
    m_worlds.insert(m_pointed);
}

template<class M>
void kstate::generate_initial_worlds(const domain &domain, const finitary_theory<M> &theory) {
    fluent_set initially_unknown_fluents;
    std::set_difference(domain.get_fluents().begin(), domain.get_fluents().end(),
                        theory.get_ck_literals().begin(), theory.get_ck_literals().end(), initially_unknown_fluents);

    // We generate all the combinations of fluents that are not known initially
    // This is equivalent to generating the power set of initially_unknown_fluents
    // Power set generation: https://rosettacode.org/wiki/Power_set#C.2B.2B
    std::set<fluent_set> power_set;
    power_set.emplace();

    for (auto&& f : initially_unknown_fluents) {
        std::set<fluent_set> subset;

        for (auto s : power_set) {
            s.insert(f);
            subset.insert(s);
        }
        power_set.insert(begin(subset), end(subset));
    }

    auto ps_it = power_set.begin();
    auto form_it = theory.get_ck_formulae().begin();
    bool is_consistent = true;

    // We add the set of initially known fluents to each combination of initially unknown fluents
    // And we create a world for each resulting fluent set
    for (; ps_it != power_set.end(); ++ps_it) {
        ps_it->insert(theory.get_ck_literals());
        // todo: implementa set di fluent set (store); probabilmente va messo in domain

        // We check that the current fluent set is consistent with the fluent formulae that are
        // Commonly known by all agents according to the theory
        while (is_consistent && form_it != theory.get_ck_formulae().end()) {
            if (!form_it->is_entailed(*ps_it)) {
                is_consistent = false;
            }
            ++form_it;
        }

        if (is_consistent) {
            kworld tmp = kworld(&*ps_it, nullptr);
            // todo: utilizza store per ottenere il puntatore
            m_worlds.insert(&tmp);
            // todo: inserisco due volte il pointed?
        }

        form_it = theory.get_ck_formulae().begin();
        is_consistent = true;
    }
/*//    fluent_set::const_iterator fs_it;
//
//    for (fs_it = initially_unknown_fluents.begin(); fs_it != initially_unknown_fluents.end(); ++fs_it) {
//        for (ps_it = power_set.begin(); ps_it != power_set.end(); ++ps_it) {
//            auto x = *ps_it;
//            auto y = (*fs_it);
//            x.insert(*fs_it);
//            (*ps_it).insert(&(*fs_it));
//            ps_it->insert(*(fs_it));
//        }
//    }*/
}

template<class M>
void kstate::generate_initial_edges(const domain &domain, const finitary_theory<M> &theory) {
    // todo: implementa classe template 'store' per collezionare elementi in un set (world, fluent_set, formule...)
    kedge_map no_good;
    std::map<const agent*, kworld_set> ag_worlds, ng_ag_worlds;
    kworld_set ws, ng_ws;

    // In 'no_good' we keep track of what edges we don't want to include
    for (const auto w1 : m_worlds) {
        ng_ag_worlds = no_good[w1];

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

    for (const auto w1 : m_worlds) {
        ag_worlds = m_edges[w1];
        ng_ag_worlds = no_good[w1];

        for (const auto ag : domain.get_agents()) {
            ws = ag_worlds[ag];
            ng_ws = ng_ag_worlds[ag];

            // We store only edges that are *not* in the no-goods map
            for (const auto w2 : m_worlds) {
                std::set_difference(m_worlds.begin(), m_worlds.end(), ng_ws.begin(), ng_ws.end(), ws.begin());
            }
        }
    }
}

/**** TRANSITION FUNCTION ****/
void kstate::update(const kstate &previous, const action &action) {
    // todo: implement
}

kstate kstate::compute_succ(const action & act) const {
    /** \warning executability should be check in \ref state (or \ref planner).*/
    kstate tmp;
    return tmp;
    //return product_update::update(*this, act);
}

/**** BISIMULATION ****/
automaton kstate::kstate_to_automaton(std::vector<kworld*> & kworld_vec, const std::map<agent, bis_label> & agent_to_label) const {
//    std::map<int, int> compact_indices;
//    std::map<kworld*, int> index_map;
//    kbislabel_map label_map; // Map: from -> (to -> ag_set)

    automaton *a;
//    unsigned long Nvertex = get_worlds().size();
//    // \todo: SISTEMA
//    unsigned long ag_set_size = 0; // domain.get_agents().size();
//    //BIS_ADAPTATION For the loop that identifies the id (We add one edge for each node)
//    v_elem *Vertex;
//
//    Vertex = (v_elem *) malloc(sizeof(v_elem) * Nvertex);
//
//    // Initializating vertices
//    kworld_set::const_iterator it_kwps;
//    kbislabel_map::const_iterator it_klm;
//    bis_label_set::const_iterator it_bislab;
//    std::map<kworld*, bis_label_set>::const_iterator it_kw_bislab;
//
//    //std::cerr << "\nDEBUG: Inizializzazione Edges\n";
//
//    // The pointed world is set to the index 0. This ensures that, when deleting the bisimilar nodes, the pointed kworld
//    // is always chosen as the first of its block. Therefore, we do not need to update it when converting back to a kstate
//    index_map[get_pointed()] = 0;
//    kworld_vec.push_back(get_pointed());
//    compact_indices[static_cast<int> (get_pointed()->get_id())] = 0;
//
//    //For the loop that identifies the id
//    //BIS_ADAPTATION For the loop that identifies the id (+1)
//    ///@bug: If the pointed has no self-loop to add
//    //kworld_ptr_set pointed_adj = adj_list.at(get_pointed());
//
//    Vertex[0].ne = 0; // pointed_adj.size(); // edge_counter[get_pointed()];
//    //    if (pointed_adj.find(get_pointed()) == pointed_adj.end()) {
//    //        Vertex[0].ne++;
//    //    }
//    //    Vertex[0].e = (e_elem *) malloc(sizeof(e_elem) * Vertex[0].ne);
//
//    int i = 1, c = 1;
//
//    //std::cerr << "\nDEBUG: Inizializzazione Vertex\n";
//
//    for (it_kwps = m_worlds.begin(); it_kwps != m_worlds.end(); it_kwps++) {
//        if (!(*it_kwps == get_pointed())) {
//            index_map[*it_kwps] = i;
//            kworld_vec.push_back(*it_kwps);
//
//            // if (compact_indices.find(it_kwps->get_id()) == compact_indices.end()) {
//            if (compact_indices.insert({it_kwps->get_id(), c}).second) {
//                // compact_indices[it_kwps->get_id()] = c;
//                c++;
//                //std::cerr << "\nDEBUG: Added:" << it_kwps->get_id() << "\n";
//            }
//
//            //BIS_ADAPTATION For the loop that identifies the id (+1)
//            Vertex[i].ne = 0; //adj_list.at(*it_kwps).size(); // edge_counter[*it_kwps];
//            //            Vertex[i].e = (e_elem *) malloc(sizeof(e_elem) * Vertex[i].ne);
//            i++;
//        }
//        //BIS_ADAPTATION (Added self-loop)
//        label_map[*it_kwps][*it_kwps].insert(compact_indices[static_cast<int> (it_kwps->get_id())] + ag_set_size);
//        //std::cerr << "\nDEBUG: Added to " << it_kwps->get_id() << " the label " << compact_indices[it_kwps->get_id()] + ag_set_size << std::endl;
//    }
//
//
//    //BIS_ADAPTATION For the loop that identifies the id (We add one potential label for each node)
//    unsigned long bhtabSize = ag_set_size + c;
//
//    //std::cerr << "\nDEBUG: Inizializzazione Behavs\n";
//
//    //BIS_ADAPTATION (Moved down here)
//    kedge_map::const_iterator it_kem;
//    std::map<agent, kworld_set>::const_iterator it_agkw;
//
//    for (it_kem = m_edges.begin(); it_kem != m_edges.end(); it_kem++) {
//        for (it_agkw = it_kem->second.begin(); it_agkw != it_kem->second.end(); it_agkw++) {
//            for (it_kwps = it_agkw->second.begin(); it_kwps != it_agkw->second.end(); it_kwps++) {
//                label_map[it_kem->first][*it_kwps].insert(agent_to_label.at(it_agkw->first));
//                Vertex[index_map[it_kem->first]].ne++;
//            }
//        }
//    }
//
//    /*for (it_keps = m_information_state.begin(); it_keps != m_information_state.end(); it_keps++) {
//
//        //DEBUG:Change this
//        // if (adj_list[it_keps->get_from()][it_keps->get_to()].empty())
//        // {
//        //     edge_counter[it_keps->get_from()]++;
//        // }
//
//        label_map[it_keps->get_from()][it_keps->get_to()].insert(agent_to_label.at(it_keps->get_label()));
//        Vertex[index_map[it_keps->get_from()]].ne++;
//    }*/
//
//    i = 0;
//    for (it_kwps = m_worlds.begin(); it_kwps != m_worlds.end(); it_kwps++) {
//        Vertex[i].ne++; //Self loop bisimulation
//        Vertex[i].e = (e_elem *) malloc(sizeof(e_elem) * Vertex[i].ne);
//        i++;
//    }
//
//    //std::cerr << "\nDEBUG: Fine Inizializzazione Vertex\n";
//
//
//    int from, to, j = 0; //, k = 0, nbh;
//
//    //std::cerr << "\nDEBUG: Inizializzazione Mappa\n";
//    for (it_klm = label_map.begin(); it_klm != label_map.end(); it_klm++) {
//        from = index_map[it_klm->first]; // For each kworld 'from'
//
//        //std::cerr << "\nDEBUG: Inizializzazione K\n";
//
//        for (it_kw_bislab = it_klm->second.begin(); it_kw_bislab != it_klm->second.end(); it_kw_bislab++) { // For each edge that reaches the kworld 'to'
//            to = index_map[it_kw_bislab->first];
//            //nbh = it_kw_bislab->second.size();
//
//            for (it_bislab = it_kw_bislab->second.begin(); it_bislab != it_kw_bislab->second.end(); it_bislab++) { // For each agent 'ag' in the label of the kedge
//                //std::cerr << "\nDEBUG: j is: " << j << " and k is: " << k << "\n";
//                //nbh = 1;
//                Vertex[from].e[j].nbh = 1; // Let j be the index of the adjacency list of from that stores the kedge (from, to)
//                Vertex[from].e[j].bh = (int *) malloc(sizeof(int)); // Let nbh be the number of agents in such kedge
//                Vertex[from].e[j].tv = to; // Update the value of the reache kworld
//                Vertex[from].e[j].bh[0] = *it_bislab; // Update the value of the label at index k to 'ag'
//                //std::cerr << "\nDEBUG: j is: " << j << " and k is: " << k << "\n";
//
//                j++; // Update the value of the index j
//            }
//
//            //std::cerr << "\nDEBUG J: " << j-1 << "\n";
//
//            //k = 0; // Reset k
//        }
//        //std::cerr << "\nDEBUG: Fine Inizializzazione K\n";
//
//        j = 0; // Reset j
//    }
//
//    //std::cerr << "\nDEBUG: Fine Inizializzazione Mappa\n";
//
//    //        std::vector<kworld*>::const_iterator it_kwp;
//    //    int temp_counter = 0;
//    //    for (it_kwp = kworld_vec.begin(); it_kwp != kworld_vec.end(); it_kwp++) {
//    //        //std::cerr << "DEBUG: World " << temp_counter << " has ID: " << it_kwp->get_id() << std::endl;
//    //        temp_counter++;
//    //    }
//    //
//    // Building the automaton
//    unsigned long Nbehavs = bhtabSize;
//    a = (automaton *) malloc(sizeof(automaton));
//    a->Nvertex = Nvertex;
//    a->Nbehavs = Nbehavs;
//    a->Vertex = Vertex;
//    a->Nagents = ag_set_size;

    //    std::cerr << "\nDEBUG: \n\tNvertex = " << Nvertex << std::endl;
    //    std::cerr << "\tNbehavs = " << Nbehavs << std::endl;

    return *a;
}

void kstate::automaton_to_kstate(const automaton & a, const std::vector<kworld*> & kworld_vec, const std::map<bis_label, agent> & label_to_agent) {
//    kworld_set worlds;
//    m_edges.clear();
//    // The pointed world does not change when we calculate the minimum bisimilar state
//    // Hence we do not need to update it
//
//    // \todo: SISTEMA
//    unsigned long agents_size = 0; // domain.get_agents().size();
//    int i, j, k, label;
//
//    for (i = 0; i < a.Nvertex; i++) {
//        if (a.Vertex[i].ne > 0) {
//            worlds.insert(kworld_vec[i]);
//
//            for (j = 0; j < a.Vertex[i].ne; j++) {
//                for (k = 0; k < a.Vertex[i].e[j].nbh; k++) {
//                    label = a.Vertex[i].e[j].bh[k];
//                    if (label < agents_size) {
//                        add_edge(kworld_vec[i], kworld_vec[a.Vertex[i].e[j].tv], label_to_agent.at(label));
//                        //                        edges.insert(kstore::get_instance().add_edge(kedge(kworld_vec[i], kworld_vec[a.Vertex[i].e[j].tv], label_to_agent.at(label))));
//                    }
//                }
//            }
//        }
//    }
//
//    set_worlds(worlds);
}

void kstate::calc_min_bisimilar(Bisimulation_Algorithm algorithm) {
    // ************* Cleaning unreachable kworlds *************
    std::map<bis_label, agent> label_to_agent;
    std::map<agent, bis_label> agent_to_label;

    /**
     * \todo: SISTEMARE QUESTO BLOCCO DI CODICE COMMENTATO!!!
     *        SENZA DI QUESTO BISIMULATION _NON_ FUNZIONA!!!
     */
//    auto agents = domain.get_agents();
//    auto it_ag = agents.begin();
//    bis_label ag_label = 0;
//    agent lab_agent;
//    for (; it_ag != agents.end(); it_ag++) {
//        lab_agent = *it_ag;
//        label_to_agent.insert(std::make_pair(ag_label, lab_agent));
//        agent_to_label.insert(std::make_pair(lab_agent, ag_label));
//        ag_label++;
//    }

    clean_unreachable_kworlds();

    //std::cerr << "\nDEBUG: INIZIO BISIMULATION IN KSTATE\n" << std::flush;
    std::vector<kworld*> kworld_vec; // Vector of all kworld*
    //std::cerr << "\nDEBUG: PRE-ALLOCAZIONE AUTOMA\n" << std::flush;


    //    std::cerr << "\nDEBUG: \n\tNvertex_before = " << m_worlds.size() << std::endl;
    //    std::cerr << "\tNbehavs_before = " << m_information_state.size() << std::endl;

    automaton a;
    kworld_vec.reserve(get_worlds().size());

    a = kstate_to_automaton(/*adj_list,*/ kworld_vec, agent_to_label);

    bisimulation b;
    //std::cout << "\nDEBUG: Printing automaton pre-Bisimulation\n";
    //b.VisAutoma(&a);



    if (algorithm == PAIGE_TARJAN) {
        if (b.MinimizeAutomaPT(&a)) {
            //VisAutoma(a);

            //std::cout << "\nDEBUG: Printing automaton post-Bisimulation\n";
            //b.VisAutoma(&a);
            //std::cout << "Done\n";
            automaton_to_kstate(a, kworld_vec, label_to_agent);

            //b.DisposeAutoma(&a);
        }
    } else {
        if (b.MinimizeAutomaFB(&a)) {

            //std::cerr << "\nDEBUG: Printing automaton post-Bisimulation\n";
            //b.VisAutoma(&a);
            //std::cerr << "Done\n";
            automaton_to_kstate(a, kworld_vec, label_to_agent);
            //b.DisposeAutoma(&a);
        }
    }

    //std::cerr << "\nDEBUG: PRe Clean" << std::endl;

}

/**** UTILITIES ****/
void kstate::print() const {
//    int counter = 1;
//    std::cout << std::endl;
//    std::cout << "The Pointed World has id ";
//    printer::get_instance().print_list(get_pointed().get_fluent_set());
//    std::cout << std::endl;
//    std::cout << "*******************************************************************" << std::endl;
//
//    kworld_ptr_set::const_iterator it_kwset;
//    std::cout << "World List:" << std::endl;
//
//    for (it_kwset = get_worlds().begin(); it_kwset != get_worlds().end(); it_kwset++) {
//        std::cout << "W-" << counter << ": ";
//        printer::get_instance().print_list(it_kwset->get_fluent_set());
//        std::cout << " rep:" << it_kwset->get_repetition();
//        std::cout << std::endl;
//        counter++;
//    }
//    counter = 1;
//    std::cout << std::endl;
//    std::cout << "*******************************************************************" << std::endl;
//
//    kedge_map::const_iterator it_kem;
//    std::map<agent, kworld_ptr_set>::const_iterator it_agkw;
//
//    std::cout << "Edge List:" << std::endl;
//
//    for (it_kem = m_edges.begin(); it_kem != m_edges.end(); it_kem++) {
//        for (it_agkw = it_kem->second.begin(); it_agkw != it_kem->second.end(); it_agkw++) {
//            for (it_kwset = it_agkw->second.begin(); it_kwset != it_agkw->second.end(); it_kwset++) {
//                std::cout << "E-" << counter << ": (";
//                printer::get_instance().print_list(it_kem->first.get_fluent_set());
//                std::cout << "," << it_kem->first.get_repetition();
//                std::cout << ") - (";
//                printer::get_instance().print_list(it_kwset->get_fluent_set());
//                std::cout << "," << it_kwset->get_repetition();
//                std::cout << ") ag:" << domain.get_grounder().deground_agent(it_agkw->first);
//                std::cout << std::endl;
//                counter++;
//            }
//        }
//    }
//    std::cout << "*******************************************************************" << std::endl;
}

void kstate::print_graphviz(std::ostream & graphviz) const {
//    string_set::const_iterator it_st_set;
//    fluent_set::const_iterator it_fs;
//
//
//    graphviz << "//WORLDS List:" << std::endl;
//
//    kworld_ptr_set::const_iterator it_kpset;
//    std::map<fluent_set, int> map_world_to_index;
//    std::map<unsigned short, char> map_rep_to_name;
//    char found_rep = (char) ((char) domain.get_agents().size() + 'A');
//    int found_fs = 0;
//    fluent_set tmp_fs;
//    char tmp_unsh;
//    string_set tmp_stset;
//    bool print_first;
//    kworld_ptr_set::const_iterator it_kwset;
//    for (it_kwset = get_worlds().begin(); it_kwset != get_worlds().end(); it_kwset++) {
//        if (*it_kwset == get_pointed())
//            graphviz << "    node [shape = doublecircle] ";
//        else
//            graphviz << "    node [shape = circle] ";
//
//        print_first = false;
//        tmp_fs = it_kwset->get_fluent_set();
//        if (map_world_to_index.count(tmp_fs) == 0) {
//            map_world_to_index[tmp_fs] = found_fs;
//            found_fs++;
//        }
//        tmp_unsh = static_cast<char> (it_kwset->get_repetition());
//        if (map_rep_to_name.count(tmp_unsh) == 0) {
//            map_rep_to_name[tmp_unsh] = found_rep;
//            found_rep++;
//        }
//        graphviz << "\"" << map_rep_to_name[tmp_unsh] << "_" << map_world_to_index[tmp_fs] << "\";";
//        graphviz << "// (";
//        tmp_stset = domain.get_grounder().deground_fluent(tmp_fs);
//        for (it_st_set = tmp_stset.begin(); it_st_set != tmp_stset.end(); it_st_set++) {
//            if (print_first) {
//                graphviz << ",";
//            }
//            print_first = true;
//            graphviz << *it_st_set;
//        }
//        graphviz << ")\n";
//    }
//
//    graphviz << "\n\n";
//    graphviz << "//RANKS List:" << std::endl;
//
//    std::map<int, kworld_ptr_set> for_rank_print;
//    for (it_kwset = get_worlds().begin(); it_kwset != get_worlds().end(); it_kwset++) {
//        for_rank_print[it_kwset->get_repetition()].insert(*it_kwset);
//    }
//
//    std::map<int, kworld_ptr_set>::const_iterator it_map_rank;
//    for (it_map_rank = for_rank_print.begin(); it_map_rank != for_rank_print.end(); it_map_rank++) {
//        graphviz << "    {rank = same; ";
//        for (it_kwset = it_map_rank->second.begin(); it_kwset != it_map_rank->second.end(); it_kwset++) {
//            graphviz << "\"" << map_rep_to_name[it_kwset->get_repetition()] << "_" << map_world_to_index[it_kwset->get_fluent_set()] << "\"; ";
//        }
//        graphviz << "}\n";
//    }
//
//
//    graphviz << "\n\n";
//    graphviz << "//EDGES List:" << std::endl;
//
//    std::map < std::tuple<std::string, std::string>, std::set<std::string> > edges;
//
//    std::tuple<std::string, std::string> tmp_tuple;
//    std::string tmp_string;
//
//    kedge_map::const_iterator it_kem;
//    std::map<agent, kworld_ptr_set>::const_iterator it_agkw;
//
//    for (it_kem = m_edges.begin(); it_kem != m_edges.end(); it_kem++) {
//        for (it_agkw = it_kem->second.begin(); it_agkw != it_kem->second.end(); it_agkw++) {
//            for (it_kwset = it_agkw->second.begin(); it_kwset != it_agkw->second.end(); it_kwset++) {
//                tmp_string = "_" + std::to_string(map_world_to_index[it_kem->first.get_fluent_set()]);
//                tmp_string.insert(0, 1, map_rep_to_name[it_kem->first.get_repetition()]);
//                std::get<0>(tmp_tuple) = tmp_string;
//
//                tmp_string = "_" + std::to_string(map_world_to_index[it_kwset->get_fluent_set()]);
//                tmp_string.insert(0, 1, map_rep_to_name[it_kwset->get_repetition()]);
//                std::get<1>(tmp_tuple) = tmp_string;
//
//                edges[tmp_tuple].insert(domain.get_grounder().deground_agent(it_agkw->first));
//            }
//        }
//    }
//
//    std::map < std::tuple<std::string, std::string>, std::set < std::string>>::iterator it_map;
//    std::map < std::tuple<std::string, std::string>, std::set < std::string>>::const_iterator it_map_2;
//
//    std::map < std::tuple<std::string, std::string>, std::set < std::string>> to_print_double;
//    for (it_map = edges.begin(); it_map != edges.end(); it_map++) {
//        for (it_map_2 = it_map; it_map_2 != edges.end(); it_map_2++) {
//            if (std::get<0>(it_map->first) == std::get<1>(it_map_2->first)) {
//                if (std::get<1>(it_map->first) == std::get<0>(it_map_2->first)) {
//                    if (it_map->second == it_map_2->second) {
//                        if (std::get<0>(it_map->first) != std::get<1>(it_map->first)) {
//                            to_print_double[it_map->first] = it_map->second;
//                            it_map_2 = edges.erase(it_map_2);
//                            it_map = edges.erase(it_map);
//                        }
//                    }
//                }
//            }
//        }
//    }
//
//    std::set<std::string>::const_iterator it_stset;
//    for (it_map = edges.begin(); it_map != edges.end(); it_map++) {
//        graphviz << "    \"";
//        graphviz << std::get<0>(it_map->first);
//        graphviz << "\" -> \"";
//        graphviz << std::get<1>(it_map->first);
//        graphviz << "\" ";
//        graphviz << "[ label = \"";
//        tmp_string = "";
//        for (it_stset = it_map->second.begin(); it_stset != it_map->second.end(); it_stset++) {
//            tmp_string += *it_stset;
//            tmp_string += ",";
//        }
//        tmp_string.pop_back();
//        graphviz << tmp_string;
//        graphviz << "\" ];\n";
//    }
//
//    for (it_map = to_print_double.begin(); it_map != to_print_double.end(); it_map++) {
//        graphviz << "    \"";
//        graphviz << std::get<0>(it_map->first);
//        graphviz << "\" -> \"";
//        graphviz << std::get<1>(it_map->first);
//        graphviz << "\" ";
//        graphviz << "[ dir=both label = \"";
//        tmp_string = "";
//        for (it_stset = it_map->second.begin(); it_stset != it_map->second.end(); it_stset++) {
//
//            tmp_string += *it_stset;
//            tmp_string += ",";
//        }
//        tmp_string.pop_back();
//        graphviz << tmp_string;
//        graphviz << "\" ];\n";
//    }
//
//
//    std::string color = "<font color=\"#ffffff\">";
//    graphviz << "\n\n//WORLDS description Table:" << std::endl;
//    graphviz << "    node [shape = plain]\n\n";
//    graphviz << "    description[label=<\n";
//    graphviz << "    <table border = \"0\" cellborder = \"1\" cellspacing = \"0\" >\n";
//    for (it_kwset = get_worlds().begin(); it_kwset != get_worlds().end(); it_kwset++) {
//        tmp_fs = it_kwset->get_fluent_set();
//        print_first = false;
//        graphviz << "        <tr><td>" << map_rep_to_name[it_kwset->get_repetition()] << "_" << map_world_to_index[tmp_fs] << "</td> <td>";
//        for (it_fs = tmp_fs.begin(); it_fs != tmp_fs.end(); it_fs++) {
//            if (print_first) {
//                graphviz << ", ";
//            }
//            print_first = true;
//            if (!helper::is_negate(*it_fs)) color = "<font color=\"#0000ff\"> ";
//            else color = "<font color=\"#ff1020\">";
//            graphviz << color << domain.get_grounder().deground_fluent(*it_fs) << "</font>";
//        }
//        graphviz << "</td></tr>\n";
//    }
//    graphviz << "    </table>>]\n";
//    graphviz << "    {rank = max; description};\n";

}
