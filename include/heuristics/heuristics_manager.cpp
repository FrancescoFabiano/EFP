#include "heuristics_manager.h"

heuristics_manager::heuristics_manager(heuristics used_heur)
{
	set_used_h(used_heur);
	m_goals = domain::get_instance().get_goal_description();
	switch ( m_used_heur ) {
	case GNN:{
		//This is brutally copied from planner.ipp, make sure to remove all of this from planner and make a specific calss
		std::string folder = "out/ML_HEUR_datasets/DFS/";
		std::string goal_dot_file = domain::get_instance().get_name() + "_goal_tree.dot";
		m_goal_graph_file =  folder + goal_dot_file;
		generate_goal_tree_h(m_goal_graph_file);
		break;
	}
	case L_PG:
	case S_PG:
	{
		expand_goals();
		break;

	}
	case C_PG:
	{
		std::cout << "\n\nWrong implementation of initial state from initial formuale\n";
					exit(1);

		planning_graph pg;
		if (pg.is_satisfiable()) {
			m_fluents_score = pg.get_f_scores();
			m_bf_score = pg.get_bf_scores();

		} else {
			std::cout << "\n\nIt does not exists any Plan for this domain instance:(\n";
			exit(1);
		}
		break;
	}
	case SUBGOALS:
	{
		expand_goals();
		satisfied_goals::get_instance().set(m_goals);
		break;
	}
	default:
	{
		std::cerr << "\nWrong Heuristic Selection\n";
		exit(1);
	}

	}
}

void heuristics_manager::expand_goals(unsigned short nesting)
{

	formula_list::const_iterator it_fl;

	formula_list original_goal = m_goals;

	for (it_fl = original_goal.begin(); it_fl != original_goal.end(); it_fl++) {
		produce_subgoals(nesting, 0, (*it_fl), (*it_fl).get_group_agents());
	}

	//	formula_list original_goal = m_goals;
	//	formula_list::const_iterator it_fl;
	//	agent_set::const_iterator it_agset;
	//
	//	grounder gr = domain::get_instance().get_grounder();
	//
	//	//unsigned short added_subgoals = 0;
	//	for (it_fl = original_goal.begin(); it_fl != original_goal.end(); it_fl++) {
	//		belief_formula to_explore = *it_fl;
	//		if (to_explore.get_formula_type() == C_FORMULA) {
	//			for (it_agset = to_explore.get_group_agents().begin(); it_agset != to_explore.get_group_agents().end(); it_agset++) {
	//				belief_formula new_subgoal;
	//				new_subgoal.set_formula_type(BELIEF_FORMULA);
	//				new_subgoal.set_bf1(to_explore.get_bf1());
	//				new_subgoal.set_string_agent(gr.deground_agent(*it_agset));
	//				new_subgoal.ground();
	//				m_goals.push_back(new_subgoal);
	//			}
	//			//m_goals_number++;
	//		}
	//	}
}

void heuristics_manager::produce_subgoals(unsigned short nesting, unsigned short depth, const belief_formula & to_explore, const agent_set & agents)
{

	agent_set::const_iterator it_agset;

	//unsigned short added_subgoals = 0;

	if ((to_explore.get_formula_type() == C_FORMULA && depth == 0)
		|| (to_explore.get_formula_type() == BELIEF_FORMULA && depth > 0)) {
		for (it_agset = agents.begin(); it_agset != agents.end(); it_agset++) {
			if ((to_explore.get_agent() != *it_agset) || (depth == 0)) {
				belief_formula new_subgoal;
				new_subgoal.set_formula_type(BELIEF_FORMULA);
				if (depth == 0) {
					new_subgoal.set_bf1(to_explore.get_bf1());
				} else {
					new_subgoal.set_bf1(to_explore);
				}
				new_subgoal.set_agent(*it_agset);
				new_subgoal.set_is_grounded(true);
				m_goals.push_back(new_subgoal);

				if (nesting > (depth + 1)) {
					produce_subgoals(nesting, (depth + 1), new_subgoal, agents);
				}
			}
		}
	}
}

void heuristics_manager::set_used_h(heuristics used_h)
{
	m_used_heur = used_h;
}

heuristics heuristics_manager::get_used_h() const
{
	return m_used_heur;
}

const formula_list & heuristics_manager::get_goals() const
{
	return m_goals;
}

void heuristics_manager::set_goals(const formula_list & to_set)
{
	m_goals = to_set;
}

