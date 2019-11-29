// planner.cpp: implementation of the planner class.
//////////////////////////////////////////////////////////////////////
#include "planner.h"
#include <math.h>
#include <stdio.h>
#include "kstate.h"
#include "kedge.h"
#include "kripke.h"
#include "define.h"
#include "planninggraph.h"

#include <fstream>

/********----> PLAN is:----> PLAN is:*****************************************************************
 * Planner Implementation
 *************************************************************************/
// Constructor & destructor

Planner::Planner(Reader* reader, Timer* timer)
{
	m_timer = timer;
	m_reader = reader;

	m_semantics = PC;
	m_task = SEARCH;
	m_algorithm = GREEDY;
	m_detect_essential = true;
	m_output_decisive = false;
	m_series_kripkes = false;
	debug = false;
	m_heuristic = NOTUSE;
	useHeuristic = false;
}

Planner::~Planner()
{

}

bool Planner::main()
{


#ifdef PRINT_DOMAIN
	m_reader->print();
	exit(0);
#endif


	// build the domain
	cout << "Building domain..." << endl;
	m_timer->start(BUILD_TIMER);
	build_domain();
	build_goal();
	//print_read_init();

	cout << "Building initial Kripke..." << endl;
	build_init_k();
	m_timer->end(BUILD_TIMER);

	cout << "The number of pointed Kripkes in the initial Kripke is " << m_init_cstates.size() << endl;
	if (debug) {
		for (int run = 0; run < m_init_cstates.size(); run++) {
			cout << "--------------Printing Kripke report for initial Kripke :-----------" << endl << endl;
			m_init_cstates.at(run).print_report();
			cout << "----------------------------------" << endl << endl;
		}
	}

	//    cout << "Test cartesian produc" << endl;
	//    Kripke ktest = Kripke(&(*m_init_cstates.begin()));
	//    set<int> s = set<int>();
	//    s.insert(1);
	//    s.insert(2);
	//    s.insert(3);
	//    EpistemicModelsNode a = EpistemicModelsNode(&ktest,s);
	//    EpistemicModelsNode b = EpistemicModelsNode(&ktest,s);
	//    EpistemicModelsNode d = EpistemicModelsNode(&ktest,set<int>());
	//    vector<EpistemicModelsNode>  c = vector<EpistemicModelsNode>();
	//    c.push_back(a);
	//    c.push_back(b);
	//    c.push_back(d);
	//    PGraphENodeLevel pg = PGraphENodeLevel();
	//    pg.cartesian_product_setint(c);
	//    return 1;
	//
	//    cout << "Tesing Planning Graph..." << endl;
	//    Kripke ktest = Kripke(&(*m_init_cstates.begin()));
	//    //ktest.print_report();
	//
	//    PlanningGraph pg = PlanningGraph(this);
	//    int l = pg.constructEPG(&ktest);
	//    //pg.print_info();
	//    cout << "length is " << l << endl;
	//
	//
	//    cout << "--------------------------" << endl;
	//    cout << "--------------executing right for kripke at 0 ------------------------" << endl;
	//    int indexact = m_action_map.find("left")->second;
	//    KActionList::iterator itacl = ontic.begin();
	//    itacl = itacl + indexact;
	//    Kripke* right1 = execute_ontic(m_init_cstates.at(0), *itacl);
	//    cout << "compute plan graph...right1" << endl;
	//    l = pg.constructEPG(right1);
	//    cout << "length of right is " << l << endl;
	//    return 1;
	//
	//    cout << "--------------------------" << endl;
	//    cout << "--------------executing left for kripke at 1 ------------------------" << endl;
	//    indexact = m_action_map.find("right")->second;
	//    itacl = ontic.begin();
	//    itacl = itacl + indexact;
	//    Kripke* right2 = execute_ontic(*right1, *itacl);
	//    cout << "compute plan graph...left" << endl;
	//    l = pg.constructEPG(right2);
	//    cout << "length of right is " << l << endl;
	//
	//    cout << "--------------------------" << endl;
	//    cout << "--------------executing left for kripke at 1 ------------------------" << endl;
	//    indexact = m_action_map.find("right")->second;
	//    itacl = ontic.begin();
	//    itacl = itacl + indexact;
	//    Kripke* right3 = execute_ontic(*right2, *itacl);
	//    cout << "compute plan graph...left" << endl;
	//    l = pg.constructEPG(right3);
	//    cout << "length of right is " << l << endl;
	//
	//    cout << "--------------------------" << endl;
	//    cout << "--------------executing left for kripke at 1 ------------------------" << endl;
	//    indexact = m_action_map.find("right")->second;
	//    itacl = ontic.begin();
	//    itacl = itacl + indexact;
	//    Kripke* right4 = execute_ontic(*right3, *itacl);
	//    cout << "compute plan graph...left" << endl;
	//    l = pg.constructEPG(right4);
	//    cout << "length of right is " << l << endl;
	//
	//    cout << "--------------------------" << endl;
	//    cout << "--------------executing left for kripke at 1 ------------------------" << endl;
	//    indexact = m_action_map.find("right")->second;
	//    itacl = ontic.begin();
	//    itacl = itacl + indexact;
	//    Kripke* right5 = execute_ontic(*right4, *itacl);
	//    cout << "compute plan graph...left" << endl;
	//    l = pg.constructEPG(right5);
	//    cout << "length of right is " << l << endl;
	//
	//    cout << "--------------------------" << endl;
	//    cout << "--------------executing left for kripke at 1 ------------------------" << endl;
	//    indexact = m_action_map.find("right")->second;
	//    itacl = ontic.begin();
	//    itacl = itacl + indexact;
	//    Kripke* right6 = execute_ontic(*right5, *itacl);
	//    cout << "compute plan graph...left" << endl;
	//    l = pg.constructEPG(right6);
	//    cout << "length of right is " << l << endl;
	//
	//    return 1;
	//
	//    cout << "--------------------------" << endl;
	//    cout << "--------------executing left_a for kripke at 0 ------------------------" << endl;
	//    int indexact = m_action_map.find("left_a")->second;
	//    KActionList::iterator itacl = ontic.begin();
	//    itacl = itacl + indexact;
	//    Kripke* left_a = execute_ontic(m_init_cstates.at(0), *itacl);
	//    cout << "compute plan graph...left_a" << endl;
	//    l = pg.constructEPG(left_a);
	//    cout << "length of left_a is " << l << endl;
	//    cout << "--------------------------" << endl;
	//    cout << "--------------executing left_b for kripke at 0 ------------------------" << endl;
	//    indexact = m_action_map.find("left_b")->second;
	//    itacl = ontic.begin();
	//    itacl = itacl + indexact;
	//    Kripke* left_b = execute_ontic(m_init_cstates.at(0), *itacl);
	//    cout << "compute plan graph...left_b" << endl;
	//    l = pg.constructEPG(left_b);
	//    cout << "length of left_b is " << l << endl;
	//    cout << "--------------------------" << endl;
	//    cout << "--------------executing right_a for kripke at 0 ------------------------" << endl;
	//    indexact = m_action_map.find("right_a")->second;
	//    itacl = ontic.begin();
	//    itacl = itacl + indexact;
	//    Kripke* right_a = execute_ontic(m_init_cstates.at(0), *itacl);
	//    cout << "compute plan graph...right_a" << endl;
	//    l = pg.constructEPG(right_a);
	//    cout << "length of right_a is " << l << endl;
	//    cout << "--------------------------" << endl;
	//    cout << "--------------executing right_b for kripke at 0 ------------------------" << endl;
	//    indexact = m_action_map.find("right_b")->second;
	//    itacl = ontic.begin();
	//    itacl = itacl + indexact;
	//    Kripke* right_b = execute_ontic(m_init_cstates.at(0), *itacl);
	//    cout << "compute plan graph...right_b" << endl;
	//    l = pg.constructEPG(right_b);
	//    cout << "length of right_b is " << l << endl;
	//
	//
	//    return 1;

	//    cout << "--------------executing open_a for kripke at 0 ------------------------" << endl;
	//    int indexact = m_action_map.find("open_a")->second;
	//    KActionList::iterator itacl = ontic.begin();
	//    itacl = itacl + indexact;
	//    Kripke* prin0 = execute_ontic(m_init_cstates.at(0), *itacl);
	//    prin0->print_report();
	//    //m_init_cstates.at(1).print_report();
	//    //Kripke* prin1 = execute_sensing(m_init_cstates.at(1), *itacl);
	//    //prin1->print_report();
	//    cout << "start computing Planning graph" << endl;
	//        PlanningGraph pg = PlanningGraph(this);
	//        int l = pg.constructEPG(prin0);
	//        //pg.print_info();
	//        cout << "length is " << l << endl;

	//    cout << "---------executing signal_c_b------" << endl;
	//    indexact = m_action_map.find("signal_c_b")->second;
	//    itacl = ontic.begin();
	//    itacl = itacl + indexact;
	//    //
	//    //cout << "state 0 is executable:" << is_executable(m_init_cstates.at(0), *itacl) << endl;
	//    //cout << "state 1 is executable:" << is_executable(m_init_cstates.at(1), *itacl) << endl;
	//    Kripke* prin1 = execute_ontic(*prin0, *itacl);
	//    prin1->print_report();
	//    return 1;

	cout << "Solving..." << endl;


	m_timer->start(SEARCH_TIMER);

	CKripke* sol;
	if (useHeuristic) {
		sol = searchwithheur();
	} else {
		if (m_task == DOPLAN) {
			sol = doplan_search();
		} else
			sol = search();

	}
	m_timer->end(SEARCH_TIMER);
	if (sol != nullptr) {
		vector<string> solution = sol->get_path();
		vector<string>::iterator isol;
		std::cout << "Plan Length: " << (solution.size() - 1);
		std::cout << "\n\nExecuted actions: ";
		for (isol = std::next(solution.begin(), 1); isol != solution.end(); isol++) {
			cout << *isol;
			if ((isol + 1) != solution.end()) {
				std::cout << ", ";
			}
		}
		cout << endl;
		//sol->get_result();
	} else {
		cout << "----> THERE IS NO PLAN FOR SUCH GOALS!!!! " << endl;
	}
	if (debug) {
		print_interal_domain();
		cout << endl;
	}
	print_statistics();

	return true;
};

