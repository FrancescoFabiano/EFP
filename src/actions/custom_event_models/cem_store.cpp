/**
 * \brief Implementation of \ref cem_store.h
 * \copyright GNU Public License.
 *
 * \author Francesco Fabiano, Alessandro Burigana.
 * \date June 24, 2021
 */

#include "cem_store.h"
#include <boost/algorithm/string.hpp>

cem_store::cem_store()
{
	//belief_formula TRUE;
	//TRUE.set_formula_type(BF_EMPTY);
	//m_epsilon = cem_store::get_instance().add_event(cevent(EPSILON, false, {TRUE}));
}

cem_store& cem_store::get_instance()
{
	static cem_store instance;
	return instance;
}

//void cem_store::generate(const std::string & file)
//{
//    cem_parser::parse(<#initializer#>, file);
//	return;
//}

const cevent_ptr cem_store::add_event(const cevent & to_add)
{
	auto tmp_ptr = std::make_shared<cevent>(*(std::get<0>(m_created_events.insert(to_add))));
	return tmp_ptr;
}

const cem_ptr cem_store::add_cem(const cem & to_add)
{
	auto tmp_ptr = std::make_shared<cem>(*(std::get<0>(m_created_cems.insert(to_add))));
	return tmp_ptr;
}

void cem_store::add_agent_group(const std::string & to_add)
{
	//if (m_agent_group_ids.find(to_add) == m_agent_group_ids.end()) {
	//Insert does not override

	m_agent_group_ids.insert(std::pair<std::string, agent_group_id>(boost::algorithm::to_lower_copy(to_add), m_agent_group_ids.size()));

	//	m_inverse_agent_group_ids.insert(std::pair<agent_group, std::string>(m_inverse_agent_group_ids.size(),to_add));

	//}


}


const cevent_ptr cem_store::get_event(event_id id) const
{
	cevent tmp;
	tmp.set_id(id);

	auto ptr = m_created_events.find(tmp);

	if (ptr != m_created_events.end()) {
		return std::make_shared<cevent>(*ptr);
	} else {
		std::cerr << "\nError: you are requesting an not-existing cevent!\n";
		exit(1);
	}
}

const cem_ptr cem_store::get_cem(cem_id id) const
{
	cem tmp;
	tmp.set_id(id);

	auto ptr = m_created_cems.find(tmp);

	if (ptr != m_created_cems.end()) {
		return std::make_shared<cem>(*ptr);
	} else {
		std::cerr << "\nError: you are requesting an not-existing cem!\n";
		exit(1);
	}
}

agent_group_id cem_store::get_agent_group(const std::string & to_get) const
{
	auto ptr = m_agent_group_ids.find(boost::algorithm::to_lower_copy(to_get));

	if (ptr != m_agent_group_ids.end()) {
		return ptr->second;
	} else {
		std::cerr << "\nError: you are requesting: " << to_get << " that is an not-existing agent group!\n";
		exit(1);
	}

}

std::string cem_store::get_agent_group_name(agent_group_id id) const
{
	for (auto it_map = m_agent_group_ids.begin(); it_map != m_agent_group_ids.end(); ++it_map) {
		if (it_map->second == id)
			return it_map->first;
	}

	std::cerr << "\nError: you are requesting: " << id << " that is an not-existing agent group!\n";
	exit(1);
}

cem_id cem_store::get_cem_id(const std::string & to_get)
{
	//if (m_cem_ids_map.find(to_add) == m_cem_ids_map.end()) {
	//Insert does not override
	auto ret = m_cem_ids_map.insert(std::pair<std::string, cem_id>(boost::algorithm::to_lower_copy(to_get), m_cem_ids_map.size()));
	return ret.first->second;
	//}
}

bool cem_store::exists_cem(cem_id to_check) const
{
	//if (m_cem_ids_map.find(to_add) == m_cem_ids_map.end()) {
	//Insert does not override
	auto it = m_created_cems.find(to_check);
	if (it != m_created_cems.end()) {
		return true;
	}
	return false;
	//}
}

std::string cem_store::get_cem_name(cem_id id) const
{
	for (auto it_map = m_cem_ids_map.begin(); it_map != m_cem_ids_map.end(); ++it_map) {
		if (it_map->second == id)
			return it_map->first;
	}

	std::cerr << "\nError: you are requesting: " << id << " that is an not-existing cem!\n";
	exit(1);
}

event_id cem_store::get_event_id(const std::string & to_get)
{
	//if (m_cem_ids_map.find(to_add) == m_cem_ids_map.end()) {
	//Insert does not override
	auto ret = m_events_ids_map.insert(std::pair<std::string, event_id>(boost::algorithm::to_lower_copy(to_get), m_events_ids_map.size()));
	return ret.first->second;
	//}
}

short cem_store::get_agent_group_number() const
{
	return m_agent_group_ids.size();
}

/*******FOR MAL OPTIMIZATION******/
void cem_store::add_cem_name(const std::string & to_add)
{
    cem_id id_added_cem = m_cem_ids_map.size();
    m_created_cems.insert(cem(id_added_cem));
    m_cem_ids_map.insert(std::pair<std::string, cem_id>(boost::algorithm::to_lower_copy(to_add), id_added_cem));
//    for (auto it = m_cem_ids_map.begin(); it != m_cem_ids_map.end(); ++it)
//    {
//        std::cerr << "\nRecord is: < " << it->first << ", " << it->second <<">" << std::endl;
//    }

}

//void cem_store::add_action_cem(const cevent & to_add, event_id id)
//{
//	add_action_cem(add_event(to_add), id);
//}
//
//void cem_store::add_action_cem(const cevent_ptr & to_add, event_id id)
//{
//	if (m_created_events.find(to_add.get_ptr()) == m_created_events.end()) {
//        m_created_events.insert(std::pair<event_id, cevent_ptr>(id, to_add));
//	} else {
//		std::cerr << "\nYou are trying to allocate two Event Models with the same id, this is not possible.";
//		exit(1);
//	}
//}
//
//const cevent_ptr & cem_store::get_action_cem(event_id id)
//{
//	if (m_created_cems.find(id) != m_created_cems.end()) {
//		return m_created_cems[id];
//	} else {
//		std::cerr << "\nYou have requested an Event Model that does not exists.";
//		exit(1);
//	}
//}