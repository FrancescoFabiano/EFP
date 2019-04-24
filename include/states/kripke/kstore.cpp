/*
 * \brief Implementation of \ref kstore.h
 * \copyright GNU Public License.
 *
 * \author Francesco Fabiano.
 * \date April 9, 2019
 */


#include "kstore.h"

kstore::kstore()
{
}

kstore& kstore::get_instance()
{
	static kstore instance;
	return instance;
}

kedge_ptr kstore::add_edge(const kedge & to_add)
{
	/*      kedge_set::iterator it_edset;
	      it_edset = m_created_edges.find(edge);
	      if (it_edset != m_created_edges.end()) {
		  return *it_edset;
	      }
	      else*/
	//It returns the pointer to the newly inserted element if it doesn't exit (The old one otherwise)
	//The pair is <iterator,bool>
	//return std::get<0>(m_created_edges.insert(edge));

	/// \bug wrong implementaion.
	return std::shared_ptr<const kedge>(&to_add);
}

kworld_ptr kstore::add_world(const kworld & to_add)
{
	//It returns the pointer to the newly inserted element if it doesn't exit (The old one otherwise)
	//The pair is <iterator,bool>
	//@TODO: Make sure is a shared_ptr
	//return *((m_created_worlds.insert(world))->first);

	/// \bug No need to create the kworld kk or the iterator.

	kworld_set::iterator it_kwset;
	it_kwset = std::get<0>(m_created_worlds.insert(to_add));
	kworld kk = *it_kwset;
	return std::shared_ptr<const kworld>(&kk);
	//return &(*()));
}

//Implement in case the list double checking is cheaper than building a world....
/*const kworld& kstore::add_world(const fluent_formula &ff)
{	
	//If already exists return the pointer without creating a new world
	kworld_ptr_set::const_iterator it_worldset;
	it_worldset = m_created_worlds.find(hash_ff(ff));
	if (it_worldset != m_created_worlds.end())
		return m_created_worlds;
	return add_world(kworld(ff));
}*/

