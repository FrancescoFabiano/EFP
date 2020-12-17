/*
 * \brief Implementation of \ref attitude.h.
 *
 * \copyright GNU Public License.
 *
 * \author Francesco Fabiano.
 * \date December 17, 2020
 */
#include "attitude.h"
#include "domain.h"

agents_attitudes attitude::get_type() const
{
	if (m_type.compare("P_KEEPER") == 0) {
		return P_KEEPER;
	} else if (m_type.compare("P_INSECURE") == 0) {
		return P_INSECURE;
	} else if (m_type.compare("F_TRUSTY") == 0) {
		return F_TRUSTY;
	} else if (m_type.compare("F_MISTRUSTY") == 0) {
		return F_MISTRUSTY;
	} else if (m_type.compare("F_UNTRUSTY") == 0) {
		return F_UNTRUSTY;
	} else if (m_type.compare("F_STUBBORN") == 0) {
		return F_STUBBORN;
	} else {
		return P_KEEPER;
	}
}

const std::string & attitude::get_string_type() const
{
	return m_type;
}

agent attitude::get_agent() const
{
	return domain::get_instance().get_grounder().ground_agent(m_agent);
}

agent attitude::get_executor() const
{
	return domain::get_instance().get_grounder().ground_agent(m_executor);
}

const belief_formula & attitude::get_attitude_conditions() const
{
	return m_attitude_conditions;
}

void attitude::set_type(const std::string & to_set)
{
	m_type = to_set;
}

void attitude::set_agent(const std::string & to_set)
{
	m_agent = to_set;
}

void attitude::set_executor(const std::string & to_set)
{
	m_executor = to_set;
}

void attitude::set_attitude_conditions(const belief_formula & to_set)
{
	m_attitude_conditions = to_set;
	m_attitude_conditions.ground();

}

void attitude::print() const
{
	std::cout << m_agent << " has attitude " << m_type << " when " << m_executor << " is the executor of the action if ";
	get_attitude_conditions().print();
}

bool attitude::operator=(const attitude& to_copy){
	m_agent = domain::get_instance().get_grounder().deground_agent(to_copy.get_agent());
	m_executor = domain::get_instance().get_grounder().deground_agent(to_copy.get_executor());
	m_type = to_copy.get_string_type();
	m_attitude_conditions = to_copy.get_attitude_conditions();
	
	return true;
}
