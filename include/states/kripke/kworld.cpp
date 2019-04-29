/*
 * \brief Implementation of \ref kworld.h and \ref kworld_ptr.h.
 *
 * \copyright GNU Public License.
 *
 * \author Francesco Fabiano.
 * \date March 31, 2019
 */

#include "kworld.h"

#include "kstore.h"

#include <stdexcept>

kworld::kworld()
{
}

kworld::kworld(const fluent_set & description)
{
	/*
	 * No need because consistent for construction
	 * \throw std::invalid_argument whenever \p description is not consistent.
	 *
	 *try {*/
	set_fluent_set(description);
	set_id();
	/*} catch (const std::invalid_argument& ia) {
	 *	throw ia;
	 *} */
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

void kworld::set_fluent_set(const fluent_set & description)
{
	/*
	 * \throw std::invalid_argument whenever \p description is not consistent.
	 *
	if (consistent(description))*/
	m_fluent_set = description;
	/*else
		throw std::invalid_argument("Non consistent set of fluent");*/
}

bool kworld::consistent(const fluent_set & to_check)
{
	fluent_set::const_iterator it_flset;
	fluent_set::const_iterator it_flset_tmp;

	for (it_flset = to_check.begin(); it_flset != to_check.end(); it_flset++) {
		/* If the pointed fluent is in modulo 2 it means is the positive and if
		 * its successor (the negative version) is in the set then is not consistent.*/
		if ((*it_flset) % 2 == 0) {
			//The std::set has is elements ordered so we can just check its successor.
			it_flset_tmp = it_flset;
			it_flset_tmp++;
		}
		if (it_flset_tmp != to_check.end()) {
			if (*it_flset_tmp == ((*it_flset) + 1)) {
				std::cout << "\nCheck: " << *it_flset_tmp << " and " << (*it_flset) + 1 << std::endl;
				return false;
			}
		}
		/**\todo check that all the possible \ref fluent are there.
		 * \bug change and use find.
		 */
	}
	return true;
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
 * \todo Check for the size = 0?
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
 * \todo Check for the size = 0?
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

/*-***************************************************************************************************************-*/

kworld_ptr::kworld_ptr()
{
}

kworld_ptr::kworld_ptr(const std::shared_ptr<const kworld> & ptr)
{
	set_ptr(ptr);
}

kworld_ptr::kworld_ptr(std::shared_ptr<const kworld>&& ptr)
{
	set_ptr(ptr);
}

void kworld_ptr::set_ptr(const std::shared_ptr<const kworld> & ptr)
{
	m_ptr = ptr;
}

void kworld_ptr::set_ptr(std::shared_ptr<const kworld>&& ptr)
{
	m_ptr = ptr;
}

std::shared_ptr<const kworld> kworld_ptr::get_ptr() const
{
	return m_ptr;
}

bool kworld_ptr::operator=(const kworld_ptr & kptr)
{
	set_ptr(kptr.get_ptr());
	return true;
}

bool kworld_ptr::operator<(const kworld_ptr & kptr) const
{
	if ((*m_ptr)<(*(kptr.get_ptr()))) {
		return true;
	}
	return false;
}