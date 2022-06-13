/*
 * \brief Implementation of \ref pworld.h and \ref pworld_ptr.h.
 *
 * \copyright GNU Public License.
 *
 * \author Francesco Fabiano, Alessandro Burigana.
 * \date September 14, 2019
 */

#include "pworld.h"

pworld::pworld() :
        m_fluent_set(nullptr),
        m_event(nullptr),
        m_id(0) {}

pworld::pworld(const fluent_ptr_set *fluent_set, const cevent *event, const unsigned long id) :
        m_fluent_set(fluent_set),
        m_event(event),
        m_id(id) {}

const fluent_ptr_set * pworld::get_fluent_set() const {
    return m_fluent_set;
}

unsigned long pworld::get_id() const {
    return m_id;
}

bool pworld::operator==(const pworld& to_compare) const {
    return m_id == to_compare.get_id();
}

bool pworld::operator<(const pworld& to_compare) const {
    return m_id < to_compare.get_id();
}

bool pworld::operator>(const pworld& to_compare) const {
    return m_id < to_compare.get_id();
}

pworld &pworld::operator=(const pworld &world) {
//    pworld(world).swap(*this);
    // todo: check for self-assignment
    return *this;
//    if (this != &world) {
//        return *this;
//    }
}