CKripke* Planner::doplan_search()
{
	CKripke* out;
	CKripke* ck = new CKripke(this);
	ck->add_path("start");
	vector<Kripke>::iterator itk;
	for (itk = m_init_cstates.begin(); itk != m_init_cstates.end(); itk++) {
		Kripke* k = new Kripke(&*itk);
		ck->add_kripke(k);
	}

	CKripkeQueue* queue = new CKripkeQueue();
	queue->push(ck);
	if (queue->size() != 0) {
		CKripke* n = queue->front();
		queue->pop();
		out = doplan_search(n, queue);
		return out;
	} else {
		return nullptr;
	}
}

CKripke* Planner::doplan_search(CKripke* cur, CKripkeQueue* queue)
{
	KActionList::iterator it;
	CKripke* out;
	for (it = ontic.begin(); it != ontic.end();) {
		if (it->get_name().compare(m_plan.front()) == 0) {
			m_plan.pop_front();
			CKripke* next = cur->next_ckripke(*it);
			if (next != nullptr) {
				/*if (next->satisfy()) {
					cout << "Congratulation. Goal found!..... :)" << endl;
					return next;
				} else {*/
				//cout << "size of path is " << next->get_path().size() << endl;
				queue->push(next);
				//}
			}
			it = ontic.end();
		} else {
			it++;
		}
	}
	if (m_plan.size() != 0) {
		CKripke* n = queue->front();
		queue->pop();
		out = doplan_search(n, queue);
		return out;
	} else {
		return nullptr;
	}
}

CKripke* Planner::search()
{
	CKripke* out;
	CKripke* ck = new CKripke(this);
	ck->add_path("start");
	vector<Kripke>::iterator itk;
	for (itk = m_init_cstates.begin(); itk != m_init_cstates.end(); itk++) {
		Kripke* k = new Kripke(&*itk);
		ck->add_kripke(k);
	}
	//Kripke* k = new Kripke(&m_init_cstate);
	//ck->set_result(k);
	if (ck->satisfy()) {
		cout << "Congratulation. Goal found!................ :)" << endl;
		return ck;
	}

	CKripkeQueue* queue = new CKripkeQueue();
	queue->push(ck);
	if (queue->size() != 0) {
		CKripke* n = queue->front();
		queue->pop();
		out = search(n, queue);
		return out;
	} else {
		return NULL;
	}
}

CKripke* Planner::search(CKripke* cur, CKripkeQueue* queue)
{
	KActionList::iterator it;
	CKripke* out;
	for (it = ontic.begin(); it != ontic.end(); it++) {
		//cout << " current is working on " << it->get_name() << endl;
		CKripke* next = cur->next_ckripke(*it);
		if (next != NULL) {
			if (next->satisfy()) {
				cout << "Congratulation. Goal found!..... :)" << endl;
				return next;
			} else {
				//cout << "size of path is " << next->get_path().size() << endl;
				queue->push(next);
			}
		}
	}
	if (queue->size() != 0) {
		CKripke* n = queue->front();
		queue->pop();
		out = search(n, queue);
		return out;
	} else {
		return NULL;
	}
}

/* search with heuristic */

CKripke* Planner::searchwithheur()
{
	CKripke* out;
	CKripke* ck = new CKripke(this);
	ck->add_path("start");
	vector<Kripke>::iterator itk;
	for (itk = m_init_cstates.begin(); itk != m_init_cstates.end(); itk++) {
		Kripke* k = new Kripke(&*itk);
		ck->add_kripke(k);
	}

	//Kripke* k = new Kripke(&m_init_cstate);
	//ck->set_result(k);
	if (ck->satisfy()) {
		cout << "Congratulation. Goal found!..... :)" << endl;
		return ck;
	}
	Priority_Queue* queue = new Priority_Queue(this);
	queue->insert(ck);
	if (queue->front != NULL) {
		CKripke* n = queue->pop();
		if (debug == true) {
			vector<string> fordebug = n->get_path();
			for (vector<string>::iterator i = fordebug.begin(); i != fordebug.end(); i++) {
				cout << *i << "-";
			}
			cout << endl;
		}
		out = searchwithheur(n, queue);

		return out;
	} else {
		return NULL;
	}
	//CKripke* n = queue->pop();
	//out = searchwithheur(n, queue);
	//return out;
}

CKripke* Planner::searchwithheur(CKripke* cur, Priority_Queue* queue)
{
	KActionList::iterator it;
	CKripke* out;
	for (it = ontic.begin(); it != ontic.end(); it++) {
		CKripke* next = cur->next_ckripke(*it);
		if (next != NULL) {
			if (next->satisfy()) {
				cout << "Congratulation. Goal found!..... :)" << endl;
				return next;
			} else {
				//cout << "size of path is " << next->get_path().size() << endl;
				bool tiep;
				if (queue->front == NULL) {
					tiep = true;
				} else {
					tiep = false;
				}
				queue->insert(next);
			}
		}
	}
	if (queue->front != NULL) {
		CKripke* n = queue->pop();
		if (debug) {
			vector<string> p = n->get_path();
			for (vector<string>::iterator ip = p.begin(); ip != p.end(); ip++) {
				cout << *ip;
			}
			cout << endl;
		}
		out = searchwithheur(n, queue);
		return out;
	} else {
		return NULL;
	}
	//CKripke* n = queue->pop();
	//if (n == NULL) {
	//	return NULL;
	//}
	//out = searchwithheur(n, queue);
	//return out;
}
//---------------------

