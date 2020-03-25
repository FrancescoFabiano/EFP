#include "asp_maker.h"

asp_maker::asp_maker()
{
	m_grounder = domain::get_instance().get_grounder();
	m_already_printed_formulae.insert("");
	m_already_printed_agents_sets.insert("");
}

std::string asp_maker::print_ag_set(const agent_set & ags, std::ofstream & to_print)
{
	std::string ret = "";
	agent_set::const_iterator it_ags;

	for (it_ags = ags.begin(); it_ags != ags.end(); it_ags++) {
		ret += m_grounder.deground_agent(*it_ags);
	}
	if (m_already_printed_agents_sets.insert(ret).second) {
		to_print << "agent_set(" << ret << ").\n";
		for (it_ags = ags.begin(); it_ags != ags.end(); it_ags++) {
			to_print << "contains(" << ret << "," << m_grounder.deground_agent(*it_ags) << ").\n";
		}
	}

	return ret;
}

std::string asp_maker::print_subformula(const fluent & fl, std::ofstream & to_print)
{
	std::string ret = m_grounder.deground_fluent(fl);

	if (m_already_printed_formulae.insert(ret).second) {
		to_print << "formula(" << ret << ").\n";
	}
	return ret;
}

std::string asp_maker::print_subformula(const fluent_set & fluents, std::ofstream & to_print)
{
	std::string ret = "";

	if (fluents.size() > 1) {

		fluent_set::const_iterator it_fls;
		for (it_fls = fluents.begin(); it_fls != fluents.end(); it_fls++) {
			if (std::next(it_fls, 1) != fluents.end()) {
				ret += "and(" + print_subformula(*it_fls, to_print) + ",";
			} else {
				ret += print_subformula(*it_fls, to_print) + ")";
			}
			//ret +=")";
		}

	} else if (fluents.size() == 1) {
		ret += print_subformula(*fluents.begin(), to_print);
		//return ret;
	} else {
		std::cerr << "\nError in parsing fluent_set for asp conversion\n";
		exit(1);
	}

	if (m_already_printed_formulae.insert(ret).second) {
		to_print << "formula(" << ret << ").\n";
	}
	return ret;
}

std::string asp_maker::print_subformula(const fluent_formula & ff, std::ofstream & to_print)
{
	if (ff.size() != 1) {
		//		std::cerr << "\nError in parsing fluent_formula for asp conversion\n";
		//		exit(1);
		return "";
	}
	return print_subformula(*ff.begin(), to_print);
}

std::string asp_maker::print_subformula(const belief_formula & bf, std::ofstream & to_print)
{
	std::string ret = "";

	switch ( bf.get_formula_type() ) {

	case FLUENT_FORMULA:
	{
		ret += print_subformula(bf.get_fluent_formula(), to_print);
		break;
	}

	case BELIEF_FORMULA:
		//ret += parse_formula(bf.get_fluent_formula());
		ret += "b(" + m_grounder.deground_agent(bf.get_agent()) + "," + print_subformula(bf.get_bf1(), to_print) + ")";

		break;

	case PROPOSITIONAL_FORMULA:
		switch ( bf.get_operator() ) {
		case BF_NOT:
			ret += "not(" + print_subformula(bf.get_bf1(), to_print) + ")";
			//return !entails(bf.get_bf1(), world);
			break;
		case BF_OR:
			ret += "or(" + print_subformula(bf.get_bf1(), to_print) + "," + print_subformula(bf.get_bf2(), to_print) + ")";
			//return entails(bf.get_bf1(), world) || entails(bf.get_bf2(), world);
			break;
		case BF_AND:
			ret += "and(" + print_subformula(bf.get_bf1(), to_print) + "," + print_subformula(bf.get_bf2(), to_print) + ")";

			//return entails(bf.get_bf1(), world) && entails(bf.get_bf2(), world);
			break;
		case BF_FAIL:
		default:
			std::cerr << "Something went wrong in transforming asp for Propositional formula";
			exit(1);
		}
		break;
	case C_FORMULA:
		ret += "c(" + print_ag_set(bf.get_group_agents(), to_print) + "," + print_subformula(bf.get_bf1(), to_print) + ")";
		break;
	case BF_EMPTY:
	{
		break;
	}
	case BF_TYPE_FAIL:
	default:
		std::cerr << "Something went wrong in transforming asp for Propositional formula";
		exit(1);
	}

	if (m_already_printed_formulae.insert(ret).second) {
		to_print << "formula(" << ret << ").\n";
	}
	return ret;
}

