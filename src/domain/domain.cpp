/**
 * \brief Implementation of \ref domain.h.
 *
 * \copyright GNU Public License.
 *
 * \author Francesco Fabiano, Alessandro Burigana.
 * \date April 1, 2019
 */
#include <boost/dynamic_bitset.hpp>
#include <utility>

#include "domain.h"
#include "../../include/definitions/domain_def.h"
#include "../actions/action.h"
#include "../utilities/helper.h"

domain::domain(domain_config config, cem_store store, finitary_theory theory)
        : m_finitary_theory(std::move(theory)), m_store(std::move(store)), m_config(std::move(config)) {}

const domain_config & domain::get_config() const {
    return m_config;
}

void domain::set_config(const domain_config & to_set_config) {
    domain::m_config = to_set_config;
    domain::m_grounder = grounder();
	m_intial_description = initially();
}

void domain::set_store(const cem_store &store) {
    m_store = store;
}

const cem_store domain::get_store() const {
    return m_store;
}

const grounder & domain::get_grounder() const {
	return domain::m_grounder;
}

const fluent_ptr_set & domain::get_fluent_set() const {
	return m_fluent_ptr_set;
}

unsigned int domain::get_fluent_number() const {
	return(m_fluent_set.size() / 2);
}

unsigned int domain::get_size_fluent()  const {
	return(m_fluent_set.begin()->size());
}

const action_set & domain::get_actions() const {
	return m_action_set;
}

const agent_ptr_set & domain::get_agent_set() const {
	return m_agent_ptr_set;
}

const finitary_theory &domain::get_finitary_theory() const {
    return m_finitary_theory;
}

const initially & domain::get_initial_description() const {
	return m_intial_description;
}

const formula_list & domain::get_goal_description() const {
	return m_goal_description;
}

void domain::build(const grounder &grounder, reader &reader, const printer &printer) {
    build_agents(reader);
    build_fluents(reader);
    build_actions(grounder, reader);
    build_initially(grounder, reader, printer);
    build_goal(grounder, reader);
}

void domain::build_agents(const reader &reader) {
	/*
	 * This function set the grounder agent map with the correct values.
	 */
	agent_map domain_agent_map;
	std::cout << "\nBuilding agent list..." << std::endl;
	string_set::const_iterator it_agents;
	int i = 0;

	//ottengo il numero degli agenti e inizio a generare un bitset dinamico con n bit
	//posizione x se bit a 1 esiste un agente altrimenti 0.
	int agents_length = helper::lenght_to_power_two(reader.m_agents.size());

	for (it_agents = reader.m_agents.begin(); it_agents != reader.m_agents.end(); it_agents++) {
		const agent ag(agents_length, i);
        m_agent_set.insert(ag);
        domain_agent_map.insert(agent_map::value_type(*it_agents, &ag));

		if (m_config.is_debug()) {
			std::cout << "Agent " << *it_agents << " is " << ag << std::endl;

		}
		i++;
	}
	domain::m_grounder.set_agent_map(domain_agent_map);
}

void domain::build_fluents(const reader &reader) {
	/*
	 * This function set the grounder fluent map with the correct values.
	 */
	fluent_map domain_fluent_map;
	std::cout << "\nBuilding fluent literals..." << std::endl;
	string_set::const_iterator it_fluents;
	int i = 0;
	int bit_size = helper::lenght_to_power_two(reader.m_fluents.size());
	//todo prende numero fluenti*2 e generare i bit necessari
	for (it_fluents = reader.m_fluents.begin(); it_fluents != reader.m_fluents.end(); it_fluents++) {
        // todo: chiedi a Francesco come gestisce i fluenti
		fluent fluentReal(bit_size + 1, i);
		fluentReal.set(fluentReal.size() - 1, 0);
		domain_fluent_map.insert(fluent_map::value_type(*it_fluents, &fluentReal));
		m_fluent_set.insert(fluentReal);

		if (m_config.is_debug()) {
			std::cout << "Literal " << *it_fluents << " is " << " " << fluentReal << std::endl;
		}

        fluent fluent_negate_real(bit_size + 1, i);
		fluent_negate_real.set(fluent_negate_real.size() - 1, 1);
		domain_fluent_map.insert(fluent_map::value_type(NEGATION_SYMBOL + *it_fluents, &fluent_negate_real));
		m_fluent_set.insert(fluent_negate_real);
		i++;
		if (m_config.is_debug()) {
			std::cout << "Literal not " << *it_fluents << " is " << (i - 1) << " " << fluent_negate_real << std::endl;
		}
	}
	domain::m_grounder.set_fluent_map(domain_fluent_map);
}

