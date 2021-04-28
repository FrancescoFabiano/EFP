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

#include "../../domain/domain.h"
#include "../../utilities/helper.h"

pem::pem()
{
}

pem::pem(const fluent_set & description)
{
    set_fluent_set(description);
    set_id();
}

pem::pem(const fluent_set & description, const pedges_opt & edges)
{
    set_fluent_set(description);
    set_edges(edges);
    set_id();
}

pem::pem(const pem & action)
{
    set_fluent_set(action.get_fluent_set());
    set_edges(action.get_edges());
    set_id();
}

void pem::set_type(const event_type type)
{
    m_type = type;
}

void pem::set_precondition(const belief_formula & pre)
{
    m_pre = pre;
}

void pem::set_postconditions(const postconditions & post)
{
    m_post = post;
}

void pem::set_edges(const pem_edges & edges)
{
    m_edges = edges;
}

const event_type pem::get_type() const
{
    return m_type;
}

const belief_formula::get_precondition() const
{
    return m_pre;
}

const belief_formula::get_postconditions() const
{
    return m_post;
}

const pedges_opt& pem::get_edges() const
{
    return m_edges;
}

bool pem::operator=(const pem & to_copy)
{
    set_type(to_copy.get_type());
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

const event_type pem_ptr::get_precondition() const
{
    if (m_ptr != nullptr) {
        return get_ptr()->get_precondition();
    }
    std::cerr << "Error in creating a pem_ptr\n";
    exit(1);
}

const event_type pem_ptr::get_postconditions()
{
    if (m_ptr != nullptr) {
        return get_ptr()->get_postconditions();
    }
    std::cerr << "Error in creating a pem_ptr\n";
    exit(1);
}

const event_type pem_ptr::get_edges() const
{
    if (m_ptr != nullptr) {
        return get_ptr()->get_edges();
    }
    std::cerr << "Error in creating a pem_ptr\n";
    exit(1);
}
