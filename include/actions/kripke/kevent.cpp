/**
 * \brief Implementation of \ref kevent.h and \ref event_ptr.h.
 *
 * \copyright GNU Public License.
 * 
 * @todo: Redefine the insert so that we have the bulk check (Bisimulation with all the graphs). 
 *
 * \author Francesco Fabiano.
 * \date June 24, 2021
 */

#include <iostream>
#include <stdexcept>

#include "kevent.h"
#include "../../utilities/printer.h"
#include "../../utilities/helper_t.ipp"

kevent::kevent()
{
    m_id = -1;
}

/*kevent::kevent(event_id id, bool ontic_change)
{
	set_id(id);
	set_ontic_change(ontic_change);
}

kevent::kevent(event_id id, bool ontic_change, const formula_list & pre)
{

	set_id(id);
	set_ontic_change(ontic_change);
	set_precondition(pre);
}

kevent::kevent(event_id id, bool ontic_change, const formula_list & pre, const event_postconditions & post)
{
	set_id(id);
	set_ontic_change(ontic_change);
	set_precondition(pre);
	set_postconditions(post);
}*/


kevent::kevent(event_id id, const formula_list & pre, const event_metacond & meta_pre, const event_postconditions & post, const event_metacond & meta_post)
{
    set_id(id);
    set_precondition(pre);
    set_meta_precondition(meta_pre);
    set_postconditions(post);
    set_meta_postconditions(meta_post);
    set_ontic_change();
}

kevent::kevent(event_id id, const event_metacond & meta_pre, const event_metacond & meta_post)
{
    set_id(id);
    set_meta_precondition(meta_pre);
    set_meta_postconditions(meta_post);
    set_ontic_change();
}

kevent::kevent(const ::kevent & to_copy)
{
    set_id(to_copy.get_id());
    set_meta_precondition(to_copy.get_meta_precondition());
    set_meta_postconditions(to_copy.get_meta_postconditions());
    set_ontic_change(to_copy.get_ontic_change());
}

void kevent::set_id(const event_id to_set)
{
    m_id = to_set;
}

void kevent::set_precondition(const formula_list & to_set)
{
    m_pre = to_set;
}

void kevent::set_meta_precondition(const event_metacond & to_set)
{
    m_meta_pre = to_set;
}

void kevent::set_postconditions(const event_postconditions & to_set)
{
    if (to_set.size() > 1) {
        std::cerr << "\n\n***Non determinism in event postconditions is not yet supported.***\n\n";
        exit(1);
    }
    m_post = to_set;
}

void kevent::set_meta_postconditions(const event_metacond & to_set)
{
    m_meta_post = to_set;

}

void kevent::set_ontic_change()
{
    if (m_meta_post.size() == 0) {
        set_ontic_change(false);
    } else if (m_meta_post.size() > 1) {
        set_ontic_change(true);
    } else if (*m_meta_post.begin() == none) {
        set_ontic_change(false);
    } else {
        set_ontic_change(true);
    }
}

void kevent::set_ontic_change(bool to_set)
{
    m_ontic_change = to_set;
}

const event_id kevent::get_id() const
{
    return m_id;
}

const formula_list & kevent::get_precondition(const kstate & s, const action & act) const
{
    formula_list ret;
    formula_list action_pre = act.get_executability();
    fluent_formula action_eff = helper_t::get_effects_if_entailed(act.get_effects(), s);

    event_metacond::const_iterator it_meta_pre;
    formula_list::const_iterator it_act_pre;

    for (it_meta_pre = get_meta_precondition().begin(); it_meta_pre != get_meta_precondition().end(); ++it_meta_pre) {
        belief_formula tmp_bf;

        switch ( *it_meta_pre ) {
            case act_eff:
                tmp_bf.set_from_ff(action_eff);
                ret.push_back(tmp_bf);
                break;
            case neg_act_eff:
                tmp_bf.set_from_ff(helper::negate_fluent_formula(action_eff));
                ret.push_back(tmp_bf);
                break;
            case act_pre:
                for (it_act_pre = action_pre.begin(); it_act_pre != action_pre.end(); ++it_act_pre) {
                    ret.push_back(*it_act_pre);
                }
                break;
            case none:
            default:
                ret.clear();
                return ret;
        }
    }
    return ret;
}

const event_postconditions & kevent::get_postconditions(const kstate & s, const action & act) const
{
    fluent_formula action_eff = helper_t::get_effects_if_entailed(act.get_effects(), s);

    if (get_meta_postconditions().size() > 1) {
        std::cerr << "Error: malformed action postcondition in pevent " << get_id() << std::endl;
        exit(1);
    } else if (get_meta_postconditions().empty()) {
        fluent_formula empty;
        return empty;
    } else {
        switch (*(get_meta_postconditions().begin())) {
            case act_eff:
                return action_eff;
            case neg_act_eff:
                return helper::negate_fluent_formula(action_eff);
                //Not break because it means it is empty
            case none:
            default:
                fluent_formula empty;
                return empty;
        }
    }

    //Need to insert the merge with specific postcondition
}


const event_metacond & kevent::get_meta_precondition() const
{
    return m_meta_pre;
}