bool Planner::build_domain()
{
	int i;
	StringList::const_iterator itf;
	typedef map<string, Literal>::value_type value_type;
	typedef map<string, Agent>::value_type value_type2;
	PropositionList::const_iterator it_prop;
	KAction* k_act;
	StaticLaw stat;
	Literals hd, bd;

	cout << "Building agent list..." << endl;
	StringList::iterator ita;
	i = 0;
	for (ita = m_reader->m_agents.begin(); ita != m_reader->m_agents.end(); ita++) {
		a_map.insert(value_type2(*ita, i));
		m_agents.insert(i++);
	}


	cout << "Building fluent literals" << endl;
	// build fluent literals;
	i = 0;
	for (itf = m_reader->m_fluents.begin();
		itf != m_reader->m_fluents.end(); itf++) {
		m_map.insert(value_type(*itf, i));
		m_literals.insert(i++);
		if (debug) {
			cout << "Print literal " << *itf << " is " << (i - 1) << endl;
		}
		m_map.insert(value_type(NEGATION_SYMBOL + *itf, i));
		m_literals.insert(i++);

	}

	// build action list
	cout << "Building action list" << endl;
	for (it_prop = m_reader->m_propositions.begin();
		it_prop != m_reader->m_propositions.end(); it_prop++) {
		hd = ground((*it_prop).get_effect());
		bd = ground((*it_prop).get_precondition());

		BFNode b;
		switch ( (*it_prop).n_type ) {
		case STATIC: //tiep: check this if still valid or not
			stat.set_head(hd);
			stat.set_body(bd);
			m_statics.push_back(stat);
			break;
		case DYNAMIC:
			k_act = add_ontic((*it_prop).act_name, DYNAMIC);
			b = (*it_prop).bel_form;
			update_node(&b);
			k_act->add_effect(ground(&(it_prop->effect)), b);
			break;
		case EXECUTABILITY:
			k_act = add_ontic((*it_prop).act_name, DYNAMIC);
			b = (*it_prop).bel_form;
			update_node(&b);
			k_act->add_execs(b);
			break;


			//    case IMPOSSIBILITY:
			//      //cout << "Adding action "<< (*it_prop).act_name << " to ontic (Impossibility)" << endl;
			//      //p_act = add_ontic((*it_prop).act_name);		//Ben: add_action -> add_ontic
			//      //p_act->add_imposs(bd);
			//      break;


		case DETERMINATION:
			k_act = add_ontic((*it_prop).act_name, DETERMINATION);
			k_act->add_sensedFluents(ground(&(it_prop->effect)));
			break;
		case ANNOUNCEMENT: //Ben
			k_act = add_ontic((*it_prop).act_name, ANNOUNCEMENT);
			k_act->add_ann_ff(*convert(it_prop->flu_form));
			//k_act->add_agent(-1);
			break;
		case OBSERVANCE:
			k_act = add_ontic((*it_prop).act_name, IMPOSSIBILITY);
			k_act->add_observes(a_map.find((*it_prop).agentPro)->second, *convert((*it_prop).flu_form));
			//k_act->add_cond(convert((*it_prop).flu_form));
			break;
		case AWARENESS:
			k_act = add_ontic((*it_prop).act_name, IMPOSSIBILITY);
			k_act->add_awares(a_map.find((*it_prop).agentPro)->second, *convert((*it_prop).flu_form));
			break;
		default:
			break;
		}
	}
	cout << "Building InitRequire class" << endl;
	Nodes::const_iterator itn;
	ini = InitRequire();
	FluentFormula* phi1 = new FluentFormula;
	FluentFormula* phi2 = new FluentFormula;
	bool firstCheckFor12 = true;
	bool firstCheckFor13 = true;

	for (itn = m_reader->k_init.begin(); itn != m_reader->k_init.end(); itn++) {
		if (itn->node_type == fluForm) //initially phi
		{
			if ((itn->flu_form).size() == 0) {
				cout << "-------ERROR: INITIAL DECLARATION INCONSISTENT------- " << endl;
				exit(1);
			}
			if (firstCheckFor12) {
				phi1 = convert(itn->flu_form);
				firstCheckFor12 = false;
			} else {
				phi1 = and2ff(phi1, convert(itn->flu_form));
			}
			continue;
		}


		if (itn->node_type == CForm && itn->bfnode1->node_type == fluForm) //initially C(phi)
		{
			if (firstCheckFor13) {
				phi2 = convert(itn->bfnode1->flu_form);
				firstCheckFor13 = false;
			} else {
				phi2 = and2ff(phi2, convert(itn->bfnode1->flu_form));
			}
			continue;
		}
		/* Deleted because new version of initial state
		if(itn->node_type == CForm && itn->bfnode1->node_type == BForm) //initially C(B(i,phi))
		{
		if(a_map.find(itn->bfnode1->agentPro) == a_map.end())
		{
		cout << "-------ERROR: AGENT NOT DECLARED------- " << endl;
		exit(1);
		}

		Agent ag = a_map.find(itn->bfnode1->agentPro)->second;
		FluentFormula ff = *(convert(itn->bfnode1->bfnode1->flu_form));
		ini.add_correct(ag,ff);
		continue;
		}*/

		if (itn->node_type == CForm && itn->bfnode1->node_type == BForm) //initially C(B(i,phi))
		{
			if (a_map.find(itn->bfnode1->agentPro) == a_map.end()) {
				cout << "-------ERROR: AGENT NOT DECLARED------- " << endl;
				exit(1);
			}

			//Tiep Le: comment below for combine C(B(i,phi)) with C(phi) since they should be considered the same
			//            Agent ag = a_map.find(itn->bfnode1->agentPro)->second;
			//            FluentFormula ff = *(convert(itn->bfnode1->bfnode1->flu_form));
			//            ini.add_correct(ag,ff);
			//            continue;
			//Tiep Le: fix for combine C(B(i,phi)) with C(phi) since they should be considered the same
			if (firstCheckFor13) {
				phi2 = convert(itn->bfnode1->bfnode1->flu_form);
				firstCheckFor13 = false;
			} else {
				phi2 = and2ff(phi2, convert(itn->bfnode1->bfnode1->flu_form));
			}
			continue;
		}
		if (itn->node_type == CForm && itn->bfnode1->node_type == propForm
			&& itn->bfnode1->bfOperator == BFOR) //initially C(B(i,phi) \/ B(i,-phi))
		{
			if (a_map.find(itn->bfnode1->bfnode1->agentPro) == a_map.end()) {
				cout << "-------ERROR: AGENT NOT DECLARED------- " << endl;
				exit(1);
			}

			Agent ag = a_map.find(itn->bfnode1->bfnode1->agentPro)->second;
			FluentFormula ff = *(convert(itn->bfnode1->bfnode1->bfnode1->flu_form));
			ini.add_same(ag, ff);
			continue;
		}
	}
	ini.pointed = *phi1;
	ini.musthave = *phi2;


	return true;
}

void Planner::print_read_init()
{
	FluentFormula pointed = ini.pointed;
	FluentFormula musthave = ini.musthave;
	Agent_FF_list correctintepret = ini.correctintepret;
	Agent_FF_list sameintepret = ini.sameintepret;
	cout << "----------print_read_init--------------" << endl;
	cout << "printing pointed" << endl;
	print_fluform(pointed);
	cout << "print musthave" << endl;
	print_fluform(musthave);
	Agent_FF_list::iterator it;
	cout << "print correct" << endl;
	for (it = correctintepret.begin(); it != correctintepret.end(); it++) {
		Agent ag = it->get_head();
		FluentFormula ff = it->get_body();
		cout << ag << " ---> ";
		print_fluform(ff);
	}
	cout << "print same" << endl;
	for (it = sameintepret.begin(); it != sameintepret.end(); it++) {
		Agent ag = it->get_head();
		FluentFormula ff = it->get_body();
		cout << ag << " ---> ";
		print_fluform(ff);
	}
}

bool Planner::build_goal()
{
	Nodes::iterator it;
	Literals st;

	// build the goal
	//int n = 0;
	for (it = (m_reader->m_gd).begin(); it != (m_reader->m_gd).end(); it++) {
		//cout << "goal " << n++ << endl;
		update_node(&(*it));
		m_gd.push_back(*it);
	}
	return true;
}