std::string asp_maker::print_subformula(const formula_list & fl, std::ofstream & to_print)
{
	std::string ret = "";

	if (fl.size() > 1) {
		formula_list::const_iterator it_fl;
		for (it_fl = fl.begin(); it_fl != fl.end(); it_fl++) {
			if (std::next(it_fl, 1) != fl.end()) {
				ret += "and(" + print_subformula(*it_fl, to_print) + ",";
			} else {
				ret += print_subformula(*it_fl, to_print) + ")";
			}
		}
	} else if (fl.size() == 1) {
		ret += print_subformula(*fl.begin(), to_print);
		//	return ret;
	} else {
		std::cerr << "\nError in parsing fluent_list for asp conversion\n";
		exit(1);
	}

	if (m_already_printed_formulae.insert(ret).second) {
		to_print << "formula(" << ret << ").\n";
	}
	return ret;
}

void asp_maker::print_all()
{
	std::ofstream result;
	std::string folder = "out/ASP_encoding/";
	system(("mkdir -p " + folder).c_str());
	std::string filename = folder + domain::get_instance().get_name() + ".lp";
	remove(filename.c_str());
	result.open(filename, std::ofstream::out | std::ofstream::app);

	print_fluents(result);
	print_agents(result);
	print_actions(result);
	print_initially(result);
	print_goals(result);
	print_worlds(result);

	//print_formulae(result);

	result.close();

}

void asp_maker::print_fluents(std::ofstream & to_print) const
{
	to_print << "% *** FLUENTS ***\n";
	fluent_set::const_iterator it_fls;
	fluent_set fluents = domain::get_instance().get_fluents();
	for (it_fls = fluents.begin(); it_fls != fluents.end(); it_fls++) {
		if (*it_fls % 2 == 0) {
			to_print << "fluent(" << m_grounder.deground_fluent(*it_fls) << ").\n";
		}
	}
	to_print << "\n\n";
}

void asp_maker::print_actions(std::ofstream & to_print)
{
	to_print << "% *** ACTIONS ***\n";
	action_set::const_iterator it_acs;
	std::string act_name;
	action_set actions = domain::get_instance().get_actions();
	for (it_acs = actions.begin(); it_acs != actions.end(); it_acs++) {
		act_name = it_acs->get_name();
		to_print << "action(" << act_name << ").\n";
		switch ( it_acs->get_type() ) {
		case ONTIC:
			to_print << "ontic_action(" << act_name << ").\n";
			break;
		case SENSING:
			to_print << "sensing_action(" << act_name << ").\n";
			break;
		case ANNOUNCEMENT:
			to_print << "announcement_action(" << act_name << ").\n";
			break;
		default:
			to_print << "ontic_action(" << act_name << ").\n";
			break;
		}

		//it_acs->get_executability()
		if (it_acs->get_executability().size() > 0) {
			std::string ex_conditions = print_subformula(it_acs->get_executability(), to_print);
			to_print << "executable(" << act_name << ", ";

			if (!ex_conditions.empty()) {
				to_print << ex_conditions;
			} else {
				to_print << "true";
			}
			to_print << ").\n";
		}


		observability_map::const_iterator it_obs;
		observability_map tmp_map = it_acs->get_fully_observants();
		for (it_obs = tmp_map.begin(); it_obs != tmp_map.end(); it_obs++) {

			std::string obs_conditions = print_subformula(it_obs->second, to_print);
			to_print << "fully_obs(" << act_name << ", ";
			to_print << m_grounder.deground_agent(it_obs->first);
			if (!obs_conditions.empty()) {
				to_print << ", " << obs_conditions;
			} else {
				to_print << ", true";
			}
			to_print << ").\n";
		}

		tmp_map = it_acs->get_partially_observants();
		for (it_obs = tmp_map.begin(); it_obs != tmp_map.end(); it_obs++) {
			std::string obs_conditions = print_subformula(it_obs->second, to_print);
			to_print << "partial_obs(" << act_name << ", ";
			to_print << m_grounder.deground_agent(it_obs->first);
			if (!obs_conditions.empty()) {
				to_print << ", " << obs_conditions;
			} else {
				to_print << ", true";
			}
			to_print << ").\n";
		}


		to_print << "\n\n";
	}
	to_print << "\n\n";
}

