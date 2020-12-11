
#include <map>

#include "domain.h"
#include "attitudes_table.h"
#include "include/states/state_T.h"

attitudes_table::attitudes_table()
{
	agent_set tot_ags = domain::get_instance().get_agents();
	agent_set::const_iterator it_ag;
	agent_set::const_iterator it_internal_ag;

	formula_list false_fl;
	belief_formula false_bf;
	fluent_formula false_ff;

	fluent_set tot_fl = domain.get_instance().get_fluents();
	fluent f1 = *(tot_fl.begin());
	fluent f1_negated = helper.negate_fluent(*(tot_fl.begin()));

	//The formula is "fluent_number_1 and -fluent_number_1" which is always false  
	false_ff.insert(f1);
	flase_ff.insert(f1_negated);

	false_bf.set_formula_type(FLUENT_FORMULA);
	false_bf.set_fluent_formula(false_ff);
	//Maybe add ground/deground
	false_fl.insert(false_bf);


	//Empty is true
	formula_list true_fl;

	std::map<agent, std::map<agents_attitudes, formula_list>> map_midP;
	std::map<agent, std::map<agents_attitudes, formula_list>> map_midF;
	std::map<agents_attitudes, formula_list> map_intP;
	std::map<agents_attitudes, formula_list> map_intF;

	//This for loop initialize all the attitudes to be false
	for (it_ag = tot_ags.begin(); it_ag != tot_ags.end(); it_ag++) {
		for (it_internal_ag = tot_ags.begin(); it_internal_ag != tot_ags.end(); it_internal_ag++) {
			if (*it_internal_ag != *it_ag) {
				for (int i = P_KEEPER; i != attitudesEnd; i++) {

					if (i < F_TRUSTY) {
						//Default case
						if (i == P_KEEPER) {
							map_intP.insert(std::pair<agents_attitudes, formula_list>(i, true_fl));
						} else {
							map_intP.insert(std::pair<agents_attitudes, formula_list>(i, false_fl));
						}

					} else {
						//Default case
						if (i == F_TRUSTY) {
							map_intF.insert(std::pair<agents_attitudes, formula_list>(i, true_fl));
						} else {
							map_intF.insert(std::pair<agents_attitudes, formula_list>(i, false_fl));
						}
					}

				}
				map_midP.insert(std::pair<agent, std::map<agents_attitudes, formula_list >> (*it_internal_ag, map_intP));
				map_midF.insert(std::pair<agent, std::map<agents_attitudes, formula_list >> (*it_internal_ag, map_intF));
			}
		}
		m_P_attitude_wrt_exec.insert(std::pair<agent, std::map<agent, std::map<agents_attitudes, formula_list>(*it_ag, map_midP));
		m_F_attitude_wrt_exec.insert(std::pair<agent, std::map<agent, std::map<agents_attitudes, formula_list>(*it_ag, map_midF));
	}
}

void attitudes_table::add_F_attitudes(agent m_agent, agent executor, agents_attitudes attitude, const formula_list & attitude_condition)
{
	add_attitudes(m_agent, executor, attitude, attitude_condition, m_F_attitude_wrt_exec);

}

void attitudes_table::add_P_attitudes(agent m_agent, agent executor, agents_attitudes attitude, const formula_list & attitude_condition)
{
	add_attitudes(m_agent, executor, attitude, attitude_condition, m_P_attitude_wrt_exec);
}

void attitudes_table::add_attitudes(agent m_agent, agent executor, agents_attitudes attitude, const formula_list & attitude_condition, const attitudes_map & table)
{
	attitudes_map::iterator it_ext = table.find(m_agent);
	if (it_ext != table.end()) {
		std::map<agent, std::map<agents_attitudes, formula_list>>::iterator it_mid = it_ext->second.find(executor);
		if (it_mid != it_ext->second.end()) {
			std::map<agents_attitudes, formula_list> it_int = it_mid->second.find(attitude);
			if (it_int != it_mid->second.end()) {
				//Check if this work.
				it_int->second = attitude_condition;
			}
		}
	}
}

agents_attitudes attitudes_table::get_attitude(agent m_agent, agent executor, const state<T> & curr, const attitudes_map & table) const
{
	attitudes_map::iterator it_ext = table.find(m_agent);
	if (it_ext != table.end()) {
		std::map<agent, std::map<agents_attitudes, formula_list>>::iterator it_mid = it_ext->second.find(executor);
		if (it_mid != it_ext->second.end()) {
			std::map<agents_attitudes, formula_list>::const_iterator it_int;
			for (it_int = it_mid->second.begin(); it_int != it_mid->second.end(); it_int++) {
				//Check if this work.
				if (state<T>.entails(it_int->second)) {
					return it_int->first;
				}
			}
		}
	}

	std::cerr << "\nError: Some attitude declaration is missing, the agent has not any attitude specified.";
	exit(1);

}

/*agents_attitudes attitudes_table::get_F_attitude(agent m_agent, agent executor, const state<T> & curr) const
{
	get_attitude(agent m_agent, agent executor, const state<T> & curr, m_F_attitude_wrt_exec);
}

agents_attitudes attitudes_table::get_P_attitude(agent m_agent, agent executor, const state<T> & curr) const
{

	get_attitude(agent m_agent, agent executor, const state<T> & curr, m_P_attitude_wrt_exec);

}*/

const std::map<agent, agents_attitudes> & attitudes_table::get_attitudes(agent executor, const state<T> & curr, const attitudes_map & table) const
{
	agent_set tot_ags = domain::get_instance().get_agents();
	agent_set::const_iterator it_ag;


	std::map<agent, agents_attitudes> ret;
	for (it_ag = tot_ags.begin(); it_ag != tot_ags.end(); it_ag++) {
		if (*it_ag != executor) {
			ret.insert(std::pair<agent, agents_attitudes>(*it_ag, get_attitude(*it_ag, executor, curr, table)));
		}
	}

	return ret;


}

const std::map<agent, agents_attitudes> & attitudes_table::get_F_attitudes(agent executor, const state<T> & curr) const
{
	get_attitudes(agent executor, const state<T> & curr, m_F_attitude_wrt_exec);

}

const std::map<agent, agents_attitudes> & attitudes_table::get_P_attitudes(agent executor, const state<T> & curr) const
{
	get_attitudes(agent executor, const state<T> & curr, m_P_attitude_wrt_exec);

}