bool Planner::build_init_k()
{
	m_init_cstate = Kripke();
	Literals::iterator it_lits;
	States::iterator it_st;
	States::iterator it_st2;
	AgentSet::iterator it_ag;

	/*********** States Initialization **************/

	int litSize = m_literals.size() / 2;
	int stateSize = (int) pow(2.0, litSize);
	States* sl = new States;
	Kstate* s;
	for (int i = 0; i < stateSize; i++) { //states id from 0
		s = new Kstate();
		s->set_id(i);
		sl->push_back(*s);
	}

	int ids = 0;
	vector<int> pId = vector<int>();
	States* usedSl = new States;
	for (it_st = sl->begin(); it_st != sl->end(); it_st++) {
		Kstate* temp = &(*it_st);
		computeLiteral(temp);
		if (ini.musthave.size() == 0 || temp->entail_fluform(ini.musthave)) { //add only states that satisfies C(phi)
			temp->set_id(ids);
			usedSl->push_back(*temp);
			ids++;
		}
		if (temp->entail_fluform(ini.pointed)) //set pointed states if this satisfies phi
		{
			pId.push_back(temp->get_id());
		}
	}
	delete sl;
	m_init_cstate.set_stateslist(*usedSl);
	m_init_cstate.set_pId(*pId.begin());
	int edgeId = 0;

	/*********** Edges Initialization **************/

	for (it_st = usedSl->begin(); it_st != usedSl->end(); it_st++) {
		for (it_st2 = usedSl->begin(); it_st2 != usedSl->end(); it_st2++) {
			for (it_ag = m_agents.begin(); it_ag != m_agents.end(); it_ag++) {
				Agent_FF_list::iterator itaf;
				bool shouldadd = true;
				//Tiep Le: comment below since we do not check for initially C(B(i,phi)) anymore. It should be considered in C(phi).
				//                    Agent_FF_list tempcorrect = ini.correctintepret;
				//                    for(itaf = tempcorrect.begin(); itaf != tempcorrect.end(); itaf++)
				//                    {
				//                        int checkingag = itaf->get_head();
				//                        if(checkingag != *it_ag){
				//                            continue;
				//                        }
				//                        FluentFormula checkingff = itaf->get_body();
				//                        if(!it_st->entail_fluform(checkingff) || !it_st2->entail_fluform(checkingff))
				//                        {
				//                            shouldadd = false;
				//                            break;
				//                        }
				//                    }
				//                    if(!shouldadd){
				//                        shouldadd = true;
				//                        continue;
				//                    }
				for (itaf = ini.sameintepret.begin(); itaf != ini.sameintepret.end(); itaf++) {
					int checkingag = itaf->get_head();
					if (checkingag != *it_ag) {
						continue;
					}
					FluentFormula checkingff = itaf->get_body();
					if (!isSameSatisfyFF(*it_st, *it_st2, checkingff)) {
						shouldadd = false;
						break;
					}
				}
				if (!shouldadd) {
					shouldadd = true;
					continue;
				}
				//add this edge
				Kedge* newEd = new Kedge();
				int fromnew = it_st->get_id();
				newEd->set_from(fromnew);
				int tonew = it_st2->get_id();
				newEd->set_to(tonew);
				newEd->set_id(edgeId);
				newEd->set_label(*it_ag);
				m_init_cstate.add_edge(*newEd);
				//m_init_cstate.add_edge2state(fromnew, edgeId, true); //do NOT need this any more because they are implemented within add_edge
				//m_init_cstate.add_edge2state(tonew, edgeId, false);
				edgeId++;
			}
		}
	}

	//generate a set of kripke for initial e-state
	m_init_cstates = KripkeList();
	vector<int>::iterator iterPId;
	for (iterPId = pId.begin(); iterPId != pId.end(); iterPId++) {
		m_init_cstate.set_pId(*iterPId);
		Kripke m = Kripke(&m_init_cstate);
		m.set_pId(*iterPId);
		m_init_cstates.push_back(m);
	}

	return true;
}

FluentFormula* Planner::and2ff(FluentFormula* flu1, FluentFormula* flu2)
{
	FluentFormula::iterator it1;
	FluentFormula::iterator it2;
	Fluents ns;
	FluentFormula* output = new FluentFormula;

	for (it2 = flu1->begin(); it2 != flu1->end(); it2++) {
		for (it1 = flu2->begin(); it1 != flu2->end(); it1++) {
			if (is_consistent(*it1, *it2)) {
				ns = *it2;
				ns.insert(it1->begin(), it1->end());
				output->insert(ns);
			}
		}
	}

	return output;
}

bool Planner::is_consistent(Fluents f1, Fluents f2)
{
	Fluents::iterator it;
	Fluent nl;

	for (it = f2.begin(); it != f2.end(); it++) {
		if ((*it) % 2 == 0) {
			nl = (*it) + 1;
		} else {
			nl = (*it) - 1;
		}

		if (f1.find(nl) != f1.end())
			return false;
	}

	return true;
}

void Planner::computeLiteral(Kstate* s)
{
	int at = m_literals.size() - 2;
	int div, rem;
	div = s->get_id();
	Literals* lits = new Literals;
	while (at >= 0) {
		rem = div % 2;
		lits->insert(at + rem);
		div = div / 2;
		at = at - 2;
	}
	s->set_lits(*lits);

}

FluentFormula* Planner::convert(StringList2 list)
{
	StringList2::iterator it4;
	StringList::iterator it5;
	unsigned short k;
	Fluents temp;
	FluentFormula* output = new FluentFormula;

	for (it4 = list.begin(); it4 != list.end(); it4++) {
		for (it5 = it4->begin(); it5 != it4->end(); it5++) {
			k = m_map.find(*it5)->second;
			temp.insert(k);
		}

		output->insert(temp);
		temp.clear();
	}
	return output;
}




// grounding functions

Literals Planner::ground(const StringList* x) const
{
	StringList::iterator it;
	Literals y;

	for (it = x->begin(); it != x->end(); it++) {
		y.insert(ground(*it));
	}

	return y;
}

Literal Planner::ground(const string& x) const
{
	map<string, Literal>::const_iterator p = m_map.find(x);

	if (p != m_map.end()) {
		return(p->second);
	}

	cout << "ERROR: Literal " << x << " is undeclared." << endl;
	cout << "Check the fluent declaration." << endl << endl;

	exit(1);
}

StringList Planner::convert(const Literals& x) const
{
	StringList y;
	Literals::iterator it;

	for (it = x.begin(); it != x.end(); it++) {
		y.insert(convert(*it));
	}

	return y;
}

string Planner::convert(const Literal& x) const
{
	unsigned short int i = x / 2;
	StringList::iterator it;

	if (i >= m_reader->m_fluents.size())
		return NULL;

	it = m_reader->m_fluents.begin();

	for (i = 0; i < x / 2; i++) {
		it++;
	}

	if (x % 2 == 0)
		return *it;

	return(NEGATION_SYMBOL + (*it));
}

Agents Planner::convert(StringList list)
{
	Agents output;
	StringList::iterator it;

	for (it = list.begin(); it != list.end(); it++) {
		output.push_back(a_map.find(*it)->second);
	}

	return output;
}

/* printing functions */
void Planner::print(const Literal& l) const
{
	cout << convert(l);
}

void Planner::print(const Literals& x) const
{
	Literals::iterator it;
	bool comma = false;
	for (it = x.begin(); it != x.end(); it++) {
		if (comma)
			cout << ",";
		print(*it);
		comma = true;
	}
}

void Planner::print_interal_domain()
{
	KActionList::iterator it;


	StaticLaws::const_iterator its;

	cout << "Static Laws: " << endl;

	for (its = m_statics.begin(); its != m_statics.end(); ++its) {
		cout << "    ";
		print(*its->get_head());
		cout << " <- ";
		print(*its->get_body());
		cout << endl;
	}

	cout << "Ontic (normal) Actions" << endl;
	cout << "-------------------" << ontic.size() << "---------" << endl;
	for (it = ontic.begin(); it != ontic.end(); it++) {
		if (it->get_type() == DYNAMIC) {
			//print executable
			Nodes::const_iterator itn;
			cout << it->get_name() << " " << it->get_type() << endl;
			for (itn = it->get_execs()->begin(); itn != it->get_execs()->end(); itn++) {
				cout << it->get_name() << " is executable if ";
				itn->print(m_map, a_map);
				cout << endl;
			}
			//print action causes .... if ....
			Fluents_BF_list::const_iterator itfb;
			for (itfb = (*it->get_effects()).begin(); itfb != (*it->get_effects()).end(); itfb++) {
				cout << it->get_name() << " has effect ";
				print_literals(*itfb->get_head());
				cout << ", and has belief formula ";
				itfb->get_body()->print(m_map, a_map);
				//          cout << "agent is ";
				//          print_literals(it->get_agents());
				cout << endl;
			}
		} else if (it->get_type() == DETERMINATION) {
			cout << it->get_name() << " " << it->get_type() << " has sensed fluents ";
			Literals::iterator it1;
			Literals temp = it->get_sensedFluents();
			for (it1 = temp.begin(); it1 != temp.end(); it1++) {
				cout << *it1 << " ";
			}
		} else if (it->get_type() == ANNOUNCEMENT) {
			cout << it->get_name() << " " << it->get_type() << " has announce fluents ";
			print_fluform(*(it->get_ann_ff().begin()));
		}

		cout << endl;
	}





	//  // print init cstate
	//  cout <<  "INIT" << endl;
	//  cout <<  "----------------------------" << endl;
	//  m_init_cstate.print();
	//  cout << endl;
	//
	//  // print goal state
	//  cout <<  "GOAL " << endl;
	//  cout <<  "----------------------------" << endl;
	//  print(m_goal);
	//  cout << endl;

	// print statistics
	StringList2::const_iterator itst;
	unsigned int i = 0;

	cout << "STATISTICS " << endl;
	cout << "----------------------------" << endl;
	cout << "Total actions: " << sensing.size() + ann.size() + ontic.size() << endl;
	cout << "\tOntic actions: " << ontic.size() << endl;
	//cout << "\tSensing actions: " << sensing.size() << endl;
	//cout << "\tAnnouncement actions: " << ann.size() << endl;
	cout << "Total fluents: " << m_literals.size() / 2 << endl;
	cout << "Unknown fluents: " << endl;
	for (itst = m_reader->m_init.begin();
		itst != m_reader->m_init.end(); itst++) {
		cout << "\tState " << i++ << ": ";
		cout << m_literals.size() / 2 - (*itst).size();
		cout << endl;
	}
	cout << endl;
}


