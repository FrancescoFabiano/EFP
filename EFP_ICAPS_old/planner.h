#ifndef PLAN_H
#define PLAN_H

#include "define.h"
#include "reader.h"
#include "timer.h"
#include "state.h"
#include "action.h"
#include "kstate.h"
#include "kedge.h"
#include "kripke.h"
#include "planninggraph.h"



/***********************************************************************
 Other classes used in Planner
 ************************************************************************/
typedef list<string> RawPlan;
class Planner;
class PlanningGraph;

//for searching

class CKripke
{
    friend class Planner;
    friend class PlanningGraph;
protected:
    Kripke* k;
    vector<Kripke*> ks;
    vector<string> path;
    Planner* planner;
    CKripke* old;

public:
    /* constructor & destructor */
    CKripke(Planner*);
    ~CKripke();

    /* get/set functions */
    void set_result(Kripke*);
    void add_path(string);
    void set_path(vector<string> st);
    void add_kripke(Kripke*);
    vector<Kripke*> get_result();
    vector<string> get_path();
    bool satisfy();
    int satisfywithprior(); //return how many sub-goals are satisfied already
    CKripke* next_ckripke(KAction a);
    void set_old(CKripke*);
    CKripke* get_old();
    int lengthOfPlanningGraph(); //return the maximal length of Planning Graphs; each of which is a PlanningGraph of a Kripkek in ks
};
typedef queue<CKripke*> CKripkeQueue;
typedef vector<Kripke> KripkeList;

/*
 * Class Priority Queue
 */
struct node
{
    int priority;
    CKripke* ck;
    struct node *link;
};

class Priority_Queue
{
private:


public:
    node *front;
    Planner* planner;

    Priority_Queue(Planner* pl)
    {
        front = NULL;
        planner = pl;
    }

    /*
     * Insert into Priority Queue
     */
    void insert(CKripke* item)
    {
        int priority = item->lengthOfPlanningGraph();
        if (priority == -1) { //this CKripke cannot get to the goal
            return;
        }
        node *tmp, *q;
        tmp = new node;
        tmp->ck = item;
        tmp->priority = priority;
        if (front == NULL || priority < front->priority) {
            tmp->link = front;
            front = tmp;
        }
        else {
            q = front;
            while (q->link != NULL && q->link->priority < priority)
                q = q->link;
            tmp->link = q->link;
            q->link = tmp;
        }
    }

    /*
     * Delete from Priority Queue
     */
    CKripke* pop()
    {
        node *tmp;
        if (front == NULL) {
            cout << "Queue Underflow\n";
            return NULL;
        }
        else {
            tmp = front;
            //cout<<"popi item is: "<<endl;
            front = front->link;
            //free(tmp);
            return tmp->ck;
        }
    }
    //        /*
    //         * Print Priority Queue
    //         */
    //        void display()
    //        {
    //            node *ptr;
    //            ptr = front;
    //            if (front == NULL)
    //                cout<<"Queue is empty\n";
    //            else
    //            {	cout<<"Queue is :\n";
    //                cout<<"Priority       Item\n";
    //                while(ptr != NULL)
    //                {
    //                    cout<<ptr->priority<<"                 "<<endl;
    //                    ptr->ck->get_result()->print_report();
    //                    ptr = ptr->link;
    //                }
    //            }
    //        }
};

/***********************************************************************
 Main class
 ************************************************************************/
class Planner
{
    friend class State;
    friend class Action;
    friend class CKripke;
    friend class PlanningGraph;

public:
    /* input parameters */
    Semantics m_semantics;
    Task m_task;
    Algorithm m_algorithm;
    RawPlan m_plan;
    bool m_detect_essential;
    bool m_output_decisive;
    bool m_series_kripkes;
    bool debug;
    Heuristics m_heuristic;
    bool useHeuristic;
    string m_domain_name; // domain name
    bool results_file = false;
    bool givenplan = false;

protected:
    Reader* m_reader; // reader
    Timer* m_timer; // timer


