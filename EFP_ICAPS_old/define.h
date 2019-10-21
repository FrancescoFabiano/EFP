#ifndef DEFINE_H
#define DEFINE_H

#include <string>
#include <iostream>
#include <set>
#include <list>
#include <queue>
#include <map>
#include <vector>
#include <algorithm>
#include <sys/time.h>
#include <sys/resource.h>
//#include "kstate.h"
//#include "kedge.h"

using namespace std;

#define DEBUG

#define PRINT_TIME_DETAILS
//#define PRINT_DOMAIN
//#define PRINT_INTERAL_DOMAIN
//#define PRINT_SUMMARY
//#define PRINT_PICKUP_NODE
//#define PRINT_LOOKUP_TABLE


#ifdef PRINT_PICKUP_NODE
#define PRINT_MAPPING
#endif

#ifdef PRINT_LOOKUP_TABLE
#define PRINT_MAPPING
#endif

#ifdef PRINT_TIME_DETAILS
#define RESUME_TIMER(x) m_timer->start(x)
#define PAUSE_TIMER(x) m_timer->end(x)
#else
#define RESUME_TIMER(x) m_timer->end(x)
#define PAUSE_TIMER(x)
#endif

//#define USE_GOAL_LOOKUP
//#define USE_TRANSITION_LOOKUP
//#define USE_HEURISTIC_LOOKUP
#define UPDATE_SHORTER_PLAN

/***********************************************************************
 Do not change the following
************************************************************************/

class Action;
class Domain;

#define NEGATE(l,x) (x = ((l % 2 == 0) ? l+1 : l-1))

/***********************************************************************
 Constants
************************************************************************/
#define NEGATION_SYMBOL "-"

// maximum fluent/action name length
#define MAX_LEN 200

#define ERROR_001 "Invalid fluent/literal. Check the fluent declaration"
#define ERROR_002 ""
#define ERROR_003 ""
#define ERROR_004 ""

enum Algorithm {
  GREEDY
};

enum Semantics {
  PC,
  PH
};

enum Task {
  SEARCH,
  DOPLAN
};

// timers
#define READ_TIMER 0
#define BUILD_TIMER 1
#define SEARCH_TIMER 2
#define GOAL_TIMER 3
#define HEURISTIC_TIMER 4
#define RESULT_TIMER 5
#define TRANSITION_LOOKUP_TIMER 6
#define EXECUTABILITY_TIMER 7
#define ACTUAL_RESULT_TIMER 8
#define DIRECT_EFFECT_TIMER 9
#define INDIRECT_EFFECT_TIMER 10
#define IDLE_TIMER 11
#define NEXTSTATE_TIMER 12
#define COMMUNICATION_TIMER 13
#define ESSENTIAL_FLUENTS_TIMER 14

/***********************************************************************
 Types -- For scanner & parser
************************************************************************/
// for scanner & parser
typedef set<string> StringList;
typedef set<StringList> StringList2;

enum Heuristics {
    NUMSATGOALS,
    PLANNINGGRAPH,
    SUMPLANNINGGRAPH,
    NOTUSE
    
};

enum PropositionType {
  STATIC,		//not an action
  DYNAMIC,		//ontic action, an edge
  EXECUTABILITY,	//ontic action
  IMPOSSIBILITY,	//ontic action
  DETERMINATION,   	//sensing action (peek_a), creates Kripke
  ANNOUNCEMENT,    	//announcement action (shout_tail_a), creates Kripke
  OBSERVANCE,		//sensing action
  AWARENESS		//sensing action
};

/*
These statements indicate that everyone knows that A has the key and B and C do not havethe key, the box is closed, no one knows whether the coin lies head or tail up, and everyone is looking at the box.
*/
typedef unsigned short Fluent;
typedef set<Fluent> Fluents;

//Agents
typedef unsigned short Agent;
typedef vector<Agent> Agents;
typedef set<Agent> AgentSet;

// Ben: Kripke Components

typedef set<Fluents> FluentFormula; //To represent the fluent formual in DNF format


// literals
typedef unsigned short Literal;

// set of literals and states
typedef set<Literal> Literals;

/* Belief Formulae Implementation */

enum BFNodeType {
    fluForm,
    BForm,
    propForm,
    EForm,
    CForm,
    BFEmpty
};

enum BFOperator {
    BFAND,
    BFOR,
    BFNOT,
    NONE
};

class BFNode {
 public:
  //map<string,Fluent> ff_map; // for mapping string to corresponding fluent (integer)
  string agentPro;		//i, or the agent this belief node belongs to

  //agent in number form
  Agent agentPro2;

  BFNodeType node_type;		//Which case of belief formula this node is

  /* Four forms of belief formulae */

  //fluForm
  StringList2 flu_form;
  //fluent formula in number form
  FluentFormula* fluform;