//void Planner::print_summary() const
//{
//  cout <<  "SUMMARY" << endl;
//  cout <<  "---------------------" << endl;
//  cout <<  "Number of fluents: " << m_literals.size()/2 << endl;
//  cout <<  "Number of actions: " << m_actions.size() << endl;
//  cout <<  "Number of static laws: " << m_statics.size() << endl;
//}

void Planner::print_statistics() const
{
	double total = m_timer->time(READ_TIMER) +
		m_timer->time(BUILD_TIMER) + m_timer->time(SEARCH_TIMER);

	cout << "STATISTICS" << endl;
	cout << "---------------------" << endl;
	printf("Total time: %.3f (sec) \n", total);
#ifdef PRINT_TIME_DETAILS
	printf("  Reading: %.3f (sec) [%.2f %%]\n",
		m_timer->time(READ_TIMER),
		100.0 * m_timer->time(READ_TIMER) / total);
	printf("  Preprocessing: %.3f (sec) [%.2f %%]\n",
		m_timer->time(BUILD_TIMER),
		100.0 * m_timer->time(BUILD_TIMER) / total);
	printf("  Search: %.3f (sec) [%.2f %%]\n",
		m_timer->time(SEARCH_TIMER),
		100.0 * m_timer->time(SEARCH_TIMER) / total);
	if (results_file) {
		std::ofstream result;
		std::string folder = "out/EFP_comparison/";
		if (givenplan) {
			folder = folder + "givenplan/";
		} else {
			folder = folder + "findingplan/";
		}
		result.open(folder + m_domain_name + ".txt", std::ofstream::out | std::ofstream::app);
		result << "EFP Version 1.0 (on KRIPKE with STRANDARD transition function) completed the search in " << m_timer->time(SEARCH_TIMER) << "\n";
		result.close();
	}
#endif
	//
	//  printf("Total states allocated: %d\n", m_states.size());
	//  printf("Total cstate(s): %d\n", m_cstates.size());
	//  printf("Total cstate(s) remaining in the queue: %d\n", m_queue.size());
}

KAction* Planner::add_ontic(const string str, PropositionType pt)
{
	map<string, int>::iterator it;
	KAction a(this, str, pt);

	it = m_action_map.find(str);

	if (it == m_action_map.end()) {
		// create a new entry
		ontic.push_back(a);
		m_action_map.insert(map<string, int>::value_type(str, ontic.size() - 1));
		return &(ontic[ontic.size() - 1]);
	}
	if (pt != IMPOSSIBILITY) {
		ontic[it->second].set_type(pt);
	}

	return &(ontic[it->second]);
}

Literals Planner::negate(const Literals& x) const
{
	Literals::const_iterator it;
	Literals y; // output literals
	Literal l;

	for (it = x.begin(); it != x.end(); ++it) {
		NEGATE(*it, l);
		y.insert(l);
	}
	return y;
}

void Planner::print_fluform(StringList2 in)
{
	StringList2::const_iterator it1;
	StringList::const_iterator it2;
	cout << "[";
	for (it1 = in.begin(); it1 != in.end(); it1++) {
		cout << "[";
		for (it2 = it1->begin(); it2 != it1->end(); it2++) {
			cout << *it2 << " ";
		}
		cout << "]";
	}
	cout << "]" << endl;
}

void Planner::print_fluform(FluentFormula in)
{
	FluentFormula::iterator it1;
	Fluents::iterator it2;
	cout << "[";
	for (it1 = in.begin(); it1 != in.end(); it1++) {
		cout << "[";
		for (it2 = it1->begin(); it2 != it1->end(); it2++) {
			cout << *it2 << " ";
		}
		cout << "]";
	}
	cout << "]" << endl;
}

void Planner::print_literals(const Literals& ls) const
{
	Literals::iterator it;
	bool begin = true;
	for (it = ls.begin(); it != ls.end(); it++) {
		if (begin) {
			cout << *it;
			begin = false;
		} else {
			cout << "," << *it;
		}
	}
	//cout << endl;
}

void Planner::print_ontic(KAction* k)
{

}

bool Planner::check_Agents(Agent ag, Agents list)
{
	Agents::iterator it;

	for (it = list.begin(); it != list.end(); it++)
		if (ag == *it)
			return true;

	return false;
}

bool Planner::is_executable(Kripke k, KAction a) const
{
	Nodes::const_iterator it;
	for (it = a.get_execs()->begin(); it != a.get_execs()->end(); it++) {
		if (!k.entail(*it)) {
			return false;
		}
	}
	return true;
}

bool Planner::is_executable1(Kripke k, KAction a) const
{
	Nodes::const_iterator it;
	for (it = a.get_execs()->begin(); it != a.get_execs()->end(); it++) {
		if (!k.entail1(*it)) {
			return false;
		}
	}
	return true;
}


//%%%%%%%%%%%---------- Execute announcement action-----------------%%%%%%%%%%%%%

