//
//  planninggraph.cpp
//  
//
//  Created by Tiep Le on 11/15/17.
//

#include "planninggraph.h"
#include "kripke.h"
#include "action.h"
#include "planner.h"
#include <list>
#include <stdlib.h>
#include <map>

/*****************************************
 EpistemicModelsNode
 class for nodes in EpistemicModelsLists in PlanningGraph
 ******************************************/
EpistemicModelsNode::EpistemicModelsNode()
{
    kripke = new Kripke();
    pointeds = set<int>();
    action = "unassign";
}

EpistemicModelsNode::EpistemicModelsNode(Kripke* k, set<int> s)
{
    kripke = k;
    pointeds = s;
}

EpistemicModelsNode::~EpistemicModelsNode()
{
    
}

string EpistemicModelsNode::get_action(){
    return action;
}

void EpistemicModelsNode::set_action(string s){
    action = s;
}


void EpistemicModelsNode::set_Kripke(Kripke* k){
    kripke = k;
}

void EpistemicModelsNode::add_pointedID(int i){
    pointeds.insert(i);
}

Kripke* EpistemicModelsNode::get_Kripke(){
    return kripke;
}

set<int> EpistemicModelsNode::get_pointeds(){
    return pointeds;
}

void EpistemicModelsNode::print_info(){
    cout << "printing a EpistemicModelsNode..." << endl;
    cout << " THE KRIPKE STRUCTURE AFTER EXECUTING "<< action << " IS..." << endl;
    kripke->print_report();
    cout << "THE SET OF POINTED STATE ID is..." << endl;
    for(set<int>::iterator i = pointeds.begin(); i!= pointeds.end(); i++){
        cout << *i << " , " ;
    }
    cout << endl;
    cout << "------------------------------" << endl;
}

/*******************
 PGraphActionLevel
 class for action_list level in epistemic planning graph
 ******************/
PGraphActionLevel::PGraphActionLevel()
{
    actionVec = set<string>();
}

PGraphActionLevel::~PGraphActionLevel()
{
    
}

void PGraphActionLevel::set_ActionVec(set<string> actions){
    actionVec = actions;
}

void PGraphActionLevel::add_action(string a){
    actionVec.insert(a);
}

set<string> PGraphActionLevel::get_ActionVec(){
    return actionVec;
}

void PGraphActionLevel::print_info(){
    cout << "printing a PGraphActionLevel..." << endl;
    for(set<string>::iterator i = actionVec.begin(); i!=actionVec.end();i++){
        cout << "include action " << *i << endl;
    }
    
}
/*******************
 PGraphENodeLevel
 class for epistemic_model_list level in epistemic planning graph
 ******************/
/* constructor and destructor */
PGraphENodeLevel::PGraphENodeLevel()
{
    eNodes = vector<EpistemicModelsNode>();
}

PGraphENodeLevel::PGraphENodeLevel(vector<EpistemicModelsNode> eNs)
{
    eNodes = eNs;
}
PGraphENodeLevel::~PGraphENodeLevel()
{
    
}

