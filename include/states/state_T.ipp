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
	//T m_representation;
}

/*template <class T>
state<T>::state(const state<T> & given_state)
{
	(*this) = given_state;
}*/

template <class T>
state<T>::state(const state<T> & prev_state, const action & executed_action)
{
	(*this) = prev_state.compute_succ(executed_action);
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
const action_id_list & state<T>::get_executed_actions() const
{
	return m_executed_actions_id;
}

template <class T>
unsigned short state<T>::get_plan_length() const
{
	return m_plan_length;
}

template <class T>
int state<T>::get_heuristic_value() const
{
	return m_heuristic_value;
}

template <class T>
const T & state<T>::get_representation() const
{
	return m_representation;
}

template <class T>
bool state<T>::operator=(const state<T> & given_state)
{
	/**\warning each T must implement the operator =*/
	set_representation(given_state.get_representation());
	set_executed_actions(given_state.get_executed_actions());
	set_plan_length(given_state.get_plan_length());
	set_heuristic_value(given_state.get_heuristic_value());
	return true;
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
void state<T>::set_representation(const T & to_set)
{
	m_representation = to_set;
}

template <class T>
bool state<T>::entails(const fluent_set & to_check) const
{
	return m_representation.entails(to_check);
}

template <class T>
bool state<T>::entails(const fluent_formula & to_check) const
{
	return m_representation.entails(to_check);
}

template <class T>
bool state<T>::entails(const formula_list & to_check) const
{
	//formula_list expresses CNF formula
	formula_list::const_iterator it_fl;
	for (it_fl = to_check.begin(); it_fl != to_check.end(); it_fl++) {
		if (!m_representation.entails(*it_fl)) {
			return false;
		}
	}
	return true;
}

template <class T>
void state<T>::build_initial()
{
	//To implement constructor
	//representation = T(init);
	m_representation.build_initial();
}

template <class T>
state<T> state<T>::compute_succ(const action & act) const
{
	/**\todo Myabe is better if used in \ref planner or \ref domain (myabe a bool as **param[out]**.*/
	state<T> ret;
	if (is_executable(act)) {
		std::cout << "Debug in Exec\n";
		get_representation().compute_succ(act);
		//ret.set_representation(get_representation().compute_succ(act));
		//ret.set_executed_actions(get_executed_actions().add(act.get_id()));
		//ret.set_plan_length(get_plan_length() + 1);
		//set_heuristic_value(get_representation().compute_heuristic_value());
		return ret;
	} else {
		ret = (*this);
	}
	return ret;
}

template <class T>
bool state<T>::is_executable(const action & act) const
{
	return entails(act.get_executability());
}