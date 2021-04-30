/**
 * \brief Implementation of \ref pem_store.h
 * \copyright GNU Public License.
 *
 * \author Francesco Fabiano.
 * \date April 28, 2021
 */

#include "pem_store.h"

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

const pem_ptr & pem_store::add_pem(const pem & to_add)
{
	auto tmp_ptr = std::make_shared<pem>(*(std::get<0>(m_created_pems.insert(to_add))));
	return tmp_ptr;
}

void pem_store::add_action_pem(const pem & to_add, pem_id id)
{
	add_action_pem(add_pem(to_add), id);
}

void pem_store::add_action_pem(const pem_ptr & to_add, pem_id id)
{
	if (m_action_pems.find(id) == m_action_pems.end()) {
		m_action_pems.insert(std::pair<pem_id, pem_ptr>(id, to_add));
	} else {
		std::cerr << "\nYou are trying to allocate two Event Models with the same id, this is not possible.";
		exit(1);
	}
}

const pem_ptr & pem_store::get_action_pem(pem_id id)
{
	if (m_action_pems.find(id) != m_action_pems.end()) {
		return m_action_pems[id];
	} else {
		std::cerr << "\nYou have requested an Event Model that does not exists.";
		exit(1);
	}
}