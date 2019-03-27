//
//  planninggraph_h
//
//
//  Created by Tiep Le on 11/15/17.
//

#ifndef planninggraph_h
#define planninggraph_h

#include <stdio.h>
#include <list>
#include <map>
#include "define.h"
#include "kripke.h"
#include "action.h"
#include "kstate.h"
#include "planner.h"

class EpistemicModelsNode; // for each node in the epistemic_model_list level in epistemic planning graph
class PGraphActionLevel; //for action_list level in epistemic planning graph
class PGraphENodeLevel; // for epistemic_model_list level in epistemic planning graph

/****************************
 CLASS EpistemicModelsNode
 ***************************/
class EpistemicModelsNode{
protected:
    Kripke* kripke;
    set<int> pointeds; //set of states is as pointed states
    //vector<Kripke*> kripkeVec;
    string action;
    
public:
    EpistemicModelsNode();
    EpistemicModelsNode(Kripke*, set<int>);
    ~EpistemicModelsNode();
    void set_Kripke(Kripke*);
    void add_pointedID(int);
    void set_action(string);
    
    Kripke* get_Kripke();
    set<int> get_pointeds();
    string get_action();
    //printing
    void print_info();
};


/***********************************************************************
 class: PGraphActionLevel
 ************************************************************************/

class PGraphActionLevel{
protected:
    set<string> actionVec;
    
public:
    PGraphActionLevel();
    ~PGraphActionLevel();
    void set_ActionVec(set<string>);
    void add_action(string);
    
    set<string> get_ActionVec();
    
    //printing
    void print_info();
};

/***********************************************************************
 class: PGraphENodeLevel
 ************************************************************************/

class PGraphENodeLevel{
protected:
    vector<EpistemicModelsNode> eNodes;
    
private:
    
    
public:
    PGraphENodeLevel();
    PGraphENodeLevel(vector<EpistemicModelsNode>);
    ~PGraphENodeLevel();
    
    void set_eNodes(vector<EpistemicModelsNode>);
    void add_eNode(EpistemicModelsNode);
    
    vector<EpistemicModelsNode> get_eNodes();
    
    //check satisfaction of a belief formula given an epistemic_model_list level (i.e., eNodes)
    bool emNodes_entailment(vector<EpistemicModelsNode> v, BFNode bf);
    bool emNodes_entailment(BFNode bf);
    
    bool executable(KAction);
    
    //printing
    void print_info();
};
/***********************************************************************
 class: PlanningGraph
 ************************************************************************/

class PlanningGraph{
protected:
    Planner* m_planner;
    PGraphENodeLevel k_levels; //for union of \K_i levels for the sake of saving memory
    vector<PGraphActionLevel> a_levels; //for A_i levels
    
private:
    EpistemicModelsNode effect_sensing(Literal l, AgentSet fd, AgentSet pd, AgentSet od, string action);
    EpistemicModelsNode effect_ontic(Literal l, AgentSet fd, AgentSet od, string action);
    EpistemicModelsNode effect_announce(FluentFormula ff, AgentSet fd, AgentSet pd, AgentSet od, string action);
    void cartesian_product( list< vector<Literal> > sets, list< vector<Literal> >& product);
    
public:
    PlanningGraph(Planner* planner);
    ~PlanningGraph();
    
    void add_K_level(PGraphENodeLevel);
    void add_A_level(PGraphActionLevel);
    
    int constructEPG(Kripke*); //construct planning graph and return the its level satisfied the goal.
    
    //reimplement these methods to get fd, pd, od, without respect to a kripke structure because we do not consider ``if" in a observes and aware_of statement
    AgentSet get_fd(const KAction& act) const;
    AgentSet get_pd(const KAction& act) const;
    AgentSet get_od(const KAction& act) const;
    
    //printing
    void print_info();
    
};

#endif /* planninggraph_h */









