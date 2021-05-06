/**
 * \brief Implementation of \ref event.h and \ref event_ptr.h.
 *
 * \copyright GNU Public License.
 * 
 * @todo: Redefine the insert so that we have the bulk check (Bisimulation with all the graphs). 
 *
 * \author Francesco Fabiano.
 * \date April 28, 2021
 */

#include <iostream>
#include <stdexcept>

#include "event.h"

event::event()
{
}

event::event(event_id id, bool ontic_change)
{
    set_id(id);
    set_ontic_change(ontic_change);
}

event::event(event_id id, bool ontic_change, const formula_list & pre)
{
    set_id(id);
    set_ontic_change(ontic_change);
    set_precondition(pre);
}

event::event(event_id id, bool ontic_change, const formula_list & pre, const effects_map & post)
{
    set_id(id);
    set_ontic_change(ontic_change);
    set_precondition(pre);
    set_postconditions(post);
}

event::event(const event & to_copy)
{
    set_id(to_copy.get_id());
    set_precondition(to_copy.get_precondition());
    set_postconditions(to_copy.get_postconditions());
    set_ontic_change(to_copy.get_ontic_change());
}

void event::set_id(const event_id to_set)
{
    m_id = to_set;
}

void event::set_precondition(const formula_list & to_set)
{
    m_pre = to_set;
}

void event::set_postconditions(const effects_map & to_set)
{
    m_post = to_set;
    set_ontic_change(true);
}

void event::set_ontic_change(bool to_set) {
    m_ontic_change = to_set;
}

const event_id event::get_id() const
{
    return m_id;
}

const formula_list & event::get_precondition() const
{
    return m_pre;
}

const effects_map & event::get_postconditions() const
{
    return m_post;
}

const bool event::get_ontic_change() const
{
    return m_ontic_change;
}

bool event::operator<(const event & to_compare) const
{
    if (m_id < to_compare.get_id()) {     // We compare the ids since we assume that each PEM has at most one event for each id
        return true;
    }
    return false;
}

bool event::operator>(const event & to_compare) const
{
    if (m_id > to_compare.get_id()) {
        return true;
    }
    return false;
}

bool event::operator==(const event & to_compare) const
{
    /**std way*/
    if (!(*this < to_compare) && !(to_compare < *this)) {
        return true;
    }
    return false;
}

bool event::operator=(const event & to_copy)
{
    set_id(to_copy.get_id());
    set_precondition(to_copy.get_precondition());
    set_postconditions(to_copy.get_postconditions());
    return true;
}

/****************************************************************************/

event_ptr::event_ptr()
{
}

event_ptr::event_ptr(const std::shared_ptr <event> &ptr)
{
    set_ptr(ptr);
}

event_ptr::event_ptr(std::shared_ptr <event> &&ptr)
{
    set_ptr(ptr);
}

event_ptr::event_ptr(const event &action)
{
    m_ptr = std::make_shared<event>(action);
}

void event_ptr::set_ptr(const std::shared_ptr <event> &ptr)
{
    m_ptr = ptr;
}

void event_ptr::set_ptr(std::shared_ptr <event> &&ptr)
{
    m_ptr = ptr;
}

void event_ptr::set_postconditions(const effects_map & to_set)
{
    m_ptr->set_postconditions(to_set);
}

std::shared_ptr <event> event_ptr::get_ptr() const
{
    return m_ptr;
}

const event_id event_ptr::get_id() const
{
    if (m_ptr != nullptr) {
        return get_ptr()->get_id();
    }
    std::cerr << "Error in creating a event_ptr\n";
    exit(1);
}

const formula_list & event_ptr::get_precondition() const
{
    if (m_ptr != nullptr) {
        return get_ptr()->get_precondition();
    }
    std::cerr << "Error in creating a event_ptr\n";
    exit(1);
}

const effects_map & event_ptr::get_postconditions() const
{
    if (m_ptr != nullptr) {
        return get_ptr()->get_postconditions();
    }
    std::cerr << "Error in creating a event_ptr\n";
    exit(1);
}

const bool event_ptr::get_ontic_change() const
{
    if (m_ptr != nullptr) {
        return get_ptr()->get_ontic_change();
    }
    std::cerr << "Error in creating a event_ptr\n";
    exit(1);
}

bool event_ptr::operator<(const event_ptr & to_compare) const
{
    if (get_id() < to_compare.get_id()) {
        return true;
    }
    return false;
}

bool event_ptr::operator>(const event_ptr & to_compare) const
{
    if (get_id() > to_compare.get_id()) {
        return true;
    }
    return false;
}

bool event_ptr::operator==(const event_ptr & to_compare) const
{
    /**std way*/
    if (!(*this < to_compare) && !(to_compare < (*this))) {
        return true;
    }
    return false;
}

bool event_ptr::operator=(const event_ptr & to_copy)
{
    set_ptr(to_copy.get_ptr());
    return true;
}