  //BForm
  // B(i,φ): where φ is a belief formula
  BFNode* bfnode1;


  //propForm
  //a formula of the form φ1 ∨ φ2, φ1 ∧ φ2, φ1 ⇒ φ2, or ¬φ1 where φ1, φ2 are belief formulae
  // use BFNode bfnode1;
  BFNode* bfnode2;
  BFOperator bfOperator;


  //EForm or CForm
  //a formula of the form E(α,φ) or C(α,φ): where φ is a belief formula and ∅≠ α⊆AG;  E(α,φ)/C(α,φ) are group formulae
  // use BFNode bfnode1;
  StringList groupAgent;
  //groupAgent in number form
  Agents agents;


  void set_flu(FluentFormula* in){
        fluform = in;
  }

  void print() const {

    bool first = true;
    StringList::const_iterator it;
    StringList2::const_iterator it2;
    bool first2 = true;
	cout << "(";
    switch (node_type) {

    case fluForm:
       for (it2 = flu_form.begin(); it2 != flu_form.end(); it2++) {
       if (!first2)
         cout << " | ";
       first2 = false;
       first = true;
       cout << "(";
       for (it = it2->begin(); it != it2->end(); it++) {
         if (!first)
   	   cout << " AND ";
           first = false;
           cout << *it;
       }
       cout << ")";
     }
     cout << ")";
    break;

    case BForm:
       cout << "B(" << agentPro << ",";
       bfnode1->print();
       cout << ")";
       cout << ")";
    break;

    case CForm:
       cout << "C(";
       for (it = groupAgent.begin(); it != groupAgent.end(); it++) {
      	 if (!first)
		cout << ",";
      	 first = false;
      	 cout << *it;
    	 }
       cout << ",";
       bfnode1->print();
       cout << ")";
       cout << ")";
     break;

     case EForm:
       cout << "E(";
       for (it = groupAgent.begin(); it != groupAgent.end(); it++) {
      	 if (!first)
		cout << ",";
      	 first = false;
      	 cout << *it;
    	 }
       cout << ",";
       bfnode1->print();
       cout << ")";
       cout << ")";
     break;

     case propForm:
       if(bfOperator == BFNOT)
         cout << " NOT ";
       else if(bfOperator == NONE)
           cout << "(";
       bfnode1->print();
       if(bfOperator == NONE)
           cout << ")";
       if(bfOperator == BFAND)
         cout << " AND ";
       else if(bfOperator == BFOR)
	     cout << " OR ";
       if(bfnode2 != NULL)
          bfnode2->print();
       cout << ")";
     break;

     case BFEmpty: /* static */
     break;

     default: /* static */
      break;

	} //switch
  }; // print
    void print(map<string,Fluent> ff_map,map<string,Agent> a_map) const {

        bool first = true;
        StringList::const_iterator it;
        StringList2::const_iterator it2;
        bool first2 = true;
        cout << "(";
        switch (node_type) {

            case fluForm:
                for (it2 = flu_form.begin(); it2 != flu_form.end(); it2++) {
                    if (!first2)
                        cout << " | ";
                    first2 = false;
                    first = true;
                    cout << "(";
                    for (it = it2->begin(); it != it2->end(); it++) {
                        if (!first)
                            cout << " AND ";
                        first = false;
                        cout << ground_lit(*it,ff_map);
                    }
                    cout << ")";
                }
                cout << ")" ;
                break;

            case BForm:
                cout << "B(" << ground_agent(agentPro,a_map) << ",";
                bfnode1->print(ff_map,a_map);
                cout << ")";
                cout << ")";
                break;

            case CForm:
                cout << "C(";
                for (it = groupAgent.begin(); it != groupAgent.end(); it++) {
                    if (!first)
                        cout << ",";
                    first = false;
                    cout << ground_agent(*it,a_map);
                }
                cout << ",";
                bfnode1->print(ff_map,a_map);
                cout << ")";
                cout << ")";
                break;

            case EForm:
                cout << "E(";
                for (it = groupAgent.begin(); it != groupAgent.end(); it++) {
                    if (!first)
                        cout << ",";
                    first = false;
                    cout << ground_agent(*it,a_map);
                }
                cout << ",";
                bfnode1->print(ff_map,a_map);
                cout << ")";
                cout << ")";
                break;

            case propForm:
                if(bfOperator == BFNOT)
                    cout << " NOT ";
                else if(bfOperator == NONE)
                    cout << "(";
                bfnode1->print(ff_map,a_map);
                if(bfOperator == NONE)
                    cout << ")";

                if(bfOperator == BFAND)
                    cout << " & ";
                else if(bfOperator == BFOR)
                    cout << " | ";
                if(bfnode2 != NULL)
                    bfnode2->print(ff_map,a_map);
                cout << ")" ;
                break;

            case BFEmpty: /* static */
                break;

            default: /* static */
                break;

        } //switch
    }; // print

