/*
 * \brief Class that implements planning_graph.h.
 *
 *
 * \copyright GNU Public License.
 *
 * \author Francesco Fabiano.
 * \date November 1, 2019
 */

#include "planning_graph.h"
#include "../domain/domain.h"
//pg_eState::pg_eState()
//{
//	set_action(-1);
//}
//
//pg_eState::pg_eState(const T & partial_kripke, const kworld_ptr_set & pointeds)
//{
//	set_partial_kripke(partial_kripke);
//	set_pointeds(pointeds);
//}
//
//void pg_eState::set_partial_kripke(const T & partial_kripke)
//{
//	m_partial_kripke = partial_kripke;
//}
//
//void pg_eState::set_pointeds(const kworld_ptr_set & pointeds)
//{
//	m_pointeds = pointeds;
//}
//
//void pg_eState::add_pointed(kworld_ptr pointed)
//{
//	m_pointeds.insert(pointed);
//}
//
//void pg_eState::set_action(action_id action)
//{
//	m_action = action;
//}
//
//const T & pg_eState::get_partial_kripke() const
//{
//	return m_partial_kripke;
//}
//
//const kworld_ptr_set & pg_eState::get_pointeds() const
//{
//	return m_pointeds;
//}
//
//action_id pg_eState::get_action() const
//{
//	return m_action;
//}
////printing
//
//void pg_eState::print() const
//{
//	std::cout << "\n The partial Kripke structure in the node after executing " << action << " is:\n";
//	m_partial_kripke.print();
//	std::cout << "\n The set of pointed states is: ";
//	printer::get_instance().print_list(m_pointeds);
//	std::cout << "\n------------------------------\n";
//}

pg_action_level::pg_action_level()
{

	set_depth(0);
}

pg_action_level::pg_action_level(const action_set & actions)
{

	set_actions(actions);
	set_depth(0);
}

pg_action_level::pg_action_level(const action_set & actions, unsigned short depth)
{

	set_actions(actions);
	set_depth(depth);
}

void pg_action_level::set_actions(const action_set & actions)
{

	m_actions = actions;
}

void pg_action_level::add_action(const action & act)
{

	m_actions.insert(act);
}

void pg_action_level::set_depth(unsigned short depth)
{
	m_depth = depth;
}

unsigned short pg_action_level::get_depth() const
{
	return m_depth;
}

const action_set & pg_action_level::get_actions() const
{

	return m_actions;
}

bool pg_action_level::operator=(const pg_action_level& to_assign)
{
	set_actions(to_assign.get_actions());
	set_depth(to_assign.get_depth());
	return true;
}


//printing

void pg_action_level::print() const
{
	std::cout << "\nPrinting an Action Level:";
	for (action_set::iterator it_acts = m_actions.begin(); it_acts != m_actions.end(); it_acts++) {

		std::cout << "\nAction " << it_acts->get_name() << std::endl;
	}
}

/*\*********************************************************************************************/
template <class T>
pg_state_level<T>::pg_state_level()
{
	set_depth(0);
}

template <class T>
pg_state_level<T>::pg_state_level(const std::set<T> & eStates)
{

	set_eStates(eStates);
	set_depth(0);

}

template <class T>
pg_state_level<T>::pg_state_level(const std::set<T> & eStates, unsigned short depth)
{

	set_eStates(eStates);
	set_depth(depth);

}

template <class T>
void pg_state_level<T>::set_eStates(const std::set<T> & eStates)
{

	m_eStates = eStates;
}

template <class T>
bool pg_state_level<T>::add_eState(const T & eState)
{
	return(m_eStates.insert(eState)).second;
}

template <class T>
void pg_state_level<T>::set_depth(unsigned short depth)
{
	m_depth = depth;
}

template <class T>
unsigned short pg_state_level<T>::get_depth() const
{
	return m_depth;
}

template <class T>
const std::set<T>& pg_state_level<T>::get_eStates() const
{

	return m_eStates;
}

//check satisfaction of a belief formula given an epistemic_model_list level (i.e., eNodes)

template <class T>
bool pg_state_level<T>::pg_entailment(const std::set<T> & eStates, const belief_formula & bf) const
{
	typename std::set<T>::const_iterator it_kstset;
	for (it_kstset = eStates.begin(); it_kstset != eStates.end(); it_kstset++) {
		if ((*it_kstset).entails(bf)) {

			return true;
		}
	}

	return false;
}

template <class T>
bool pg_state_level<T>::pg_entailment(const belief_formula & bf) const
{

	return pg_entailment(m_eStates, bf);
}

template <class T>
bool pg_state_level<T>::pg_entailment(const std::set<T> & eStates, const formula_list & fl) const
{

	formula_list::const_iterator it_fl;
	for (it_fl = fl.begin(); it_fl != fl.end(); it_fl++) {
		if (!pg_entailment(eStates, (*it_fl))) {
			return false;
		}
	}
	return true;
}

template <class T>
bool pg_state_level<T>::pg_entailment(const formula_list & fl) const
{

	return pg_entailment(m_eStates, fl);
}