Kripke* Planner::execute_ann(Kripke k, KAction a)
{
	int oldpId = k.get_id();

	//check executable
	if (!is_executable(k, a)) {
		return NULL;
	}
	//check special excutable condition for announce action.
	if (!k.entail_ff_list(a.get_ann_ff())) {
		return NULL;
	}


	AgentSet fd = get_fd(a, k);
	AgentSet od = get_od(a, k);
	AgentSet pd = get_pd(a, k);


	/*****************Correction of false beliefs**********************/
	//We check all the fully observant
	AgentSet::iterator it_fd;
	list<FluentFormula>::iterator it_ff;

	//-----------
	//Add a copy of the list because a.get_ann_ff() is a const list
	list<FluentFormula> copyList = a.get_ann_ff();

	Edges to_add;
	int id_edge = k.get_edgeslist().size();

	for (it_fd = fd.begin(); it_fd != fd.end(); it_fd++) {
		for (it_ff = copyList.begin(); it_ff != copyList.end(); it_ff++) {
			k.correctFalseBeliefs(*it_ff, *it_fd, id_edge, to_add);
		}
	}

	//Add all the found edges
	Edges::iterator it_ed;
	for (it_ed = to_add.begin(); it_ed != to_add.end(); it_ed++) {
		k.add_edge(*it_ed);
	}

	/******************************************************************/



	AgentSet::iterator itera;

	/*Assume a is announce phi
	the list of added edges
	for each agent k in fd:
	if Kripke entails B_k -phi:
	Yes: compute ajacent matrix M_1k = M_2k (2 copies)
	update M_1_k with reversing edges
	update M_1_k with transitive closure
	compare M_1_k with M_2_k, if new edge -> create an Kedge object -> add this to list of added edges

	for each edge e in list of added edges, kripke->add_edge(e)
	 */


	/*
	for(itera = fd.begin();itera != fd.end(); itera++)
	{
	cout << "fd: " << *itera;
	}
	cout << endl;
	for(itera = pd.begin();itera != pd.end(); itera++)
	{
	cout << "pd: " << *itera;
	}
	cout << endl;
	for(itera = od.begin();itera != od.end(); itera++)
	{
	cout << "od: " << *itera;
	}
	cout << endl;
	 */
	Kripke* m = new Kripke(&k);
	States remS; //remaining states
	States sl = k.get_stateslist();
	States::iterator it;
	set<int> inRes; // set of id of remaining states
	for (it = sl.begin(); it != sl.end(); it++) {
		/*
		m->set_pId(it->get_id());
		if(it->get_id()==oldpId)
		{
		if(is_executable(*m, a)){
		Kstate* goodstate = (*it).copy();
		remS.push_back(*goodstate);
		inRes.insert(goodstate->get_id());
		}
		}else{
		if(is_executable1(*m, a)){
		Kstate* goodstate = (*it).copy();
		remS.push_back(*goodstate);
		inRes.insert(goodstate->get_id());
		}
		}
		 */

		Kstate std = m->get_stateslist().at(oldpId);
		Kstate check = m->get_stateslist().at(it->get_id());
		//Note: consider announce action appear at most one time in the form of a announce phi. so check only the first fluent formula
		if (isSameSatisfyFF(std, check, *(a.get_ann_ff().begin()))) {
			Kstate* goodstate = (*it).copy();
			remS.push_back(*goodstate);
			inRes.insert(goodstate->get_id());
		}

	}

	Edges el = k.get_edgeslist();
	Edges remE;
	Edges::iterator it1;
	for (it1 = el.begin(); it1 != el.end(); it1++) {
		Kedge e = *it1;
		int from = it1->get_from();
		int to = it1->get_to();
		int label = it1->get_label();
		if (od.find(label) != od.end()) {
			continue; //remove link relating to i in od;
		}
		if (inRes.find(from) == inRes.end() || inRes.find(to) == inRes.end()) {
			continue;
		}
		if (fd.find(label) != fd.end()) { // label i in fd
			//check different in aggreing on announced fluent formulas.
			//consider announce action appear at most one time in the form of a announce phi. so check only the first fluent formula
			if (!isSameSatisfyFF(sl.at(from), sl.at(to), *(a.get_ann_ff().begin()))) {
				continue;
			}
		}
		Kedge* addede = new Kedge(&e);
		remE.push_back(addede);
	}

	Kripke res;
	res.set_stateslist(remS);
	res.set_edgeslist(remE);
	res.set_pId(oldpId);
	delete m;


	Kripke* m1 = new Kripke(&k);
	map<int, int>* savedResS = new map<int, int>;
	merge_kripkes(m1, &res, savedResS);

	//add edges between 2 kripke based on od
	Edges edgek = k.get_edgeslist();
	int start = m1->get_edgeslist().size();
	for (it1 = edgek.begin(); it1 != edgek.end(); it1++) {
		int idek = it1->get_label();
		if (od.find(idek) == od.end()) {
			continue;
		}
		Kedge* newEd = new Kedge();

		if (savedResS->find(it1->get_from()) == savedResS->end()) {
			continue;
		}
		int fromnew = savedResS->find(it1->get_from())->second;
		newEd->set_from(fromnew);
		int tonew = it1->get_to();
		newEd->set_to(tonew);
		newEd->set_id(start);
		newEd->set_label(it1->get_label());

		m1->add_edge2state(fromnew, start, true);
		m1->add_edge2state(tonew, start, false);
		m1->add_to_edges_list(*newEd);

		start = start + 1;
	}
	//cout << "chan " << endl;
	//m1->print_report();
	Kripke* m2 = update_kripke(m1);
	/*
	if(!(m2->isValid(m_agents)))
	{
	return NULL;
	}
	 */
	return m2;
}

//%%%%%%%%%%%---------- Execute sensing action-----------------%%%%%%%%%%%%%

Kripke* Planner::execute_sensing(Kripke k, KAction a)
{
	int oldpId = k.get_id();

	if (!is_executable(k, a)) {
		return NULL;
	}

	AgentSet fd = get_fd(a, k);
	AgentSet od = get_od(a, k);
	AgentSet pd = get_pd(a, k);

	/*
	AgentSet::iterator itera;
	for(itera = fd.begin();itera != fd.end(); itera++)
	{
	cout << "fd: " << *itera;
	}
	cout << endl;
	for(itera = pd.begin();itera != pd.end(); itera++)
	{
	cout << "pd: " << *itera;
	}
	cout << endl;
	for(itera = od.begin();itera != od.end(); itera++)
	{
	cout << "od: " << *itera;
	}
	cout << endl;
	 */


	Literals sensedFlu = a.get_sensedFluents();

	/*****************Correction of false beliefs**********************/

	//We check all the fully observant
	AgentSet::iterator it_fd;
	Literals::const_iterator it_li;

	//Transform into FluentsFormula single fluent
	list<FluentFormula> toEntail;

	for (it_li = sensedFlu.begin(); it_li != sensedFlu.end(); it_li++) {
		Fluents tmp;
		tmp.insert(*it_li);
		FluentFormula ff;
		ff.insert(tmp);
		toEntail.push_back(ff);
	}

	list<FluentFormula>::iterator it_lff;

	Edges to_add;
	int id_edge = k.get_edgeslist().size();

	for (it_fd = fd.begin(); it_fd != fd.end(); it_fd++) {
		for (it_lff = toEntail.begin(); it_lff != toEntail.end(); it_lff++) {
			k.correctFalseBeliefs(*it_lff, *it_fd, id_edge, to_add);
		}
	}

	//Add all the found edges
	Edges::iterator it_ed;
	for (it_ed = to_add.begin(); it_ed != to_add.end(); it_ed++) {
		k.add_edge(*it_ed);
	}

	/******************************************************************/

	Kripke* m = new Kripke(&k);
	States remS; //remaining states
	States sl = k.get_stateslist();
	States::iterator it;
	set<int> inRes; // set of id of remaining states
	for (it = sl.begin(); it != sl.end(); it++) {

		m->set_pId(it->get_id());
		if (it->get_id() == oldpId) {
			if (is_executable(*m, a)) {
				Kstate* goodstate = (*it).copy();
				remS.push_back(*goodstate);
				inRes.insert(goodstate->get_id());
			}
		} else {
			if (is_executable1(*m, a)) {
				Kstate* goodstate = (*it).copy();
				remS.push_back(*goodstate);
				inRes.insert(goodstate->get_id());
			}
		}

		/*
		Kstate std = m->get_stateslist().at(oldpId);
		Kstate check = m->get_stateslist().at(it->get_id());
		bool ent = std.entail_lits(sensedFlu);
		if(ent == check.entail_lits(sensedFlu))
		{
		Kstate* goodstate = (*it).copy();
		remS.push_back(*goodstate);
		inRes.insert(goodstate->get_id());
		}
		 */
	}

	Edges el = k.get_edgeslist();
	Edges remE;
	Edges::iterator it1;
	for (it1 = el.begin(); it1 != el.end(); it1++) {
		Kedge e = *it1;
		int from = it1->get_from();
		int to = it1->get_to();
		int label = it1->get_label();
		if (od.find(label) != od.end()) {
			continue; //remove link relating to i in od;
		}
		if (inRes.find(from) == inRes.end() || inRes.find(to) == inRes.end()) {
			continue;
		}
		if (fd.find(label) != fd.end()) { // label i in fd
			if (!isSameInterpret(sl.at(from), sl.at(to), sensedFlu)) //different interpretation about sensedfluent
			{
				continue;
			}
		}
		Kedge* addede = new Kedge(&e);
		remE.push_back(addede);
	}
	Kripke res;
	res.set_stateslist(remS);
	res.set_edgeslist(remE);
	res.set_pId(oldpId);
	delete m;


	Kripke* m1 = new Kripke(&k);

	map<int, int>* savedResS = new map<int, int>;
	merge_kripkes(m1, &res, savedResS);

	//add edges between 2 kripke based on od
	Edges edgek = k.get_edgeslist();
	int start = m1->get_edgeslist().size();
	for (it1 = edgek.begin(); it1 != edgek.end(); it1++) {
		int idek = it1->get_label();
		if (od.find(idek) == od.end()) {
			continue;
		}
		Kedge* newEd = new Kedge();

		if (savedResS->find(it1->get_from()) == savedResS->end()) {
			continue;
		}
		int fromnew = savedResS->find(it1->get_from())->second;
		newEd->set_from(fromnew);
		int tonew = it1->get_to();
		newEd->set_to(tonew);
		newEd->set_id(start);
		newEd->set_label(it1->get_label());

		m1->add_edge2state(fromnew, start, true);
		m1->add_edge2state(tonew, start, false);
		m1->add_to_edges_list(*newEd);
		start = start + 1;
	}

	Kripke* m2 = update_kripke(m1);
	/*
	if(!(m2->isValid(m_agents)))
	{
	return NULL;
	}
	 */
	return m2;


}