//smart function
bool PGraphENodeLevel::emNodes_entailment(vector<EpistemicModelsNode> v, BFNode bf)
{
    vector<EpistemicModelsNode>::iterator iv;
    switch (bf.node_type)
    {
        case fluForm:
        {
            FluentFormula ff = *bf.fluform;
            //For example, assume ff = [ [1,2] , [3,4] ], i.e., (1 and 2) or (3 and 4)
            for(FluentFormula::iterator fi = ff.begin(); fi!=ff.end(); fi++){
                //Fluent Formula is in DNF form, so need one of *fi satisfied to finish
                // *fi = [1 ,2 ]
                bool andSatisfied = true;
                for(Fluents::iterator i = fi->begin(); i != fi->end(); i++){
                    //need all *i satisfied to finish
                    //*i  = 1
                    //generate BFNode to test
                    BFNode test = BFNode();
                    test.node_type = fluForm;
                    Fluents fs = Fluents();
                    fs.insert(*i);
                    FluentFormula ffs = FluentFormula();
                    ffs.insert(fs);
                    test.fluform = &ffs;
                    
                    //test for each EpistemicModelsNode, need at least one node to satisfy
                    bool litsatisfy = false;
                    for(EpistemicModelsNode emn : v){
                        Kripke* curK = emn.get_Kripke();
                        set<int> pts = emn.get_pointeds();
                        //need all pId in pts of this node to satisfy
                        bool nodefail = false;
                        for(int p : pts){
                            curK->set_pId(p);
                            if(!curK->entail(test)){
                                nodefail = true;
                                break;
                            }
                        }
                        if(!nodefail){
                            litsatisfy = true;
                            break;
                        }
                    }
                    if(!litsatisfy){
                        andSatisfied = false;
                        break;
                    }
                }
                if(andSatisfied){
                    return true;
                }
            }
            return false;
        }
        case BForm:
        {
            Agent ag = bf.agentPro2;
            BFNode bfnext = *bf.bfnode1;
            //move edge of label ag for each element of v.
            vector<EpistemicModelsNode> newV = vector<EpistemicModelsNode> ();
            for(iv = v.begin(); iv!= v.end(); iv++)
            {
                Kripke* kstd = (*iv).get_Kripke();
                set<int> pointeds = (*iv).get_pointeds();
                set<int> nextPointeds = set<int>();
                for(set<int>::iterator ip = pointeds.begin(); ip!= pointeds.end(); ip++){
                    map<int, set<int> > k1map = kstd->get_stateslist().at(*ip).get_preparedMap();
                    if(k1map.find(ag)!= k1map.end()){
                        set<int> forcheck = k1map[ag];
                        nextPointeds.insert(forcheck.cbegin(), forcheck.cend());
                    }
                }
                newV.push_back(EpistemicModelsNode(kstd, nextPointeds));
            }
            return emNodes_entailment(newV,bfnext);
        }
        case propForm:
        {
            if(bf.bfOperator == BFNOT)
            {
                //TODO: check here again, for now we assume all -B_i \phi is satisfied because we cannot verify this.
                //return !emNodes_entailment(v,*bf.bfnode1);
                return true;
            }else if(bf.bfOperator == NONE)
            {
                return emNodes_entailment(v,*bf.bfnode1);
            }else if(bf.bfOperator == BFOR)
            {
                return emNodes_entailment(v,*bf.bfnode1) || emNodes_entailment(v,*bf.bfnode2);
            }else if(bf.bfOperator==BFAND)
            {
                return emNodes_entailment(v,*bf.bfnode1) && emNodes_entailment(v,*bf.bfnode2);
            }else{
                return false;
            }
        }
        case EForm:
        {
            Agents::iterator it;
            Agents bfag = bf.agents;
            BFNode bfnext = *bf.bfnode1;
            //move edge of label ag for each element of v.
            vector<EpistemicModelsNode> newV = vector<EpistemicModelsNode> ();
            for(iv = v.begin(); iv!= v.end(); iv++)
            {
                Kripke* kstd = (*iv).get_Kripke();
                set<int> pointeds = (*iv).get_pointeds();
                set<int> nextPointeds = set<int>();
                for(set<int>::iterator ip = pointeds.begin(); ip!= pointeds.end(); ip++){
                    map<int, set<int> > k1map = kstd->get_stateslist().at(*ip).get_preparedMap();
                    for(it = bfag.begin(); it != bfag.end(); it++){
                        Agent ag = *it;
                        if(k1map.find(ag)!= k1map.end()){
                            set<int> forcheck = k1map[ag];
                            nextPointeds.insert(forcheck.cbegin(), forcheck.cend());
                        }
                    }
                }
                newV.push_back(EpistemicModelsNode(kstd, nextPointeds));
            }
            return emNodes_entailment(newV,bfnext);
        }
        case CForm:
        {
            Agents ags = bf.agents;
            BFNode bfnext = *bf.bfnode1;
            vector<EpistemicModelsNode> newV = vector<EpistemicModelsNode> ();
            for(iv = v.begin(); iv!= v.end(); iv++)
            {
                Kripke* kstd = (*iv).get_Kripke();
                set<int> pointeds = (*iv).get_pointeds();
                set<int> nextPointeds = set<int>();
                for(set<int>::iterator ip = pointeds.begin(); ip!= pointeds.end(); ip++){
                    StateList sl = kstd->reachableStates(*ip,ags);
                    nextPointeds.insert(sl.cbegin(), sl.cend());
                }
                newV.push_back(EpistemicModelsNode(kstd, nextPointeds));
            }
            return emNodes_entailment(newV,bfnext);
        }
        case BFEmpty:
            return true;
    }
}

bool PGraphENodeLevel::emNodes_entailment(BFNode bf){
    return emNodes_entailment(eNodes,bf);
}


void PGraphENodeLevel::set_eNodes(vector<EpistemicModelsNode> nodes){
    eNodes = nodes;
}

void PGraphENodeLevel::add_eNode(EpistemicModelsNode node){
    eNodes.push_back(node);
}

vector<EpistemicModelsNode> PGraphENodeLevel::get_eNodes(){
    return eNodes;
}

bool PGraphENodeLevel::executable(KAction a){
    Nodes::const_iterator it;
    for (it = a.get_execs()->begin(); it != a.get_execs()->end(); it++)
    {
        if (!emNodes_entailment(eNodes,*it))
        {
            return false;
        }
    }
    return true;
}


void PGraphENodeLevel::print_info(){
    cout << "printing a PGraphENodeLevel..." << endl;
    for(vector<EpistemicModelsNode>::iterator i = eNodes.begin(); i!=eNodes.end(); i++){
        (*i).print_info();
        cout << "----------------------------" << endl;
    }
    
}


/*********************************************************************
 PlanningGraph implementation
 **********************************************************************/
/* constructor and destructor */
PlanningGraph::PlanningGraph(Planner* planner)
{
    m_planner = planner;
    k_levels = PGraphENodeLevel(); //for \K_i levels
    a_levels = vector<PGraphActionLevel>(); //for A_i levels
}

PlanningGraph::~PlanningGraph()
{
    
}

void PlanningGraph::add_K_level(PGraphENodeLevel e_nodes){
    k_levels = e_nodes;
}

void PlanningGraph::add_A_level(PGraphActionLevel a_nodes){
    a_levels.push_back(a_nodes);
}

