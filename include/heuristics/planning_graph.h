/**
 * \class planning_graph
 * \brief Class that implements the epistemic planning graph data structure for heuristics extrapolation.
 *
 * \details An e-Planning graph is a structure introduced in <https://aaai.org/ocs/index.php/ICAPS/ICAPS18/paper/view/17733>
 *  that, as the planning graph from the classical planning environment, it is used to extrapolate
 * qualitative value for the states.
 * 
 *
 * \copyright GNU Public License.
 * 
 * \author Francesco Fabiano.
 * \date September 24, 2019
 */

#pragma once

#include <stdio.h>
#include <list>
#include <map>

#include "../utilities/define.h"
#include "../actions/action.h"
#include "../states/state_T.h"
#include "../states/kripke/kstate.h"
#include "../states/possibilities/pstate.h"
#include "../formulae/belief_formula.h"

/****************************
 CLASS EpistemicModelsNode
 ***************************/
//class pg_eState
//{
//private:
//    kstate m_partial_kripke;
//    kworld_ptr_set m_pointeds; //set of states is as pointed states
//    //vector<Kripke*> kripkeVec;
//    action_id m_action;
//
//public:
//    pg_eState();
//    pg_eState(const kstate & partial_kripke, const kworld_ptr_set & pointeds);
//    void set_partial_kripke(const kstate & partial_kripke);
//    void set_pointeds(const kworld_ptr_set & pointeds);
//    void add_pointed(kworld_ptr pointed);
//    void set_action(action_id action);
//
//    const kstate & get_partial_kripke() const;
//    const kworld_ptr_set & get_pointeds() const;
//    action_id get_action() const;
//    //printing
//    void print() const;
//};
//
//typedef std::vector<pg_eState> pg_eState_set; /**< \brief A vector of \ref pg_eState.*/

/***********************************************************************
 class: PGraphActionLevel
 ************************************************************************/

class pg_action_level
{
protected:
    //Waste memory
    //Use ptr
    action_set m_actions;
    unsigned short m_depth = 0;

public:
    pg_action_level();
    pg_action_level(const action_set & actions);
    pg_action_level(const action_set & actions, unsigned short depth);

    void set_depth(unsigned short depth);
    unsigned short get_depth()const;

    void set_actions(const action_set & actions);
    void add_action(const action & act);

    //Use ptr instead
    const action_set & get_actions() const;
    bool operator=(const pg_action_level& to_assign);


    //printing
    void print() const;
};
//
///***********************************************************************
// class: PGraphENodeLevel
// ************************************************************************/
//

template <class T>
class pg_state_level
{
private:
    //Is a class of pointer, not much memory required
    //Maybe make it templatic
    std::set<T> m_eStates;
    unsigned short m_depth = 0;

public:
    pg_state_level();
    pg_state_level(const std::set<T> & eStates);
    pg_state_level(const std::set<T> & eStates, unsigned short depth);


    void set_eStates(const std::set<T> & eStates);
    bool add_eState(const T & eState);

    void set_depth(unsigned short depth);
    unsigned short get_depth()const;

    const std::set<T>& get_eStates() const;

    //check satisfaction of a belief formula given an epistemic_model_list level (i.e., eNodes)
    bool pg_entailment(const std::set<T> & eStates, const belief_formula & bf) const;
    bool pg_entailment(const belief_formula & bf) const;

    bool pg_entailment(const std::set<T> & eStates, const formula_list & fl) const;
    bool pg_entailment(const formula_list & fl) const;

    bool pg_executable(const action & act) const;

    bool operator=(const pg_state_level& to_assign);
};
//
///***********************************************************************
// class: PlanningGraph
// ************************************************************************/
/***********************************************************************
 class: PlanningGraph
 ************************************************************************/

//We now consider conditional execution

typedef std::map<fluent_set, unsigned short> pg_worlds_score;
typedef std::map<belief_formula, unsigned short> pg_bfs_score;

template <class T>
class planning_graph
{
private:
    //std::vector<pg_state_level> m_state_levels; //for union of \K_i levels for the sake of saving memory
    std::vector< pg_state_level<T> > m_state_levels; //for union of \K_i levels for the sake of saving memory
    std::vector<pg_action_level> m_action_levels; //for A_i levels
    unsigned short m_pg_length = 0;
    bool m_satisfiable;
    formula_list m_goal;
    pg_worlds_score m_worlds_score;
    pg_bfs_score m_bfs_score;
    //use ptr instead
    action_set m_never_executed;
    void set_satisfiable(bool sat);
    void pg_build_layer();
    void add_state_level(const pg_state_level<T> & s_level);
    void add_action_level(const pg_action_level & a_level);

public:

    planning_graph(const T& pg_init);
    planning_graph(const std::set<T>& pg_init);

    void set_length(unsigned short length); //construct planning graph and return the level that satisfied the goal.


    bool is_satisfiable(); //construct planning graph and return the level that satisfied the goal.
    unsigned short get_length(); //construct planning graph and return the level that satisfied the goal.
    const pg_worlds_score & get_worlds_score();
    const pg_bfs_score & get_bfs_score();
};