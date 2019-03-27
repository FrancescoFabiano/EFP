#ifndef KRIPKE_H
#define KRIPKE_H
#include "define.h"
#include "kstate.h"
#include "kedge.h"

typedef vector<Kstate> States;
typedef set<int> StateList; // vector of states' id.
typedef vector< vector<int> > B;

class Kripke{
   protected:
	States states_list;
    Edges edges_list;
    B edges;
    int pId;
    
private:
    bool entail_BForm(BFNode,Agent,bool) const;
    
   public:
	Kripke();
	Kripke(Kripke*);
	~Kripke();
    
    Kripke* make_copy();
	/* operators */
	
	bool entail(BFNode) const;
	bool entail1(BFNode) const;
	
	bool entail_ff_list(const list<FluentFormula>) const;
	
    States get_stateslist();
	B get_edges();
    Edges get_edgeslist();
	Kstate* get_pstate();
	int get_id();
	void set_stateslist(States);
	void set_edges(B);
	void set_pId(int);
    void set_edgeslist(Edges);
    void set_edgeslist_fix_states(Edges);
    void set_stateslist_fix_states(States);
    void add_kstate(Kstate);
	void print_report();
	void add_edge2state(int stIndex, int edgeIndex, bool isOut);
	void add_edge(Kedge);
    void add_to_edges_list(Kedge k); //add to edges list only, without add to states
	StateList reachableStates(int, Agents) const;
	bool isValid(AgentSet);
    
    

	/*Francesco*/

	void get_the_new_edges(Agent ag, int &id_edge, Edges &to_add);

	void correctFalseBeliefs(FluentFormula ff, Agent ag, int &id_edge, Edges &to_add);
};


#endif
