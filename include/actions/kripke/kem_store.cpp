/**
 * \brief Implementation of \ref kem_store.h
 * \copyright GNU Public License.
 *
 * \author Francesco Fabiano.
 * \date June 24, 2021
 */

#include "kem_store.h"
#include "kem.h"
#include "../../parse/em_parser.h"

kem_store::kem_store()
{
    //belief_formula TRUE;
    //TRUE.set_formula_type(BF_EMPTY);
    //m_epsilon = kem_store::get_instance().add_event(kevent(EPSILON, false, {TRUE}));
}

kem_store& kem_store::get_instance()
{
    static kem_store instance;
    return instance;
}

void kem_store::generate(const std::string & file)
{
    pem_parser::parse(file);
    return;
}

const kevent_ptr kem_store::add_event(const kevent & to_add)
{
    auto tmp_ptr = std::make_shared<kevent>(*(std::get<0>(m_created_events.insert(to_add))));
    return tmp_ptr;
}

const kem_ptr kem_store::add_kem(const kem & to_add)
{
    auto tmp_ptr = std::make_shared<kem>(*(std::get<0>(m_created_kems.insert(to_add))));
    return tmp_ptr;
}

void kem_store::add_agent_group(const std::string & to_add)
{
    //if (m_agent_group_ids.find(to_add) == m_agent_group_ids.end()) {
    //Insert does not override
    m_agent_group_ids.insert(std::pair<std::string, agent_group_id>(to_add, m_agent_group_ids.size()));

    //	m_inverse_agent_group_ids.insert(std::pair<agent_group, std::string>(m_inverse_agent_group_ids.size(),to_add));

    //}


}

const kevent_ptr kem_store::get_event(event_id id) const
{
    kevent tmp;
    tmp.set_id(id);

    auto ptr = m_created_events.find(tmp);

    if (ptr != m_created_events.end()) {
        return std::make_shared<kevent>(*ptr);
    } else {
        std::cerr << "\nError: you are requesting an not-existing kevent!\n";
        exit(1);
    }
}

const kem_ptr kem_store::get_kem(kem_id id) const
{
    kem tmp;
    tmp.set_id(id);

    auto ptr = m_created_kems.find(tmp);

    if (ptr != m_created_kems.end()) {
        return std::make_shared<kem>(*ptr);
    } else {
        std::cerr << "\nError: you are requesting an not-existing kem!\n";
        exit(1);
    }
}

agent_group_id kem_store::get_agent_group(const std::string & to_get) const
{
    auto ptr = m_agent_group_ids.find(to_get);

    if (ptr != m_agent_group_ids.end()) {
        return ptr->second;
    } else {
        std::cerr << "\nError: you are requesting: " << to_get << " that is an not-existing agent group!\n";
        exit(1);
    }

}

std::string kem_store::get_agent_group_name(agent_group_id id) const
{
    for (auto it_map = m_agent_group_ids.begin(); it_map != m_agent_group_ids.end(); ++it_map) {
        if (it_map->second == id)
            return it_map->first;
    }

    std::cerr << "\nError: you are requesting: " << id << " that is an not-existing agent group!\n";
    exit(1);
}

kem_id kem_store::get_kem_id(const std::string & to_get)
{
    //if (m_kem_ids_map.find(to_add) == m_kem_ids_map.end()) {
    //Insert does not override
    auto ret = m_kem_ids_map.insert(std::pair<std::string, kem_id>(to_get, m_kem_ids_map.size()));
    return ret.first->second;
    //}
}

bool kem_store::exist_kem(kem_id to_check)
{
    //if (m_kem_ids_map.find(to_add) == m_kem_ids_map.end()) {
    //Insert does not override
    auto it = m_created_kems.find(to_check);
    if (it != m_created_kems.end()) {
        return true;
    }
    return false;
    //}
}

std::string kem_store::get_kem_name(kem_id id) const
{
    for (auto it_map = m_kem_ids_map.begin(); it_map != m_kem_ids_map.end(); ++it_map) {
        if (it_map->second == id)
            return it_map->first;
    }

    std::cerr << "\nError: you are requesting: " << id << " that is an not-existing kem!\n";
    exit(1);
}

event_id kem_store::get_event_id(const std::string & to_get)
{
    //if (m_kem_ids_map.find(to_add) == m_kem_ids_map.end()) {
    //Insert does not override
    auto ret = m_events_ids_map.insert(std::pair<std::string, event_id>(to_get, m_events_ids_map.size()));
    return ret.first->second;
    //}
}

short kem_store::get_agent_group_number() const
{
    return m_agent_group_ids.size();
}



//void kem_store::add_action_kem(const kevent & to_add, event_id id)
//{
//	add_action_kem(add_event(to_add), id);
//}
//
//void kem_store::add_action_kem(const kevent_ptr & to_add, event_id id)
//{
//	if (m_created_events.find(to_add.get_ptr()) == m_created_events.end()) {
//        m_created_events.insert(std::pair<event_id, kevent_ptr>(id, to_add));
//	} else {
//		std::cerr << "\nYou are trying to allocate two Event Models with the same id, this is not possible.";
//		exit(1);
//	}
//}
//
//const kevent_ptr & kem_store::get_action_kem(event_id id)
//{
//	if (m_created_kems.find(id) != m_created_kems.end()) {
//		return m_created_kems[id];
//	} else {
//		std::cerr << "\nYou have requested an Event Model that does not exists.";
//		exit(1);
//	}
//}