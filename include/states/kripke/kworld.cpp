/* 
 * File:   kworld.h
 * Author: Francesco
 *
 * Created on April 4, 2019, 11:39 AM
 */

#include "kworld.h"

#include "kstore.h"

kworld::kworld()
{
}

kworld::kworld(const fluent_set & fl)
{
	set_fluent_set(fl);
	set_id();
}
//generate an unique id given the state information -> the literals

kworld_id kworld::hash_fluents_into_id(const fluent_set& fl)
{
	kworld_id ret;
	fluent_set::const_iterator it_fl;
	for (it_fl = fl.begin(); it_fl != fl.end(); it_fl++) {
		ret.append(std::to_string(*it_fl) + "-");
	}
	//Remove last "-" that is useless
	ret.pop_back();
	return ret;
}

kworld_id kworld::hash_fluents_into_id()
{
	return hash_fluents_into_id(m_fluent_set);
}

void kworld::set_fluent_set(const fluent_set & fl)
{
	m_fluent_set = fl;
}

void kworld::set_id()
{
	m_id = hash_fluents_into_id();
}

const fluent_set & kworld::get_fluent_set() const
{
	return m_fluent_set;
}

kworld_id kworld::get_id() const
{
	return m_id;
}

bool kworld::entails(fluent to_check) const
{
	return(m_fluent_set.find(to_check) != m_fluent_set.end());
}

/**
 * \warning Check for the size = 0?
 */
bool kworld::entails(const fluent_set & to_check) const
{
	//fluent_set expresses conjunctive set of \ref fluent
	fluent_set::const_iterator it_fl;
	for (it_fl = to_check.begin(); it_fl != to_check.end(); it_fl++) {
		if (!entails(*it_fl)) {
			return false;
		}
	}
	return true;
}

/**
 * \warning Check for the size = 0?
 */
bool kworld::entails(const fluent_formula & to_check) const
{
	fluent_formula::const_iterator it_fl;
	for (it_fl = to_check.begin(); it_fl != to_check.end(); it_fl++) {
		if (entails(*it_fl)) {
			return true;
		}
	}
	return false;
}

bool kworld::operator<(const kworld& world) const
{
	if (m_id.compare(world.get_id()) < 0)
		return true;
	return false;
}

void kworld::print_info(const grounder& gr)
{
	std::cout << "World id: " << get_id();
	std::cout << "\nFluents: " << get_id();
	gr.print_ff(m_fluent_set);
}
