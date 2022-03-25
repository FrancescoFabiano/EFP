/**
 * \brief Implementation of \ref domain.h.
 *
 * \copyright GNU Public License.
 *
 * \author Francesco Fabiano, Alessandro Burigana.
 * \date April 1, 2019
 */
#include "domain.h"
#include "../actions/action.h"
#include <boost/dynamic_bitset.hpp>

domain::domain() = default;

const domain_config & domain::get_config() const {
    return config;
}

void domain::set_config(const domain_config & to_set_config) // (std::string name, bool debug, state_type stype, bool k_opt, boost::shared_ptr<reader> reader, domain_restriction ini_res, domain_restriction goal_res, bool is_global_obsv, action_check act_check, bool check_visited, bis_type bisimulation)
{
    domain::config = to_set_config;
    domain::domain_grounder = grounder();
	m_intial_description = initially();
}

const grounder & domain::get_grounder() const {
	return domain::domain_grounder;
}

const fluent_set & domain::get_fluents() const {
	return m_fluents;
}

unsigned int domain::get_fluent_number() const {
	return(m_fluents.size() / 2);
}

unsigned int domain::get_size_fluent()  const {
	auto fluent_first = m_fluents.begin();
	return(fluent_first->size());
}

const action_set & domain::get_actions() const {
	return m_actions;
}

const agent_set & domain::get_agents() const {
	return m_agents;
}

const initially & domain::get_initial_description() const {
	return m_intial_description;
}

const formula_list & domain::get_goal_description() const {
	return m_goal_description;
}

void domain::build(const grounder& grounder) {
	build_agents();
	build_fluents();
	build_actions(grounder);
	build_initially(grounder);
	build_goal(grounder);
}

void domain::build_agents() {
	/*
	 * This function set the grounder agent map with the correct values.
	 */
	agent_map domain_agent_map;
	std::cout << "\nBuilding agent list..." << std::endl;
	string_set::const_iterator it_agents;
	int i = 0;

	//ottengo il numero degli agenti e inizio a generare un bitset dinamico con n bit
	//posizione x se bit a 1 esiste un agente altrimenti 0.
	int agents_length = helper::lenght_to_power_two(reader::get_instance().m_agents.size());

	for (it_agents = reader::get_instance().m_agents.begin(); it_agents != reader::get_instance().m_agents.end(); it_agents++) {
		boost::dynamic_bitset<> agent(agents_length, i);
		domain_agent_map.insert(agent_map::value_type(*it_agents, agent));
		m_agents.insert(agent);
		if (config.is_debug()) {
			std::cout << "Agent " << *it_agents << " is " << agent << std::endl;

		}
		i++;
	}
	domain::domain_grounder.set_agent_map(domain_agent_map);
}

void domain::build_fluents() {
	/*
	 * This function set the grounder fluent map with the correct values.
	 */
	fluent_map domain_fluent_map;
	std::cout << "\nBuilding fluent literals..." << std::endl;
	string_set::const_iterator it_fluents;
	int i = 0;
	int bit_size = helper::lenght_to_power_two(reader::get_instance().m_fluents.size());
	//todo prende numero fluenti*2 e generare i bit necessari
	for (it_fluents = reader::get_instance().m_fluents.begin();
		it_fluents != reader::get_instance().m_fluents.end(); it_fluents++) {
		boost::dynamic_bitset<> fluentReal(bit_size + 1, i);
		fluentReal.set(fluentReal.size() - 1, 0);
		domain_fluent_map.insert(fluent_map::value_type(*it_fluents, fluentReal));
		m_fluents.insert(fluentReal);

		if (config.is_debug()) {
			std::cout << "Literal " << *it_fluents << " is " << " " << fluentReal << std::endl;
		}

		boost::dynamic_bitset<> fluent_negate_real(bit_size + 1, i);
		fluent_negate_real.set(fluent_negate_real.size() - 1, 1);
		domain_fluent_map.insert(fluent_map::value_type(NEGATION_SYMBOL + *it_fluents, fluent_negate_real));
		m_fluents.insert(fluent_negate_real);
		i++;
		if (config.is_debug()) {
			std::cout << "Literal not " << *it_fluents << " is " << (i - 1) << " " << fluent_negate_real << std::endl;
		}
	}
	domain::domain_grounder.set_fluent_map(domain_fluent_map);
}