int PlanningGraph::constructEPG(Kripke* kinput){
    
    //make copy of kinput to avoid changing kinput
    Kripke* k = kinput->make_copy();
    //make copy of all actions for checking termination condition
    vector<KAction> copyActs = vector<KAction>();
    // prepare for checking termination of ontic action, initialize with empty Literals for each action (key is the name of the action
    map<string, Literals> checkedcauses;
    KActionList::iterator ita;
    for (ita = m_planner->ontic.begin(); ita != m_planner->ontic.end(); ita++){
        copyActs.push_back(*ita);
        if((*ita).get_type() == DYNAMIC || (*ita).get_type() == DETERMINATION){
            Literals tmp = Literals();
            checkedcauses[(*ita).get_name()] = tmp;
        }
    }
    //prepare for checking goals and compute the sum of levels that satisfy each goal as heuristic
    Nodes::iterator in;
    set<int> satisfiedGoalsIndex = set<int>();
    int sumLevels = 0;
    
    
    
    //create \K_0 which is PGraphENodeLevel that contains one eNode being a EpistemicModelsNode
    set<int> curPointed = set<int>();
    curPointed.insert(k->get_id());
    EpistemicModelsNode eNode = EpistemicModelsNode(k,curPointed);
    
    PGraphENodeLevel k0 = PGraphENodeLevel();
    k0.add_eNode(eNode);
    
    PGraphENodeLevel  k_i = PGraphENodeLevel();
    //perform noop action
    k_i.set_eNodes(k0.get_eNodes());
    
    //save goals that are already satisfied to ignore them when computing sum_planning_graph heuristics
    int ind  = 0;
    for (in = m_planner->m_gd.begin(); in != m_planner->m_gd.end(); in++)
    {
        if (k0.emNodes_entailment(*in))
        {
            satisfiedGoalsIndex.insert(ind);
        }
        ind++;
    }
    
    bool first = true;
    int lengthToGoal = 1;
    int times = 1;
    int level = 0;
    while(true){
        level++;
        //compute executable actions given the last PGraphENodeLevel
        //consider last PGraphENodeLevel is k0 to save memory and due to noop action
        PGraphActionLevel a_i = PGraphActionLevel();
    
        if(!first) {
            a_i.set_ActionVec(a_levels.back().get_ActionVec());
            //check if previous level satisfies goal or not
            //Note that we dont need to check for the 0-th PGraphENodeLevel level
            bool satisfied = true;
            //Nodes::iterator in;
            int index = 0;
            for (in = m_planner->m_gd.begin(); in != m_planner->m_gd.end(); in++)
            {
                if (!k0.emNodes_entailment(*in))
                {
                    index++;
                    satisfied = false;
                    break;
                }
                //this goal is satisfied, then check if it is just satisfied or had satisfied from previous.
                if(satisfiedGoalsIndex.find(index) == satisfiedGoalsIndex.end()){
                    //new goal is satisfied at this level.
                    satisfiedGoalsIndex.insert(index);
                    sumLevels = sumLevels + level;
                }
                index++;
                
                
            }
            if(satisfied){ //goals are satisfied at previous level, then return the lengthToGoal
                if(m_planner->m_heuristic == PLANNINGGRAPH){
                    return lengthToGoal;
                }else if(m_planner->m_heuristic == SUMPLANNINGGRAPH){
                    return sumLevels;
                }
            }else {
                lengthToGoal = lengthToGoal +1;// for max level of K level that satisfies each goal
            }
        }
        //terminate here if copyActs.size() == 0; otherwise check if there is any new actions added
        bool newActAdded = false;
        if(copyActs.size()==0){
            break;
        }
        if(m_planner->debug == true){
            cout << "PlanningGraph: Constructing K" << times++ << "----------" << endl;
        }
        for (ita = copyActs.begin(); ita != copyActs.end();)
        {
            if(m_planner->debug == true){
                cout << "Planning Graph: checking action " << (*ita).get_name() << endl;
            }
            if(k0.executable(*ita)){
                if(m_planner->debug == true){
                    cout << "Planning Graph: action " << (*ita).get_name() << " is executable-----" << endl;
                }
                bool removethisact = false;
                //CONTINUE: effect_sensing(1, vector<Agent>(), vector<Agent>(), vector<Agent>());
                //check if we should delete this action for checking termination in next loop
                //delete when:
                //1/ it is an announcement action,
                //2/ it is a sensing action ``a determines f" and both f and \neg f are satisfied by the last \K level, or
                //3/ it is an ontic action, and all ``a causes g if f" are satisfied, i.e., all f are satisfied by the last \K level.
                
                //for ontic action
                if((*ita).get_type() == DYNAMIC)
                {
                    //assume remove this ontic action unless there is at least one CAUSES need to check for its ``if" condition
                    //This CAUSES is the one that gives NEW knowledge of a literal, which is not in checkedcauses[(*ita).get_name()] yet.
                    removethisact = true;
                    
                    AgentSet fd = get_fd(*ita);
                    AgentSet pd = get_pd(*ita);
                    AgentSet od = get_od(*ita);
                    
                    //compute the effects of the ontic action
                    Literals out;
                    Fluents_BF_list::const_iterator it;
                    Literals::const_iterator it2;
                    for (it = (*ita).get_effects()->begin(); it != (*ita).get_effects()->end(); it++)
                    {
                        bool checkthiscause = false;
                        Literals t = checkedcauses[(*ita).get_name()];
                        for (it2 = it->get_head()->begin();
                             it2 != it->get_head()->end(); it2++)
                        {
                            if(t.find(*it2) == t.end()){
                                checkthiscause = true;
                                removethisact = false; //there is still more CAUSES need to check, then not remove this action
                                break;
                            }
                        }
                        //if do not need to check this CAUSES since it has been already checked, then continue
                        if(!checkthiscause){
                            continue; //to check next CAUSES
                        }//Otherwise check the ``if" condition of this CAUSES
                        if (k0.emNodes_entailment(*(it->get_body())))
                        {
                            for (it2 = it->get_head()->begin();
                                 it2 != it->get_head()->end(); it2++)
                            {
                                out.insert(*it2);
                                //add to the Literals corresponding to this action_name key to saved literals caused by this action
                                Literals tmp = checkedcauses[(*ita).get_name()];
                                if(tmp.find(*it2) == tmp.end()){
                                    newActAdded = true;
                                    out.insert(*it2);
                                    tmp.insert(*it2);
                                    checkedcauses[(*ita).get_name()] = tmp;
                                }
                            }
                        }
                    }
                    //for each new literals in satisfied CAUSES add a new EpistemicModelsNode to \K_i
                    if(out.size()!=0){
                        a_i.add_action((*ita).get_name());
                        for(Literals::iterator i1 = out.begin();i1 != out.end(); i1++){
                            k_i.add_eNode(effect_ontic(*i1, fd, od, (*ita).get_name()));
                        }
                    }
                }
                else if((*ita).get_type() == ANNOUNCEMENT)  //for announcement action
                {
                    if(k0.executable(*ita)){
                        removethisact = true;
                        newActAdded = true;
                        
                        AgentSet fd = get_fd(*ita);
                        AgentSet pd = get_pd(*ita);
                        AgentSet od = get_od(*ita);
                        
                        //Note: consider announce action appear at most one time in the form of a announce phi. so check only the first fluent formula
                        
                        k_i.add_eNode(effect_announce(*((*ita).get_ann_ff().begin()), fd, pd, od, (*ita).get_name()));
                        a_i.add_action((*ita).get_name());
                    }
                }
                else //for sensing action
                {
                    removethisact = true;
                    
                    AgentSet fd = get_fd(*ita);
                    AgentSet pd = get_pd(*ita);
                    AgentSet od = get_od(*ita);
                    
                    Literals t = checkedcauses[(*ita).get_name()];
                    
                    Literals::const_iterator it;
                    Literals::const_iterator it2;
                    Literals sensedf = (*ita).get_sensedFluents();
                    
                    for (it = sensedf.begin(); it != sensedf.end(); it++)
                    {
                        if(m_planner->debug == true){
                            cout << "Planning Graph: checking sensing literal " << *it << endl;
                        }
                        //check for the corresponding literal
                        if(t.find(*it)==t.end()){
                            removethisact = false;
                            //generate BFNode corresponding to this literal
                            BFNode bn = BFNode();
                            bn.node_type = fluForm;
                            Fluents f1 = Fluents();
                            f1.insert(*it);
                            FluentFormula f = FluentFormula();
                            f.insert(f1);
                            bn.fluform = &f;
                            if(k0.emNodes_entailment(bn)){
                                newActAdded = true;
                                k_i.add_eNode(effect_sensing(*it, fd, pd, od,(*ita).get_name()));
                                a_i.add_action((*ita).get_name());
                                //update literals checked to checkedcauses
                                t = checkedcauses[(*ita).get_name()];
                                t.insert(*it);
                                checkedcauses[(*ita).get_name()] = t;
                            }
                        }
                        
                        //check for the corresponding \neg literal
                        if(t.find((*it)+1)==t.end()){
                            removethisact = false;
                            //generate BFNode corresponding to this \neg literal
                            BFNode bn = BFNode();
                            bn.node_type = fluForm;
                            Fluents f1 = Fluents();
                            f1.insert((*it)+1);
                            FluentFormula f = FluentFormula();
                            f.insert(f1);
                            bn.fluform = &f;
                            
                            if(k0.emNodes_entailment(bn)){
                                newActAdded = true;
                                k_i.add_eNode(effect_sensing((*it)+1, fd, pd, od,(*ita).get_name()));
                                a_i.add_action((*ita).get_name());
                                //update literals checked to checkedcauses
                                t = checkedcauses[(*ita).get_name()];
                                t.insert((*it)+1);
                                checkedcauses[(*ita).get_name()] = t;
                            }
                        }
                    }
                }
                //remove this action or not
                if(removethisact){
                    ita = copyActs.erase(ita);
                }else{
                    ++ita;
                }
            }else //this action is not executable
            {
                ++ita;
            }
        }
        //add k_i, e_i to the plannging graph
        add_K_level(k_i);
        k0 = k_i;
        add_A_level(a_i);
        if(!newActAdded){
            break;
        }
        first = false;
    }
    if(m_planner->debug == true){
        cout << "the final ki that is not satisfied is ---------" << endl;
        k_i.print_info();
    }
    return -1; //return -1 is for the goal is not satisfied.
}

