/** \brief Implementation of helper_t.h
 *
 * \copyright GNU Public License.
 *
 * \author Francesco Fabiano, Alessandro Burigana.
 * \date April 30, 2021
 */

#include "helper_t.h"
#include "helper.h"

template <class T>
void helper_t::sum_set(std::set<T> & to_modify, const std::set<T> & factor2)
{
	typename std::set<T>::const_iterator it_pwset;
	for (it_pwset = factor2.begin(); it_pwset != factor2.end(); it_pwset++) {
		to_modify.insert(*it_pwset);
	}
}

template <class T>
void helper_t::minus_set(std::set<T> & to_modify, const std::set<T> & factor2)
{
	typename std::set<T>::const_iterator it_pwset;
	for (it_pwset = factor2.begin(); it_pwset != factor2.end(); it_pwset++) {
		to_modify.erase(*it_pwset);
	}
}

template <class T>
agent_set helper_t::get_agents_if_entailed(const observability_map& map, const T & s) {
	agent_set ret;
//	observability_map::const_iterator it_map;
//	for (it_map = map.begin(); it_map != map.end(); it_map++) {
//		if (s.entails(it_map->second)) {
//			ret.insert(it_map->first);
//		}
//	}
	return ret;
}

template <class T>
fluent_formula helper_t::get_effects_if_entailed(const effects_map & map, const T & s) {
	fluent_formula ret;
//	effects_map::const_iterator it_map;
//	for (it_map = map.begin(); it_map != map.end(); it_map++) {
//		if (s.entails(it_map->second)) {
//			ret = helper::and_ff(ret, it_map->first);
//		}
//	}
//	if (ret.size() > 1) {
//
//		std::cerr << "\nNon determinism in action effect is not supported-1.\n";
//		exit(1);
//	}
	return ret;
}
