/**
 * \brief Implementation of the \ref planner.h.
 *
 * \copyright GNU Public License.
 *
 * \author Francesco Fabiano.
 * \date May 6, 2019
 */
#include "planner.h"
#include "../domain/domain.h"

/*planner::planner(state_type given_state_type)
{
	m_state_type = given_state_type;
	switch ( m_state_type ) {
	case KRIPKE:
	{
		state<kstate> initial;
		m_search_space.push(initial.build_initial());
		std::cout << "\nInitial Done...\n";
	}
	case POSSIBILITIES:
	case OBDD:
	default:
		std::cerr << "\nNot implemented yet\n";
		exit(1);
	}
}*/

template <class T>
void planner<T>::print_results(std::chrono::duration<double> elapsed_seconds, T goal, bool old_check, bool givenplan)
{
	std::cout << "\n\n\nWell Done, Goal found in " << elapsed_seconds.count() << " :)\n";
	goal.print();
	if (old_check) {
		std::ofstream result;
		std::string folder = "out/EFP_comparison/";
		if (givenplan) {
			folder = folder + "givenplan/";
		} else {
			folder = folder + "findingplan/";
		}
		system(("mkdir -p " + folder).c_str());
		result.open(folder + domain::get_instance().get_name() + ".txt");
		result << "EFP V 2.0 completed the search in " << elapsed_seconds.count() << "\n";
		result.close();
	}
}

template <class T>
bool planner<T>::search_BFS(bool old_check)
{
	T initial;
	auto start_timing = std::chrono::system_clock::now();
	initial.build_initial();
	auto end_timing = std::chrono::system_clock::now();
	std::chrono::duration<double> elapsed_seconds = end_timing - start_timing;

	std::cout << "\nInitial Built in " << elapsed_seconds.count() << " seconds\n";

	action_set actions = domain::get_instance().get_actions();
	action_set::const_iterator it_acset;
	T popped_state;
	T tmp_state;
	action tmp_action;

	start_timing = std::chrono::system_clock::now();
	if (initial.is_goal()) {
		end_timing = std::chrono::system_clock::now();
		elapsed_seconds = end_timing - start_timing;
		print_results(elapsed_seconds, initial, old_check, false);
		return true;
	}

	m_search_space.push(initial);


	while (!m_search_space.empty()) {
		popped_state = m_search_space.front();
		m_search_space.pop();
		for (it_acset = actions.begin(); it_acset != actions.end(); it_acset++) {
			tmp_action = *it_acset;
			if (popped_state.is_executable(tmp_action)) {
				tmp_state = popped_state.compute_succ(tmp_action);
				if (tmp_state.is_goal()) {
					end_timing = std::chrono::system_clock::now();
					elapsed_seconds = end_timing - start_timing;
					print_results(elapsed_seconds, tmp_state, old_check, false);
					return true;
				}
				m_search_space.push(tmp_state);
			}
		}
	}
	std::cout << "\nNo plan found for this goal.\n";
	return false;
}

template <class T>
void planner<T>::execute_given_actions(const std::vector<std::string> act_name)
{
	T state;
	state.build_initial();

	if (state.is_goal()) {
		std::cout << "\nInitial was Goal:)\n";
	} else {
		std::cout << "\n\n";
	}

	if (domain::get_instance().get_debug()) state.print_graphviz();


	std::vector<std::string>::const_iterator it_stset;
	std::vector<std::string>::const_iterator it_stset2;

	action_set::const_iterator it_acset;
	for (it_stset = act_name.begin(); it_stset != act_name.end(); it_stset++) {
		for (it_acset = domain::get_instance().get_actions().begin(); it_acset != domain::get_instance().get_actions().end(); it_acset++) {
			if ((*it_acset).get_name().compare(*it_stset) == 0) {
				//std::cout << "\n\nTrying action " << (*it_acset).get_name() << "\n";
				if (state.is_executable(*it_acset)) {
					state = state.compute_succ(*it_acset);
					if (domain::get_instance().get_debug()) state.print_graphviz();
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
		system(("sh scripts/generate_pdf.sh out/state/" + domain::get_instance().get_name()).c_str());
	}

	return;
}

template <class T>
/**\todo just for confrantation with old*/
void planner<T>::execute_given_actions_timed(const std::vector<std::string> act_name)
{
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

	print_results(elapsed_seconds, state, true, true);

	return;
}