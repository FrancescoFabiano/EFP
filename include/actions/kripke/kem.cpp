#include "kem.h"

#include "kevent.h"
#include "kem_store.h"

kem::kem()
{
}

kem::kem(kem_id id)
{
    set_id(id);
}

kem::kem(const kem & to_copy)
{
    set_id(to_copy.get_id());
    set_edges(to_copy.get_edges());
}

void kem::set_id(kem_id to_set)
{
    m_id = to_set;
}

void kem::set_pointed_id(event_id to_set)
{
    m_pointed_id = to_set;
}

void kem::set_edges(const kem_edges & to_set)
{
    m_edges = to_set;
}

kem_id kem::get_id() const
{
    return m_id;
}

event_id kem::get_pointed_id() const
{
    return m_pointed_id;
}

const kem_edges & kem::get_edges() const
{
    return m_edges;
}

bool kem::operator<(const kem & to_compare) const
{
    if (m_id < to_compare.get_id()) {
        return true;
    }
    return false;
}

bool kem::operator>(const kem & to_compare) const
{
    if (m_id > to_compare.get_id()) {
        return true;
    }
    return false;
}

bool kem::operator==(const kem & to_compare) const
{
    /**std way*/
    if (!(*this < to_compare) && !(to_compare < *this)) {
        return true;
    }
    return false;
}

bool kem::operator=(const kem & to_copy)
{
    set_id(to_copy.get_id());
    set_edges(to_copy.get_edges());
    return true;
}

void kem::print() const
{
    std::cout << "\nModel (" << get_id() << ") has pointed kevent: " << get_pointed_id() << " and edges: ";

    //	for (auto it_fl = m_edges.begin(); it_fl != m_edges.end(); ++it_fl) {
    //		for (auto it_edge = it_fl->second.begin(); it_edge != it_fl->second.end(); ++it_edge) {
    //			std::cout << " | " << kem_store::get_instance().get_agent_group_name(it_fl->first) << "-" << it_edge->first.get_id() << "-" << it_edge->second.get_id();
    //		}
    //std::cout << std::endl;
    //	}
}

/****************************************************************************/

kem_ptr::kem_ptr()
{
}

kem_ptr::kem_ptr(const std::shared_ptr<kem> &ptr)
{
    set_ptr(ptr);
}

kem_ptr::kem_ptr(std::shared_ptr<kem> &&ptr)
{
    set_ptr(ptr);
}

kem_ptr::kem_ptr(const kem &action)
{
    m_ptr = std::make_shared<kem>(action);
}

void kem_ptr::set_ptr(const std::shared_ptr<kem> &ptr)
{
    m_ptr = ptr;
}

void kem_ptr::set_ptr(std::shared_ptr<kem> &&ptr)
{
    m_ptr = ptr;
}

std::shared_ptr<kem> kem_ptr::get_ptr() const
{
    return m_ptr;
}

void kem_ptr::set_id(kem_id to_set)
{
    m_ptr->set_id(to_set);
}

void kem_ptr::set_pointed_id(event_id to_set)
{
    m_ptr->set_pointed_id(to_set);
}

void kem_ptr::set_edges(const kem_edges & to_set)
{
    m_ptr->set_edges(to_set);
}

kem_id kem_ptr::get_id() const
{
    if (m_ptr != nullptr) {
        return get_ptr()->get_id();
    }
    std::cerr << "Error in creating a kem_ptr\n";
    exit(1);
}

event_id kem_ptr::get_pointed_id() const
{
    if (m_ptr != nullptr) {
        return get_ptr()->get_pointed_id();
    }
    std::cerr << "Error in creating a kem_ptr\n";
    exit(1);
}

const kem_edges & kem_ptr::get_edges() const
{
    if (m_ptr != nullptr) {
        return get_ptr()->get_edges();
    }
    std::cerr << "Error in creating a kem_ptr\n";
    exit(1);
}

bool kem_ptr::operator<(const kem_ptr & to_compare) const
{
    if (get_id() < to_compare.get_id()) {
        return true;
    }
    return false;
}

bool kem_ptr::operator>(const kem_ptr & to_compare) const
{
    if (get_id() > to_compare.get_id()) {
        return true;
    }
    return false;
}

bool kem_ptr::operator==(const kem_ptr & to_compare) const
{
    /**std way*/
    if (!(*this < to_compare) && !(to_compare < (*this))) {
        return true;
    }
    return false;
}

bool kem_ptr::operator=(const kem_ptr & to_copy)
{
    set_ptr(to_copy.get_ptr());
    return true;
}

void kem_ptr::print()const
{
    m_ptr->print();
}
