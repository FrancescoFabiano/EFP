/**
 * \brief Implementation of the \ref planner.h.
 *
 * \copyright GNU Public License.
 *
 * \author Francesco Fabiano, Alessandro Burigana.
 * \date May 6, 2019
 */
#include <algorithm>
#include "planner.h"
//#include "../update/product_update.h"
//#include "../actions/custom_event_models/cem_store.h"

template <class T>
void planner<T>::print_results(const domain& domain, std::chrono::duration<double> elapsed_seconds, T goal,
                               bool given_plan) {
	std::cout << "\n\n\nWell Done, Goal found in " << elapsed_seconds.count() << " :)\n";
	goal.print();

    const auto& config = domain.get_config();

	if (config.is_results_file()) {
		std::ofstream result;
		std::string folder = "out/EFP_comparison/";
		if (given_plan) {
			folder = folder + "givenplan/";
		} else {
			folder = folder + "findingplan/";
		}
		system(("mkdir -p " + folder).c_str());
		result.open(folder + config.get_domain_name() + ".txt", std::ofstream::out | std::ofstream::app);
		result << "EFP Version 2.0 (";
		switch ( config.get_state_type() ) {
		case KRIPKE:
			if (config.is_kopt()) {
				result << "on KRIPKE with OPTIMIZED transition function";
			} else {
				result << "on KRIPKE with STANDARD transition function";
			}

			if (config.get_bisimulation() == PAIGE_TARJAN) {
				result << " and Paige-Tarjan Bisimulation";
			} else if (config.get_bisimulation() == FAST_BISIMULATION) {
				result << " and Fast Bismulation";
			}

			break;
		case POSSIBILITIES:
			result << "on POSS with STANDARD transition function";
			break;
		default:
			result << "on UNKNOWN with UNKNOWN transition function";
			break;
		}
		result << ") ";

		if (given_plan) {
			result << "with ";
		} else if (config.get_used_heur() != NO_H) {
			result << "with ";
		} else {
			switch (config.get_used_search()) {
                case DFS:
                    result << "with DFS ";
                    break;
                case ITER_DFS:
                    result << "with I_DFS ";
                    break;
                case BFS:
                default:
                    result << "with BFS ";
                    break;
			}
		}

		switch (config.get_used_heur()) {
            case S_PG:
                result << "SUM_PG heuristic ";
                break;
            case L_PG:
                result << "LENGTH_PG heuristic ";
                break;
            case C_PG:
                result << "CLASSIC_PG heuristic ";
                break;
            case SUB_GOALS:
                result << "SUBGOALS heuristic ";
                break;
            case NO_H:
            default: break;
		}

		if (config.is_check_visited()) {
			result << "and VISITED-STATE ";
		}

		result << "completed the search in " << elapsed_seconds.count() << "\n";
		result.close();
	} else {
		std::cout << "\ncompleted the search in " << elapsed_seconds.count() << "\n";
	}
}

template <class T>
bool planner<T>::search(const domain& domain) {
    const auto& config = domain.get_config();
    bool result;

    if (config.is_execute_given_actions()) {
        if (config.is_results_file()) {
            result = execute_given_actions_timed(domain);
        } else {
            result = execute_given_actions(domain);
        }
        std::cout << "\n\n\n*****THE END*****\n";
    } else {
        if (config.get_used_heur() == Heuristic::NO_H) {
            switch (config.get_used_search()) {
                case Search_Strategy::DFS: {
                    result = search_DFS(domain);
                    break;
                }
                case Search_Strategy::ITER_DFS: {
                    result = search_iterative_DFS(domain);
                    break;
                }
                case Search_Strategy::BFS:
                default: {
                    result = search_BFS(domain);
                    break;
                }
            }
        } else {
            result = search_heuristic(domain);
        }
        if (result) {
            std::cout << "\n\n\n*****THE END*****\n";
        } else {
            std::cout << "\n\n\n*****THE SAD END*****\n";
        }
    }
	return result;
}

