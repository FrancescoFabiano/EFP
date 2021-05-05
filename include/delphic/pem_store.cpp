/**
 * \brief Implementation of \ref pem_store.h
 * \copyright GNU Public License.
 *
 * \author Francesco Fabiano.
 * \date April 28, 2021
 */

#include "pem_store.h"
#include "pem.h"

pem_store::pem_store()
{
}

pem_store& pem_store::get_instance()
{
	static pem_store instance;
	return instance;
}


void pem_store::generate(const std::string & file){
	return;
}

const event_ptr pem_store::add_event(const event & to_add)
{
	auto tmp_ptr = std::make_shared<event>(*(std::get<0>(m_created_events.insert(to_add))));
	return tmp_ptr;
}

const pem_ptr pem_store::add_pem(const pem & to_add)
{
    auto tmp_ptr = std::make_shared<pem>(*(std::get<0>(m_created_pems.insert(to_add))));
    return tmp_ptr;
}

//void pem_store::add_action_pem(const event & to_add, event_id id)
//{
//	add_action_pem(add_event(to_add), id);
//}
//
//void pem_store::add_action_pem(const event_ptr & to_add, event_id id)
//{
//	if (m_created_events.find(to_add.get_ptr()) == m_created_events.end()) {
//        m_created_events.insert(std::pair<event_id, event_ptr>(id, to_add));
//	} else {
//		std::cerr << "\nYou are trying to allocate two Event Models with the same id, this is not possible.";
//		exit(1);
//	}
//}
//
//const event_ptr & pem_store::get_action_pem(event_id id)
//{
//	if (m_created_pems.find(id) != m_created_pems.end()) {
//		return m_created_pems[id];
//	} else {
//		std::cerr << "\nYou have requested an Event Model that does not exists.";
//		exit(1);
//	}
//}