template <class T>
bool pg_state_level<T>::pg_executable(const action & act) const
{
	return pg_entailment(act.get_executability());
}

template <class T>
bool pg_state_level<T>::operator=(const pg_state_level& to_assign)
{
	set_eStates(to_assign.get_eStates());
	set_depth(to_assign.get_depth());
	return true;
}

/*\******************************************************************************************************************/

/*\*****PLANNING GRAPH TIME MEASURE*******/
std::chrono::duration<double> t1, t2, t3, t4;

/*\***************************************/
template <class T>
planning_graph<T>::planning_graph(const T& state_init, const formula_list & goal)
{
	/*\*****PLANNING GRAPH TIME MEASURE*******/
	auto start_pg_build = std::chrono::system_clock::now();
	/*\***************************************/


	set_goal(goal);
	pg_state_level<T> pg_init;
	pg_init.add_eState(state_init);
	m_state_levels.push_back(pg_init);
	m_never_executed = domain::get_instance().get_actions();
	set_length(0);
	set_sum(0);


	bool goal_reached = true;
	formula_list::const_iterator it_fl;

	/*\*****PLANNING GRAPH TIME MEASURE*******/
	auto start_pg_goal_ini = std::chrono::system_clock::now();
	/*\***************************************/

	for (it_fl = m_goal.begin(); it_fl != m_goal.end();) {
		if (m_state_levels.back().pg_entailment(*it_fl)) {
			it_fl = m_goal.erase(it_fl);

		} else {
			goal_reached = false;
			it_fl++;
		}
	}



	/*\*****PLANNING GRAPH TIME MEASURE*******/
	auto end_pg_goal_ini = std::chrono::system_clock::now();
	std::chrono::duration<double> pg_goal_ini_time = end_pg_goal_ini - start_pg_goal_ini;
	t1 = std::chrono::milliseconds::zero();
	t2 = std::chrono::milliseconds::zero();
	t3 = std::chrono::milliseconds::zero();
	t4 = std::chrono::milliseconds::zero();
	/*\***************************************/

	if (!goal_reached) {
		pg_build_layer();

	} else {
		std::cerr << "\nBUILDING: The given state is goal\n";
		exit(1);
	}

	/*\*****PLANNING GRAPH TIME MEASURE*******/
	auto end_pg_build = std::chrono::system_clock::now();
	std::chrono::duration<double> pg_build_time = end_pg_build - start_pg_build;
	std::cout << "\n\nGenerated Planning Graph of length " << get_length() << " in " << pg_build_time.count() << " seconds of which:";
	std::cout << "\nFirst goal check:      " << pg_goal_ini_time.count();
	std::cout << "\nAction Level creation: " << t1.count();
	std::cout << "\n\nState Level Creation:  " << t2.count() << " of which:";
	std::cout << "\nState equality:        " << t4.count();
	std::cout << "\n\nGoals Check:           " << t3.count() << std::endl;
	/*\***************************************/


}


//template <class T>
//planning_graph<T>::planning_graph(const std::set<T>& pg_init)
//{
//	m_goal = domain::get_instance().get_goal_description();
//	m_state_levels.push_back(pg_state_level<T>(pg_init));
//	m_never_executed = domain::get_instance().get_actions();
//	set_length(0);
//	set_sum(0);
//
//	bool goal_reached = true;
//	formula_list::const_iterator it_fl;
//	//Remove each sub goal already satisfied: it will always be and we just need to check it once
//	for (it_fl = m_goal.begin(); it_fl != m_goal.end(); it_fl++) {
//
//		if (m_state_levels.back().pg_entailment(*it_fl)) {
//			it_fl = m_goal.erase(it_fl);
//		} else {
//			goal_reached = false;
//		}
//	}
//
//	if (!goal_reached) {
//		pg_build_layer();
//	} else {
//		std::cerr << "The given state is goal";
//	}
//
//}

template <class T>
void planning_graph<T>::set_satisfiable(bool sat)
{

	m_satisfiable = sat;
}

template <class T>
bool planning_graph<T>::is_satisfiable()
{

	return m_satisfiable;
}