template <class T>
bool planner<T>::search_BFS(const domain& domain) {
    const auto& config = domain.get_config();
	T initial;

	bool check_visited = config.is_check_visited();
	bool bisimulation = false;
	if (config.get_bisimulation() != Bisimulation_Algorithm::NO_BISIMULATION) {
		bisimulation = true;
	}
	bisimulation = false;
	std::set<T> visited_states;

	auto start_timing = std::chrono::system_clock::now();

    if (config.get_initial_state_mode() == Initial_State_Mode::FINITARY_S5_THEORY) {
        initial.generate_from_theory(domain.get_initial_description());
    }

    /**
     * \todo: IMPLEMENTARE CASO CON USER-PROVIDED CUSTOM INITIAL STATE
     */

	if (bisimulation) {
        initial.calc_min_bisimilar(NO_BISIMULATION);
	}
	auto end_timing = std::chrono::system_clock::now();
	std::chrono::duration<double> elapsed_seconds = end_timing - start_timing;

	std::cout << "\nInitial Built in " << elapsed_seconds.count() << " seconds" << std::endl;

    const action_set& actions = domain.get_actions();
    action_set::const_iterator it_acset;
    T popped_state;
    T tmp_state;
    action tmp_action;

    start_timing = std::chrono::system_clock::now();
    if (initial.is_goal(domain.get_goal_description())) {
        end_timing = std::chrono::system_clock::now();
        elapsed_seconds = end_timing - start_timing;
        print_results(domain, elapsed_seconds, initial, false);

        return true;
    }

    m_search_space.push(initial);
    if (check_visited) {
        visited_states.insert(initial);
    }


	while (!m_search_space.empty()) {
		popped_state = m_search_space.front();
		m_search_space.pop();

		for (it_acset = actions.begin(); it_acset != actions.end(); it_acset++) {
			tmp_action = *it_acset;

			if (popped_state.is_executable(tmp_action)) {

				tmp_state = popped_state.compute_succ(tmp_action);
				if (bisimulation) {
                    tmp_state.calc_min_bisimilar(NO_BISIMULATION);
				}

				if (tmp_state.is_goal(domain.get_goal_description())) {
					end_timing = std::chrono::system_clock::now();
					elapsed_seconds = end_timing - start_timing;
                    print_results(domain, elapsed_seconds, tmp_state, false);
					return true;
				}
				if (!check_visited || visited_states.insert(tmp_state).second) {
					m_search_space.push(tmp_state);
				}
			}
		}
	}
	std::cout << "\nNo plan found for this goal.\n";
	return false;
}

template <class T>
bool planner<T>::search_iterative_DFS(const domain& domain) {
    const auto& config = domain.get_config();
	std::stack<T> supportSearch; //stack di supporto per i risultati della ricerca.
	T initial;

	std::set<T> visited_states;

	auto start_timing = std::chrono::system_clock::now();

    if (config.get_initial_state_mode() == Initial_State_Mode::FINITARY_S5_THEORY) {
        initial.generate_from_theory(domain.get_initial_description());
    }

    /**
     * \todo: IMPLEMENTARE CASO CON USER-PROVIDED CUSTOM INITIAL STATE
     */

	if (config.get_bisimulation() != Bisimulation_Algorithm::NO_BISIMULATION) {
        initial.calc_min_bisimilar(NO_BISIMULATION);
	}

	int maxDepth = config.get_iter_dfs_max_depth();
	int step = config.get_iter_dfs_step();

	auto end_timing = std::chrono::system_clock::now();
	std::chrono::duration<double> elapsed_seconds = end_timing - start_timing;

	std::cout << "\nInitial Built in " << elapsed_seconds.count() << " seconds\n";

	action_set actions = domain.get_actions();
	action_set::const_iterator it_acset;
	T popped_state, popped_state2;
	T tmp_state;
	action tmp_action;

	start_timing = std::chrono::system_clock::now();
	if (initial.is_goal(domain.get_goal_description())) {
		end_timing = std::chrono::system_clock::now();
		elapsed_seconds = end_timing - start_timing;
        print_results(domain, elapsed_seconds, initial, false);

		return true;
	}

	m_search_space_DFS.push(initial);
	visited_states.insert(initial);

	while (true) {
		m_search_space_DFS.push(initial);
		maxDepth += step;
		visited_states.clear();
		visited_states.insert(initial);

		while (!m_search_space_DFS.empty()) {
			popped_state = m_search_space_DFS.top();
			m_search_space_DFS.pop();
			for (it_acset = actions.begin(); it_acset != actions.end(); it_acset++) {
				tmp_action = *it_acset;
				if (popped_state.is_executable(tmp_action)) {
					tmp_state = popped_state.compute_succ(tmp_action);
					//tmp_state.print();
					if (config.get_bisimulation() != Bisimulation_Algorithm::NO_BISIMULATION) {
                        tmp_state.calc_min_bisimilar(NO_BISIMULATION);
					}
					if (tmp_state.is_goal(domain.get_goal_description())) {
						end_timing = std::chrono::system_clock::now();
						elapsed_seconds = end_timing - start_timing;
                        print_results(domain, elapsed_seconds, tmp_state, false);
						return true;
					}

					if (visited_states.insert(tmp_state).second) {
						if (tmp_state.get_plan_length() < maxDepth) {
							m_search_space_DFS.push(tmp_state);
						}
					}
				}
			}

		}
	}
}

