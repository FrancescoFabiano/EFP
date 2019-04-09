/* 
 * File:   kstore.cpp
 * Author: Francesco
 *
 * Created on April 9, 2019, 5:48 PM
 */

#include "kstore.h"

kstore::kstore()
{
}

kstore::kstore& get_instance()
{
	static kstore instance;
	return instance;
}

//@TODO: smart pointer instead?
const kedge& kstore::add_edge(const kedge & edge)
{
	/*      kedge_set::iterator it_edset;
	      it_edset = m_created_edges.find(edge);
	      if (it_edset != m_created_edges.end()) {
		  return *it_edset;
	      }
	      else*/
	//It returns the pointer to the newly inserted element if it doesn't exit (The old one otherwise)
	//THe tuple is <iterator,bool>
	return *(std::get<0> (m_created_edges.insert(edge)));
}

const kworld& kstore::add_world(const kworld & world)
{
	//It returns the pointer to the newly inserted element if it doesn't exit (The old one otherwise)
	//THe tuple is <iterator,bool>
	return *(std::get<0> (m_created_worlds.insert(world)));
}