void domain::build_actions(const grounder& grounder) {
	/*
	 * This function set the grounder action map with the correct values.
	 */
	auto tot_ags = get_agents();
	auto tot_fluents = get_fluents();
	action_name_map domain_action_name_map;
	std::cout << "\nBuilding action list..." << std::endl;
	string_set::const_iterator it_actions_name;
	int i = 0;
	unsigned long numberOfActions = reader::get_instance().m_actions.size();
	int bit_size = helper::lenght_to_power_two(numberOfActions);

	for (it_actions_name = reader::get_instance().m_actions.begin();
		it_actions_name != reader::get_instance().m_actions.end(); it_actions_name++) {
		boost::dynamic_bitset<> action_bitset(bit_size, i);
		action tmp_action(*it_actions_name, action_bitset, m_fluents, m_agents);
		domain_action_name_map.insert(action_name_map::value_type(*it_actions_name, action_bitset));
		i++;
		m_actions.insert(tmp_action);
		if (config.is_debug()) {
			std::cout << "Action " << tmp_action.get_name() << " is " << tmp_action.get_id() << std::endl;
		}
	}

	domain::domain_grounder.set_action_name_map(domain_action_name_map);
	printer::get_instance().set_grounder(domain::domain_grounder);

	build_propositions(grounder);

	if (config.is_debug()) {
		std::cout << "\nPrinting complete action list..." << std::endl;
		action_set::const_iterator it_actions;

		for (it_actions = m_actions.begin();
			it_actions != m_actions.end(); it_actions++) {
			it_actions->print();
		}
	}

}

void domain::build_propositions(const grounder& grounder) {
	//Adding propositions to actions list
	std::cout << "\nAdding propositions to actions..." << std::endl;
	proposition_list::iterator it_prop;
	action_id action_to_modify;
	for (it_prop = reader::get_instance().m_propositions.begin();
		it_prop != reader::get_instance().m_propositions.end(); it_prop++) {
		action_to_modify = domain::domain_grounder.ground_action(it_prop->get_action_name());
		//To change remove and add the updated --> @TODO: find better like queue

		for (std::set<action>::iterator actionsList = m_actions.begin();
			actionsList != m_actions.end(); actionsList++) {
			action_id actionTemp = action_to_modify;
			actionTemp.set(actionTemp.size() - 1, 0);
			if (m_actions.size() > actionTemp.to_ulong() && actionsList->get_id() == action_to_modify) {
				action_set::iterator it_action_set = actionsList;
				action tmp = *it_action_set;
				tmp.add_proposition(grounder,*it_prop);
				m_actions.erase(it_action_set);
				m_actions.insert(tmp);
				break;
			}
		}
	}
}

void domain::build_initially(const grounder& grounder) {
	std::cout << "\nAdding to pointed world and initial conditions..." << std::endl;
	formula_list::iterator it_fl;

	for (it_fl = reader::get_instance().m_bf_initially.begin(); it_fl != reader::get_instance().m_bf_initially.end(); it_fl++) {
		it_fl->ground(grounder);

		switch ( it_fl->get_formula_type() ) { //initially phi
			//S5 -> pointed world
            case FLUENT_FORMULA: {
                m_intial_description.add_pointed_condition(it_fl->get_fluent_formula());

                if (config.is_debug()) {
                    std::cout << "	Pointed world: ";
                    printer::get_instance().print_list(it_fl->get_fluent_formula());
                    std::cout << std::endl;
                }
                break;
            }
            //S5 -> Edge
            case C_FORMULA:
                //No more S5
            case D_FORMULA:
                //Possible S5 -> initally [C(phi2), C(phi2)]
            case PROPOSITIONAL_FORMULA:
                //No more S5
            case BELIEF_FORMULA:
                //No more S5
            case E_FORMULA: {
                m_intial_description.add_initial_condition(config.get_initial_state_mode(), *it_fl);
                if (config.is_debug()) {
                    std::cout << "Added to initial conditions: ";
                    it_fl->print();
                    std::cout << std::endl;
                }
                break;
            }
            case BF_EMPTY:
                break;
            default:
                std::cerr << "Error in the 'initially' conditions." << std::endl;
                exit(1);
		}
	}

	// Given the initial state mode, the initial state might need different function
	switch (config.get_initial_state_mode()) {
        case Initial_State_Mode::FINITARY_S5_THEORY: {
            m_intial_description.set_ff_forS5(config.get_initial_state_mode());
            break;
        }
        case Initial_State_Mode::CUSTOM_INITIAL_STATE:
        default: {
            break;
        }
	}
}

void domain::build_goal(const grounder& grounder) {
	std::cout << "\nAdding to Goal..." << std::endl;
	formula_list::iterator it_fl;

	for (it_fl = (reader::get_instance().m_bf_goal).begin(); it_fl != (reader::get_instance().m_bf_goal).end(); it_fl++) {
        it_fl->ground(grounder);
        m_goal_description.push_back(*it_fl);
        if (config.is_debug()) {
            std::cout << "	";
            it_fl->print();
            std::cout << std::endl;
        }
	}
}
