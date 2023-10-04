#ifndef ACTION_H
#define ACTION_H

#include "define.h"
#include "state.h"
#include "kstate.h"

class Planner;

/***********************************************************************
 class: Action
 ************************************************************************/
class KAction {
protected:
    const Planner* m_planner;
    string m_name;          // action name
    PropositionType m_type;
    Nodes m_execs;       // executability conditions
    Fluents_BF_list m_effects;   // effects
    Agent_FF_list m_observes;      // sensed fluent
    Agent_FF_list m_awares;      // sensed fluent
    list<Literals> sensedFluents;
    list<FluentFormula> annFFs;
    
    
public:
    /* constructor & destructor */
    KAction(const Planner* planner, const string& name, const PropositionType pt);
    ~KAction();
    
    /* get/set functions */
    string get_name() const;
    void set_name(const string& name);
    void set_type(const PropositionType);
    
    
    const Nodes* get_execs() const;
    const Fluents_BF_list* get_effects() const;
    Agent_FF_list get_m_observes() const;
    Agent_FF_list get_m_awares() const;
    const PropositionType get_type() const;
    const Literals get_sensedFluents() const;
    const list<FluentFormula> get_ann_ff() const;
    
    void add_execs(const BFNode&);
    void add_effect(Literals, const BFNode&);
    void add_observes(const Agent&,  const FluentFormula&);
    void add_awares(const Agent&,  const FluentFormula&);
    void add_sensedFluents(const Literals&);
    void add_ann_ff(const FluentFormula&);
    
    /* main functions */
    // bool is_executable(const State* s) const;
    // bool is_executable(const CState* cs) const;
    void print() const;
};

typedef vector<KAction> KActionList;

#endif
