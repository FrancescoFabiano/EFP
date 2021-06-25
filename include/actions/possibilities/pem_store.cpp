/**
 * \brief Implementation of \ref pem_store.h
 * \copyright GNU Public License.
 *
 * \author Francesco Fabiano.
 * \date April 28, 2021
 */

#include "pem_store.h"
#include "pem.h"
#include "../../parse/pem_parser.h"

pem_store::pem_store()
{
	//belief_formula TRUE;
	//TRUE.set_formula_type(BF_EMPTY);
	//m_epsilon = pem_store::get_instance().add_event(pevent(EPSILON, false, {TRUE}));
}

pem_store& pem_store::get_instance()
{
	static pem_store instance;
	return instance;
}

void pem_store::generate(const std::string & file)
{
	pem_parser::parse(file);
	return;
}

const pevent_ptr pem_store::add_event(const pevent & to_add)
{
	auto tmp_ptr = std::make_shared<pevent>(*(std::get<0>(m_created_events.insert(to_add))));
	return tmp_ptr;
}

const pem_ptr pem_store::add_pem(const pem & to_add)
{
	auto tmp_ptr = std::make_shared<pem>(*(std::get<0>(m_created_pems.insert(to_add))));
	return tmp_ptr;
}

void pem_store::add_agent_group(const std::string & to_add)
{
	//if (m_agent_group_ids.find(to_add) == m_agent_group_ids.end()) {
	//Insert does not override	
	m_agent_group_ids.insert(std::pair<std::string, agent_group_id>(to_add, m_agent_group_ids.size()));

	//	m_inverse_agent_group_ids.insert(std::pair<agent_group, std::string>(m_inverse_agent_group_ids.size(),to_add));

	//}


}

const pevent_ptr pem_store::get_event(event_id id) const
{
	pevent tmp;
	tmp.set_id(id);

	auto ptr = m_created_events.find(tmp);

	if (ptr != m_created_events.end()) {
		return std::make_shared<pevent>(*ptr);
	} else {
		std::cerr << "\nError: you are requesting an not-existing pevent!\n";
		exit(1);
	}
}

const pem_ptr pem_store::get_pem(pem_id id) const
{
	pem tmp;
	tmp.set_id(id);

	auto ptr = m_created_pems.find(tmp);

	if (ptr != m_created_pems.end()) {
		return std::make_shared<pem>(*ptr);
	} else {
		std::cerr << "\nError: you are requesting an not-existing pem!\n";
		exit(1);
	}
}

agent_group_id pem_store::get_agent_group(const std::string & to_get) const
{
	auto ptr = m_agent_group_ids.find(to_get);

	if (ptr != m_agent_group_ids.end()) {
		return ptr->second;
	} else {
		std::cerr << "\nError: you are requesting: " << to_get << " that is an not-existing agent group!\n";
		exit(1);
	}

}

std::string pem_store::get_agent_group_name(agent_group_id id) const
{
	for (auto it_map = m_agent_group_ids.begin(); it_map != m_agent_group_ids.end(); ++it_map) {
		if (it_map->second == id)
			return it_map->first;
	}

	std::cerr << "\nError: you are requesting: " << id << " that is an not-existing agent group!\n";
	exit(1);
}

pem_id pem_store::get_pem_id(const std::string & to_get)
{
	//if (m_pem_ids_map.find(to_add) == m_pem_ids_map.end()) {
	//Insert does not override
	auto ret = m_pem_ids_map.insert(std::pair<std::string, pem_id>(to_get, m_pem_ids_map.size()));
	return ret.first->second;
	//}
}

bool pem_store::exist_pem(pem_id to_check)
{
	//if (m_pem_ids_map.find(to_add) == m_pem_ids_map.end()) {
	//Insert does not override
	auto it = m_created_pems.find(to_check);
	if (it != m_created_pems.end()) {
		return true;
	}
	return false;
	//}
}

std::string pem_store::get_pem_name(pem_id id) const
{
	for (auto it_map = m_pem_ids_map.begin(); it_map != m_pem_ids_map.end(); ++it_map) {
		if (it_map->second == id)
			return it_map->first;
	}

	std::cerr << "\nError: you are requesting: " << id << " that is an not-existing pem!\n";
	exit(1);
}

event_id pem_store::get_event_id(const std::string & to_get)
{
	//if (m_pem_ids_map.find(to_add) == m_pem_ids_map.end()) {
	//Insert does not override
	auto ret = m_events_ids_map.insert(std::pair<std::string, event_id>(to_get, m_events_ids_map.size()));
	return ret.first->second;
	//}
}

short pem_store::get_agent_group_number() const
{
	return m_agent_group_ids.size();
}



//void pem_store::add_action_pem(const pevent & to_add, event_id id)
//{
//	add_action_pem(add_event(to_add), id);
//}
//
//void pem_store::add_action_pem(const pevent_ptr & to_add, event_id id)
//{
//	if (m_created_events.find(to_add.get_ptr()) == m_created_events.end()) {
//        m_created_events.insert(std::pair<event_id, pevent_ptr>(id, to_add));
//	} else {
//		std::cerr << "\nYou are trying to allocate two Event Models with the same id, this is not possible.";
//		exit(1);
//	}
//}
//
//const pevent_ptr & pem_store::get_action_pem(event_id id)
//{
//	if (m_created_pems.find(id) != m_created_pems.end()) {
//		return m_created_pems[id];
//	} else {
//		std::cerr << "\nYou have requested an Event Model that does not exists.";
//		exit(1);
//	}
//}