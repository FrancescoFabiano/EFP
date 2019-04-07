/* 
 * File:   state.cpp
 * Author: Francesco
 *
 * Created on March 31, 2019, 11:34 AM
 */
#include "state.h"

/***************************************************************************************
 * Implementation of the functions that don't require state specifications to work .
 * These constructors are called before the specific ones.
****************************************************************************************/

state::state (const state & given_state)
{
	set_state(given_state);
}
state::state (const state & prev_state, const action & executed_action)
{
	set_state(prev_state.compute_succ(executed_action));
}
state::state (const action_list & executed_actions, unsigned short length)
{
	set_executed_actions(executed_actions);
	set_plan_length(length);
	set_heuristic_value();
}
state::state (const action_list & executed_actions, unsigned short length, int heuristic_value)
{
	set_executed_actions(executed_actions);
	set_plan_length(length);
	set_heuristic_value(heuristic_value);
}
    
action_list state::get_executed_actions()
{
	return m_executed_actions;
}
unsigned short state::get_plan_length()
{
	return m_plan_length;
}
int state::get_heuristic_value()
{
	return m_heuristic_value;
}

void state::set_state (const state & given_state)
{
	set_executed_actions(given_state.get_executed_actions());
	set_plan_length(given_state.get_plan_length());
	set_heuristic_value(given_state.get_heuristic_value());
}
//@TODO: to Implement
void state::set_executed_actions(const action_list & executed);
void state::set_plan_length(unsigned short length)
{
	m_plan_length = length;
}
void state::set_heuristic_value(int heuristic_value)
{
	m_heuristic_value = heuristic_value;
}
//@TODO: to Implement
bool state::is_goal();