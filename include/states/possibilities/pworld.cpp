/*
 * \brief Implementation of \ref pworld.h and \ref pworld_ptr.h.
 *
 * \copyright GNU Public License.
 *
 * \author Francesco Fabiano.
 * \date September 14, 2019
 */

#include "pworld.h"

#include "pstore.h"

#include <stdexcept>

pworld::pworld()
{
}

pworld::pworld(const fluent_set & description)
{
    set_fluent_set(description);
    set_id();
}
//generate an unique id given the state information -> the literals

pworld::pworld(const pworld & world)
{
	set_fluent_set(world.get_fluent_set());
	set_id();
}

pworld_id pworld::hash_fluents_into_id(const fluent_set& fl)
{
	pworld_id ret;
	fluent_set::const_iterator it_fl;
	for (it_fl = fl.begin(); it_fl != fl.end(); it_fl++) {
		ret.append(std::to_string(*it_fl) + "-");
	}
	//Remove last "-" that is useless
	//ret.pop_back();
	return ret;
}

pworld_id pworld::hash_fluents_into_id()
{
	return hash_fluents_into_id(m_fluent_set);
}

void pworld::set_fluent_set(const fluent_set & description)
{
	/*
	 * \throw std::invalid_argument whenever \p description is not consistent.
	 *
	if (consistent(description))*/
	m_fluent_set = description;
	/*else
		throw std::invalid_argument("Non consistent set of fluent");*/
}

bool pworld::consistent(const fluent_set & to_check)
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

void pworld::set_id()
{
	m_id = hash_fluents_into_id();
}

const fluent_set & pworld::get_fluent_set() const
{
	return m_fluent_set;
}

pworld_id pworld::get_id() const
{
	return m_id;
}

bool pworld::entails(fluent to_check) const
{
	return(m_fluent_set.find(to_check) != m_fluent_set.end());
}

/**
 * \todo Check for the size = 0?
 */
bool pworld::entails(const fluent_set & to_check) const
{
	//fluent_set expresses conjunctive set of \ref fluent
	if (to_check.size() == 0) {
		return true;
	}
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
bool pworld::entails(const fluent_formula & to_check) const
{
	if (to_check.size() == 0) {
		return true;
	}
	fluent_formula::const_iterator it_fl;
	for (it_fl = to_check.begin(); it_fl != to_check.end(); it_fl++) {
		if (entails(*it_fl)) {
			return true;
		}
	}
	return false;
}

bool pworld::operator<(const pworld& to_compare) const
{
	if (m_id.compare(to_compare.get_id()) < 0)
		return true;
	return false;
}

bool pworld::operator==(const pworld& to_compare) const
{
	/**std way*/
	if (!((*this) < to_compare) && !(to_compare < (*this))) {
		return true;
	}
	return false;
}

bool pworld::operator=(const pworld & to_assign)
{
	set_fluent_set(to_assign.get_fluent_set());
	set_id();
	return true;
}

void pworld::print() const
{
	std::cout << "\nFluents: " << get_id();
	printer::get_instance().print_list(m_fluent_set);
}

/*-***************************************************************************************************************-*/

pworld_ptr::pworld_ptr()
{
}

pworld_ptr::pworld_ptr(const std::shared_ptr<const pworld> & ptr, unsigned short repetition)
{
	set_ptr(ptr);
	set_repetition(repetition);
}

pworld_ptr::pworld_ptr(std::shared_ptr<const pworld>&& ptr, unsigned short repetition)
{
	set_ptr(ptr);
	set_repetition(repetition);

}

pworld_ptr::pworld_ptr(const pworld & world, unsigned short repetition)
{
	m_ptr = std::make_shared<pworld>(world);
	set_repetition(repetition);

}

void pworld_ptr::set_ptr(const std::shared_ptr<const pworld> & ptr)
{
	m_ptr = ptr;
}

void pworld_ptr::set_ptr(std::shared_ptr<const pworld>&& ptr)
{
	m_ptr = ptr;
}

std::shared_ptr<const pworld> pworld_ptr::get_ptr() const
{
	return m_ptr;
}

const fluent_set & pworld_ptr::get_fluent_set() const
{
	if (m_ptr != nullptr) {
		return get_ptr()->get_fluent_set();
	}
	std::cerr << "Error in creating a pworld_ptr\n";
	exit(1);
}

pworld_id pworld_ptr::get_id() const
{
	if (m_ptr != nullptr) {
		return(get_ptr()->get_id()).append(std::to_string(get_repetition()));
	}
	std::cerr << "\nError in creating a pworld_ptr\n";
	exit(1);
}

void pworld_ptr::set_repetition(unsigned short to_set)
{
	m_repetition = to_set;
}

unsigned short pworld_ptr::get_repetition() const
{
	return m_repetition;

}

bool pworld_ptr::entails(fluent to_check) const
{
	return m_ptr->entails(to_check);
}

bool pworld_ptr::entails(const fluent_set& to_check) const
{
	return m_ptr->entails(to_check);
}

bool pworld_ptr::entails(const fluent_formula & to_check) const
{
	return m_ptr->entails(to_check);
}

bool pworld_ptr::operator<(const pworld_ptr & to_compare) const
{
	if (get_id().compare(to_compare.get_id()) < 0) {
		return true;
	}
	return false;
}

bool pworld_ptr::operator==(const pworld_ptr & to_compare) const
{
	/**std way*/
	if (!((*this) < to_compare) && !(to_compare < (*this))) {
		return true;
	}
	return false;
}

bool pworld_ptr::operator=(const pworld_ptr & to_copy)
{
	set_ptr(to_copy.get_ptr());
	set_repetition(to_copy.get_repetition());
	return true;
}