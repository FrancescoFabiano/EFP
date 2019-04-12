/* 
 * File:   state_T.h
 * Author: Francesco
 *
 * Created on April 11, 2019, 7:05 PM
 */

#pragma once

#include "kripke/kstate.h"

#include "../utilities/define.h"
#include "../actions/action.h"
#include "../domain/initially.h"
//#include "belief_formulae.h"

/***************************************************************************************
The class that encode the state of the planner; this is the @TEMPLATE and will be used 
as black box from @planner: its implementation will depend on the initial choices:
 * .Kripke
 * .Possibilities
 * .OBDDs
This class should be used to check entailment and to produce successors
 * 
 * Template and not virtual to keep the pointer and, since the type of search is decided
 * at compile-time virtual overhead is not necessary
 * 
 * asserts not necessary due to the compile errors
 ****************************************************************************************/
template <class T>
class state
{
private:
    T representation;

    action_id_list m_executed_actions_id;
    unsigned short m_plan_length;
    int m_heuristic_value;

    void set_state(const state<T> &);
    void set_state(const T &);

    void set_executed_actions(const action_id_list &);
    void set_plan_length(unsigned short);
    void set_heuristic_value(int);

public:

    //Implement in each class with a kstate instead of a state
    state<T>();
    state<T>(const state<T> &);
    state<T>(const T &);
    state<T>(const state<T> &, const action &);
    state<T>(const action_id_list &, unsigned short);
    state<T>(const action_id_list &, unsigned short, int);

    const action_id_list & get_executed_actions();
    unsigned short get_plan_length();
    int get_heuristic_value();

    bool entails(const fluent_list &);
    bool entails(const fluent_formula &);
    bool entails(const formula_list &);

    void build_initial(const initially &);
    T compute_succ(const action &);

   //@TODO: Move to goal class bool is_goal();
};