template <class T>
bool planner<T>::search_DFS(const domain& domain) {
    const auto& config = domain.get_config();
	T initial;

	std::set<T> visited_states;

	auto start_timing = std::chrono::system_clock::now();

    if (config.get_initial_state_mode() == Initial_State_Mode::FINITARY_S5_THEORY) {
        initial.generate_from_theory(domain.get_initial_description());
    }

    /**
     * \todo: IMPLEMENTARE CASO CON USER-PROVIDED CUSTOM INITIAL STATE
     */

	if (config.get_bisimulation() != Bisimulation_Algorithm::NO_BISIMULATION) {
        initial.calc_min_bisimilar(NO_BISIMULATION);
	}

	auto end_timing = std::chrono::system_clock::now();
	std::chrono::duration<double> elapsed_seconds = end_timing - start_timing;

	std::cout << "\nInitial Built in " << elapsed_seconds.count() << " seconds\n";

	action_set actions = domain.get_actions();
	action_set::const_iterator it_acset;
	T popped_state, popped_state2;
	T tmp_state;
	action tmp_action;

	start_timing = std::chrono::system_clock::now();
	if (initial.is_goal(domain.get_goal_description())) {
		end_timing = std::chrono::system_clock::now();
		elapsed_seconds = end_timing - start_timing;
        print_results(domain, elapsed_seconds, initial, false);

		return true;
	}

	m_search_space_DFS.push(initial);
	visited_states.insert(initial);
	while (!m_search_space_DFS.empty()) {
		popped_state = m_search_space_DFS.top();
		m_search_space_DFS.pop();

		//esecuzione azioni
		for (it_acset = actions.begin(); it_acset != actions.end(); it_acset++) {
			tmp_action = *it_acset;
			if (popped_state.is_executable(tmp_action)) {
				tmp_state = popped_state.compute_succ(tmp_action);
				//tmp_state.print();
				if (config.get_bisimulation() != Bisimulation_Algorithm::NO_BISIMULATION) {
                    tmp_state.calc_min_bisimilar(NO_BISIMULATION);
				}
				if (tmp_state.is_goal(domain.get_goal_description())) {
					end_timing = std::chrono::system_clock::now();
					elapsed_seconds = end_timing - start_timing;
                    print_results(domain, elapsed_seconds, tmp_state, false);
					return true;
				}

				if (visited_states.insert(tmp_state).second) {
					m_search_space_DFS.push(tmp_state);
				}
			}

		}
	}
	std::cout << "\nNo plan found for this goal.\n";
	return false;
}