void domain::build_actions(const grounder &grounder, reader &reader) {
	/*
	 * This function set the grounder action map with the correct values.
	 */
	auto tot_ags = get_agent_set();
	auto tot_fluents = get_fluent_set();
	action_name_map domain_action_name_map;
	std::cout << "\nBuilding action list..." << std::endl;
	string_set::const_iterator it_actions_name;
	int i = 0;
	unsigned long numberOfActions = reader.m_actions.size();
	int bit_size = helper::lenght_to_power_two(numberOfActions);

	for (it_actions_name = reader.m_actions.begin();
		it_actions_name != reader.m_actions.end(); it_actions_name++) {
		boost::dynamic_bitset<> action_bitset(bit_size, i);
		action tmp_action(*it_actions_name, action_bitset, m_fluent_set, m_agent_set, 0);     // \todo: ***FIX*** AGENT GROUPS NUMBER
		domain_action_name_map.insert(action_name_map::value_type(*it_actions_name, action_bitset));
		i++;
		m_action_set.insert(tmp_action);
		if (m_config.is_debug()) {
			std::cout << "Action " << tmp_action.get_name() << " is " << tmp_action.get_id() << std::endl;
		}
	}

	domain::m_grounder.set_action_name_map(domain_action_name_map);
//	printer::get_instance().set_grounder(domain::domain_grounder);

    build_propositions(grounder, reader);

	if (m_config.is_debug()) {
		std::cout << "\nPrinting complete action list..." << std::endl;
		action_set::const_iterator it_actions;

		for (it_actions = m_action_set.begin();
             it_actions != m_action_set.end(); it_actions++) {
			it_actions->print();
		}
	}

}

void domain::build_propositions(const grounder &grounder, reader &reader) {
	//Adding propositions to actions list
	std::cout << "\nAdding propositions to actions..." << std::endl;
	proposition_list::iterator it_prop;
	action_id action_to_modify;
	for (it_prop = reader.m_propositions.begin();
		it_prop != reader.m_propositions.end(); it_prop++) {
		action_to_modify = domain::m_grounder.ground_action(it_prop->get_action_name());
		//To change remove and add the updated --> @TODO: find better like queue

		for (std::set<action>::iterator actionsList = m_action_set.begin();
             actionsList != m_action_set.end(); actionsList++) {
			action_id actionTemp = action_to_modify;
			actionTemp.set(actionTemp.size() - 1, 0);
			if (m_action_set.size() > actionTemp.to_ulong() && actionsList->get_id() == action_to_modify) {
				action_set::iterator it_action_set = actionsList;
				action tmp = *it_action_set;
				tmp.add_proposition(grounder,*it_prop);
				m_action_set.erase(it_action_set);
				m_action_set.insert(tmp);
				break;
			}
		}
	}
}

void domain::build_initially(const grounder &grounder, reader &reader, const printer &printer) {
	std::cout << "\nAdding to pointed world and initial conditions..." << std::endl;
	formula_list::iterator it_fl;

	for (it_fl = reader.m_bf_initially.begin(); it_fl != reader.m_bf_initially.end(); it_fl++) {
		it_fl->ground(grounder);

		switch ( it_fl->get_formula_type() ) { //initially phi
			//S5 -> pointed world
            case FLUENT_FORMULA: {
                m_intial_description.add_pointed_condition(it_fl->get_fluent_formula());

                if (m_config.is_debug()) {
                    std::cout << "	Pointed world: ";
                    printer.print_list(it_fl->get_fluent_formula());
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
                m_intial_description.add_initial_condition(m_config.get_initial_state_mode(), *it_fl);
                if (m_config.is_debug()) {
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
	switch (m_config.get_initial_state_mode()) {
        case Initial_State_Mode::FINITARY_S5_THEORY: {
            m_intial_description.set_ff_forS5(m_config.get_initial_state_mode());
            break;
        }
        case Initial_State_Mode::CUSTOM_INITIAL_STATE:
        default: {
            break;
        }
	}
}

void domain::build_goal(const grounder &grounder, reader &reader) {
	std::cout << "\nAdding to Goal..." << std::endl;
	formula_list::iterator it_fl;

	for (it_fl = reader.m_bf_goal.begin(); it_fl != reader.m_bf_goal.end(); it_fl++) {
        it_fl->ground(grounder);
        m_goal_description.push_back(*it_fl);
        if (m_config.is_debug()) {
            std::cout << "	";
            it_fl->print();
            std::cout << std::endl;
        }
	}
}
