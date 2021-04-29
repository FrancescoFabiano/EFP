/**
 * \brief Implementation of \ref pem.h and \ref pem_ptr.h.
 *
 * \copyright GNU Public License.
 * 
 * @todo: Redefine the insert so that we have the bulk check (Bisimulation with all the graphs). 
 *
 * \author Francesco Fabiano.
 * \date April 28, 2021
 */

#include <iostream>
#include <tuple>
#include <stdexcept>

#include "pem.h"
#include "pem_store.h"

#include "../domain/domain.h"
#include "../utilities/helper.h"

pem::pem()
{
}

pem::pem(const event_type type, const belief_formula & pre, const postconditions & post, const pem_edges & edges)
{
    set_type(type);
    set_precondition(pre);
    set_postconditions(post);
    set_edges(edges);
}

pem::pem(const pem & action)
{
    set_type(action.get_type());
    set_precondition(action.get_precondition());
    set_postconditions(action.get_postconditions());
    set_edges(action.get_edges());
}

void pem::set_type(const event_type to_set)
{
    m_type = to_set;
}

void pem::set_precondition(const belief_formula & to_set)
{
    m_pre = to_set;
}

void pem::set_postconditions(const postconditions & to_set)
{
    m_post = to_set;
}

void pem::set_edges(const pem_edges & to_set)
{
    m_edges = to_set;
}

const event_type pem::get_type() const
{
    return m_type;
}

const belief_formula & pem::get_precondition() const
{
    return m_pre;
}

const postconditions & pem::get_postconditions() const
{
    return m_post;
}

const pem_edges & pem::get_edges() const
{
    return m_edges;
}

bool pem::operator<(const pem & to_compare) const
{
    if (m_type < to_compare.get_type())     // We compare the types since we assume that each action has at most one PEM for each type
        return true;
    return false;
}

bool pem::operator>(const pem & to_compare) const
{
    if (m_type > to_compare.get_type())
        return true;
    return false;
}

bool pem::operator==(const pem & to_compare) const
{
    /**std way*/
    if (!(*this < to_compare) && !(to_compare < *this)) {
        return true;
    }
    return false;
}

bool pem::operator=(const pem & to_copy)
{
    set_type(to_copy.get_type());
    set_precondition(to_copy.get_precondition());
    set_postconditions(to_copy.get_postconditions());
    set_edges(to_copy.get_edges());
    return true;
}

/****************************************************************************/

pem_ptr::pem_ptr()
{
}

pem_ptr::pem_ptr(const std::shared_ptr <pem> &ptr)
{
    set_ptr(ptr);
}

pem_ptr::pem_ptr(std::shared_ptr <pem> &&ptr)
{
    set_ptr(ptr);
}

pem_ptr::pem_ptr(const pem &action)
{
    m_ptr = std::make_shared<pem>(action);
}

void pem_ptr::set_ptr(const std::shared_ptr <pem> &ptr)
{
    m_ptr = ptr;
}

void pem_ptr::set_ptr(std::shared_ptr <pem> &&ptr)
{
    m_ptr = ptr;
}

std::shared_ptr <pem> pem_ptr::get_ptr() const
{
    return m_ptr;
}

const event_type pem_ptr::get_type() const
{
    if (m_ptr != nullptr) {
        return get_ptr()->get_type();
    }
    std::cerr << "Error in creating a pem_ptr\n";
    exit(1);
}

const belief_formula & pem_ptr::get_precondition() const
{
    if (m_ptr != nullptr) {
        return get_ptr()->get_precondition();
    }
    std::cerr << "Error in creating a pem_ptr\n";
    exit(1);
}

const postconditions & pem_ptr::get_postconditions() const
{
    if (m_ptr != nullptr) {
        return get_ptr()->get_postconditions();
    }
    std::cerr << "Error in creating a pem_ptr\n";
    exit(1);
}

const pem_edges & pem_ptr::get_edges() const
{
    if (m_ptr != nullptr) {
        return get_ptr()->get_edges();
    }
    std::cerr << "Error in creating a pem_ptr\n";
    exit(1);
}

bool pem_ptr::operator<(const pem_ptr & to_compare) const
{
    if (get_type() < to_compare.get_type()) {
        return true;
    }
    return false;
}

bool pem_ptr::operator>(const pem_ptr & to_compare) const
{
    if (get_type() > to_compare.get_type()) {
        return true;
    }
    return false;
}

bool pem_ptr::operator==(const pem_ptr & to_compare) const
{
    /**std way*/
    if (!(*this < to_compare) && !(to_compare < (*this))) {
        return true;
    }
    return false;
}

bool pem_ptr::operator=(const pem_ptr & to_copy)
{
    set_ptr(to_copy.get_ptr());
    return true;
}