    Literal ground_lit(const string& x,map<string,Fluent> ff_map) const
    {
        map<string,Literal>::const_iterator p = ff_map.find(x);

        if (p != ff_map.end()) {
            return (p->second);
        }

        cout << "ERROR: Literal " << x << " is undeclared." << endl;
        cout << "Check the fluent declaration." << endl << endl;

        exit(1);
    }
    Agent ground_agent(const string& x,map<string,Agent> a_map) const
    {
        map<string,Literal>::const_iterator p = a_map.find(x);

        if (p != a_map.end()) {
            return (p->second);
        }

        cout << "ERROR: Agent " << x << " is undeclared." << endl;
        cout << "Check the agent declaration." << endl << endl;

        exit(1);
    }
}; //class


typedef list<BFNode> Nodes;


class Proposition {
 public:
  PropositionType n_type;
  string act_name;
  StringList precond;
  StringList effect;
  string agentPro;
  //string fluentPro;
  StringList2 flu_form;
  BFNode bel_form;


  const StringList* get_precondition() const{
    return &precond;
  };

  const StringList* get_effect() const {
    return &effect;
  };

  void print() const {
    bool first = true;
    bool first2 = true;
    bool if_flu_form = false;     //to stop “if” from printing too many times
    StringList::const_iterator it;
    StringList2::const_iterator it2;

    switch (n_type) {
    case DYNAMIC:
      cout << act_name << " causes ";
      break;
    case EXECUTABILITY:
      cout << "executable " << act_name << " if ";
            bel_form.print();
      break;
    case IMPOSSIBILITY:
      cout << "impossible " << act_name << " ";
      break;
    case DETERMINATION:
      cout << act_name << " determines ";
      break;
    case ANNOUNCEMENT:
      cout << act_name << " announces ";
      break;
    case OBSERVANCE:
      cout << agentPro << " observes " << act_name;
      break;
    case AWARENESS:
      cout << agentPro << " aware of " << act_name;
      break;
    default: /* static */
      break;
    }


    for (it = effect.begin(); it != effect.end(); it++) {
      if (!first)
	cout << ",";
      first = false;
      cout << *it;
    }

    first = true;
    for (it = precond.begin(); it != precond.end(); it++) {
      if (!first)
	cout << ",";
      else
	cout << " if ";
      first = false;
      cout << *it;
    }


     first2 = true;
     for (it2 = flu_form.begin(); it2 != flu_form.end(); it2++) {
       if( (n_type == AWARENESS || n_type == OBSERVANCE) && !if_flu_form){
       	cout << " if ";
        if_flu_form = true;
        }
       if (!first2)
         cout << " | ";
       first2 = false;
       first = true;
       cout << "(";
       for (it = it2->begin(); it != it2->end(); it++) {
         if (!first)
   	   cout << " AND ";
           first = false;
           cout << *it;
       }
       cout << ")";
     }

     if(bel_form.bfnode1 != NULL || n_type == DYNAMIC){
        cout << " if ";
	bel_form.print();
	}
     cout << endl;

  };

};

typedef list<Proposition> PropositionList;

class Fluents_BF{
protected:
    Literals l_head;
    BFNode b_body;
public:
    Fluents_BF();
    ~Fluents_BF();

    void set_l_head(Literals bd);
    void set_bn_body(const BFNode&);

    const Literals* get_head() const;
    const BFNode* get_body() const;
};

typedef list<Fluents_BF> Fluents_BF_list;

class Agent_FF{
protected:
    Agent a_head;
    FluentFormula ff_body;
public:
    Agent_FF();
    ~Agent_FF();

    void set_a_head(Agent bd);
    void set_ff_body(FluentFormula);

    Agent get_head() const;
    FluentFormula get_body() const;
};

typedef list<Agent_FF> Agent_FF_list;

class InitRequire{
public:
	FluentFormula pointed; //for pointed state
	FluentFormula musthave; //check for every state
	Agent_FF_list correctintepret; //check for edges connecting states have a fluentformula
	Agent_FF_list sameintepret; //check for edges connecting states have the same interpret on a fluentformula


	//void set_pointed(FluentFormula);
	//void set_musthave(FluentFormula);
	void add_correct(const Agent& ag,  const FluentFormula& ff){
		Agent_FF af;
		af.set_a_head(ag);
		af.set_ff_body(ff);
		correctintepret.push_back(af);
	};
	void add_same(const Agent& ag,  const FluentFormula& ff){
		Agent_FF af;
		af.set_a_head(ag);
		af.set_ff_body(ff);
		sameintepret.push_back(af);
	};

	//const FluentFormula get_pointed() const;
	//const FluentFormula get_musthave() const;
	//const Agent_FF_list get_correct() const;
	//const Agent_FF_list get_same() const;

};

#endif
