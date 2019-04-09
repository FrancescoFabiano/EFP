/* 
 * File:   formula_manipulation.cpp
 * Author: Francesco
 *
 * Created on April 7, 2019, 4:40 PM
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

bool formula_manipulation::is_consistent(const fluent_list &fl1, const fluent_list& fl2)
{

	//@TODO: Add static_laws
	fluent_list::iterator it_fl;

	for (it_fl = fl2.begin(); it_fl != fl2.end(); it_fl++) {

		if (fl1.find(negate_fluent(*it_fl)) != fl1.end())
			return false;
	}

	return true;
}


//@TODO: Check the return value

fluent_list formula_manipulation::and_ff(const fluent_list& fl1, const fluent_list& fl2)
{
	fluent_list ret;
	if (!fl1.empty() && !fl2.empty()) {

		if (is_consistent(fl1, fl2)) {
			ret = fl1;
			ret.insert(fl2.begin(), fl2.end());
		}

	} else if (fl1.empty()) {
		return fl2;
	} else if (fl2.empty()) {
		return fl1;
	}
	else{
		std::cerr << "Bad formula declaration." << std::endl;
	}

	return ret;
}
//@TODO: check if the life of this does not depend on the reader
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
	}
	else{
		std::cerr << "Bad formula declaration." << std::endl;
	}
	return ret;
}