const event_metacond & kevent::get_meta_postconditions() const
{
    return m_meta_post;
}

const bool kevent::get_ontic_change() const
{
    return m_ontic_change;
}

bool kevent::operator<(const ::kevent & to_compare) const
{
    if (m_id < to_compare.get_id()) { // We compare the ids since we assume that each KEM has at most one event for each id
        return true;
    }
    return false;
}

bool kevent::operator>(const ::kevent & to_compare) const
{
    if (m_id > to_compare.get_id()) {
        return true;
    }
    return false;
}

bool kevent::operator==(const ::kevent & to_compare) const
{
    /**std way*/
    if (!(*this < to_compare) && !(to_compare < *this)) {
        return true;
    }
    return false;
}

bool kevent::operator=(const ::kevent & to_copy)
{
    set_id(to_copy.get_id());
    return true;
}

void kevent::print()const
{
    std::cout << "\nEvent (" << get_id() << ") has meta_preconditions: ";


    for (auto it_fl = m_meta_pre.begin(); it_fl != m_meta_pre.end(); ++it_fl) {
        std::cout << " | " << *it_fl;
        //std::cout << std::endl;
    }

    std::cout << " and has meta_postconditions: ";
    for (auto it_fl = m_meta_post.begin(); it_fl != m_meta_post.end(); ++it_fl) {
        std::cout << " | " << *it_fl;
        //std::cout << std::endl;
    }

    //	for (auto it_fl = m_pre.begin(); it_fl != m_pre.end(); ++it_fl) {
    //		std::cout << " | ";
    //		it_fl->print();
    //		//std::cout << std::endl;
    //	}
    //	std::cout << " and has postconditions: ";
    //	for (auto it_effmap = m_post.begin(); it_effmap != m_post.end(); ++it_effmap) {
    //		std::cout << " | ";
    //		printer::get_instance().print_list(it_effmap->first);
    //		std::cout << " if ";
    //		it_effmap->second.print();
    //	}
    std::cout << std::endl;
}

/****************************************************************************/

kevent_ptr::kevent_ptr()
{
}

kevent_ptr::kevent_ptr(const std::shared_ptr <::kevent> &ptr)
{
    set_ptr(ptr);
}

kevent_ptr::kevent_ptr(std::shared_ptr <::kevent> &&ptr)
{
    set_ptr(ptr);
}

kevent_ptr::kevent_ptr(const ::kevent &action)
{
    m_ptr = std::make_shared<::kevent>(action);
}

void kevent_ptr::set_ptr(const std::shared_ptr <::kevent> &ptr)
{
    m_ptr = ptr;
}

void kevent_ptr::set_ptr(std::shared_ptr <::kevent> &&ptr)
{
    m_ptr = ptr;
}

std::shared_ptr <::kevent> kevent_ptr::get_ptr() const
{
    return m_ptr;
}

const event_id kevent_ptr::get_id() const
{
    if (m_ptr != nullptr) {
        return get_ptr()->get_id();
    }
    std::cerr << "Error in creating a event_ptr\n";
    exit(1);
}

const formula_list & kevent_ptr::get_precondition(const kstate & s, const action & act) const
{
    if (m_ptr != nullptr) {
        return get_ptr()->get_precondition(s, act);
    }
    std::cerr << "Error in creating a event_ptr\n";
    exit(1);
}

const event_postconditions & kevent_ptr::get_postconditions(const kstate & s, const action & act) const
{
    if (m_ptr != nullptr) {
        return get_ptr()->get_postconditions(s, act);
    }
    std::cerr << "Error in creating a event_ptr\n";
    exit(1);
}

const event_metacond & kevent_ptr::get_meta_precondition() const
{
    if (m_ptr != nullptr) {
        return get_ptr()->get_meta_precondition();
    }
    std::cerr << "Error in creating a event_ptr\n";
    exit(1);
}

const event_metacond & kevent_ptr::get_meta_postconditions() const
{
    if (m_ptr != nullptr) {
        return get_ptr()->get_meta_postconditions();
    }
    std::cerr << "Error in creating a event_ptr\n";
    exit(1);
}

bool kevent_ptr::get_ontic_change() const
{
    if (m_ptr != nullptr) {
        return get_ptr()->get_ontic_change();
    }
    std::cerr << "Error in creating a event_ptr\n";
    exit(1);
}

bool kevent_ptr::operator<(const kevent_ptr & to_compare) const
{
    if (get_id() < to_compare.get_id()) {
        return true;
    }
    return false;
}

bool kevent_ptr::operator>(const kevent_ptr & to_compare) const
{
    if (get_id() > to_compare.get_id()) {
        return true;
    }
    return false;
}

bool kevent_ptr::operator==(const kevent_ptr & to_compare) const
{
    /**std way*/
    if (!(*this < to_compare) && !(to_compare < (*this))) {
        return true;
    }
    return false;
}

bool kevent_ptr::operator=(const kevent_ptr & to_copy)
{
    set_ptr(to_copy.get_ptr());
    return true;
}

void kevent_ptr::print() const
{
    get_ptr()->print();
}