EpistemicModelsNode PlanningGraph::effect_ontic(Literal l, AgentSet fd, AgentSet od, string action){
    //consider having 2 worlds w0, w1 generate edges connecting between world
    int id_edge = 0;

    vector<Kstate> stateList = vector<Kstate>();
    
    //generate 2 worlds
    Kstate w0 = Kstate();
    w0.set_id(0);
    Literals l0 = Literals();
    l0.insert(l);
    w0.set_lits(l0);
    stateList.push_back(w0);
    
    //if(od.size()!= 0){
        //compute negation of l
        Literal negOfl;
        NEGATE(l, negOfl);
        Kstate w1 = Kstate();
        w1.set_id(1);
        Literals l1 = Literals();
        l1.insert(negOfl);
        w1.set_lits(l1);
        stateList.push_back(w1);
    //}
    
    
    Kripke* krip = new Kripke();
    krip->set_stateslist(stateList);
    
    //edges for agents in FD
    for(AgentSet::iterator ia = fd.begin(); ia != fd.end(); ia++){
        //from w0 -> w0, label in F
        Kedge tmp = Kedge();
        tmp.set_from(0);
        tmp.set_to(0);
        tmp.set_label(*ia);
        tmp.set_id(id_edge);
        krip->add_edge(tmp);
        id_edge++;
        
        //if(od.size()!= 0){
            //from w1 -> w1, label in F
            tmp = Kedge();
            tmp.set_from(1);
            tmp.set_to(1);
            tmp.set_label(*ia);
            tmp.set_id(id_edge);
            krip->add_edge(tmp);
            id_edge++;
       // }
    }
    
    //edges for agents in OD
    for(AgentSet::iterator ia = od.begin(); ia != od.end(); ia++){
        //from w1 -> w1, label in O
        Kedge tmp = Kedge();
        tmp.set_from(1);
        tmp.set_to(1);
        tmp.set_label(*ia);
        tmp.set_id(id_edge);
        krip->add_edge(tmp);
        id_edge++;
        
        //from w0 -> w1, label in O
        tmp = Kedge();
        tmp.set_from(0);
        tmp.set_to(1);
        tmp.set_label(*ia);
        tmp.set_id(id_edge);
        krip->add_edge(tmp);
        id_edge++;
    }
    krip->set_pId(0);
    
    set<int> pointed = set<int>();
    pointed.insert(0);
    EpistemicModelsNode out = EpistemicModelsNode(krip,pointed);
    out.set_action(action);
    return out;
}

