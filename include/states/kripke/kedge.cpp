
/*
 * \brief Class that implements kedge.h.
 *
 *
 * \copyright GNU Public License.
 *
 * \author Francesco Fabiano.
 * \date March 31, 2019
 */
#include "kedge.h"

#include "kstore.h"

/*********************************************************************
 kedge implementation
 *********************************************************************/

kedge::kedge()
{
}

kedge::kedge(const kworld & from, const kworld & to, agent label)
{
	set_from(from);
	set_to(to);
	set_label(label);
	set_id();
}

/**
 * \todo the hash function is maybe to simple.
 */
kedge_id kedge::hash_info_into_id(kworld_ptr from, kworld_ptr to, agent label)
{
	return(from->get_id() + "-" + to->get_id() + "-" + std::to_string(label));
}

kedge_id kedge::hash_info_into_id()
{
	return(hash_info_into_id(m_from, m_to, m_label));
}

kworld_ptr kedge::get_from() const
{
	return m_from;
}

kworld_ptr kedge::get_to() const
{
	return m_to;
}

agent kedge::get_label() const
{
	return m_label;
}

kedge_id kedge::get_id() const
{
	return m_edge_id;
}

void kedge::set_from(const kworld & from)
{
	/*Get the reference of the world from the static class kstore
	 *
	 * If the world doesn't exists it create a new one in kstore and get its address
	 * otherwise it get the already existing one's address.
	 */
	m_from = kstore::get_instance().add_world(from);
}

void kedge::set_to(const kworld & to)
{
	/*Get the reference of the world from the static class kstore
	 *
	 * If the world doesn't exists it create a new one in kstore and get its address
	 * otherwise it get the already existing one's address.
	 */
	m_to = kstore::get_instance().add_world(to);
}

void kedge::set_label(agent label)
{
	m_label = label;
}

void kedge::set_id()
{
	m_edge_id = hash_info_into_id();
}

bool kedge::operator<(const kedge& edge) const
{
	if (m_edge_id.compare(edge.get_id()) < 0)
		return true;
	return false;
}

void kedge::print()
{
	std::cout << "edge id " << get_id() << " label: " << get_label() << " from: " << get_from()->get_id() << " to " << get_to()->get_id() << std::endl;
}