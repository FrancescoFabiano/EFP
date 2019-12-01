#include "heuristics_manager.h"

heuristics_manager::heuristics_manager(heuristics used_heur)
{
	set_used_h(used_heur);
	m_goals = domain::get_instance().get_goal_description();
	switch ( m_used_heur ) {
	case L_PG:
	case S_PG:
	{
		expand_goals();
		break;

	}
	case SUBGOALS:
	{
		expand_goals();
		satisfied_goals::get_instance().set(m_goals);
		break;
	}
	default:
	{
		std::cerr << "\nWrong Heuristic Selection\n";
		exit(1);
	}

	}
}

void heuristics_manager::expand_goals()
{

	formula_list original_goal = m_goals;
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

void heuristics_manager::set_used_h(heuristics used_h)
{
	m_used_heur = used_h;
}

heuristics heuristics_manager::get_used_h() const
{
	return m_used_heur;
}

const formula_list & heuristics_manager::get_goals() const
{
	return m_goals;
}

void heuristics_manager::set_goals(const formula_list & to_set)
{
	m_goals = to_set;
}

bool heuristics_manager::operator=(const heuristics_manager& to_copy)
{
	set_used_h(to_copy.get_used_h());
	set_goals(to_copy.get_goals());
	return true;
}

