#include "cem.h"

//#include "cem_store.h"

cem::cem()
{
}

cem::cem(cem_id id)
{
	set_id(id);
}

cem::cem(const cem & to_copy)
{
	set_id(to_copy.get_id());
	set_pointed_id(to_copy.get_pointed_id());
	set_edges(to_copy.get_edges());
}

void cem::set_id(cem_id to_set)
{
	m_id = to_set;
}

void cem::set_pointed_id(event_id to_set)
{
	m_pointed_id = to_set;
}

void cem::set_edges(const cem_edges & to_set)
{
	m_edges = to_set;
}

cem_id cem::get_id() const
{
	return m_id;
}

const event_id cem::get_pointed_id() const
{
	return m_pointed_id;
}

const cem_edges & cem::get_edges() const
{
	return m_edges;
}

bool cem::operator<(const cem & to_compare) const
{
	if (m_id < to_compare.get_id()) {
		return true;
	}
	return false;
}

bool cem::operator>(const cem & to_compare) const
{
	if (m_id > to_compare.get_id()) {
		return true;
	}
	return false;
}

bool cem::operator==(const cem & to_compare) const
{
	/**std way*/
	if (!(*this < to_compare) && !(to_compare < *this)) {
		return true;
	}
	return false;
}

bool cem::operator=(const cem & to_copy)
{
	set_id(to_copy.get_id());
	set_pointed_id(to_copy.get_pointed_id());
	set_edges(to_copy.get_edges());
	return true;
}

void cem::print() const {
//	std::cout << "\nModel (" << get_id() << ") has:\n\tPointed cevent = " << get_pointed_id() << ";\n\tEdges:\n";
//
//	for (auto it_fl = m_edges.begin(); it_fl != m_edges.end(); ++it_fl) {
//		for (auto it_edge = it_fl->second.begin(); it_edge != it_fl->second.end(); ++it_edge) {
//			for (auto it_internal = it_edge->second.begin(); it_internal != it_edge->second.end(); ++it_internal) {
//				std::cout << "\t\t- " << it_fl->first.get_id() << "-" << cem_store::get_instance().get_agent_group_name(it_edge->first) << "-" << it_internal->get_id() << std::endl;
//			}
//		}
//	}
}

/****************************************************************************/

cem_ptr::cem_ptr()
{
}

cem_ptr::cem_ptr(const std::shared_ptr<cem> &ptr)
{
	set_ptr(ptr);
}

cem_ptr::cem_ptr(std::shared_ptr<cem> &&ptr)
{
	set_ptr(ptr);
}

cem_ptr::cem_ptr(const cem &action)
{
	m_ptr = std::make_shared<cem>(action);
}

void cem_ptr::set_ptr(const std::shared_ptr<cem> &ptr)
{
	m_ptr = ptr;
}

void cem_ptr::set_ptr(std::shared_ptr<cem> &&ptr)
{
	m_ptr = ptr;
}

std::shared_ptr<cem> cem_ptr::get_ptr() const
{
	return m_ptr;
}

void cem_ptr::set_id(cem_id to_set)
{
	m_ptr->set_id(to_set);
}

void cem_ptr::set_pointed_id(event_id to_set)
{
	m_ptr->set_pointed_id(to_set);
}

void cem_ptr::set_edges(const cem_edges & to_set)
{
	m_ptr->set_edges(to_set);
}

cem_id cem_ptr::get_id() const
{
	if (m_ptr != nullptr) {
		return get_ptr()->get_id();
	}
	std::cerr << "Error in creating a cem_ptr\n";
	exit(1);
}

const event_id cem_ptr::get_pointed_id() const
{
	if (m_ptr != nullptr) {
		return get_ptr()->get_pointed_id();
	}
	std::cerr << "Error in creating a cem_ptr\n";
	exit(1);
}

const cem_edges & cem_ptr::get_edges() const
{
	if (m_ptr != nullptr) {
		return get_ptr()->get_edges();
	}
	std::cerr << "Error in creating a cem_ptr\n";
	exit(1);
}

bool cem_ptr::operator<(const cem_ptr & to_compare) const
{
	if (get_id() < to_compare.get_id()) {
		return true;
	}
	return false;
}

bool cem_ptr::operator>(const cem_ptr & to_compare) const
{
	if (get_id() > to_compare.get_id()) {
		return true;
	}
	return false;
}

bool cem_ptr::operator==(const cem_ptr & to_compare) const
{
	/**std way*/
	if (!(*this < to_compare) && !(to_compare < (*this))) {
		return true;
	}
	return false;
}

bool cem_ptr::operator=(const cem_ptr & to_copy)
{
	set_ptr(to_copy.get_ptr());
	return true;
}

void cem_ptr::print()const
{
	m_ptr->print();
}
