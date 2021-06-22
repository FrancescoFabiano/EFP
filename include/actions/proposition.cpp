/*
 * \brief Implementation of \ref proposition.h.
 *
 * \copyright GNU Public License.
 *
 * \author Francesco Fabiano.
 * \date April 1, 2019
 */
#include <iostream>

#include "proposition.h"
#include "../domain/domain.h"
#include "pem_store.h"

proposition_type proposition::get_type() const
{
	return m_type;
}

const std::string & proposition::get_action_name() const
{
	return m_action_name;
}

act_type proposition::get_action_type() const
{
	return m_act_type;
}

fluent_formula proposition::get_action_effect() const
{
	return domain::get_instance().get_grounder().ground_fluent(m_action_effect);
}

agent proposition::get_agent() const
{
	return domain::get_instance().get_grounder().ground_agent(m_agent);
}

agent_group proposition::get_agent_group() const
{
	return m_agent_group;
}

const belief_formula & proposition::get_conditions() const
{
	return m_conditions;
}

const belief_formula & proposition::get_grounded_conditions()
{
	m_conditions.ground();
	return m_conditions;
}

void proposition::set_type(proposition_type to_set)
{
	m_type = to_set;
}

void proposition::set_action_name(const std::string & to_set)
{
	m_action_name = to_set;
}

void proposition::set_action_type(const std::string & to_set)
{
	m_act_type = pem_store::get_instance().get_pem_id(to_set);
}

void proposition::add_action_effect(const string_set & to_add)
{
	m_action_effect.insert(to_add);
}

void proposition::set_action_effect(const string_set_set & to_set)
{
	m_action_effect = to_set;
}

void proposition::set_agent(const std::string & to_set)
{
	m_agent = to_set;
}

void proposition::set_agent_group(const std::string & to_set)
{
	m_agent_group = pem_store::get_instance().get_agent_group(to_set);
}

void proposition::set_conditions(const belief_formula & to_set)
{
	m_conditions = to_set;
}

void proposition::print() const
{
	switch ( m_type ) {
	case EFFECTS:
		std::cout << get_action_name() << " has_effects ";
		printer::get_instance().print_list(get_action_effect());
		std::cout << " if ";
		get_conditions().print();
		break;
	case EXECUTABILITY:
		std::cout << get_action_name() << " executable if ";
		get_conditions().print();
		break;
	case OBSERVABILITY:
		std::cout << get_agent() << " belongs to group " << pem_store::get_instance().get_agent_group_name(get_agent_group()) << " if ";
		get_conditions().print();
	case TYPE:
		std::cout << get_action_name() << " has_type " << pem_store::get_instance().get_pem_name(get_action_type());
	default: /* static */
		break;
	}
};