    map<string, int> m_action_map; // action mapping
    Literals m_literals; // fluent literals
    //ActionList m_actions;         // action description
    //Currently, all actions of all types are included in ontic. No action in sensing and in ann.
    KActionList ontic; // Ben: ontic (normal) action description
    KActionList sensing; // Ben: sensing action description
    KActionList ann; // Ben: announcement action description
    StaticLaws m_statics; // static laws
    Kripke m_init_cstate; // init kripke
    KripkeList m_init_cstates; // set of init kripke
    Literals m_goal; // set of literals in the goal
    Nodes m_gd; // goal description, in CNF form:
    // {(f ^ h ) v g} is represented as
    // {{f,g},{h,g}}
    map<string, Literal> m_map; // literal mapping
    set<Agent> m_agents; // agents
    map<string, Agent> a_map; // agent mapping
    Nodes k_init; //bfnodes
    InitRequire ini;

protected:
    AgentSet get_fd(const KAction&, Kripke) const;
    AgentSet get_pd(const KAction&, Kripke) const;
    AgentSet get_od(const KAction&, Kripke) const;
    bool isSameInterpret(Kstate, Kstate, Literals) const;
    bool isSameSatisfyFF(Kstate, Kstate, FluentFormula) const;
    //  /* for searching */
    //  StateTable m_states;   // state table (reached states)
    //  CStateTable m_cstates; // visited cstates
    //  PriorityCStateQueue m_queue; // current queue

public:
    /* constructor and destructor */
    Planner(Reader* reader, Timer* timer);
    virtual ~Planner();

    /* main function */
    bool main();
    Nodes get_goal();
    /* generic functions */
    bool intersect(const Literals* x, const Literals* y) const;
    void computeLiteral(Kstate*);

    CKripke* doplan_search();
    CKripke* doplan_search(CKripke* cur, CKripkeQueue* queue);

    CKripke* search();
    CKripke* search(CKripke* cur, CKripkeQueue* queue);

    CKripke* searchwithheur();
    CKripke* searchwithheur(CKripke*, Priority_Queue*);

    /* grounding functions */
    Literal ground(const string& x) const;
    Literals ground(const StringList* x) const;
    string convert(const Literal& x) const;
    StringList convert(const Literals& x) const;
    FluentFormula* convert(StringList2);
    Agents convert(StringList);

    /* build the domain & check errors */
    bool build_domain();
    bool build_goal();

    KAction* add_ontic(const string str, PropositionType);
    FluentFormula* and2ff(FluentFormula*, FluentFormula*);
    bool is_consistent(Fluents, Fluents);

    /* printing functions */
    void print_statistics() const;
    void print(const Literal& l) const;
    void print(const Literals& x) const;
    void print_interal_domain();
    void print_fluform(StringList2);
    void print_fluform(FluentFormula);
    void print_ontic(KAction*);
    void print_literals(const Literals&) const;
    void print_read_init();

    //execute function. 
    bool is_executable(Kripke, KAction) const;
    bool is_executable1(Kripke, KAction) const; //true for B(a,phi) even for there is no edge of label a out;
    Literals compute_effect(KAction*, Kripke*);
    Kstate* comp_nextstate(Kstate, Literals);

    Kripke* execute_ontic(Kripke, KAction);
    Kripke* execute_sensing(Kripke, KAction);
    Kripke* execute_ann(Kripke, KAction);
    Kripke* merge_kripkes(Kripke*, Kripke*, map<int, int>*);
    Kripke* update_kripke(Kripke k);

    bool check_Agents(Agent, Agents);

    void update_node(BFNode*);
    EdgeList updateEdgesListId(EdgeList, map<int, int>);

    bool check_lits(Literal, Literals);


    /* Kripke functions */
    bool build_init_k();

    /* functions used during the search */
protected:
    Literals negate(const Literals& x) const;

};

#endif
