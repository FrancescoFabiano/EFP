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
#include "../states/kripke/kstate.h"
#include "../formulae/belief_formula.h"

/****************************
 CLASS EpistemicModelsNode
 ***************************/
class pg_eState
{
private:
    kstate m_partial_kripke;
    kworld_ptr_set m_pointeds; //set of states is as pointed states
    //vector<Kripke*> kripkeVec;
    action_id m_action;

public:
    pg_eState();
    pg_eState(const kstate & partial_kripke, const kworld_ptr_set & pointeds);
    void set_partial_kripke(const kstate & partial_kripke);
    void set_pointeds(const kworld_ptr_set & pointeds);
    void add_pointed(kworld_ptr pointed);
    void set_action(action_id action);

    const kstate & get_partial_kripke() const;
    const kworld_ptr_set & get_pointeds() const;
    action_id get_action() const;
    //printing
    void print() const;
};

typedef std::vector<pg_eState> pg_eState_set; /**< \brief A vector of \ref pg_eState.*/

/***********************************************************************
 class: PGraphActionLevel
 ************************************************************************/

class pg_action_level
{
protected:
    //Waste memory
    //Use ptr
    action_set m_actions;

public:
    pg_action_level();
    pg_action_level(const action_set & actions);

    void set_actions(const action_set & actions);
    void add_action(const action & act);

    const action_set & get_actions() const;

    //printing
    void print() const;
};
//
///***********************************************************************
// class: PGraphENodeLevel
// ************************************************************************/
//

class pg_state_level
{
private:
    pg_eState_set m_eStates;
public:
    pg_state_level();
    pg_state_level(const pg_eState_set & eStates);

    void set_eStates(const pg_eState_set & eStates);
    void add_eState(const pg_eState & eState);

    const pg_eState_set& get_eState();

    //check satisfaction of a belief formula given an epistemic_model_list level (i.e., eNodes)
    bool pg_entailment(pg_eState_set eStates, const belief_formula & bf) const;
    bool pg_entailment(const belief_formula & bf) const;

    bool pg_executable(const action & act) const;

    //printing
    void print() const;
};
//
///***********************************************************************
// class: PlanningGraph
// ************************************************************************/
//
//class PlanningGraph
//{
//protected:
//    Planner* m_planner;
//    PGraphENodeLevel k_levels; //for union of \K_i levels for the sake of saving memory
//    vector<PGraphActionLevel> a_levels; //for A_i levels
//
//private:
//    EpistemicModelsNode effect_sensing(Literal l, AgentSet fd, AgentSet pd, AgentSet od, string action);
//    EpistemicModelsNode effect_ontic(Literal l, AgentSet fd, AgentSet od, string action);
//    EpistemicModelsNode effect_announce(FluentFormula ff, AgentSet fd, AgentSet pd, AgentSet od, string action);
//    void cartesian_product(list< vector<Literal> > sets, list< vector<Literal> >& product);
//
//public:
//    PlanningGraph(Planner* planner);
//    ~PlanningGraph();
//
//    void add_K_level(PGraphENodeLevel);
//    void add_A_level(PGraphActionLevel);
//
//    int constructEPG(Kripke*); //construct planning graph and return the its level satisfied the goal.
//
//    //reimplement these methods to get fd, pd, od, without respect to a kripke structure because we do not consider ``if" in a observes and aware_of statement
//    AgentSet get_fd(const KAction& act) const;
//    AgentSet get_pd(const KAction& act) const;
//    AgentSet get_od(const KAction& act) const;
//
//    //printing
//    void print_info();
//
//};
//
//
//
//
//
//
//
//
//
//
