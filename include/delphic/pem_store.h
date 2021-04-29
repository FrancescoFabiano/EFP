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

class pem_store
{
private:
    /** \brief All the created \ref pem, all the other class only have pointers to elements of this set.
     * 
     * \todo should it be static?*/
    pem_set m_created_pems;

    /** \brief Private constructor since it is a Singleton class.*/
    pem_store();
public:

    /** \brief To get always (the same instance of) *this* and the same instantiated fields.*/
    static pem_store& get_instance();

    /** \brief Function that return the pointer to the given \ref pem.
     * 
     * If the \ref pem didn't exist then it is inserted to \ref m_created_pems and then the pointer is returned;
     * if it existed a pointer to the already existing version is returned.
     * 
     * @param[in] to_add: the \ref pem to add to the collection of created pems.
     * 
     * @return the \ref pem_ptr to \p to_add.
     * 
     * \todo Param ok because set makes copy?*/
//    const pem_ptr add_pem(const pem & to_add);

    /** \brief Copy constructor removed since is Singleton class. */
    pem_store(pem_store const&) = delete;
    /** \brief Copy operator removed since Singleton class. */
    void operator=(pem_store const&) = delete;

};