bool Planner::isSameSatisfyFF(Kstate k1, Kstate k2, FluentFormula check) const
{
	if (k1.entail_fluform(check) && !k2.entail_fluform(check)) {
		return false;
	} else if (!k1.entail_fluform(check) && k2.entail_fluform(check)) {
		return false;
	}
	return true;
}

bool Planner::isSameInterpret(Kstate k1, Kstate k2, Literals check) const
{
	Literals::iterator it;
	for (it = check.begin(); it != check.end(); it++) {
		if (k1.entail_lit(*it) && !k2.entail_lit(*it)) {
			return false;
		} else if (!k1.entail_lit(*it) && k2.entail_lit(*it)) {
			return false;
		}
	}
	return true;
}

Kripke* Planner::execute_ontic(Kripke k, KAction a)
{
	int oldpId = k.get_id();

	if (!is_executable(k, a)) {
		return NULL;
	}

	//cout << "begin computing fd, pd, od" << endl;
	AgentSet fd = get_fd(a, k);
	AgentSet pd = get_pd(a, k);
	AgentSet od = get_od(a, k);

	/*
	AgentSet::iterator itera;
	for(itera = fd.begin();itera != fd.end(); itera++)
	{
	cout << "-----fd: " << *itera;
	}
	cout << endl;
	for(itera = pd.begin();itera != pd.end(); itera++)
	{
	cout << "----pd: " << *itera;
	}
	cout << endl;
	for(itera = od.begin();itera != od.end(); itera++)
	{
	cout << "----od: " << *itera;
	}
	cout << endl;
	 */

	Kripke* m = new Kripke(&k);


	//compute Res(a,M,s)
	Kripke res;

	//compute Res(a,M,s)[S] and Res(a,M,s)[pi]
	States resS;

	States sl = m->get_stateslist();
	States::iterator it;
	//cout << "begin computing Res(a,M,s)[S] and Res(a,M,s)[pi]" << endl;
	set<int> inRes;
	Literals lits = compute_effect(&a, &k);
	//int tiep = 0;
	//int exe = 0;
	for (it = sl.begin(); it != sl.end(); it++) {
		//cout << "states " << tiep++ << endl;
		Kstate u = *it;
		k.set_pId(u.get_id());
		if (is_executable(k, a)) //if a is executable in (k,u)
		{
			//cout << "state exe " << exe++ << endl;
			Kstate* ns = comp_nextstate(u, lits);
			resS.push_back(*ns);
			inRes.insert(ns->get_id());
		}
	}
	res.set_stateslist(resS);
	//compute Res(a,M,s)[i]
	Edges resI;

	//cout << "begin computing compute Res(a,M,s)[i]" << endl;

	Edges eds = m->get_edgeslist();
	Edges::iterator it1;
	for (it1 = eds.begin(); it1 != eds.end(); it1++) {
		if (fd.find(it1->get_label()) == fd.end()) //label not in Fd(a,M,s)
		{
			continue;
		}
		Kedge temp = *it1;
		Kedge* e = new Kedge(&temp);
		int from = e->get_from();
		int to = e->get_to();
		if (inRes.find(from) != inRes.end() && inRes.find(to) != inRes.end()) {
			resI.push_back(*e);
		}
	}
	res.set_edgeslist(resI);
	res.set_pId(oldpId);

	//cout << "begin computing compute final Kripke" << endl;
	map<int, int>* savedResS = new map<int, int>;
	merge_kripkes(m, &res, savedResS);
	Edges edgek = k.get_edgeslist();
	int start = m->get_edgeslist().size();
	for (it1 = edgek.begin(); it1 != edgek.end(); it1++) {
		int idek = it1->get_label();

		if (od.find(idek) == od.end()) {
			//cout << "id edege " << idek << endl;
			continue;
		}
		Kedge* newEd = new Kedge();

		if (savedResS->find(it1->get_from()) == savedResS->end()) {
			continue;
		}
		int fromnew = savedResS->find(it1->get_from())->second;
		newEd->set_from(fromnew);
		int tonew = it1->get_to();
		newEd->set_to(tonew);
		newEd->set_id(start);
		newEd->set_label(it1->get_label());

		m->add_edge2state(fromnew, start, true);
		m->add_edge2state(tonew, start, false);
		m->add_to_edges_list(*newEd);

		start = start + 1;
	}
	//Literals changed =
	//cout << "begin computing update_kripke" << endl;
	Kripke* m1 = update_kripke(*m);
	/*
	if(!(m1->isValid(m_agents)))
	{
	return NULL;
	}
	 */
	return m1;

}

bool Planner::check_lits(Literal a, Literals list)
{
	Literals::iterator it;

	for (it = list.begin(); it != list.end(); it++)
		if (a == *it)
			return true;

	return false;
}

//udpate_kripke removes edges and states that are not reachable from pointed and prepares the hashmap ``mapEdgelabelNextState" for each state

Kripke* Planner::update_kripke(Kripke k)
{
	Kripke* out = new Kripke();
	AgentSet ags = m_agents;
	Agents agl;
	AgentSet::iterator ita;
	for (ita = ags.begin(); ita != ags.end(); ita++) {
		agl.push_back(*ita);
	}
	StateList sl = k.reachableStates(k.get_id(), agl);
	sl.insert(k.get_id());
	if (sl.size() == k.get_stateslist().size()) {
		out = new Kripke();
		out->set_stateslist_fix_states(k.get_stateslist());
		out->set_edgeslist_fix_states(k.get_edgeslist());
		out->set_pId(k.get_id());
		return out;
	}
	States::iterator its;
	States slk = k.get_stateslist();
	States res;
	int id = 0;
	map<int, int> savedId;
	for (its = slk.begin(); its != slk.end(); its++) {
		int old = its->get_id();
		if (sl.find(old) != sl.end()) {
			Kstate* n = its->copy();
			n->set_id(id);
			savedId.insert(map<int, int>::value_type(old, id));
			id = id + 1;
			n->set_in(EdgeList());
			n->set_out(EdgeList());
			res.push_back(*n);
		}
	}
	out->set_stateslist(res);
	out->set_pId(savedId.find(k.get_id())->second);

	Edges edg;
	Edges edgold = k.get_edgeslist();
	Edges::iterator ite;
	int start = 0;
	for (ite = edgold.begin(); ite != edgold.end(); ite++) {
		int from = ite->get_from();
		int to = ite->get_to();


		if (sl.find(from) == sl.end() || sl.find(to) == sl.end()) {
			continue;
		}
		Kedge* newEd = new Kedge();

		int fromnew = savedId.find(from)->second;
		newEd->set_from(fromnew);
		int tonew = savedId.find(to)->second;
		newEd->set_to(tonew);
		newEd->set_id(start);
		newEd->set_label(ite->get_label());

		out->add_edge(*newEd);
		//out->add_edge2state(fromnew, start, true); // do NOT need this any more because they are implemented in add_edge
		//m->get_stateslist().at(fromnew).get_out().push_back(start);
		//out->add_edge2state(tonew, start, false);
		//m->get_stateslist().at(tonew).get_in().push_back(start);
		start = start + 1;
	}
	return out;
}

void Planner::update_node(BFNode* node)
{
	switch ( node->node_type ) {
	case fluForm: //base case
		//node->agentPro2 = a_map.find(node->agentPro)->second;
		node->fluform = convert(node->flu_form);
		break;

	case BForm:
		node->agentPro2 = a_map.find(node->agentPro)->second;
		update_node(node->bfnode1);
		break;

	case propForm:
		update_node(node->bfnode1);
		if (node->bfOperator == BFNOT || node->bfOperator == NONE) {
			break;
		}
		update_node(node->bfnode2);
		break;

	case EForm:
		node->agents = convert(node->groupAgent);
		update_node(node->bfnode1);
		break;

	case CForm:
		node->agents = convert(node->groupAgent);
		update_node(node->bfnode1);
		break;

	case BFEmpty:
		break;

	default:
		break;
	}
}

