/**
 * \brief Implementation of the \ref planner.h.
 *
 * \copyright GNU Public License.
 *
 * \author Francesco Fabiano.
 * \date May 6, 2019
 */
#include <stdlib.h>

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
bool planner<T>::search_BFS()
{
	T initial;
	initial.build_initial();

	if (initial.is_goal()) {
		std::cout << "\nWell Done, Goal found:)\n";
		initial.print();
		return true;
	}

	m_search_space.push(initial);
	std::cout << "\nInitial Done...\n";

	action_set actions = domain::get_instance().get_actions();
	action_set::const_iterator it_acset;
	T popped_state;
	T tmp_state;
	action tmp_action;
	while (!m_search_space.empty()) {
		popped_state = m_search_space.front();
		m_search_space.pop();
		for (it_acset = actions.begin(); it_acset != actions.end(); it_acset++) {
			tmp_action = *it_acset;
			if (popped_state.is_executable(tmp_action)) {
				tmp_state = popped_state.compute_succ(tmp_action);
				if (tmp_state.is_goal()) {
					tmp_state.print();
					std::cout << "\nWell Done, Goal found:)\n";
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