template <class T>
bool planner<T>::search_heuristic(const domain& domain) {
    const auto& config = domain.get_config();

	T initial;
	bool check_visited = config.is_check_visited();
	std::set<T> visited_states;
	bool bisimulation = false;
	if (config.get_bisimulation() != NO_BISIMULATION) {
		bisimulation = true;
	}

	auto start_timing = std::chrono::system_clock::now();

    if (config.get_initial_state_mode() == Initial_State_Mode::FINITARY_S5_THEORY) {
        initial.generate_from_theory(domain.get_initial_description());
    }

    /**
     * \todo: IMPLEMENTARE CASO CON USER-PROVIDED CUSTOM INITIAL STATE
     */

	if (bisimulation) initial.calc_min_bisimilar(NO_BISIMULATION);
	auto end_timing = std::chrono::system_clock::now();
	std::chrono::duration<double> elapsed_seconds = end_timing - start_timing;

	std::cout << "\nInitial Built in " << elapsed_seconds.count() << " seconds\n";

	start_timing = std::chrono::system_clock::now();
	heuristics_manager h_manager(domain, config.get_used_heur());
	end_timing = std::chrono::system_clock::now();
	elapsed_seconds = end_timing - start_timing;
	std::cout << "\nHeuristic Built in " << elapsed_seconds.count() << " seconds\n";


	action_set actions = domain.get_actions();
	action_set::const_iterator it_acset;
	T popped_state;
	T tmp_state;
	action tmp_action;

	start_timing = std::chrono::system_clock::now();
	if (initial.is_goal(domain.get_goal_description())) {
		end_timing = std::chrono::system_clock::now();
		elapsed_seconds = end_timing - start_timing;
        print_results(domain, elapsed_seconds, initial, false);
		return true;
	}

	if (check_visited) {
		visited_states.insert(initial);
	}
	m_heur_search_space.push(initial);
	while (!m_heur_search_space.empty()) {
		popped_state = m_heur_search_space.top();
		//std::cerr << "\nDEBUG: Picked the state with heur = " << popped_state.get_heuristic_value() << "\n\n\n";
		m_heur_search_space.pop();

		for (it_acset = actions.begin(); it_acset != actions.end(); it_acset++) {
			tmp_action = *it_acset;
			if (popped_state.is_executable(tmp_action)) {
				tmp_state = popped_state.compute_succ(tmp_action);
				if (bisimulation) {
                    tmp_state.calc_min_bisimilar(NO_BISIMULATION);
				}
				if (tmp_state.is_goal(domain.get_goal_description())) {
					end_timing = std::chrono::system_clock::now();
					elapsed_seconds = end_timing - start_timing;
                    print_results(domain, elapsed_seconds, tmp_state, false);
					return true;
				}
				if (!check_visited || visited_states.insert(tmp_state).second) {
					h_manager.set_heuristic_value(tmp_state);

					if ((tmp_state.get_heuristic_value() >= 0) || (config.get_used_heur() == C_PG)) {
						//std::cerr << "\nDEBUG: Generated a state with heur = " << tmp_state.get_heuristic_value();
						m_heur_search_space.push(tmp_state);
					}
				}
			}
		}
	}
	std::cout << "\n\nIt does not exists any Plan for this domain instance:(\n";
	return false;
}

