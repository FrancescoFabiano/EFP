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
bool planner<T>::ML_dataset_creation(ML_Dataset_Params *ML_dataset){
	std::string folder = "out/ML_HEUR_datasets/";
	if (ML_dataset->useDFS) {
		folder = folder + "DFS/";
	} else {
		folder = folder + "BFS/";
	}
	system(("mkdir -p " + folder).c_str());
	
	std::string fname      = domain::get_instance().get_name();
	fname = fname + "_d_" + std::to_string(ML_dataset->depth);
	std::string file_extension = ".csv";

	std::string fpath = folder + fname + file_extension;

	std::ofstream result;
	result.open(fpath);
	result << "State, Distance From Goal, Depth" << std::endl;
	result.close();



	std::cout << "NOTICE: creation of the ML dataset with BFS and DFS is a work in progress. Currently only -1 (not goal) and 0 (goal) values are represented." << std::endl; //TODO - remove this line when DFS is implemented.
	return create_dataset(ML_dataset->depth, fpath, ML_dataset->useDFS);	
}

template <class T>
void planner<T>::append_to_dataset(mlds_struct current, std::string fpath){
	//initialize values
	std::string comma = " , ";
	std::ofstream result;
	std::streambuf *backup, *psbuf;
	backup= std::cout.rdbuf();
	psbuf = result.rdbuf();

	//write opening quotation mark so csv file can identify state.print as one cell
	result.open(fpath, std::ofstream::app);
	result << "\"";
	result.close();

	//redirect state.print output from std::cout to the file at fpath
	result.open(fpath, std::ofstream::app);
	psbuf = result.rdbuf();
	std::cout.rdbuf(psbuf); 
	current.state.print();
	std::cout.rdbuf(backup);
	result.close();
	
	//write closing quotation mark for state.print, as well as the distance to goal and depth values.
	result.open(fpath, std::ofstream::app);
	result << "\n\"" << comma << current.dist_goal << comma << current.depth << std::endl;
	result.close();
}

template <class T>
bool planner<T>::create_dataset(int depth, std::string fpath, bool useDFS){
	//initialization
	int numGoals = 0;
	int expanded_nodes = 0;
	bool plan_found = false;
	mlds_struct initial_struct;
	initial_struct.depth = 0;
	bool check_visited = domain::get_instance().check_visited();
	bool bisimulation = false;
	if (domain::get_instance().get_bisimulation() != BIS_NONE) { bisimulation = true; }
	bisimulation = false;
	std::set<T> visited_states;
	auto start_timing = std::chrono::system_clock::now();
	initial_struct.state.build_initial();
	if (bisimulation) { initial_struct.state.calc_min_bisimilar(); }
	auto end_timing = std::chrono::system_clock::now();
	std::chrono::duration<double> elapsed_seconds = end_timing - start_timing;
	std::cout << "\nInitial Built in " << elapsed_seconds.count() << " seconds" << std::endl;

	//post-initialization
	action_set actions = domain::get_instance().get_actions();
	action_set::const_iterator it_acset;
	action tmp_action;
	start_timing = std::chrono::system_clock::now();

	//if root is the goal
	if (initial_struct.state.is_goal()) {
		initial_struct.dist_goal = 0;
		end_timing = std::chrono::system_clock::now();
		elapsed_seconds = end_timing - start_timing;
		plan_found = true;
		numGoals++;
	}

	//push the initial struct to the stack
	if(useDFS) mlds_DFS_structure.push(initial_struct);
	else	   mlds_BFS_structure.push(initial_struct);
	if (check_visited) { visited_states.insert(initial_struct.state); }
	
	//while there are states to be searched
	while (!mlds_DFS_structure.empty() || !mlds_BFS_structure.empty()) {
		mlds_struct popped_struct;
		if(useDFS) {
			popped_struct = mlds_DFS_structure.top();
			mlds_DFS_structure.pop();
		}else{
		    popped_struct = mlds_BFS_structure.front();
			mlds_BFS_structure.pop();
		}
		append_to_dataset(popped_struct, fpath);
		if(popped_struct.depth >= depth){continue;}
		expanded_nodes++;

		//loop through the set of actions available to the problem 
		for (it_acset = actions.begin(); it_acset != actions.end(); it_acset++) {
			mlds_struct tmp_struct;
			tmp_action = *it_acset;

			//if this state can take the action tmp_action
			if (popped_struct.state.is_executable(tmp_action)) {

				//by taking tmp_action in this state, we get the next state: tmp_state
				tmp_struct.state     = popped_struct.state.compute_succ(tmp_action);
				tmp_struct.depth     = popped_struct.depth + 1;
				if (bisimulation) { tmp_struct.state.calc_min_bisimilar(); }

				//if tmp_state is a goal 
				if (tmp_struct.state.is_goal()) {
					tmp_struct.dist_goal = 0;
					end_timing = std::chrono::system_clock::now();
					elapsed_seconds = end_timing - start_timing;
					plan_found = true;
					numGoals++;
				}

				// mark tmp_state as visited and push it to the search space
				if (!check_visited || visited_states.insert(tmp_struct.state).second) { 
					if(useDFS) mlds_DFS_structure.push(tmp_struct); 
					else       mlds_BFS_structure.push(tmp_struct); 
				}
			}
		}
	}

	//print end of dataset creation information
	end_timing = std::chrono::system_clock::now();
	elapsed_seconds = end_timing - start_timing;
	std::cout << "Completed dataset creation in " << elapsed_seconds.count() << " seconds. This dataset contains " << numGoals << " goals, and " << expanded_nodes << " instances." << std::endl;
	return plan_found;
}

/*\ IMPLEMENTATION OF OTHER TEMPLATIC FUNCTIONS INSTANCIATED WITH A TEMPLATIC STATE*/
#include "../heuristics/heuristics_template.ipp"
#include "../heuristics/planning_graph_template.ipp"
