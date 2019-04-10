/* 
 * File:   kworld.h
 * Author: Francesco
 *
 * Created on April 4, 2019, 11:39 AM
 */

#include "kworld.h"

kworld::kworld()
{
}

kworld::kworld(const fluent_list & fl)
{
	m_fluent_list = fl;
}
 //generate an unique id given the state information -> the literals
kworld_id kworld::hash_fluents_into_id(const fluent_list& fl)
{
	kworld_id ret;
	fluent_list::const_iterator it_fl;
	for(it_fl = fl.begin(); it_fl != fl.end(); it_fl++){
		ret.append(std::to_string(*it_fl) + "-");
	}
	//Remove last "-" that is useless
	ret.pop_back();
	return ret;
}
kworld_id kworld::hash_fluents_into_id()
{
return hash_fluents_into_id(m_fluent_list);
}

void kworld::set_fluent_list(const fluent_list & fl);
void kworld::set_id();

const fluent_list & kworld::get_fluent_list();
kworld_id kworld::get_id();

void kworld::print_info();

bool kworld::entails(fluent) const;
bool kworld::entails(const fluent_list &) const;
bool kworld::entails(const fluent_formula &) const;

bool kworld::operator<(const kworld&) const;