template <class T>
bool planner<T>::execute_given_actions(const domain& domain) {
    const auto& config = domain.get_config();
	check_actions_names(domain);
	std::set<T> visited_states;

	T state;
    state.generate_from_theory(domain.get_initial_description());
    bool found_plan;

	if (state.is_goal(domain.get_goal_description())) {
		std::cout << "\nInitial was Goal:)\n";
        found_plan = true;
	} else {
		std::cout << "\n\n";
	}

	std::string bis_postfix;
	if (config.get_bisimulation() != Bisimulation_Algorithm::NO_BISIMULATION) {
        state.calc_min_bisimilar(NO_BISIMULATION);
		bis_postfix = "__b";
	}
	if (config.is_debug()) {
		state.print_graphviz(bis_postfix);
	}
	// DEBUG
	visited_states.insert(state);

    auto given_actions = config.get_given_actions();
    auto action_library = domain.get_actions();

	std::vector<std::string>::const_iterator it_stset;
	std::vector<std::string>::const_iterator it_stset2;

	action_set::const_iterator it_acset;
	for (it_stset = given_actions.begin(); it_stset != given_actions.end(); it_stset++) {
		for (it_acset = action_library.begin(); it_acset != action_library.end(); it_acset++) {
			if (it_acset->get_name() == *it_stset) {
				if (state.is_executable(*it_acset)) {
					state = state.compute_succ(*it_acset);
					if (config.get_bisimulation() != Bisimulation_Algorithm::NO_BISIMULATION) {
                        state.calc_min_bisimilar(NO_BISIMULATION);
					}
					if (config.is_debug()) {
						state.print_graphviz(bis_postfix);
					}
					if (state.is_goal(domain.get_goal_description())) {
						std::cout << "\n\nWell Done, Goal found after the execution of ";
						for (it_stset2 = given_actions.begin(); it_stset2 != it_stset; it_stset2++) {
							std::cout << *it_stset2 << ", ";
						}
						std::cout << *it_stset << " :)\n\n\n";
                        found_plan = true;
					}
				} else {
					std::cout << "The action " << (*it_acset).get_name() << " was not executable\n";
					return false;
				}
			}
		}
	}

	if (config.is_debug()) {
		std::cout << "\nGenerating the graphical representation of the states ...\n";

		std::string name_folder_graphviz = "out/state/";
		name_folder_graphviz += config.get_domain_name();
		switch ( config.get_state_type() ) {
            case KRIPKE:
                name_folder_graphviz += "_kripke";
                if (config.is_kopt()) {
                    name_folder_graphviz += "_opt";
                }
                break;
            case POSSIBILITIES:
                name_folder_graphviz += "_poss";
                break;
            default:
                name_folder_graphviz += "_unknown";
                break;
		}
		system(("sh ../scripts/generate_pdf.sh " + name_folder_graphviz).c_str());
	}
    return found_plan;
}

template <class T>
/**\todo just for confrontation with old*/
bool planner<T>::execute_given_actions_timed(const domain& domain) {
    const auto& config = domain.get_config();
	check_actions_names(domain);

	T state;
    // todo: sistema
//    state.generate_from_theory(domain.get_initial_description());

	std::vector<std::string>::const_iterator it_stset;
	action_set::const_iterator it_acset;

	auto start_timing = std::chrono::system_clock::now();

	for (it_stset = config.get_given_actions().begin(); it_stset != config.get_given_actions().end(); it_stset++) {
		for (it_acset = domain.get_actions().begin(); it_acset != domain.get_actions().end(); it_acset++) {
			if (it_acset->get_name() == *it_stset) {
				if (state.is_executable(*it_acset)) {
					state = state.compute_succ(*it_acset);
				}
			}
		}
	}

	auto end_timing = std::chrono::system_clock::now();
	std::chrono::duration<double> elapsed_seconds = end_timing - start_timing;

    print_results(domain, elapsed_seconds, state, true);
    return state.is_goal(domain.get_goal_description());
}

template <class T>
/**\todo just for confrontation with old*/
void planner<T>::check_actions_names(const domain& domain) {
    const auto& config = domain.get_config();

	string_set domain_act;
	action_set::const_iterator it_acset;
	std::vector<std::string>::const_iterator it_stset;

	for (it_acset = domain.get_actions().begin(); it_acset != domain.get_actions().end(); it_acset++) {
		domain_act.insert(it_acset->get_name());
	}


	for (it_stset = config.get_given_actions().begin(); it_stset != config.get_given_actions().end(); it_stset++) {
		if (domain_act.find(*it_stset) == domain_act.end()) {
			std::cerr << "\nERROR in giving the action list: the action " << *it_stset << " does not exist.\n";
			exit(1);
		}
	}
}

/*\ IMPLEMENTATION OF OTHER TEMPLATIC FUNCTIONS INSTANCIATED WITH A TEMPLATIC STATE*/
#include "../heuristics/heuristics_template.ipp"
//#include "../heuristics/planning_graph_template.ipp"
