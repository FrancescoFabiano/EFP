/**
 * \class state
 * \brief Template-based Class that encodes an action of planner.h.
 *
 * \details  This is the *TEMPLATE* and will be used as black box from planner.h:
 * its implementation will depend on the initial choices.
 *
 * Template and not virtual to keep the pointer and, since the type of search is decided
 * at compile-time virtual overhead is not necessary.
 *
 * \todo Check if is the correct way to usa a templatic class.
 * \copyright GNU Public License.
 *
 * \author Francesco Fabiano.
 * \date June 25, 2021
 */

#pragma once

#include "action.h"

template<class T>
class action_wrapper {
private:
    /** \brief The type of state m_representation.
     *
     * One of:
     *    - \ref KRIPKE
     *    - \ref POSSIBILITIES
     */
    T m_representation;

    action act;

public:
    /** \brief Constructor without parameters.
     *
     * It creates \ref m_representation calling its **T** constructor.*/
    action_wrapper<T>();

    action_wrapper<T>(const state<T> & prev_state, const action & act);
};