AgentSet Planner::get_fd(const KAction& act, Kripke k) const
{
	AgentSet out = AgentSet();
	Agent_FF_list::iterator ia;
	Agent_FF_list temp = act.get_m_observes();
	for (ia = temp.begin(); ia != temp.end(); ia++) {
		if (ia->get_body().size() == 0) {
			out.insert(ia->get_head());
			continue;
		}
		FluentFormula ff = ia->get_body();
		BFNode temp;
		temp.node_type = fluForm;
		temp.fluform = &ff;
		if (k.entail(temp)) {
			out.insert(ia->get_head());
		}
	}
	return out;
}

AgentSet Planner::get_pd(const KAction& act, Kripke k) const
{
	AgentSet out = AgentSet();
	Agent_FF_list::iterator ia;
	Agent_FF_list temp = act.get_m_awares();
	for (ia = temp.begin(); ia != temp.end(); ia++) {
		if (ia->get_body().size() == 0) {
			out.insert(ia->get_head());
			continue;
		}
		FluentFormula ff = ia->get_body();
		BFNode temp;
		temp.node_type = fluForm;
		temp.fluform = &ff;
		if (k.entail(temp)) {
			out.insert(ia->get_head());
		}
	}
	return out;
}

AgentSet Planner::get_od(const KAction& act, Kripke k) const
{
	AgentSet ags = m_agents;
	AgentSet pd = get_pd(act, k);
	AgentSet fd = get_fd(act, k);
	AgentSet::iterator it;
	if (pd.size() != 0) {
		for (it = pd.begin(); it != pd.end(); it++) {
			ags.erase(*it);
		}
	}
	if (fd.size() != 0) {
		for (it = fd.begin(); it != fd.end(); it++) {
			ags.erase(*it);
		}
	}
	return ags;
}
//////////////////////////

Literals Planner::compute_effect(KAction* a, Kripke* k)
{
	//cout << "computing the effects of " << a->get_name() << endl;
	Literals out;
	Fluents_BF_list::const_iterator it;
	Literals::const_iterator it2;

	for (it = a->get_effects()->begin(); it != a->get_effects()->end(); it++) {
		if (k->entail(*(it->get_body()))) {
			for (it2 = it->get_head()->begin();
				it2 != it->get_head()->end(); it2++) {
				out.insert(*it2);
			}
		}
	}
	//cout << "effect size is " << out.size() << endl;
	return out;
}

//required KAction is executable here already

Kstate* Planner::comp_nextstate(Kstate prev, Literals lits)
{
	Kstate* next = prev.copy();
	Literals lit = next->get_lits();
	Literals::const_iterator it;
	for (it = lits.begin(); it != lits.end(); it++) {
		if ((*it) % 2 == 1) // this literal is negation
		{
			lit.erase((*it) - 1);
			lit.insert(*it);
		} else { // this literal is not a negation
			lit.erase((*it) + 1);
			lit.insert(*it);
		}
	}
	next->set_lits(lit);
	return next;
}

Kripke* Planner::merge_kripkes(Kripke* k1, Kripke* k2, map<int, int>* saved)
{
	States sl = k1->get_stateslist();
	int start = sl.size();

	States* addingsl = new States;
	States::iterator it;
	States working = k2->get_stateslist();
	for (it = working.begin(); it != working.end(); it++) {
		Kstate k;
		k.set_lits(it->get_lits());
		k.set_in(it->get_in());
		k.set_out(it->get_out());
		int old = it->get_id();
		k.set_id(start);
		addingsl->push_back(k);
		saved->insert(map<int, int>::value_type(old, start));
		start = start + 1;
	}
	Edges edg = k1->get_edgeslist();
	map<int, int> savedEdges;
	start = edg.size();

	Edges::iterator it1;
	Edges edgworking = k2->get_edgeslist();
	for (it1 = edgworking.begin(); it1 != edgworking.end(); it1++) {
		Kedge e = *it1;
		int from = e.get_from();
		int to = e.get_to();
		map<int, int>::iterator it2;
		int fromnew = saved->find(from)->second;
		int tonew = saved->find(to)->second;

		e.set_from(fromnew);
		e.set_to(tonew);
		savedEdges.insert(map<int, int>::value_type(e.get_id(), start));
		e.set_id(start);
		edg.push_back(e);
		start = start + 1;
	}
	k1->set_edgeslist(edg);
	for (it = addingsl->begin(); it != addingsl->end(); it++) {
		EdgeList temp1 = it->get_in();
		temp1 = updateEdgesListId(temp1, savedEdges);
		it->set_in(temp1);
		temp1 = it->get_out();
		temp1 = updateEdgesListId(temp1, savedEdges);
		it->set_out(temp1);
		sl.push_back(*it);
	}
	k1->set_stateslist(sl);
	k1->set_pId(saved->find(k2->get_id())->second); //get the pId from pId of k2
	return k1;
}

EdgeList Planner::updateEdgesListId(EdgeList edgs, map<int, int> maps)
{
	EdgeList out;
	EdgeList::iterator it;
	for (it = edgs.begin(); it != edgs.end(); it++) {
		if (maps.find(*it) != maps.end()) {
			out.push_back(maps.find(*it)->second);
		}
	}
	return out;
}

Nodes Planner::get_goal()
{
	return m_gd;
}

/*******************
 CKripke class
 ******************/
CKripke::CKripke(Planner* pl)
{
	planner = pl;
	old = NULL;
	path = vector<string>();
	ks = vector<Kripke*>();
}

CKripke::~CKripke()
{

};

void CKripke::set_result(Kripke* ik)
{
	k = ik;
};

void CKripke::add_path(string name)
{
	path.push_back(name);
}

vector<Kripke*> CKripke::get_result()
{
	return ks;
}

vector<string> CKripke::get_path()
{
	return path;
}

bool CKripke::satisfy()
{
	Nodes::iterator it;
	vector<Kripke*>::iterator itk;
	for (itk = ks.begin(); itk != ks.end(); itk++) {
		for (it = planner->m_gd.begin(); it != planner->m_gd.end(); it++) {
			if (!(*itk)->entail(*it)) {
				return false;
			}
		}
	}
	return true;

}

int CKripke::lengthOfPlanningGraph() //return the maximal length of Planning Graphs; each of which is a PlanningGraph of a Kripkek in ks
{
	int out = 0;
	vector<Kripke*>::iterator itk;
	for (itk = ks.begin(); itk != ks.end(); itk++) {
		PlanningGraph pg = PlanningGraph(planner);
		//(*itk)->print_report();
		int l = pg.constructEPG(*itk);
		if (l == -1) {
			return -1; //one of kripke in vector<Kripke*> ks cannot get to the goal -> this CKripke should be dropped;
		} else {
			if (l > out) {
				out = l;
			}
		}
	}
	return out;

}
//return how many sub-goals are satisfied already

int CKripke::satisfywithprior()
{
	int out = 0;
	Nodes::iterator it;
	for (it = planner->m_gd.begin(); it != planner->m_gd.end(); it++) {
		if (k->entail(*it)) {
			out++;
		}
	}
	return out;
}

void CKripke::set_path(vector<string> st)
{
	path = st;
}

void CKripke::set_old(CKripke* o)
{
	old = o;
}

CKripke* CKripke::get_old()
{
	return old;
}

CKripke* CKripke::next_ckripke(KAction a)
{
	CKripke* n = new CKripke(planner);
	n->set_path(path);
	n->add_path(a.get_name());
	n->set_old(this);
	Kripke* nk;
	vector<Kripke*>::iterator itk;
	for (itk = ks.begin(); itk != ks.end(); itk++) {
		if (a.get_type() == DYNAMIC) {
			//cout << a.get_name() << " ontic" << endl;
			nk = planner->execute_ontic(*itk, a);
		} else if (a.get_type() == DETERMINATION) {
			//cout << a.get_name() << " sensing" << endl;
			nk = planner->execute_sensing(*itk, a);
		} else { //ANNOUNCEMENT
			//cout << a.get_name() << " announce" << endl;
			nk = planner->execute_ann(*itk, a);
		}
		if (nk == NULL) {
			return NULL; //is not executable
		} else {
			n->add_kripke(nk);
		}
	}
	/*
	vector<string>::iterator isol1;
	vector<string> pp = n->get_path();
	for(isol1=pp.begin();isol1!= pp.end();isol1++)
	{
	cout << *isol1 << " ";
	}
	cout << endl;
	 */
	return n;
}

void CKripke::add_kripke(Kripke* k)
{
	ks.push_back(k);
}
