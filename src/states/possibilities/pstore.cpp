/*
 * \brief Implementation of \ref pstore.h
 * \copyright GNU Public License.
 *
 * \author Francesco Fabiano, Alessandro Burigana.
 * \date September 14, 2019
 */


#include "pstore.h"

pstore::pstore()
{
}

pstore& pstore::get_instance()
{
	static pstore instance;
	return instance;
}

const pworld* pstore::add_world(const pworld *to_add) {
    auto res = m_created_worlds.insert(to_add);
    return *res.first;
}
