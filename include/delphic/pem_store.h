/**
 * \class pem_store
 * \brief Singleton class that stores the unique copy of each \ref pem created.
 * 
 * \details All the other class only store pointers of \ref pem and this class takes care of creating new
 * one in case the requested one doesn't exist.
 * 
 * @see pem.
 * 
 * \copyright GNU Public License.
 *
 * \author Francesco Fabiano.
 * \date April 28, 2021
 */

#pragma once

#include <set>
#include "pem.h"

typedef std::set<pem> pem_set; /**< \brief A set of \ref pworld, used to store all the created ones.*/

class pem_store
{
private:

    /** \brief A set that contains all the generated pems (even the pems that are "internal")*/
    pem_set m_created_pems;

    /** \brief All the pem that represent an action (given as input).
     *
     * These pems are pointer to the ones in \ref m_created_pems.*/
    pem_map m_action_pems;

    /** \brief Private constructor since it is a Singleton class.*/
    pem_store();
public:
    /**Function that populates the list m_action_pems with the event model given as input in \ref file*/
    void generate(const std::string & file);

    /** \brief To get always (the same instance of) *this* and the same instantiated fields.*/
    static pem_store& get_instance();

    /** \brief Function that inserts a \ref pem given its id to \ref m_action_pems.
     * @param[in] to_add: The event model that represent the graph of an action.
     * @parma[in] id: The id associated to \ref to_add.
     */
    void add_action_pem(const pem & to_add, pem_id id);




    /** \brief Function that inserts a \ref pem given its id to \ref m_action_pems.
     * @param[in] to_add: The event model (pointer) that represent the graph of an action.
     * @parma[in] id: The id associated to \ref to_add.
     */
    void add_action_pem(const pem_ptr & to_add, pem_id id);

    /** \brief Function that retrieves a \ref pem from its \ref pem_id.
     * @parma[in] id: The id of the \ref pem that needs to be returned.
     * @return: The event model that represent the graph of the action associated to \ref id.
     */
    const pem_ptr & get_action_pem(pem_id id);


    /** \brief Function that return the pointer to the given \ref pem.
     * 
     * If the \ref pem didn't exist then it is inserted to \ref m_created_pems and then the pointer is returned;
     * if it existed a pointer to the already existing version is returned.
     * 
     * @param[in] to_add: the \ref pem to add to the collection of created pems.
     * 
     * @return the \ref pem_ptr to \p to_add.*/
    const pem_ptr & add_pem(const pem & to_add);

    /** \brief Copy constructor removed since is Singleton class. */
    pem_store(pem_store const&) = delete;
    /** \brief Copy operator removed since Singleton class. */
    void operator=(pem_store const&) = delete;

};

