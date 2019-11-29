#include "heuristic_manager.h"

template <class T>
heuristic_manager<T>::heuristic_manager(heuristics used_heur)
{
	set_used_h(used_heur);
	//	if (m_used_heur == SUBGOALS) {
	//		satisfied_goals<T> tmp_sat_goal;
	//		m_sat_goal = tmp_sat_goal;
	//	}
}

template <class T>
void heuristic_manager<T>::set_heuristic_value(T & eState)
{

	switch ( m_used_heur ) {
	case L_PG:
	{
		planning_graph<T> pg(eState);
		if (pg.is_satisfiable()) {
			eState.set_heuristic_value(pg.get_length());
		} else {
			eState.set_heuristic_value(-1);
		}
		break;
	}
	case S_PG:
	{
		planning_graph<T> pg(eState);

		if (pg.is_satisfiable()) {
			eState.set_heuristic_value(pg.get_sum());
		} else {
			eState.set_heuristic_value(-1);
		}
		break;
	}
	case SUBGOALS:
	{
		eState.set_heuristic_value(m_sat_goal.get_unsatisfied_goals(eState));
		break;
	}
	default:
	{
		std::cerr << "\nWrong Heuristic Selection\n";
		exit(1);
	}

	}
}

template <class T>
void heuristic_manager<T>::set_used_h(heuristics used_h)
{
	m_used_heur = used_h;
}

template <class T>
heuristics heuristic_manager<T>::get_used_h() const
{
	return m_used_heur;
}

template <class T>
void heuristic_manager<T>::set_sat_goal(const satisfied_goals<T> & sat_goal)
{
	m_sat_goal = sat_goal;
}

template <class T>
const satisfied_goals<T>& heuristic_manager<T>::get_sat_goal() const
{
	return m_sat_goal;
}

template <class T>
bool heuristic_manager<T>::operator=(const heuristic_manager& to_copy)
{
	set_used_h(to_copy.get_used_h());
	set_sat_goal(to_copy.get_sat_goal());
	return true;
}

