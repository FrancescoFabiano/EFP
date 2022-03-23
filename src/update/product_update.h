/**
 * \brief Class that contains the union update function.
 *
 * \details pstatehe union update returns the possibility after the execution of a cEM
 *
 * @see pstate and cevent.
 *
 * \copyright GNU Public License.
 *
 * \author Francesco Fabiano, Alessandro Burigana.
 * \date April 29, 2021
 */
#pragma once

#include "../states/possibilities/pstate.h"
#include "../../include/definitions/define.h"

class product_update
{
public:
    //static agent_group_map build_agent_group_map();
    /** \brief Calculates the \ref pstate resulting from the given current \ref pstate and \ref cem.
     *
     * @param [in] state: the current \ref pstate.
     * @param [in] e: the \ref cem to apply.
     * @return the updated \ref pstate.
    static const pstate & update(const pstate & state, const action & act);
    static const pworld_ptr & update_helper(pstate & ret, const pstate & state, const action & act, const cem_ptr & cem, const pworld_ptr & pw, const cevent_ptr & ev, cupdate_map & u_map, const agent_group_map & a_map);*/
    static pstate update(const pstate & state, const action & act);
};
