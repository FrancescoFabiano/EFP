/* \brief Implementation of formula_manipulation.h
 *
 * \copyright GNU Public License.
 *
 * \author Francesco Fabiano.
 * \date April 7, 2019
 */

#include "formula_manipulation.h"

fluent formula_manipulation::negate_fluent(const fluent f)
{
	if (f % 2 == 0) {
		return(f + 1);
	} else {
		return(f - 1);
	}

}

bool formula_manipulation::is_consistent(const fluent_set &fl1, const fluent_set& fl2)
{

	//@TODO: Add static_laws
	fluent_set::iterator it_fl;

	for (it_fl = fl2.begin(); it_fl != fl2.end(); it_fl++) {

		if (fl1.find(negate_fluent(*it_fl)) != fl1.end())
			return false;
	}

	return true;
}

fluent_set formula_manipulation::and_ff(const fluent_set& fl1, const fluent_set& fl2)
{
	fluent_set ret;
	if (!fl1.empty() && !fl2.empty()) {

		if (is_consistent(fl1, fl2)) {
			ret = fl1;
			ret.insert(fl2.begin(), fl2.end());
		}

	} else if (fl1.empty()) {
		return fl2;
	} else if (fl2.empty()) {
		return fl1;
	} else {
		std::cerr << "Bad formula declaration." << std::endl;
	}

	return ret;
}

fluent_formula formula_manipulation::and_ff(const fluent_formula& ff1, const fluent_formula& ff2)
{
	fluent_formula::iterator it_ff1;
	fluent_formula::iterator it_ff2;
	fluent_formula ret;
	if (!ff1.empty() && !ff2.empty()) {
		for (it_ff2 = ff1.begin(); it_ff2 != ff1.end(); it_ff2++) {
			for (it_ff1 = ff2.begin(); it_ff1 != ff2.end(); it_ff1++) {
				ret.insert(and_ff(*it_ff2, *it_ff1));
			}
		}
	} else if (ff1.empty()) {
		return ff2;
	} else if (ff2.empty()) {
		return ff1;
	} else {
		std::cerr << "Bad formula declaration." << std::endl;
		exit(1);
	}
	return ret;
}

bool formula_manipulation::check_Bff_Bnotff(const belief_formula& to_check_1, const belief_formula& to_check_2, std::shared_ptr<fluent_formula> ret)
{
	/*GENERIC, to much
	if (to_check_1.m_operator == BF_NOT) {
		std::cout << "\nVado 1NOT check\n";
		return *(to_check_1.m_bf1) == to_check_2;
	} else if (to_check_2.m_operator == BF_NOT) {
		std::cout << "\nVado 2NOT check\n";
		return *(to_check_2.m_bf1) == to_check_1;
	}*/
	/**\todo For now just fluent check. Maybe add some kind of formula normalization and then with \ref fluent_formula.
	 * \todo we assume that the fluent_formula has just one element (the fluent).
	 */

	if (to_check_1.m_formula_type == BELIEF_FORMULA && to_check_2.m_formula_type == BELIEF_FORMULA) {
		belief_formula to_check_nested_1 = *to_check_1.m_bf1;
		belief_formula to_check_nested_2 = *to_check_2.m_bf1;
		if (to_check_nested_1.m_formula_type == FLUENT_FORMULA && to_check_nested_2.m_formula_type == FLUENT_FORMULA) {

			fluent_set tmp = *((to_check_nested_1.m_fluent_formula).begin());
			fluent f_to_check_1 = *(tmp.begin());
			tmp = *((to_check_nested_2.m_fluent_formula).begin());
			fluent f_to_check_2 = *(tmp.begin());
			if (f_to_check_1 == negate_fluent(f_to_check_2)) {

				if (ret != nullptr) {
					ret->insert(tmp);
				}
				return true;
			}
		}
	}
	return false;
}

/* Using the std::set == operator
static bool formula_manipulation::is_the_same_ff(const fluent_set& to_check_1, const fluent_set& to_check_2)
{
	//If the size is the same we just need to check all the element in to_check_1 because they are sets
	//and there aren't repetitions on the elements.
	if (to_check_1.size() == to_check_2.size()) {
		fluent_set::conts_iterator it_fs;
		for (it_fs = to_check_1.begin(); it_fs != to_check_1.end(); it_fs++) {
			if (to_check_2.find(*it_fs) == to_check_2.end()) {
				return false;
			}
		}
		return true;
	}
	return false;
}

static bool formula_manipulation::is_the_same_ff(const fluent_formula& to_check_1, const fluent_formula& to_check_2)
{
	//If the size is the same we just need to check all the element in to_check_1 because they are sets
	//and there aren't repetitions on the elements.
	if (to_check_1.size() == to_check_2.size()) {
		fluent_formula::conts_iterator it_ff;
		for (it_ff = to_check_1.begin(); it_ff != to_check_1.end(); it_ff++) {
		}
		return true;
	}
	return false;
}*/