EpistemicModelsNode PlanningGraph::effect_sensing(Literal l, AgentSet fd, AgentSet pd, AgentSet od, string action){
    //assume for now fd is not empty
    //consider having 4 worlds w0, w1, w2, w3, generate edges connecting between world
    int id_edge = 0;
    vector<Kstate> stateList = vector<Kstate>();
    
    //generate 4 worlds
    Kstate w0 = Kstate();
    w0.set_id(0);
    Literals l0 = Literals();
    l0.insert(l);
    w0.set_lits(l0);
    stateList.push_back(w0);
    
    //compute negation of l
    Literal negOfl;
    NEGATE(l, negOfl);
    
    
    //if(pd.size()!=0){

        Kstate w1 = Kstate();
        w1.set_id(1);
        Literals l1 = Literals();
        l1.insert(negOfl);
        w1.set_lits(l1);
        stateList.push_back(w1);
    //}
    
    //if(od.size()!= 0){
        Kstate w2 = Kstate();
        w2.set_id(2);
        Literals l2 = Literals();
        l2.insert(l);
        w2.set_lits(l2);
        
        Kstate w3 = Kstate();
        w3.set_id(3);
        Literals l3 = Literals();
        l3.insert(negOfl);
        w3.set_lits(l3);
        
        stateList.push_back(w2);
        stateList.push_back(w3);
    //}
    
    
    Kripke* krip = new Kripke();
    krip->set_stateslist(stateList);
    
    //edges for agents in FD
    for(AgentSet::iterator ia = fd.begin(); ia != fd.end(); ia++){
        //from w0 -> w0, label in F
        Kedge tmp = Kedge();
        tmp.set_from(0);
        tmp.set_to(0);
        tmp.set_label(*ia);
        tmp.set_id(id_edge++);
        krip->add_edge(tmp);
        
        //if(pd.size()!=0){
            //from w1 -> w1, label in F
            tmp = Kedge();
            tmp.set_from(1);
            tmp.set_to(1);
            tmp.set_label(*ia);
            tmp.set_id(id_edge++);
            krip->add_edge(tmp);
       // }
        
       // if(od.size()!=0){
            //from w2 -> w2, label in F
            tmp = Kedge();
            tmp.set_from(2);
            tmp.set_to(2);
            tmp.set_label(*ia);
            tmp.set_id(id_edge++);
            krip->add_edge(tmp);
            
            //from w3 -> w3, label in F
            tmp = Kedge();
            tmp.set_from(3);
            tmp.set_to(3);
            tmp.set_label(*ia);
            tmp.set_id(id_edge++);
            krip->add_edge(tmp);
        
            //from w2 -> w3, label in F
            tmp = Kedge();
            tmp.set_from(2);
            tmp.set_to(3);
            tmp.set_label(*ia);
            tmp.set_id(id_edge++);
            krip->add_edge(tmp);
            
            //from w3 -> w2, label in F
            tmp = Kedge();
            tmp.set_from(3);
            tmp.set_to(2);
            tmp.set_label(*ia);
            tmp.set_id(id_edge++);
            krip->add_edge(tmp);
       // }
    }
    
    //edges for agents in PD
    for(AgentSet::iterator ia = pd.begin(); ia != pd.end(); ia++){
        //from w0 -> w0, label in P
        Kedge tmp = Kedge();
        tmp.set_from(0);
        tmp.set_to(0);
        tmp.set_label(*ia);
        tmp.set_id(id_edge++);
        krip->add_edge(tmp);
        
        //if(pd.size()!=0){
            //from w1 -> w1, label in P
            tmp = Kedge();
            tmp.set_from(1);
            tmp.set_to(1);
            tmp.set_label(*ia);
            tmp.set_id(id_edge++);
            krip->add_edge(tmp);
            
            //from w0 -> w1, label in P
            tmp = Kedge();
            tmp.set_from(0);
            tmp.set_to(1);
            tmp.set_label(*ia);
            tmp.set_id(id_edge++);
            krip->add_edge(tmp);
            
            //from w1 -> w0, label in P
            tmp = Kedge();
            tmp.set_from(1);
            tmp.set_to(0);
            tmp.set_label(*ia);
            tmp.set_id(id_edge++);
            krip->add_edge(tmp);
       // }

        
        //if(od.size()!=0){
            //from w2 -> w2, label in P
            tmp = Kedge();
            tmp.set_from(2);
            tmp.set_to(2);
            tmp.set_label(*ia);
            tmp.set_id(id_edge++);
            krip->add_edge(tmp);
            
            //from w3 -> w3, label in P
            tmp = Kedge();
            tmp.set_from(3);
            tmp.set_to(3);
            tmp.set_label(*ia);
            tmp.set_id(id_edge++);
            krip->add_edge(tmp);
            
            
            //from w2 -> w3, label in P
            tmp = Kedge();
            tmp.set_from(2);
            tmp.set_to(3);
            tmp.set_label(*ia);
            tmp.set_id(id_edge++);
            krip->add_edge(tmp);
            
            //from w3 -> w2, label in P
            tmp = Kedge();
            tmp.set_from(3);
            tmp.set_to(2);
            tmp.set_label(*ia);
            tmp.set_id(id_edge++);
            krip->add_edge(tmp);
       // }
    }
    
    //edges for agents in OD
    for(AgentSet::iterator ia = od.begin(); ia != od.end(); ia++){
        //from w2 -> w2, label in 0
        Kedge tmp = Kedge();
        tmp.set_from(2);
        tmp.set_to(2);
        tmp.set_label(*ia);
        tmp.set_id(id_edge++);
        krip->add_edge(tmp);
        
        //from w3 -> w3, label in 0
        tmp = Kedge();
        tmp.set_from(3);
        tmp.set_to(3);
        tmp.set_label(*ia);
        tmp.set_id(id_edge++);
        krip->add_edge(tmp);
        
        //from w2 -> w3, label in 0
        tmp = Kedge();
        tmp.set_from(2);
        tmp.set_to(3);
        tmp.set_label(*ia);
        tmp.set_id(id_edge++);
        krip->add_edge(tmp);
        
        //from w3 -> w2, label in 0
        tmp = Kedge();
        tmp.set_from(3);
        tmp.set_to(2);
        tmp.set_label(*ia);
        tmp.set_id(id_edge++);
        krip->add_edge(tmp);
        
        //from w0 -> w2, label in 0
        tmp = Kedge();
        tmp.set_from(0);
        tmp.set_to(2);
        tmp.set_label(*ia);
        tmp.set_id(id_edge++);
        krip->add_edge(tmp);
        
        //from w0 -> w3, label in 0
        tmp = Kedge();
        tmp.set_from(0);
        tmp.set_to(3);
        tmp.set_label(*ia);
        tmp.set_id(id_edge++);
        krip->add_edge(tmp);
        
        //from w1 -> w2, label in 0
        tmp = Kedge();
        tmp.set_from(1);
        tmp.set_to(2);
        tmp.set_label(*ia);
        tmp.set_id(id_edge++);
        krip->add_edge(tmp);
        
        //from w1 -> w3, label in 0
        tmp = Kedge();
        tmp.set_from(1);
        tmp.set_to(3);
        tmp.set_label(*ia);
        tmp.set_id(id_edge++);
        krip->add_edge(tmp);
    }
    krip->set_pId(0);
    
    set<int> pointed = set<int>();
    pointed.insert(0);
    EpistemicModelsNode out = EpistemicModelsNode(krip,pointed);
    out.set_action(action);
    return out;
}