template <class T>
void planning_graph<T>::pg_build_layer()
{
	pg_state_level<T> s_level_curr = m_state_levels.back();
	pg_action_level a_level_curr;
	a_level_curr.set_depth(get_length());

	/*\*****PLANNING GRAPH TIME MEASURE*******/
	auto start = std::chrono::system_clock::now();
	/*\***************************************/

	if (m_action_levels.size() > 0) {
		a_level_curr = m_action_levels.back();
	}
	action_set::iterator it_actset;
	for (it_actset = m_never_executed.begin(); it_actset != m_never_executed.end();) {
		if (s_level_curr.pg_executable(*it_actset)) {
			a_level_curr.add_action(*it_actset);
			it_actset = m_never_executed.erase(it_actset);
		} else {
			it_actset++;
		}
	}
	/*\*****PLANNING GRAPH TIME MEASURE*******/
	auto end = std::chrono::system_clock::now();
	t1 += end - start;
	/*\***************************************/


	add_action_level(a_level_curr);
	set_length(get_length() + 1);

	//The no-op is done with the copy
	pg_state_level<T> s_level_next = s_level_curr;
	s_level_next.set_depth(get_length());
	bool new_state_insertion = false;
	T tmp_state;

	/*\*****PLANNING GRAPH TIME MEASURE*******/
	start = std::chrono::system_clock::now();
	/*\***************************************/

	/*\*****PLANNING GRAPH TIME MEASURE*******/
	std::cerr << "\n\nNew Plan Length: " << get_length();
	if (get_length() > 3) {
		std::cerr << "\nAction Level creation: " << t1.count();
		std::cerr << "\nState Level Creation:  " << t2.count() << " of which:";
		std::cerr << "\nState equality:        " << t4.count();
	}
	/*\***************************************/


	for (it_actset = a_level_curr.get_actions().begin(); it_actset != a_level_curr.get_actions().end(); it_actset++) {

		for (typename std::set<T>::const_iterator it_kstset = s_level_curr.get_eStates().begin(); it_kstset != s_level_curr.get_eStates().end(); it_kstset++) {
			tmp_state = *it_kstset;
			//std::cerr << "\nChecking New State";

			/*\*****PLANNING GRAPH TIME MEASURE*******/
			auto startN = std::chrono::system_clock::now();
			bool tmptest = s_level_next.add_eState(tmp_state.compute_succ(*it_actset));
			auto endN = std::chrono::system_clock::now();
			t4 += endN - startN;
			if (tmptest) {
				//				std::cerr << "\nAdded New State";
				new_state_insertion = true;
			} else {
				//				std::cerr << "\nNOT Added New State";

			}
			/*\***************************************/

			//			if (s_level_next.add_eState(tmp_state.compute_succ(*it_actset))) {
			//				new_state_insertion = true;
			//			}

		}
	}
	/*\*****PLANNING GRAPH TIME MEASURE*******/
	end = std::chrono::system_clock::now();
	t2 += end - start;
	/*\***************************************/


	add_state_level(s_level_next);

	bool goal_reached = true;
	//unsigned short goal_index = 0;


	formula_list::const_iterator it_fl;

	/*\*****PLANNING GRAPH TIME MEASURE*******/
	start = std::chrono::system_clock::now();
	/*\***************************************/


	//Remove each sub goal already satisfied: it will always be and we just need to check it once
	for (it_fl = m_goal.begin(); it_fl != m_goal.end();) {

		if (s_level_next.pg_entailment(*it_fl)) {
			it_fl = m_goal.erase(it_fl);
			m_pg_sum += m_pg_length;

		} else {
			it_fl++;
			goal_reached = false;
		}
	}
	/*\*****PLANNING GRAPH TIME MEASURE*******/
	end = std::chrono::system_clock::now();
	t3 += end - start;
	/*\***************************************/


	if (goal_reached) {
		set_satisfiable(true);
		return;
	} else if (!new_state_insertion) {
		set_satisfiable(false);
		return;
	} else {
		pg_build_layer();
	}
}

template <class T>
void planning_graph<T>::add_state_level(const pg_state_level<T> & s_level)
{

	m_state_levels.push_back(s_level);
}

template <class T>
void planning_graph<T>::add_action_level(const pg_action_level & a_level)
{

	m_action_levels.push_back(a_level);
}

template <class T>
void planning_graph<T>::set_length(unsigned short length)
{

	m_pg_length = length;
}

template <class T>
void planning_graph<T>::set_sum(unsigned short sum)
{

	m_pg_sum = sum;
}

template <class T>
unsigned short planning_graph<T>::get_length()
{
	if (is_satisfiable()) {
		return m_pg_length;
	}
	std::cerr << "\nThe planning graph could not find any solution. Check for the satisfiability before calling \"get_length\"\n";
	exit(1);
}//construct planning graph and return the level that satisfied the goal.

template <class T>
unsigned short planning_graph<T>::get_sum()
{
	if (is_satisfiable()) {
		return m_pg_sum;
	}
	std::cerr << "\nThe planning graph could not find any solution. Check for the satisfiability before calling \"get_sum\"\n";
	exit(1);
}//

template <class T>
void planning_graph<T>::set_goal(const formula_list & goal)
{
	m_goal = goal;
}

/*template <class T>
const pg_worlds_score & planning_graph<T>::get_worlds_score()
{
	if (is_satisfiable()) {

		return NULL;
	}
	std::cerr << "\nThe planning graph could not find any solution. Check for the satisfiability before calling \"get_worlds_score\"\n";
	exit(1);
}

template <class T>
const pg_bfs_score & planning_graph<T>::get_bfs_score()
{
	if (is_satisfiable()) {
		return NULL;
	}
	std::cerr << "\nThe planning graph could not find any solution. Check for the satisfiability before calling \"get_bfs_score\"\n";
	exit(1);
} FOR FUTURE USE */