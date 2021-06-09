/**
 * \brief Class that contains the union update function.
 *
 * \details The union update returns the possibility after the execution of a PEM
 *
 * @see pstate and pevent.
 *
 * \copyright GNU Public License.
 *
 * \author Francesco Fabiano.
 * \date April 29, 2021
 */
#pragma once

#include "../states/kripke/kstate.h"

#include "../states/possibilities/pstate.h"
#include "../utilities/define.h"

class union_update
{
public:
    /** \brief Calculates the PEM relative to the \ref action description and the current \ref pstate.
     *
     * @param [in] act: the \ref action description.
     * @return the resulting \ref pem.*/
    static pem_ptr build_pem(const action & act);

    static agent_group_map build_agent_group_map();
    /** \brief Calculates the \ref pstate resulting from the given current \ref pstate and \ref pem.
     *
     * @param [in] state: the current \ref pstate.
     * @param [in] e: the \ref pem to apply.
     * @return the updated \ref pstate.*/
    static const pstate & u_update(const pstate & state, const action & act);
    static const pworld_ptr & u_update_helper(pstate & ret, const pstate & state, const action & act, const pem_ptr & pem, const pworld_ptr & pw, const pevent_ptr & ev, update_map & u_map, const agent_group_map & a_map);
    // todo: to remove
    static const kstate & u_update(const kstate & state, const action & act);


    //Francesco: addition
    static const pworld & world_cartesian_product(const pworld & world, const pevent_ptr & e);
    static fluent_formula get_total_effects(const pstate & state, const action & act, const pevent_ptr & e);
    static formula_list get_total_pre(const pstate & state, const action & act, const pevent_ptr & e);
};
