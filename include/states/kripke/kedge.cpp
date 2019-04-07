/* 
 * File:   kedge.h
 * Author: Francesco
 *
 * Created on March 31, 2019, 3:18 PM
 */
#include "kedge.h"

/*********************************************************************
 kedge implementation
 *********************************************************************/
 
 kedge::kedge()
 {}
 kedge::kedge(kedge* edge)
 {
	set_from(edge->get_from());
	set_to(edge->get_to());
	set_label(edge->get_label());
	set_id(edge->get_id());
	set_exists(edge->get_exist());
 }
 
 kedge::kedge(kworld_id from, kworld_id to, agent label, kedge_id edge_id, bool exists)
 {
	set_from(from);
	set_to(to);
	set_label(label);
	set_id(edge_id);
	set_exists(exists);
 }
 

kworld_id kedge::get_from()
{
	return m_from;
}
kworld_id kedge::get_to()
{
	return m_to;
}
agent kedge::get_label()
{
	return m_label;
}
kedge_id kedge::get_id()
{
	return m_edge_id;
}
bool kedge::get_exist()
{
	return m_exists;
}
  
void kedge::set_from(kworld_id from)
{
	m_from = from;
}
void kedge::set_to(kworld_id to)
{
	m_to = to;
}
void kedge::set_label(agent label)
{
	m_label = label;
}
void kedge::set_id(kedge_id edge_id)
{
	m_edge_id = edge_id;
}
void kedge::set_exists(bool exists)
{
	m_exists = exists;
}

void kedge::print()
{
	std::cout << "edge id " << get_id() << " label: " << get_label() << " from: " << get_from() << " to " << get_to() << std::endl;
}