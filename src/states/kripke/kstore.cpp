/**
 * \brief Implementation of \ref kstore.h
 * \copyright GNU Public License.
 *
 * \author Francesco Fabiano, Alessandro Burigana.
 * \date April 9, 2019
 */


#include "kstore.h"

kstore::kstore() = default;

kstore & kstore::get_instance() {
    static kstore instance;
    return instance;
}

const kworld* kstore::add_world(const kworld *to_add) {
    auto res = m_created_worlds.insert(to_add);
    return *res.first;
}