bool heuristics_manager::operator=(const heuristics_manager& to_copy)
{
	set_used_h(to_copy.get_used_h());
	set_goals(to_copy.get_goals());
	return true;
}




//This is brutally copied from planner.ipp, make sure to remove all of this from planner and make a specific calss
int heuristics_manager::get_id_from_map(const std::map<boost::dynamic_bitset<>, int>& id_map, const boost::dynamic_bitset<>& key, const std::string& type_name) {
    auto it = id_map.find(key);
    if (it != id_map.end()) {
        return it->second;
    } else {
        throw std::runtime_error(type_name + " not found in map");
    }
}

void heuristics_manager::populate_ids_from_bitset(const std::set<boost::dynamic_bitset<>>& keys_set, std::map<boost::dynamic_bitset<>, int>& id_map, int start_id) {
	int current_id = start_id;
	for (const auto& key : keys_set) {
        id_map[key] = current_id++;
    }
}

int heuristics_manager::get_unique_f_id_from_map(fluent fl) {
    return get_id_from_map(m_fluent_to_id, fl, "Fluent");
}

int heuristics_manager::get_unique_a_id_from_map(agent ag) {
    return get_id_from_map(m_agent_to_id, ag, "Agent");
}

void heuristics_manager::populate_fluent_ids(int start_id) {
    populate_ids_from_bitset(domain::get_instance().get_fluents(), m_fluent_to_id, start_id);
}

void heuristics_manager::populate_agent_ids(int start_id) {
    populate_ids_from_bitset(domain::get_instance().get_agents(), m_agent_to_id, start_id);
}

const std::string & heuristics_manager::generate_goal_tree_h(const std::string & goal_file_name) {
	auto goal_list = domain::get_instance().get_goal_description();
	int goal_counter = 0;
	
    std::ofstream dot_file(goal_file_name);
    if (!dot_file.is_open()) {
        std::cerr << "Unable to open output file.\n";
        exit(1);
    }

    dot_file << "digraph G {\n";

	// Populate fluent and agent ID maps

	populate_fluent_ids(0);
	populate_agent_ids(static_cast<int>(m_fluent_to_id.size())+1);

	int next_id = m_fluent_to_id.size() + m_agent_to_id.size() + 2;

	// Print fluent nodes (only by ID)
	/* for (const auto& [fluent_index, fluent_id] : m_fluent_to_id) {
		dot_file << "  F" << fluent_id << " [label=\"" << fluent_id << "\"];\n";
	}

	// Print agent nodes (only by ID)
	for (const auto& [agent_index, agent_id] : m_agent_to_id) {
		dot_file << "  A" << agent_id << " [label=\"" << agent_id << "\", shape=box];\n";
	}*/

	std::string parent_name = "init";
	for (auto& goal : goal_list) {
		print_goal_subtree(goal, ++goal_counter, next_id, parent_name,dot_file);
	}


    dot_file << "}\n";
    dot_file.close();

    std::cout << "DOT file 'graph.dot' created.\n";
	return goal_file_name;
}		


