#ifndef STATE_H
#define STATE_H

#include "define.h"

class Planner;
class Action;

/***********************************************************************
 Types -- Domain Description
************************************************************************/
class Effect {
 protected:
  Literals m_head;
  Literals m_body;
  Literals m_neg_body; // negation of the body
 public:
  Effect();
  ~Effect();

  void set_body(Literals bd);
  void set_head(Literals hd);
  const Literals* get_body() const;
  const Literals* get_neg_body() const;
  const Literals* get_head() const;
};

typedef list<Effect> EffectList;
typedef list<Literals> ExecList;

typedef Effect StaticLaw;
typedef list<StaticLaw> StaticLaws;

class State
{
public:
  Literals m_literals;
protected:
  const Planner* m_planner;
  int m_hvalue;     // heuristic value
  int m_gsatisfied; // -1: not computed; 0 : not satisfied; 1: satisfied

public:
  State(const Planner* planner);
  ~State();

  /* operators */
  void clear();
  int operator<(const State&) const;
  bool operator==(const State&) const;
  State operator=(const State& s);
  bool intersect(const Literals* x) const;
  bool intersect(const State* s) const;
  bool includes(const Literals* x) const;
  bool includes(const State* s) const;

  /* main functions */
  void closure();

  /* other functions */
  bool is_consistent() const;
  int hvalue();
  bool goal_satisfied();
  void print() const;
  bool satisfies(const Literals* x);
};

class StateComp
{
 public:
  int operator()(const State* s1, const State* s2) const {
    return *s1 < *s2;
  }
};

typedef set<State*,StateComp> StateTable;

class CState {
  friend class PriotizedCState;
  friend class CStateComp;
 public:
  set<State*> m_states;
protected:
  int m_hvalue;        // heuristic value, -1 if not computed
  int m_plan_length;   // length of the plan
  const Action* m_action;
  const Action* sensing;		//Ben: sensing actions
  const Action* ontic;			//Ben: normal actions
  const Action* ann;			//Ben: announcement actions
  const CState* m_previous_cstate;
public:
  /* constructor and destructor */
  CState();
  ~CState();

  void clear();
  bool operator<(const CState& right) const;
  CState operator=(const CState& s);
  bool goal_satisfied();
  int hvalue();

  int get_plan_length() const;
  void set_plan_length(const int& len);

  const Action* get_action() const;
  const Action* get_sensing() const;	//Ben
  const Action* get_ontic() const;	//Ben
  const Action* get_ann() const;	//Ben
  void set_action(const Action* act);
  void set_sensing(const Action* act);  //Ben
  void set_ontic(const Action* act);    //Ben
  void set_ann(const Action* act);      //Ben

  const CState* get_previous_cstate() const;
  void set_previous_cstate(const CState* cs);

  /* other functions */
  void print() const;
  void print_plan() const;
};

class CStateComp
{
 public:
  int operator()(const CState* cs1, const CState* cs2) const {
    return *cs1 < *cs2;
  }
};

typedef set<CState*,CStateComp> CStateTable;

// Friend class with function object for comparison of cnodes
class PriotizedCState {
public :
  int operator()(CState* cs1, CState* cs2) const
  {
	return cs1->hvalue() < cs2->hvalue();
  }
};

typedef priority_queue<CState*,vector<CState*>,PriotizedCState> PriorityCStateQueue;
#endif
