/* 
 * File:   state.h
 * Author: Francesco
 *
 * Created on March 31, 2019, 10:57 AM
 */
#pragma once

#include "action.h"
//#include "belief_formulae.h"

/***************************************************************************************
The class that encode the state of the planner; this is the #interface and will be used 
as black box from @planner: its implementation will depend on the initial choices:
 * .Kripke
 * .Possibilities
 * .OBDDs

This class should be used to check entailment and to produce successors
****************************************************************************************/

class state
{
public:
    
    state (const state &);
    state (const state &, const action &);
    state (action_list, unsigned short);
    state (action_list, unsigned short, int);
    
    action_list* get_executed_actions();
    unsigned short get_plan_length();
    int get_heuristic_value();
    
    void set_state(state);
    
    void set_executed_actions(const action_list &);
    void set_plan_length(unsigned short);
    void set_heuristic_value(int);
    
    virtual state compute_succ (action) = 0;
    virtual bool entails (belief_formaula) = 0;
    virtual void set_heuristic_value() = 0;

    bool is_goal();
    
protected:
    
    action_list m_executed_actions;
    unsigned short m_plan_length;
    int m_heuristic_value;
    
    virtual ~state();
};