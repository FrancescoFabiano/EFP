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

const pem_ptr pem_store::add_pem(const pem & to_add)
{
    auto tmp_ptr = std::make_shared<pem>(*(std::get<0>(m_created_pems.insert(to_add))));
    return tmp_ptr;
}
