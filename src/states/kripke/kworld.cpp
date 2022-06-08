/**
 * \brief Implementation of \ref kworld.h and \ref kworld_ptr.h.
 *
 * \copyright GNU Public License.
 *
 * \author Francesco Fabiano, Alessandro Burigana.
 * \date March 31, 2019
 */

#include "kworld.h"

kworld::kworld() :
    m_fluent_set(nullptr),
    m_event(nullptr),
    m_id(0) {}

kworld::kworld(const fluent_set *fluent_set, const cevent *event, const unsigned long id) :
    m_fluent_set(fluent_set),
    m_event(event),
    m_id(id) {}

const fluent_set * kworld::get_fluent_set() const {
    return m_fluent_set;
}

unsigned long kworld::get_id() const {
    return m_id;
}

bool kworld::operator==(const kworld& to_compare) const {
    return m_id == to_compare.get_id();
}

bool kworld::operator<(const kworld& to_compare) const {
    return m_id < to_compare.get_id();
}

bool kworld::operator>(const kworld& to_compare) const {
    return m_id < to_compare.get_id();
}

kworld &kworld::operator=(const kworld &world) {
//    kworld(world).swap(*this);
    // todo: check for self-assignment
    return *this;
//    if (this != &world) {
//        return *this;
//    }
}



//bool kworld::operator=(const kworld & to_assign) {
//    set_fluent_set(to_assign.get_fluent_set());
//    set_id();
//    return true;
//}

//void kworld::print() const
//{
//    std::cout << "\nFluents: " << get_id();
//    printer::get_instance().print_list(m_fluent_set);
//}

//*-***************************************************************************************************************-*/
//
//kworld_ptr::kworld_ptr() = default;
//
//kworld_ptr::kworld_ptr(const std::shared_ptr<const kworld> & ptr, unsigned short repetition)
//{
//    set_ptr(ptr);
//    set_repetition(repetition);
//}
//
//kworld_ptr::kworld_ptr(std::shared_ptr<const kworld>&& ptr, unsigned short repetition)
//{
//    set_ptr(ptr);
//    set_repetition(repetition);
//
//}
//
//kworld_ptr::kworld_ptr(const kworld & world, unsigned short repetition)
//{
//    m_ptr = std::make_shared<kworld>(world);
//    set_repetition(repetition);
//
//}
//
//void kworld_ptr::set_ptr(const std::shared_ptr<const kworld> & ptr)
//{
//    m_ptr = ptr;
//}
//
//void kworld_ptr::set_ptr(std::shared_ptr<const kworld>&& ptr)
//{
//    m_ptr = ptr;
//}
//
//std::shared_ptr<const kworld> kworld_ptr::get_ptr() const
//{
//    return m_ptr;
//}
//
//const fluent_set & kworld_ptr::get_fluent_set() const
//{
//    if (m_ptr != nullptr) {
//        return get_ptr()->get_fluent_set();
//    }
//    std::cerr << "Error in creating a kworld_ptr (fluent_set)\n";
//    exit(1);
//}
//
//kworld_id kworld_ptr::get_fluent_based_id() const
//{
//    if (m_ptr != nullptr) {
//        return(get_ptr()->get_id());
//    }
//    std::cerr << "\nError in creating a kworld_ptr (id)\n";
//    exit(1);
//}
//
//kworld_id kworld_ptr::get_id() const
//{
//    if (m_ptr != nullptr) {
//        return(get_ptr()->get_id()).append(std::to_string(get_repetition()));
//    }
//    std::cerr << "\nError in creating a kworld_ptr (id)\n";
//    exit(1);
//}
//
//unsigned int kworld_ptr::get_numerical_id() const
//{
//    if (m_ptr != nullptr) {
//        return(get_ptr()->get_numerical_id());
//    }
//    std::cerr << "\nError in creating a kworld_ptr (id)\n";
//    exit(1);
//}
//
//void kworld_ptr::set_repetition(unsigned short to_set)
//{
//    m_repetition = to_set;
//}
//
//unsigned short kworld_ptr::get_repetition() const
//{
//    return m_repetition;
//
//}
//
//bool kworld_ptr::entails(const fluent & to_check) const
//{
//    return m_ptr->entails(to_check);
//}
//
//bool kworld_ptr::entails(const fluent_set& to_check) const
//{
//    return m_ptr->entails(to_check);
//}
//
//bool kworld_ptr::entails(const fluent_formula & to_check) const
//{
//    return m_ptr->entails(to_check);
//}
//
//bool kworld_ptr::operator<(const kworld_ptr & to_compare) const
//{
//    if (get_id().compare(to_compare.get_id()) < 0) {
//        return true;
//    }
//    return false;
//}
//
//bool kworld_ptr::operator>(const kworld_ptr & to_compare) const
//{
//    if (get_id().compare(to_compare.get_id()) > 0) {
//        return true;
//    }
//    return false;
//}
//
//bool kworld_ptr::operator==(const kworld_ptr & to_compare) const
//{
//    /**std way*/
//    if (!((*this) < to_compare) && !(to_compare < (*this))) {
//        return true;
//    }
//    return false;
//}
//
//bool kworld_ptr::operator=(const kworld_ptr & to_copy)
//{
//    set_ptr(to_copy.get_ptr());
//    set_repetition(to_copy.get_repetition());
//    return true;
//}