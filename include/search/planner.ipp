/**
 * \brief Implementation of the \ref planner.h.
 *
 * \copyright GNU Public License.
 *
 * \author Francesco Fabiano.
 * \date May 6, 2019
 */
#include <algorithm>
#include "planner.h"

// when running parallel, [heur] is printed at the start of each line so that it can easily be seen what heuristical thread is printing what output.
// this function just takes a ptype and heuristic, and returns "[heur] " if the ptype is P_CHILD, or "" otherwise.
std::string child_string(parallel_type ptype, heuristics used_heur){
	if(ptype!=P_CHILD) return "";
	std::string heuristic_names[5] = {"[NO_H] ","[L_PG] ","[S_PG] ","[C_PG] ","[SUBGOALS] "};
	return heuristic_names[used_heur];
}

template <class T>
void planner<T>::print_results(std::chrono::duration<double> elapsed_seconds, int expanded_nodes, T goal, bool results_file, bool givenplan, search_type used_search, heuristics used_heur, parallel_type ptype)
{
	std::string child_notice = "\n\n\n";
	child_notice = child_notice + child_string(ptype,used_heur);

	std::cout << child_notice +"Well Done, Goal found in " << elapsed_seconds.count() << " with " << expanded_nodes << " expanded nodes" << " :)\n";
	goal.print(ptype,used_heur);
	std::cout << "\n\nExpanded Nodes: " << expanded_nodes << std::endl;

	if (results_file) {
		std::ofstream result;
		std::string folder = "out/EFP_comparison/";
		if (givenplan) {
			folder = folder + "givenplan/";
		} else {
			folder = folder + "findingplan/";
		}
		system(("mkdir -p " + folder).c_str());
		std::string file_extension = ".txt";
		if(ptype == P_CHILD) { //if using parallelism, modify the result file's name to be unique for each item.
			std::string file_details = "_parallel_heur" + std::to_string(used_heur);
			file_extension = file_details + file_extension;
		}
		result.open(folder + domain::get_instance().get_name() + file_extension, std::ofstream::out | std::ofstream::app);
		result << "EFP Version 2.0 (";
		switch ( domain::get_instance().get_stype() ) {
		case KRIPKE:
			if (domain::get_instance().get_k_optimized()) {
				result << "on KRIPKE with OPTIMIZED transition function";
			} else {
				result << "on KRIPKE with STANDARD transition function";
			}

			if (domain::get_instance().get_bisimulation() == PaigeTarjan) {
				result << " and Paige-Tarjan Bisimulation";
			} else if (domain::get_instance().get_bisimulation() == FastBisimulation) {
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

		if (givenplan) {
			result << "with ";
		} else if (used_heur != NO_H) {
			result << "with ";
		} else {
			switch ( used_search ) {
			case DFS:
				result << "with DFS ";
				break;
			case I_DFS:
				result << "with I_DFS ";
				break;
			case BFS:
			default:
				result << "with BFS ";
				break;
			}
		}



		switch ( used_heur ) {
		case S_PG:
			result << "SUM_PG heuristic ";
			break;
		case L_PG:
			result << "LENGTH_PG heuristic ";
			break;
		case C_PG:
			result << "CLASSIC_PG heuristic ";
			break;
		case SUBGOALS:
			result << "SUBGOALS heuristic ";
			break;
		case NO_H:
		default: break;
		}

		if (domain::get_instance().check_visited()) {
			result << "and VISITED-STATE ";
		}

		result << "completed the search in " << elapsed_seconds.count() << "\n";
		result.close();
	} else {
		std::cout << "\ncompleted the search in " << elapsed_seconds.count() << "\n";
	}
}

template <class T>
bool planner<T>::search(bool results_file, parallel_input pin, heuristics used_heur, search_type used_search, ML_Dataset_Params ML_dataset, short IDFS_d, short IDFS_s)
{
	if(ML_dataset.generate){ML_dataset_creation(&ML_dataset);}
	else
		if(pin.ptype == P_SERIAL || pin.ptype == P_CHILD){
			if (used_heur == NO_H) {
				switch ( used_search ) {
				case DFS:
					return search_DFS(results_file, pin.ptype);
				case I_DFS:
					return search_IterativeDFS(results_file, IDFS_d, IDFS_s, pin.ptype);
				case BFS:
				default:
					return search_BFS(results_file, pin.ptype);
				}
			} else {
				return search_heur(results_file, used_heur, pin.ptype);
			}
		} else {
			return parallel_search(results_file, pin, used_heur, used_search, IDFS_d, IDFS_s);
		}

	//non arrivo mai qui.
	return false;
}

template <class T>
static void* my_search_thread(void *args)
{
	pthread_params *params = (pthread_params *)args;
	planner<T> tmpObj;
	ML_Dataset_Params tmp_ml;
	
	tmpObj.search(params->results_file, params->pin, params->used_heur, params->used_search, tmp_ml, params->IDFS_d, params->IDFS_s);
}

template <class T>
bool planner<T>::parallel_search(bool results_file, parallel_input pin, heuristics used_heur, search_type used_search, short IDFS_d, short IDFS_s)
{
	int num_heuristics = 5;
	heuristics heur_list[num_heuristics]   = { NO_H ,  L_PG ,  S_PG ,  C_PG ,  SUBGOALS };
	std::string heur_names[num_heuristics] = {"NO_H", "L_PG", "S_PG", "C_PG", "SUBGOALS"};
	parallel_type ptype = pin.ptype;
	bool pwait = pin.pwait;
	
	if(ptype == P_PTHREAD){
		pthread_t tid[num_heuristics];
		pthread_params *params;
		params = (pthread_params*)malloc(num_heuristics*sizeof(pthread_params));
		for(int index = 0; index<num_heuristics ; index++){
			//set up the parameter structure for this thread
			params[index].results_file = results_file;
			params[index].pin.ptype = P_CHILD;
			params[index].used_heur = heur_list[index];
			params[index].used_search = used_search;
			params[index].IDFS_d = IDFS_d;
			params[index].IDFS_s = IDFS_s;

			int isBroken = pthread_create(&tid[index], NULL, my_search_thread<T>, &params[index]);
			if( !isBroken ){
				std::cout << "created thread tid: ["<< tid[index] <<"] for heuristic: ["<< heur_names[index] <<"]" << std::endl;
			} else {
				std::cout << "Failed to spawn pthread in parallel_search()" << std::endl;
				return false;
			}
		}

		//wait for all threads to terminate.
		if(pwait)
			for(int index=0;index<num_heuristics;index++)
				pthread_join(tid[index],NULL);
		//loop until one thread terminates.
		else{
			int running = 1;
			while(running!=0){
				for(int index=0;index<num_heuristics;index++){
					running = pthread_tryjoin_np(tid[index], NULL);
					if(running==0) break;
				}
			}
			for(int index=0 ; index < num_heuristics ; index++)
				pthread_cancel(tid[index]);
		}
		
		
	} else if(ptype == P_FORK){
		int fork_id = -1;
		int pids[num_heuristics];
		for(int index = 0; index<num_heuristics ; index++){
			//attempt to create a forked process.
			if((fork_id = fork()) < 0){
				std::cout << "Failed to fork process in parallel_search()" << std::endl;
				return false;
			}

			//parent saves child id
			if(fork_id){ 
				std::cout << "Forked process pid: ["<< fork_id <<"] for heuristic: ["<< heur_names[index] <<"]" << std::endl;
				pids[index]=fork_id; 
			}

			 //child launches search serially
			else{
				parallel_input pin_child;
				pin_child.ptype=P_CHILD;
				ML_Dataset_Params tmp_ml;
				search(results_file, pin_child, heur_list[index], used_search, tmp_ml, IDFS_d, IDFS_s);
				exit(0);
			}
		}
		//wait for all children to terminate
		if(pwait)
			for(int index=0 ; index < num_heuristics ; index++)
				waitpid(pids[index],NULL,0);
		//loop until one child terminates, then terminate them all.
		else{
			waitpid(0,NULL,0);
			for(int index=0 ; index < num_heuristics ; index++)
				kill(pids[index], SIGTERM);

		}

	} else { //this should never occur. 
		std::cout << "Illegal ptype in parallel_search()" << std::endl;
		return false;
	}

	return true;
}

template <class T>
bool planner<T>::search_BFS(bool results_file, parallel_type ptype)
{

	int expanded_nodes = 0;

	T initial;
	bool check_visited = domain::get_instance().check_visited();
	bool bisimulation = false;
	if (domain::get_instance().get_bisimulation() != BIS_NONE) {
		bisimulation = true;
	}
	bisimulation = false;
	std::set<T> visited_states;

	auto start_timing = std::chrono::system_clock::now();

	initial.build_initial();

	if (bisimulation) {
		initial.calc_min_bisimilar();
	}
	auto end_timing = std::chrono::system_clock::now();
	std::chrono::duration<double> elapsed_seconds = end_timing - start_timing;

	std::cout << "\n"+child_string(ptype,NO_H)+"Initial Built in " << elapsed_seconds.count() << " seconds" << std::endl;

	action_set actions = domain::get_instance().get_actions();
	action_set::const_iterator it_acset;
	T popped_state;
	T tmp_state;
	action tmp_action;

	start_timing = std::chrono::system_clock::now();
	if (initial.is_goal()) {
		end_timing = std::chrono::system_clock::now();
		elapsed_seconds = end_timing - start_timing;
		print_results(elapsed_seconds, expanded_nodes, initial, results_file, false, BFS, NO_H, ptype);

		return true;
	}

	m_search_space.push(initial);
	if (check_visited) {
		visited_states.insert(initial);
	}
			
	while (!m_search_space.empty()) {
		popped_state = m_search_space.front();
		m_search_space.pop();
		expanded_nodes++;

		//std::cerr << "\nDEBUG: Plan length is " << popped_state.get_plan_length();
		for (it_acset = actions.begin(); it_acset != actions.end(); it_acset++) {
			tmp_action = *it_acset;

			if (popped_state.is_executable(tmp_action)) {

				tmp_state = popped_state.compute_succ(tmp_action);
				//tmp_state.print();
				if (bisimulation) {
					tmp_state.calc_min_bisimilar();
				}

				if (tmp_state.is_goal()) {
					end_timing = std::chrono::system_clock::now();
					elapsed_seconds = end_timing - start_timing;
					print_results(elapsed_seconds, expanded_nodes, tmp_state, results_file, false, BFS, NO_H,ptype);
					return true;
				}
				if (!check_visited || visited_states.insert(tmp_state).second) {
					m_search_space.push(tmp_state);
				}
			} /*else {
				if (tmp_state.get_plan_length() == count) {
					std::cerr << "\nDEBUG: Plan length is " << count;
					count++;
				}
			}*/

		}

	}
	std::cout << "\nNo plan found for this goal.\n";
	return false;
}

template <class T>
bool planner<T>::search_IterativeDFS(bool results_file, short maxDepth_, short step_, parallel_type ptype)
{
	int expanded_nodes = 0;
	//stack di supporto per i risultati della ricerca.
	std::stack<T> supportSearch;
	T initial;

	bool bisimulation = false;
	if (domain::get_instance().get_bisimulation() != BIS_NONE) {
		bisimulation = true;
	}
	bisimulation = false;
	std::set<T> visited_states;

	auto start_timing = std::chrono::system_clock::now();
	initial.build_initial();
	if (bisimulation) {
		initial.calc_min_bisimilar();
	}

	int maxDepth = maxDepth_;
	int step = step_;

	auto end_timing = std::chrono::system_clock::now();
	std::chrono::duration<double> elapsed_seconds = end_timing - start_timing;

	std::cout << "\nInitial Built in " << elapsed_seconds.count() << " seconds\n";

	action_set actions = domain::get_instance().get_actions();
	action_set::const_iterator it_acset;
	T popped_state, popped_state2;
	T tmp_state;
	action tmp_action;

	start_timing = std::chrono::system_clock::now();
	if (initial.is_goal()) {
		end_timing = std::chrono::system_clock::now();
		elapsed_seconds = end_timing - start_timing;
		print_results(elapsed_seconds, expanded_nodes, initial, results_file, false, I_DFS, NO_H,ptype);

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
			expanded_nodes++;

			for (it_acset = actions.begin(); it_acset != actions.end(); it_acset++) {
				tmp_action = *it_acset;
				if (popped_state.is_executable(tmp_action)) {
					tmp_state = popped_state.compute_succ(tmp_action);
					//tmp_state.print();
					if (bisimulation) {
						tmp_state.calc_min_bisimilar();
					}
					if (tmp_state.is_goal()) {
						end_timing = std::chrono::system_clock::now();
						elapsed_seconds = end_timing - start_timing;
						print_results(elapsed_seconds, expanded_nodes, tmp_state, results_file, false, I_DFS, NO_H,ptype);
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
	//anche qui non ci arrivo mai con il while true
	std::cout << "\nNo plan found for this goal.\n";
	return false;
}

template <class T>
bool planner<T>::search_DFS(bool results_file, parallel_type ptype)
{
	int expanded_nodes = 0;
	T initial;

	bool bisimulation = false;
	if (domain::get_instance().get_bisimulation() != BIS_NONE) {
		bisimulation = true;
	}
	bisimulation = false;
	std::set<T> visited_states;

	auto start_timing = std::chrono::system_clock::now();
	initial.build_initial();
	if (bisimulation) {
		initial.calc_min_bisimilar();
	}

	auto end_timing = std::chrono::system_clock::now();
	std::chrono::duration<double> elapsed_seconds = end_timing - start_timing;

	std::cout << "\nInitial Built in " << elapsed_seconds.count() << " seconds\n";

	action_set actions = domain::get_instance().get_actions();
	action_set::const_iterator it_acset;
	T popped_state, popped_state2;
	T tmp_state;
	action tmp_action;

	start_timing = std::chrono::system_clock::now();
	if (initial.is_goal()) {
		end_timing = std::chrono::system_clock::now();
		elapsed_seconds = end_timing - start_timing;
		print_results(elapsed_seconds, expanded_nodes, initial, results_file, false, DFS, NO_H,ptype);

		return true;
	}

	m_search_space_DFS.push(initial);
	visited_states.insert(initial);
	while (!m_search_space_DFS.empty()) {
		popped_state = m_search_space_DFS.top();
		m_search_space_DFS.pop();
		expanded_nodes++;

		//esecuzione azioni
		for (it_acset = actions.begin(); it_acset != actions.end(); it_acset++) {
			tmp_action = *it_acset;
			if (popped_state.is_executable(tmp_action)) {
				tmp_state = popped_state.compute_succ(tmp_action);
				//tmp_state.print();
				if (bisimulation) {
					tmp_state.calc_min_bisimilar();
				}
				if (tmp_state.is_goal()) {
					end_timing = std::chrono::system_clock::now();
					elapsed_seconds = end_timing - start_timing;
					print_results(elapsed_seconds, expanded_nodes, tmp_state, results_file, false, DFS, NO_H,ptype);
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
bool planner<T>::search_heur(bool results_file, heuristics used_heur, parallel_type ptype)
{
	int expanded_nodes = 0;

	T initial;
	bool check_visited = domain::get_instance().check_visited();
	std::set<T> visited_states;
	bool bisimulation = false;
	if (domain::get_instance().get_bisimulation() != BIS_NONE) {
		bisimulation = true;
	}

	auto start_timing = std::chrono::system_clock::now();
	initial.build_initial();
	if (bisimulation) initial.calc_min_bisimilar();
	auto end_timing = std::chrono::system_clock::now();
	std::chrono::duration<double> elapsed_seconds = end_timing - start_timing;

	std::cout << "\n"+child_string(ptype,used_heur)+"Initial Built in " << elapsed_seconds.count() << " seconds\n";

	start_timing = std::chrono::system_clock::now();
	heuristics_manager h_manager(used_heur, initial);
	end_timing = std::chrono::system_clock::now();
	elapsed_seconds = end_timing - start_timing;
	std::cout << "\n"+child_string(ptype,used_heur)+"Heuristic Built in " << elapsed_seconds.count() << " seconds\n";


	action_set actions = domain::get_instance().get_actions();
	action_set::const_iterator it_acset;
	T popped_state;
	T tmp_state;
	action tmp_action;

	start_timing = std::chrono::system_clock::now();
	if (initial.is_goal()) {
		end_timing = std::chrono::system_clock::now();
		elapsed_seconds = end_timing - start_timing;
		print_results(elapsed_seconds, expanded_nodes, initial, results_file, false, BFS, used_heur,ptype);
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
		expanded_nodes++;

		for (it_acset = actions.begin(); it_acset != actions.end(); it_acset++) {
			tmp_action = *it_acset;
			if (popped_state.is_executable(tmp_action)) {
				tmp_state = popped_state.compute_succ(tmp_action);
				if (bisimulation) {
					tmp_state.calc_min_bisimilar();
				}
				if (tmp_state.is_goal()) {
					end_timing = std::chrono::system_clock::now();
					elapsed_seconds = end_timing - start_timing;
					print_results(elapsed_seconds, expanded_nodes, tmp_state, results_file, false, BFS, used_heur,ptype);
					return true;
				}
				if (!check_visited || visited_states.insert(tmp_state).second) {
					h_manager.set_heuristic_value(tmp_state);

					if ((tmp_state.get_heuristic_value() >= 0) || (used_heur == C_PG)) {
						//std::cerr << "\nDEBUG: Generated a state with heur = " << tmp_state.get_heuristic_value();
						m_heur_search_space.push(tmp_state);
					}
				}
			}
		}
	}
	std::cout << "\n\n"+child_string(ptype,used_heur)+"It does not exists any Plan for this domain instance:(\n";
	return false;
}

template <class T>
void planner<T>::execute_given_actions(std::vector<std::string>& act_name)
{
	check_actions_names(act_name);
	// DEBUG
	std::set<T> visited_states;

	bool bisimulation = false;
	if (domain::get_instance().get_bisimulation() != BIS_NONE) {
		bisimulation = true;
	}
	T state;
	state.build_initial();

	if (state.is_goal()) {
		std::cout << "\nInitial was Goal:)\n";
	} else {
		std::cout << "\n\n";
	}

	std::string bis_postfix = "";
	if (bisimulation) {
		state.calc_min_bisimilar();
		bis_postfix = "__b";
	}
	if (domain::get_instance().get_debug()) {
		state.print_graphviz(bis_postfix);

	}
	// DEBUG
	visited_states.insert(state);

	std::vector<std::string>::const_iterator it_stset;
	std::vector<std::string>::const_iterator it_stset2;

	action_set::const_iterator it_acset;
	for (it_stset = act_name.begin(); it_stset != act_name.end(); it_stset++) {
		for (it_acset = domain::get_instance().get_actions().begin(); it_acset != domain::get_instance().get_actions().end(); it_acset++) {
			if ((*it_acset).get_name().compare(*it_stset) == 0) {


				if (state.is_executable(*it_acset)) {
					state = state.compute_succ(*it_acset);
					if (bisimulation) {
						state.calc_min_bisimilar();
					}
					if (domain::get_instance().get_debug()) {
						state.print_graphviz(bis_postfix);
					}

					// DEBUG
					//					if (!visited_states.insert(state).second) {
					//						for (T tmp : visited_states) {
					//							if (!(tmp < state) && !(state < tmp) && !domain::get_instance().get_debug()) {
					//								state.min_with_print(tmp);
					//							}
					//
					//							//std::cerr << "\nDEBUG: reached already visited state with action " << (*it_acset).get_name() << "\n";
					//
					//							// if ((*it_acset).get_name() == "shout_8") {
					//							// 	state.print_graphviz(bis_postfix);
					//							// }
					//						}
					//					}

					if (state.is_goal()) {
						std::cout << "\n\nWell Done, Goal found after the execution of ";
						for (it_stset2 = act_name.begin(); it_stset2 != it_stset; it_stset2++) {
							std::cout << *it_stset2 << ", ";
						}
						std::cout << *it_stset << " :)\n\n\n";
					}
				} else {
					std::cout << "The action " << (*it_acset).get_name() << " was not executable\n";
					return;
				}
			}
		}
	}

	if (domain::get_instance().get_debug()) {
		std::cout << "\nGenerating the graphical representation of the states ...\n";

		std::string name_folder_graphviz = "out/state/";
		name_folder_graphviz += domain::get_instance().get_name();
		switch ( domain::get_instance().get_stype() ) {
		case KRIPKE:
			name_folder_graphviz += "_kripke";
			if (domain::get_instance().get_k_optimized()) {
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
		system(("sh scripts/generate_pdf.sh " + name_folder_graphviz).c_str());
	}

	return;
}

template <class T>
/**\todo just for confrontation with old*/
void planner<T>::execute_given_actions_timed(std::vector<std::string>& act_name, parallel_type ptype)
{
	check_actions_names(act_name);

	T state;
	state.build_initial();

	std::vector<std::string>::const_iterator it_stset;
	action_set::const_iterator it_acset;

	auto start_timing = std::chrono::system_clock::now();

	for (it_stset = act_name.begin(); it_stset != act_name.end(); it_stset++) {
		for (it_acset = domain::get_instance().get_actions().begin(); it_acset != domain::get_instance().get_actions().end(); it_acset++) {
			if ((*it_acset).get_name().compare(*it_stset) == 0) {
				//std::cout << "\n\nTrying action " << (*it_acset).get_name() << "\n";
				if (state.is_executable(*it_acset)) {
					state = state.compute_succ(*it_acset);
				}
			}
		}
	}

	auto end_timing = std::chrono::system_clock::now();
	std::chrono::duration<double> elapsed_seconds = end_timing - start_timing;

	print_results(elapsed_seconds, (sizeof(act_name)/sizeof(act_name[0])), state, true, true, BFS);

	return;
}

template <class T>
/**\todo just for confrontation with old*/
void planner<T>::check_actions_names(std::vector<std::string>& act_name)
{

	string_set domain_act;
	action_set::const_iterator it_acset;

	std::vector<std::string>::iterator it_stset;

	for (it_acset = domain::get_instance().get_actions().begin(); it_acset != domain::get_instance().get_actions().end(); it_acset++) {
		domain_act.insert(it_acset->get_name());
	}

	for (it_stset = act_name.begin(); it_stset != act_name.end(); it_stset++) {

		(*it_stset).erase(std::remove((*it_stset).begin(), (*it_stset).end(), ','), (*it_stset).end());

		if (domain_act.find(*it_stset) == domain_act.end()) {
			std::cerr << "\nERROR in giving the action list: the action " << *it_stset << " does not exist.\n";
			exit(1);
		}
	}
}

template <class T>
bool planner<T>::ML_dataset_creation(ML_Dataset_Params* ML_dataset) {
    std::string folder = "out/ML_HEUR_datasets/";
    folder += (ML_dataset->useDFS) ? "DFS/" : "BFS/";
    system(("mkdir -p " + folder).c_str());

    std::string fname = domain::get_instance().get_name() + "_d_" + std::to_string(ML_dataset->depth) + ".csv";
    std::string fpath = folder + fname;

    std::ofstream result(fpath);
    if (!result.is_open()) {
        std::cerr << "Error opening file: " << fpath << std::endl;
        return false;
    }
    result << "Path Hash,Path Mapped,Depth,Distance From Goal,Goal" << std::endl;
    result.close();

/*     auto goal_list = domain::get_instance().get_goal_description();

    std::stringstream buffer;
    auto original_buf = std::cout.rdbuf(buffer.rdbuf());

    bool first = true;
    for (auto& goal : goal_list) {
        if (!first) std::cout << " AND ";
        first = false;
        goal.print();
    }
    std::cout.rdbuf(original_buf);
    std::string goal_str = buffer.str(); */

	std::string goal_dot_file = domain::get_instance().get_name() + "_goal_tree.dot";
	std::string goal_file_path =  folder + goal_dot_file;
	generate_goal_tree(goal_file_path);

    return dataset_launcher(fpath, ML_dataset->depth, ML_dataset->useDFS, goal_file_path);
}

template <class T>
int planner<T>::get_id_from_map(const std::map<boost::dynamic_bitset<>, int>& id_map, const boost::dynamic_bitset<>& key, const std::string& type_name) {
    auto it = id_map.find(key);
    if (it != id_map.end()) {
        return it->second;
    } else {
        throw std::runtime_error(type_name + " not found in map");
    }
}

template <class T>
void planner<T>::populate_ids_from_bitset(const std::set<boost::dynamic_bitset<>>& keys_set, std::map<boost::dynamic_bitset<>, int>& id_map, int start_id) {
	int current_id = start_id;
	for (const auto& key : keys_set) {
        id_map[key] = current_id++;
    }
}

template <class T>
int planner<T>::get_unique_f_id_from_map(fluent fl) {
    return get_id_from_map(m_fluent_to_id, fl, "Fluent");
}

template <class T>
int planner<T>::get_unique_a_id_from_map(agent ag) {
    return get_id_from_map(m_agent_to_id, ag, "Agent");
}

template <class T>
void planner<T>::populate_fluent_ids(int start_id) {
    populate_ids_from_bitset(domain::get_instance().get_fluents(), m_fluent_to_id, start_id);
}

template <class T>
void planner<T>::populate_agent_ids(int start_id) {
    populate_ids_from_bitset(domain::get_instance().get_agents(), m_agent_to_id, start_id);
}

template <class T>
const std::string & planner<T>::generate_goal_tree(const std::string & goal_file_name) {
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

template <class T>
void planner<T>::print_goal_subtree(const belief_formula & to_print, int goal_counter, int & next_id, const std::string & parent_node, std::ofstream& dot_file) {


	int current_node_id = ++next_id;
	std::string node_name;

	switch (to_print.get_formula_type()) {
		case FLUENT_FORMULA: {
			std::string m_parent_node = parent_node;
			if  (to_print.get_fluent_formula().size() > 1) {
				node_name = "F_OR" + std::to_string(current_node_id);
				current_node_id = ++next_id;
				//dot_file << "  " << node_name << " [label=\"" << current_node_id << "\"];\n";
				dot_file << "  " << parent_node << " -> " << node_name << " [label=\"" << goal_counter << "\"];\n";
				m_parent_node = node_name;
			}

			for (const auto& fls_set : to_print.get_fluent_formula()) {
					
				std::string m_m_parent_node = m_parent_node;

				if  (fls_set.size() > 1) {
					node_name = "F_AND" + std::to_string(current_node_id);
					current_node_id = ++next_id;
					//dot_file << "  " << node_name << " [label=\"" << current_node_id << "\"];\n";
					dot_file << "  " << m_parent_node << " -> " << node_name << " [label=\"" << goal_counter << "\"];\n";
					m_m_parent_node = node_name;
				}

				for (const auto& fl : fls_set) {
					dot_file << "  " << m_m_parent_node << " -> F" << get_unique_f_id_from_map(fl) << " [label=\"" << goal_counter << "\"];\n";
				} 
			}
			break;
		}

		case BELIEF_FORMULA: {
			node_name = "B" + std::to_string(current_node_id);
			//dot_file << "  " << node_name << " [label=\"" << current_node_id << "\"];\n";
			dot_file << "  " << parent_node << " -> " << node_name << " [label=\"" << goal_counter << "\"];\n";
			dot_file << "  " << node_name << " -> A" << get_unique_a_id_from_map(to_print.get_agent()) << " [label=\"" << goal_counter << "\"];\n";
			dot_file << "  A" << get_unique_a_id_from_map(to_print.get_agent()) << " -> " << node_name << " [label=\"" << goal_counter << "\"];\n";

			print_goal_subtree(to_print.get_bf1(), goal_counter, next_id, node_name,dot_file);
			break;
		}

		case C_FORMULA: {
			node_name = "C" + std::to_string(current_node_id);
			//dot_file << "  " << node_name << " [label=\"" << current_node_id << "\"];\n";
			dot_file << "  " << parent_node << " -> " << node_name << " [label=\"" << goal_counter << "\"];\n";

			for (const auto& ag : to_print.get_group_agents()) {
				dot_file << "  " << node_name << " -> A" << get_unique_a_id_from_map(ag) << " [label=\"" << goal_counter << "\"];\n";
				dot_file << "  A" << get_unique_a_id_from_map(ag) << " -> " << node_name << " [label=\"" << goal_counter << "\"];\n";
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

			node_name = node_name + std::to_string(current_node_id);
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

template <class T>
bool planner<T>::dataset_launcher(const std::string& fpath, int max_depth, bool useDFS, const std::string& goal_str) {
    T initial;
    initial.build_initial();

    bool bisimulation = (domain::get_instance().get_bisimulation() != BIS_NONE);

    if (bisimulation) {
        initial.calc_min_bisimilar();
    }

    action_set actions = domain::get_instance().get_actions();

    auto start_time = std::chrono::system_clock::now();

    std::vector<std::string> global_dataset;
    global_dataset.reserve(m_threshold_node_generation_ML);

    bool result;
    if (useDFS) {
        result = dataset_DFS_serial(initial, max_depth, &actions, goal_str, global_dataset, bisimulation);
    } else {
        std::cerr << "Recursion through BFS is not implemented yet for ML dataset generation.\n";
        exit(1);
    }

    auto end_time = std::chrono::system_clock::now();
    std::chrono::duration<double> elapsed = end_time - start_time;
    std::cout << "\nDataset Generated in " << elapsed.count() << " seconds" << std::endl;

    std::ofstream result_file(fpath, std::ofstream::app);
    for (const auto& row : global_dataset) {
        result_file << row << "\n";
    }
    result_file.close();

    return result;
}


template <class T>
bool planner<T>::dataset_DFS_serial(T& initial_state, int max_depth, action_set* actions, const std::string& goal_str, std::vector<std::string>& global_dataset, bool bisimulation) {
    
    m_visited_states_ML.clear();

    int branching_factor = actions->size();
	if (branching_factor <= 1) {
        m_total_possible_nodes_log_ML = log(max_depth+1);
    }
	else{
		// Calculate expected log of total nodes
		double numerator_log = (max_depth + 1) * std::log(branching_factor);
		double denominator_log = std::log(branching_factor - 1);
		m_total_possible_nodes_log_ML = numerator_log - denominator_log;
	}

	
	std::cout << "Total possible nodes exceed threshold." << std::endl;
	std::cout << "Approximate number of nodes (exp(log)) = " << std::exp(m_total_possible_nodes_log_ML) << std::endl;
	std::cout << "Threshold number of nodes = " << m_threshold_node_generation_ML << std::endl;
	if (m_total_possible_nodes_log_ML > m_threshold_node_generation_log_ML) {
		std::cout << "Decision: using SPARSE DFS." << std::endl;
	} else {
		std::cout << "Decision: using COMPLETE DFS." << std::endl;
	}

    dataset_DFS_worker(initial_state, 0, max_depth, actions, goal_str, global_dataset, bisimulation);

	if (m_goal_founds_ML > 0) {
		std::cout << "Number of goals found: "<< m_goal_founds_ML << std::endl;
	} else {
		std::cout << "No goals found, this is not a good training set (recreate it)." << std::endl;
	}

    return !global_dataset.empty();
}

template <class T>
int planner<T>::dataset_DFS_worker(T& state, int depth, int max_depth, action_set* actions, const std::string& goal_str, std::vector<std::string>& global_dataset, bool bisimulation) {

    if (m_current_nodes_ML >= m_threshold_node_generation_ML) {
		if (state.is_goal()) {
			global_dataset.push_back(format_row(state, depth, 0, goal_str));
        	return 0;
		}
        return -1;
    } 
	m_current_nodes_ML++;


    int current_score = -1;

    if (m_visited_states_ML.count(state)) {
        return m_states_scores[state];
    }

    if (state.is_goal()) {
        current_score = 0;
		m_goal_founds_ML++;
		m_goal_recently_found_ML = true;
    }

    int best_successor_score = -1;
    bool has_successor = false;

	// Create a local vector from action_set
	std::vector<action> local_actions(actions->begin(), actions->end());

	// Shuffle local_actions
	std::shuffle(local_actions.begin(), local_actions.end(), gen);

	for (const auto& action : local_actions) {
		if (state.is_executable(action)) {
			T next_state = state.compute_succ(action);

			if (bisimulation) {
				next_state.calc_min_bisimilar();
			}

			if (depth >= max_depth) {
				break;
			} else {
				double discard_probability = 0.0;
				// Only start discarding if total search space is big
				if (m_total_possible_nodes_log_ML > m_threshold_node_generation_log_ML) {

					double depth_ratio = (double)depth / (double)max_depth;
					double fullness_ratio = (double)m_current_nodes_ML / (double)m_threshold_node_generation_ML;
				
					// Start discard_probability low, increase as depth grows
					discard_probability = 0.2 * std::pow(depth_ratio, 2);
				
					// Slightly boost as dataset fills up
					discard_probability += 0.2 * fullness_ratio;

					discard_probability += std::min(0.01 * std::pow(static_cast<double>(m_discard_augmentation_factor_ML) / (3*max_depth), 2), 0.1);
				
					// Boost if a nearby goal was found
					if (m_goal_recently_found_ML) {
						discard_probability += 0.2;
					}
				
					// Clamp between [0, 0.8] (still allow deeper exploration)
					discard_probability = std::min(discard_probability, 0.8);
				}

				if (dis(gen) < discard_probability) {
					m_goal_recently_found_ML = false;
					m_discard_augmentation_factor_ML = 0.0;
					continue; // Randomly skip exploration
				}

				// Increase the augmentation factor for non-discarded series
				m_discard_augmentation_factor_ML++;
				
				int child_score = dataset_DFS_worker(next_state, depth + 1, max_depth, actions, goal_str, global_dataset, bisimulation);

				if (child_score >= 0) {
					if (!has_successor || child_score < best_successor_score) {
						best_successor_score = child_score;
					}
					has_successor = true;
				}
			}
		}
	}

    if (current_score == -1 && has_successor) {
        current_score = best_successor_score + 1;
    }

    global_dataset.push_back(format_row(state, depth, current_score, goal_str));
    m_visited_states_ML.insert(state);
    m_states_scores[state] = current_score;

    return current_score;
}


template <class T>
std::string planner<T>::format_row(T& state, int depth, int score, const std::string& goal_str) {
    std::stringstream ss;
    //ss << "\"";
    //printer::get_instance().print_list(state.get_executed_actions());
    //ss << "\",";
	auto [folder, base_filename] = state.print_graphviz_ML_dataset("");

	std::string folder_hash = folder + "/hash/";
	std::string folder_emap = folder + "/emap/";
	
	std::string filename_hash = folder_hash + base_filename + "_hash.dot";
	std::string filename_emap = folder_emap + base_filename + "_emap.dot";

	if (!domain::get_instance().is_gnn_mapped_enabled() && !domain::get_instance().is_gnn_both_enabled()){
		filename_hash = "NOT CALCULATED";
	}
	if (domain::get_instance().is_gnn_mapped_enabled() && !domain::get_instance().is_gnn_both_enabled()){
		filename_emap = "NOT CALCULATED";
	}
	
	ss << filename_hash << "," << filename_emap << "," << depth << "," << score << "," << goal_str;
	
    return ss.str();
}

/*\ IMPLEMENTATION OF OTHER TEMPLATIC FUNCTIONS INSTANCIATED WITH A TEMPLATIC STATE*/
#include "../heuristics/heuristics_template.ipp"
#include "../heuristics/planning_graph_template.ipp"