void heuristics_manager::print_goal_subtree(const belief_formula & to_print, int goal_counter, int & next_id, const std::string & parent_node, std::ofstream& dot_file) {


	int current_node_id = ++next_id;
	std::string node_name;

	switch (to_print.get_formula_type()) {
		case FLUENT_FORMULA: {
			std::string m_parent_node = parent_node;
			if  (to_print.get_fluent_formula().size() > 1) {
				//REMOVE LETTERS node_name = "F_OR" + std::to_string(current_node_id);
				node_name = std::to_string(current_node_id);
				current_node_id = ++next_id;
				//dot_file << "  " << node_name << " [label=\"" << current_node_id << "\"];\n";
				dot_file << "  " << parent_node << " -> " << node_name << " [label=\"" << goal_counter << "\"];\n";
				m_parent_node = node_name;
			}

			for (const auto& fls_set : to_print.get_fluent_formula()) {
					
				std::string m_m_parent_node = m_parent_node;

				if  (fls_set.size() > 1) {
					//REMOVE LETTERS node_name = "F_AND" + std::to_string(current_node_id);
					node_name = std::to_string(current_node_id);
					current_node_id = ++next_id;
					//dot_file << "  " << node_name << " [label=\"" << current_node_id << "\"];\n";
					dot_file << "  " << m_parent_node << " -> " << node_name << " [label=\"" << goal_counter << "\"];\n";
					m_m_parent_node = node_name;
				}

				for (const auto& fl : fls_set) {
					//REMOVE LETTERS dot_file << "  " << m_m_parent_node << " -> F" << get_unique_f_id_from_map(fl) << " [label=\"" << goal_counter << "\"];\n";
					dot_file << "  " << m_m_parent_node << " -> " << get_unique_f_id_from_map(fl) << " [label=\"" << goal_counter << "\"];\n";
				} 
			}
			break;
		}

		case BELIEF_FORMULA: {
			//REMOVE LETTERS node_name = "B" + std::to_string(current_node_id);
			node_name = std::to_string(current_node_id);
			//dot_file << "  " << node_name << " [label=\"" << current_node_id << "\"];\n";
			dot_file << "  " << parent_node << " -> " << node_name << " [label=\"" << goal_counter << "\"];\n";
			//REMOVE LETTERS dot_file << "  " << node_name << " -> A" << get_unique_a_id_from_map(to_print.get_agent()) << " [label=\"" << goal_counter << "\"];\n";
			//REMOVE LETTERS dot_file << "  A" << get_unique_a_id_from_map(to_print.get_agent()) << " -> " << node_name << " [label=\"" << goal_counter << "\"];\n";
			dot_file << "  " << node_name << " -> " << get_unique_a_id_from_map(to_print.get_agent()) << " [label=\"" << goal_counter << "\"];\n";
			dot_file << "  " << get_unique_a_id_from_map(to_print.get_agent()) << " -> " << node_name << " [label=\"" << goal_counter << "\"];\n";

			print_goal_subtree(to_print.get_bf1(), goal_counter, next_id, node_name,dot_file);
			break;
		}

		case C_FORMULA: {
			//REMOVE LETTERS node_name = "C" + std::to_string(current_node_id);
			node_name = std::to_string(current_node_id);
			//dot_file << "  " << node_name << " [label=\"" << current_node_id << "\"];\n";
			dot_file << "  " << parent_node << " -> " << node_name << " [label=\"" << goal_counter << "\"];\n";

			for (const auto& ag : to_print.get_group_agents()) {
				//REMOVE LETTERS dot_file << "  " << node_name << " -> A" << get_unique_a_id_from_map(ag) << " [label=\"" << goal_counter << "\"];\n";
				//REMOVE LETTERS dot_file << "  A" << get_unique_a_id_from_map(ag) << " -> " << node_name << " [label=\"" << goal_counter << "\"];\n";
				dot_file << "  " << node_name << " -> " << get_unique_a_id_from_map(ag) << " [label=\"" << goal_counter << "\"];\n";
				dot_file << "  " << get_unique_a_id_from_map(ag) << " -> " << node_name << " [label=\"" << goal_counter << "\"];\n";
			}

			print_goal_subtree(to_print.get_bf1(), goal_counter, next_id, node_name,dot_file);
			break;
		}

		case PROPOSITIONAL_FORMULA: {
			switch (to_print.get_operator()) {
				case BF_NOT: {
					node_name = "NOT";
					break;
				}

				case BF_AND: {
					node_name = "AND";
					break;
				}

				case BF_OR: {
					node_name = "OR";
					break;
				}

				case BF_FAIL:
				default: {
					std::cerr << "\n ERROR IN DECLARATION\n.";
					exit(1);
					break;
				}
			}

			/* if (m_node_printed.find(node_name) == m_node_printed.end() || m_node_printed[node_name] == false){
				dot_file << "  " << node_name << " [label=\"" << m_special_nodes[node_name] << "\"];\n";
				m_node_printed[node_name] = true;
			} */

			//REMOVE LETTERS node_name = node_name + std::to_string(current_node_id);
			node_name = std::to_string(current_node_id);
			//dot_file << "  " << node_name << " [label=\"" << current_node_id << "\"];\n";
			dot_file << "  " << parent_node << " -> " << node_name << " [label=\"" << goal_counter << "\"];\n";
			print_goal_subtree(to_print.get_bf1(), goal_counter, next_id, node_name,dot_file);

			if (!to_print.is_bf2_null()) {
				print_goal_subtree(to_print.get_bf2(), goal_counter, next_id, node_name,dot_file);
			}

			break;
		}

		case BF_EMPTY:
		case BF_TYPE_FAIL:
		default: {
			std::cerr << "\n Unknown belief_formula type.";
			exit(1);
			break;
		}
	}
}
