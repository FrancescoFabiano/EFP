/**
 * \brief Class that contains the union update function.
 *
 * \details The union update returns the possibility after the execution of a PEM
 *
 * @see pstate and event.
 *
 * \copyright GNU Public License.
 *
 * \author Francesco Fabiano.
 * \date April 29, 2021
 */
#pragma once

#include "../states/possibilities/pstate.h"
#include "../utilities/define.h"

class delphic_helper {
    /** \brief Calculates the PEM relative to the \ref action description and the current \ref pstate.
     *
     * @param [in] state: the current \ref pstate.
     * @param [in] act: the \ref action description.
     * @return the resulting \ref event.*/
    static event get_pem(const pstate & state, const action & act);
    /** \brief Calculates the \ref pstate resulting from the given current \ref pstate and \ref event.
     *
     * @param [in] u: the current \ref pstate.
     * @param [in] e: the \ref event to apply.
     * @return the updated \ref pstate.*/
    static pstate union_update(const pstate & u, const event & e);
};
