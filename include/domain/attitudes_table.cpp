#include "attitudes_table.h"
#include "../utilities/helper.h"
#include <utility>  

void attitudes_table::set_attitudes_table(const agent_set & tot_ags, const fluent & f1)
{
	//agent_set tot_ags= domain::get_instance().get_agents();
	agent_set::const_iterator it_ag;
	agent_set::const_iterator it_internal_ag;

	belief_formula false_bf;
	fluent_formula false_ff;

	//fluent_set tot_fl = domain.get_instance().get_fluents();
	//fluent f1 = *(tot_fl.begin());
	fluent f1_negated = helper::negate_fluent(f1);

	//The formula is "fluent_number_1 and -fluent_number_1" which is always false
	fluent_set true_fs;
	fluent_set false_fs;

	true_fs.insert(f1);
	false_fs.insert(f1_negated);
	
	false_ff.insert(true_fs);
	false_ff.insert(false_fs);

	false_bf.set_formula_type(FLUENT_FORMULA);
	false_bf.set_fluent_formula(false_ff);
	//Maybe add ground/deground

	//Empty is true
	//belief_formula true_fl;

	std::map<agent, std::map<agents_attitudes, belief_formula>> map_midP;
	std::map<agent, std::map<agents_attitudes, belief_formula>> map_midF;
	std::map<agents_attitudes, belief_formula> map_intP;
	std::map<agents_attitudes, belief_formula> map_intF;

	//This for loop initialize all the attitudes to be false
	for (it_ag = tot_ags.begin(); it_ag != tot_ags.end(); it_ag++) {
		for (it_internal_ag = tot_ags.begin(); it_internal_ag != tot_ags.end(); it_internal_ag++) {
			if (*it_internal_ag != *it_ag) {
				for (int i = P_KEEPER; i != attitudesEnd; i++) {

					if (i < F_TRUSTY) {
						//Default case
						//if (i == P_KEEPER) {
						//	map_intP.insert(std::pair<agents_attitudes, belief_formula>(i, true_fl));
						//} else {
						map_intP.insert(std::make_pair(static_cast<agents_attitudes>(i), false_bf));
						//}

					} else {
						//Default case
						//if (i == F_TRUSTY) {
						//	map_intF.insert(std::pair<agents_attitudes, belief_formula>(i, true_fl));
						//} else {
						map_intF.insert(std::make_pair(static_cast<agents_attitudes>(i), false_bf));
						//}
					}

				}
				map_midP.insert(std::make_pair(*it_internal_ag, map_intP));
				map_midF.insert(std::make_pair(*it_internal_ag, map_intF));
			}
		}
		m_P_attitude_wrt_exec.insert(std::make_pair(*it_ag, map_midP));
		m_F_attitude_wrt_exec.insert(std::make_pair(*it_ag, map_midF));
	}
}

const attitudes_map & attitudes_table::get_F_attitudes() const
{
	return m_F_attitude_wrt_exec;
}

const attitudes_map & attitudes_table::get_P_attitudes() const
{
	return m_P_attitude_wrt_exec;
}

void attitudes_table::add_F_attitudes(agent m_agent, agent executor, agents_attitudes attitude, const belief_formula & attitude_condition)
{
	add_attitudes(m_agent, executor, attitude, attitude_condition, m_F_attitude_wrt_exec);

}

void attitudes_table::add_P_attitudes(agent m_agent, agent executor, agents_attitudes attitude, const belief_formula & attitude_condition)
{
	add_attitudes(m_agent, executor, attitude, attitude_condition, m_P_attitude_wrt_exec);
}

void attitudes_table::add_attitudes(agent m_agent, agent executor, agents_attitudes attitude, const belief_formula & attitude_condition, attitudes_map & table)
{
	auto it_ext = table.find(m_agent);
	if (it_ext != table.end()) {
		auto it_mid = it_ext->second.find(executor);
		if (it_mid != it_ext->second.end()) {
			auto it_int = it_mid->second.find(attitude);
			if (it_int != it_mid->second.end()) {
				//Check if this work.
				it_int->second = attitude_condition;
			}
		}
	}
}

void attitudes_table::add_attitude(const attitude & att)
{
	if (att.get_type() < F_TRUSTY) {
		add_P_attitudes(att.get_agent(), att.get_executor(), att.get_type(), att.get_attitude_conditions());
	} else {
		add_F_attitudes(att.get_agent(), att.get_executor(), att.get_type(), att.get_attitude_conditions());
	}
}



/**@todo: Make the following methods Templatic w.r.t. the State*/
/*agents_attitudes attitudes_table::get_attitude(agent m_agent, agent executor, const pstate & curr, const attitudes_map & table, bool is_fully) const
{
    attitudes_map::iterator it_ext = table.find(m_agent);
    if (it_ext != table.end()) {
	std::map<agent, std::map<agents_attitudes, belief_formula>>::iterator it_mid = it_ext->second.find(executor);
	if (it_mid != it_ext->second.end()) {
	    std::map<agents_attitudes, belief_formula>::const_iterator it_int;
	    for (it_int = it_mid->second.begin(); it_int != it_mid->second.end(); it_int++) {
		//Check if this work.
		if (curr.entails(it_int->second)) {
		    return it_int->first;
		}
	    }
	}
    }
    if (is_fully) {
	return F_TRUSTY;
    }
    return P_KEEPER;

    //	std::cerr << "\nError: Some attitude declaration is missing, the agent has not any attitude specified.";
    //	exit(1);

}

const std::map<agent, agents_attitudes> & attitudes_table::get_attitudes(agent executor, const pstate & curr, const attitudes_map & table, bool is_fully) const
{
    agent_set tot_ags = domain::get_instance().get_agents();
    agent_set::const_iterator it_ag;


    std::map<agent, agents_attitudes> ret;
    for (it_ag = tot_ags.begin(); it_ag != tot_ags.end(); it_ag++) {
	if (*it_ag != executor) {
	    ret.insert(std::pair<agent, agents_attitudes>(*it_ag, get_attitude(*it_ag, executor, curr, table, is_fully)));
	}
    }

    return ret;


}

const std::map<agent, agents_attitudes> & attitudes_table::get_F_attitudes(agent executor, const pstate & curr) const
{
    get_attitudes(agent executor, const pstate & curr, m_F_attitude_wrt_exec, true);

}

const std::map<agent, agents_attitudes> & attitudes_table::get_P_attitudes(agent executor, const pstate & curr) const
{
    get_attitudes(agent executor, const pstate & curr, m_P_attitude_wrt_exec, false);

}*/