void asp_maker::print_agents(std::ofstream & to_print) const
{
	to_print << "% *** AGENTS ***\n";
	agent_set::const_iterator it_ags;
	agent_set agents = domain::get_instance().get_agents();
	for (it_ags = agents.begin(); it_ags != agents.end(); it_ags++) {
		to_print << "agent(" << m_grounder.deground_agent(*it_ags) << ").\n";
	}
	to_print << "\n\n";
}

void asp_maker::print_initially(std::ofstream & to_print)
{
	to_print << "% *** INITIALLY ***\n";
	initially ini_conditions = domain::get_instance().get_initial_description();

	to_print << "\n% ** initial pointed **\n";
	//fluent_formula ini_ff = ini_conditions.get_pointed_world_conditions();
	fluent_set::const_iterator it_fs;
	fluent_set fs = *ini_conditions.get_pointed_world_conditions().begin();

	std::string ini_string;
	for (it_fs = fs.begin(); it_fs != fs.end(); it_fs++) {
		ini_string = print_subformula(*it_fs, to_print);
		to_print << "initially(" << ini_string << ").\n";
	}

	to_print << "\n% ** initial beliefs **\n";

	formula_list::const_iterator it_fl;
	for (it_fl = ini_conditions.get_initial_conditions().begin(); it_fl != ini_conditions.get_initial_conditions().end(); it_fl++) {
		ini_string = print_subformula(*it_fl, to_print);
		to_print << "initially(" << ini_string << ").\n";
	}
	to_print << "\n\n";
}

void asp_maker::print_goals(std::ofstream & to_print)
{
	to_print << "% *** GOALS ***\n";

	formula_list goals = domain::get_instance().get_goal_description();
	formula_list::const_iterator it_fl;

	std::string goal_string;
	for (it_fl = goals.begin(); it_fl != goals.end(); it_fl++) {
		goal_string = print_subformula(*it_fl, to_print);
		to_print << "goal(" << goal_string << ").\n";
	}
	to_print << "\n\n";
}

/*From https://www.geeksforgeeks.org/generate-all-the-binary-strings-of-n-bits/ since is like generating all the binary numbers.*/
void asp_maker::print_all_fluent_set(fluent_set& permutation, unsigned int index, int & permutation_number, std::ofstream & to_print) const
{
	fluent_set::const_iterator it_fls;
	if (index / 2 == domain::get_instance().get_fluent_number()) {
		to_print << "fluent_set(" << permutation_number << ").\t%";
		for (it_fls = permutation.begin(); it_fls != permutation.end(); it_fls++) {
			if (*it_fls % 2 == 0) {
				to_print << " ";
			}
			to_print << m_grounder.deground_fluent(*it_fls);
			if (std::next(it_fls, 1) != permutation.end()) {
				to_print << ", ";
			}
		}
		to_print << "\n";

		for (it_fls = permutation.begin(); it_fls != permutation.end(); it_fls++) {
			if (*it_fls % 2 == 0) {
				to_print << "holds(" << permutation_number << ", " << m_grounder.deground_fluent(*it_fls) << ").\n";
			}
		}
		to_print << "\n";

		permutation_number++;

		return;
	}
	fluent_set permutation_2 = permutation;
	//Add the \ref fluent in positive version
	permutation.insert(index);
	print_all_fluent_set(permutation, index + 2, permutation_number, to_print);

	//Add the \ref fluent in negative version
	permutation_2.insert(index + 1);
	print_all_fluent_set(permutation_2, index + 2, permutation_number, to_print);
}

void asp_maker::print_worlds(std::ofstream & to_print) const
{
	to_print << "% *** WORLDS ***\n";

	fluent_set permutation;
	int fl_number = 1;
	print_all_fluent_set(permutation, 0, fl_number, to_print);
}