//TODO: check if fd, pd, od is empty. What happens?
EpistemicModelsNode PlanningGraph::effect_announce(FluentFormula ff, AgentSet fd, AgentSet pd, AgentSet od, string action){
    //retrieve a list of vectors, each vector consists a fluent that appears in ff and its negation.
    
    Fluents fluentList = Fluents();
    list<vector<Literal> > sets = list<vector<Literal> >();
    for(FluentFormula::iterator it = ff.begin(); it != ff.end(); it++){
        for(Fluents::iterator it1 = (*it).begin(); it1 != (*it).end(); it1++){
            Fluent f = div(*it1, 2).rem == 0 ? *it1 : (*it1)-1;
            if(fluentList.find(f) == fluentList.end() ){
                fluentList.insert(f);
                //add f and \neg f into a vector<Literal> then add to sets
                vector<Literal> tmp = vector<Literal>();
                tmp.push_back(f);
                tmp.push_back(f+1);
                sets.push_back(tmp);
            }
        }
    }
    //perform Cartersian product on sets
    list<vector<Literal> > product;
    cartesian_product(sets, product);
    //convert vector to set
    list<set<Literal> > prod = list<set<Literal> >();
    for(list<vector<Literal> >::iterator i = product.begin(); i!= product.end();i++){
        set<Literal> tmp = set<Literal>();
        for(vector<Literal>::iterator i1 = (*i).begin(); i1 != (*i).end(); i1++){
            tmp.insert(*i1);
        }
        prod.push_back(tmp);
    }
    
    //generate a Kripke
    vector<Kstate> stateList = vector<Kstate>();
    Kripke* stdKripke = new Kripke();

    int count1 = 0;//to save the index of elements for state ID
    //generate Kstates for F and P agents' part, also generate Kstates for O agents' part with an increase of prod.size() of state id
    int inc = prod.size();
    vector<Kstate*> fpPart = vector<Kstate*>();
    vector<Kstate*> oPart = vector<Kstate*>();
    for(list<set<Literal> >::iterator i = prod.begin(); i!= prod.end();i++){
        //generate Kstate for F and P agents' part
        Kstate* tmp = new Kstate();
        tmp->set_lits(*i);
        tmp->set_id(count1);
        fpPart.push_back(tmp);
        
        //generate Kstate for O agents' part
        tmp = new Kstate();
        tmp->set_lits(*i);
        tmp->set_id(count1+inc);
        oPart.push_back(tmp);
        
        count1++;
    }
    //vector<Kstate> stateList = vector<Kstate>();
    //Kripke* stdKripke = new Kripke();
    //generate stateList for standard Kripke as union of fpPart and oPart
    for(vector<Kstate*>::iterator ks = fpPart.begin(); ks != fpPart.end(); ks++){
        stateList.push_back(**ks);
    }
    for(vector<Kstate*>::iterator ks = oPart.begin(); ks != oPart.end(); ks++){
        stateList.push_back(**ks);
    }
    stdKripke->set_stateslist(stateList);
    
    
    //int index = std::distance(aVector.begin(), it);
    int count2 = 0; //for edge ID
    //edges for agents in FD
    for(AgentSet::iterator ia = fd.begin(); ia != fd.end(); ia++){
        for(vector<Kstate*>::iterator ks = fpPart.begin(); ks != fpPart.end(); ks++){
            for(vector<Kstate*>::iterator ks1 = fpPart.begin(); ks1 != fpPart.end(); ks1++){
                if((*ks)->entail_fluform(ff) == (*ks1)->entail_fluform(ff)){
                    //add an edge between them if they both satisfies ff or if they both do not satisfy ff
                    int id1 = (*ks)->get_id();
                    int id2 = (*ks1)->get_id();
                    Kedge* edgetmp = new Kedge();
                    edgetmp->set_id(count2++);
                    edgetmp->set_label(*ia);
                    edgetmp->set_from(id1);
                    edgetmp->set_to(id2);
                    stdKripke->add_edge(*edgetmp);
                    
                    if(id1 != id2){
                        edgetmp = new Kedge();
                        edgetmp->set_id(count2++);
                        edgetmp->set_label(*ia);
                        edgetmp->set_from(id2);
                        edgetmp->set_to(id1);
                        stdKripke->add_edge(*edgetmp);
                    }
                }
            }
        }
        //generate an edge between any Kstates in O agents' part
        for(vector<Kstate*>::iterator ks = oPart.begin(); ks != oPart.end(); ks++){
            for(vector<Kstate*>::iterator ks1 = oPart.begin(); ks1 != oPart.end(); ks1++){
                int id1 = (*ks)->get_id();
                int id2 = (*ks1)->get_id();
                Kedge* edgetmp = new Kedge();
                edgetmp->set_id(count2++);
                edgetmp->set_label(*ia);
                edgetmp->set_from(id1);
                edgetmp->set_to(id2);
                stdKripke->add_edge(*edgetmp);
                
                if(id1 != id2){
                    edgetmp = new Kedge();
                    edgetmp->set_id(count2++);
                    edgetmp->set_label(*ia);
                    edgetmp->set_from(id2);
                    edgetmp->set_to(id1);
                    stdKripke->add_edge(*edgetmp);
                }
            }
        }
    }
    //for agents in PD
    for(AgentSet::iterator ia = pd.begin(); ia != pd.end(); ia++){
        //generate an edge between any Kstates in F,P agents' part
        for(vector<Kstate*>::iterator ks = fpPart.begin(); ks != fpPart.end(); ks++){
            for(vector<Kstate*>::iterator ks1 = fpPart.begin(); ks1 != fpPart.end(); ks1++){
                int id1 = (*ks)->get_id();
                int id2 = (*ks1)->get_id();
                Kedge* edgetmp = new Kedge();
                edgetmp->set_id(count2++);
                edgetmp->set_label(*ia);
                edgetmp->set_from(id1);
                edgetmp->set_to(id2);
                stdKripke->add_edge(*edgetmp);
                
                if(id1 != id2){
                    edgetmp = new Kedge();
                    edgetmp->set_id(count2++);
                    edgetmp->set_label(*ia);
                    edgetmp->set_from(id2);
                    edgetmp->set_to(id1);
                    stdKripke->add_edge(*edgetmp);
                }
            }
        }
        //generate an edge between any Kstates in O agents' part
        for(vector<Kstate*>::iterator ks = oPart.begin(); ks != oPart.end(); ks++){
            for(vector<Kstate*>::iterator ks1 = oPart.begin(); ks1 != oPart.end(); ks1++){
                int id1 = (*ks)->get_id();
                int id2 = (*ks1)->get_id();
                Kedge* edgetmp = new Kedge();
                edgetmp->set_id(count2++);
                edgetmp->set_label(*ia);
                edgetmp->set_from(id1);
                edgetmp->set_to(id2);
                stdKripke->add_edge(*edgetmp);
                
                if(id1 != id2){
                    edgetmp = new Kedge();
                    edgetmp->set_id(count2++);
                    edgetmp->set_label(*ia);
                    edgetmp->set_from(id2);
                    edgetmp->set_to(id1);
                    stdKripke->add_edge(*edgetmp);
                }
            }
        }
    }
    //for agents in OD
    for(AgentSet::iterator ia = od.begin(); ia != od.end(); ia++){
        //generate an edge from a Kstate in FP agents' part to a Kstate in O agents' part
        for(vector<Kstate*>::iterator ks = fpPart.begin(); ks != fpPart.end(); ks++){
            for(vector<Kstate*>::iterator ks1 = oPart.begin(); ks1 != oPart.end(); ks1++){
                int id1 = (*ks)->get_id();
                int id2 = (*ks1)->get_id();
                Kedge* edgetmp = new Kedge();
                edgetmp->set_id(count2++);
                edgetmp->set_label(*ia);
                edgetmp->set_from(id1);
                edgetmp->set_to(id2);
                stdKripke->add_edge(*edgetmp);
            }
        }
        //generate an edge between any Kstates in O agents' part
        for(vector<Kstate*>::iterator ks = oPart.begin(); ks != oPart.end(); ks++){
            for(vector<Kstate*>::iterator ks1 = oPart.begin(); ks1 != oPart.end(); ks1++){
                int id1 = (*ks)->get_id();
                int id2 = (*ks1)->get_id();
                Kedge* edgetmp = new Kedge();
                edgetmp->set_id(count2++);
                edgetmp->set_label(*ia);
                edgetmp->set_from(id1);
                edgetmp->set_to(id2);
                stdKripke->add_edge(*edgetmp);
                
                if(id1 != id2){
                    edgetmp = new Kedge();
                    edgetmp->set_id(count2++);
                    edgetmp->set_label(*ia);
                    edgetmp->set_from(id2);
                    edgetmp->set_to(id1);
                    stdKripke->add_edge(*edgetmp);
                }
            }
        }
    }
    stdKripke->set_pId(0);
    //generate an EpistemicModelsNode that consists of Kripkes of stadKripke, each of which has its pointed state being satisfied by ff
    set<int> pointed = set<int>();
    for(vector<Kstate*>::iterator ks = fpPart.begin(); ks != fpPart.end(); ks++){
        if((*ks)->entail_fluform(ff)){
            pointed.insert((*ks)->get_id());
            stdKripke->set_pId((*ks)->get_id());
        }
    }
    EpistemicModelsNode out = EpistemicModelsNode(stdKripke,pointed);
    out.set_action(action);
    return out;
}

