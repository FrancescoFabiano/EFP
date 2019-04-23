/**
 * \brief Implementation of \ref state_T.h
 *
 * \copyright GNU Public License.
 *
 * \author Francesco Fabiano.
 * \date April 11, 2019
 */

#include "state_T.h"

template <class T>
state<T>::state()
{
	T representation;
}

template <class T>
state<T>::state(const state<T> & given_state)
{
	set_state(given_state);
}

template <class T>
state<T>::state(const state<T> & prev_state, const action & executed_action)
{
	set_state(prev_state.compute_succ(executed_action));
}

/*template <class T>
state<T>::state(const action_id_list & executed_actions, unsigned short length)
{
	set_executed_actions(executed_actions);
	set_plan_length(length);
	set_heuristic_value();
}

template <class T>
state<T>::state(const action_id_list & executed_actions, unsigned short length, int heuristic_value)
{
	set_executed_actions(executed_actions);
	set_plan_length(length);
	set_heuristic_value(heuristic_value);
}*/

template <class T>
const action_id_list & state<T>::get_executed_actions()
{
	return m_executed_actions_id;
}

template <class T>
unsigned short state<T>::get_plan_length()
{
	return m_plan_length;
}

template <class T>
int state<T>::get_heuristic_value()
{
	return m_heuristic_value;
}

template <class T>
void state<T>::set_state(const state<T> & given_state)
{
	set_executed_actions(given_state.get_executed_actions());
	set_plan_length(given_state.get_plan_length());
	set_heuristic_value(given_state.get_heuristic_value());
}

template <class T>
void state<T>::set_executed_actions(const action_id_list & executed)
{
	m_executed_actions_id = executed;
}

template <class T>
void state<T>::set_plan_length(unsigned short length)
{
	m_plan_length = length;
}

template <class T>
void state<T>::set_heuristic_value(int heuristic_value)
{
	m_heuristic_value = heuristic_value;
}

template <class T>
bool state<T>::entails(const fluent_set & to_check)
{
	return representation.entails(to_check);
}

template <class T>
bool state<T>::entails(const fluent_formula & to_check)
{
	return representation.entails(to_check);
}

template <class T>
bool state<T>::entails(const formula_list & to_check)
{
	//formula_list expresses CNF formula
	formula_list::const_iterator it_fl;
	for (it_fl = to_check.begin(); it_fl != to_check.end(); it_fl++) {
		if (!representation.entails(to_check)) {
			return false;
		}
	}
	return true;
}

template <class T>
void state<T>::build_initial(const initially & init)
{
	//To implement constructor
	//representation = T(init);
	representation.build_initial(init);
}

template <class T>
T state<T>::compute_succ(const action & act)
{
	return representation.compute_succ(act);
}