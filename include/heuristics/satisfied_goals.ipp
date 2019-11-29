#include "satisfied_goals.h"

template <class T>
satisfied_goals<T>::satisfied_goals()
{
	m_goals = domain::get_instance().get_goal_description();
	expand_goals();
	m_goals_number = m_goals.size();
}

template <class T>
void satisfied_goals<T>::expand_goals()
{

	formula_list original_goal = domain::get_instance().get_goal_description();
	formula_list::const_iterator it_fl;
	agent_set::const_iterator it_agset;

	grounder gr = domain::get_instance().get_grounder();

	//unsigned short added_subgoals = 0;
	for (it_fl = original_goal.begin(); it_fl != original_goal.end(); it_fl++) {
		belief_formula to_explore = *it_fl;
		if (to_explore.get_formula_type() == C_FORMULA) {
			for (it_agset = to_explore.get_group_agents().begin(); it_agset != to_explore.get_group_agents().end(); it_agset++) {
				belief_formula new_subgoal;
				new_subgoal.set_formula_type(BELIEF_FORMULA);
				new_subgoal.set_bf1(to_explore.get_bf1());
				new_subgoal.set_string_agent(gr.deground_agent(*it_agset));
				new_subgoal.ground();
				m_goals.push_back(new_subgoal);
			}
			//m_goals_number++;
		}
	}
}

template <class T>
unsigned short satisfied_goals<T>::get_unsatisfied_goals(const T & eState) const
{
	unsigned short ret = m_goals_number;

	formula_list::const_iterator it_fl;
	for (it_fl = m_goals.begin(); it_fl != m_goals.end(); it_fl++) {
		if (eState.entails(*it_fl)) {
			ret--;
		}
	}
	return ret;

}

template <class T>
unsigned short satisfied_goals<T>::get_satisfied_goals(const T & eState) const
{
	unsigned short ret = 0;

	formula_list::const_iterator it_fl;
	for (it_fl = m_goals.begin(); it_fl != m_goals.end(); it_fl++) {
		if (eState.entails(*it_fl)) {
			ret++;
		}
	}
	return ret;
}

template <class T>
const formula_list & satisfied_goals<T>::get_goals() const
{
	return m_goals;
}

template <class T>
void satisfied_goals<T>::set_goals(const formula_list & to_set)
{
	m_goals = to_set;
}

template <class T>
unsigned short satisfied_goals<T>::get_goals_number() const
{
	return m_goals_number;
}

template <class T>
void satisfied_goals<T>::set_goals_number(unsigned short to_set)
{
	m_goals_number = to_set;
}

template <class T>
bool satisfied_goals<T>::operator=(const satisfied_goals<T>& to_copy)
{
	set_goals(to_copy.get_goals());
	set_goals_number(to_copy.get_goals_number());
	return true;
}