void PlanningGraph::cartesian_product( list< vector<Literal> > sets, list< vector<Literal> >& product){
    if ( sets.empty() )
        return;
    
    // get first set
    auto& s = sets.front();
    //remove first element if it is empty set of kripke*
    if(s.size() == 0 && sets.size() >= 2){
        cartesian_product( list< vector<Literal> >( ++begin( sets ), end( sets ) ), product );
        return;
    }else if(s.size()== 0){
        //if sets contains only 1 elements and it is empty, then return empty
        return;
    }
    
    if ( ++begin( sets ) == end( sets ) ) {
        for ( auto k : s ) {
            vector<Literal> tmp = vector<Literal>();
            tmp.push_back(k);
            product.push_back( tmp );
        }
        return;
    }
    
    // get cartesian product of the rest of the sets
    cartesian_product( list< vector<Literal> >( ++begin( sets ), end( sets ) ), product );
    
    // make additional copies of product and append each element of first set to them
    list< vector<Literal> > additions;
    for ( auto k = ++begin( s ); k != end( s ); ++k ) {
        list< vector<Literal> > ad = product;
        for ( auto& x : ad ) {
            x.push_back( *k );
        }
        additions.splice( end( additions ), ad );
    }
    
    // append the first element of the first set to the original product
    for ( auto& x : product ) {
        x.push_back( s.front() );
    }
    
    // append the additions to the final product
    product.splice( end( product ), additions );
    
    return;
}
//Note: it does not have ``if" in a observes statements
AgentSet PlanningGraph::get_fd(const KAction& act) const{
    AgentSet out = AgentSet();
    Agent_FF_list::iterator ia;
    Agent_FF_list temp = act.get_m_observes();
    for (ia = temp.begin();ia != temp.end(); ia++) {
            out.insert(ia->get_head());
    }
    return out;
}

//Note: it does not have ``if" in a aware_of statements
AgentSet PlanningGraph::get_pd(const KAction& act) const{
    AgentSet out = AgentSet();
    Agent_FF_list::iterator ia;
    Agent_FF_list temp = act.get_m_awares();
    for (ia = temp.begin();ia != temp.end(); ia++) {
        out.insert(ia->get_head());
    }
    return out;
}

AgentSet PlanningGraph::get_od(const KAction& act) const{
    AgentSet ags = m_planner->m_agents;
    AgentSet pd = get_pd(act);
    AgentSet fd = get_fd(act);
    AgentSet::iterator it;
    if (pd.size() != 0) {
        for (it = pd.begin();it != pd.end();it++) {
            ags.erase(*it);
        }
    }
    if (fd.size() != 0) {
        for (it = fd.begin();it != fd.end();it++) {
            ags.erase(*it);
        }
    }
    return ags;
}

void PlanningGraph::print_info(){
    cout << "Printing Planning Graph..." << endl;
    int k = 0;
    int a = 0;
    
    cout << "Printing PGraphENodeLevel........." << k++ << endl;
    k_levels.print_info();
    cout << "++++++++++++++++++" << endl;
    
    for(vector<PGraphActionLevel>::iterator i1 = a_levels.begin(); i1 != a_levels.end(); i1++){
        cout << "Printing PGraphActionLevel........." << a++ << endl;
        (*i1).print_info();
        cout << "++++++++++++++++++" << endl;
    }
    
}
