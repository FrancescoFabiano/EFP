//
//  temporary.cpp
//  
//
//  Created by Tiep Le on 11/16/17.
//

#include <stdio.h>
{
    //Tiep: for debug Planning Graph
    //create fluentformula
    Fluents t1 = Fluents();
    t1.insert(0);
    t1.insert(2);
    Fluents t2 = Fluents();
    t2.insert(3);
    t2.insert(4);
    FluentFormula t = FluentFormula();
    t.insert(t1);
    t.insert(t2);
    
    //create vector of 2 kripke
    
    Kstate kstest1_1 = Kstate();
    Literals ls1_1 = Literals();
    ls1_1.insert(0);
    ls1_1.insert(3);
    kstest1_1.set_lits(ls1_1);
    kstest1_1.set_id(0);
    EdgeList edge1_1in = EdgeList();
    edge1_1in.push_back(0);
    kstest1_1.set_in(edge1_1in);
    EdgeList edge1_1out = EdgeList();
    edge1_1out.push_back(0);
    edge1_1out.push_back(1);
    edge1_1out.push_back(2);
    kstest1_1.set_out(edge1_1out);
    
    Kstate kstest1_2 = Kstate();
    Literals ls1_2 = Literals();
    ls1_2.insert(1);
    ls1_2.insert(3);
    kstest1_2.set_lits(ls1_2);
    kstest1_2.set_id(1);
    EdgeList edge1_2in = EdgeList();
    edge1_2in.push_back(1);
    edge1_2in.push_back(2);
    kstest1_2.set_in(edge1_2in);
    EdgeList edge1_2out = EdgeList();
    kstest1_2.set_out(edge1_2out);
    
    Kedge ke1_1 = Kedge();
    ke1_1.set_from(0);
    ke1_1.set_to(0);
    ke1_1.set_label(1);
    ke1_1.set_id(0);
    
    Kedge ke1_2 = Kedge();
    ke1_2.set_from(0);
    ke1_2.set_to(1);
    ke1_2.set_label(1);
    ke1_2.set_id(1);
    
    Kedge ke1_3 = Kedge();
    ke1_3.set_from(0);
    ke1_3.set_to(1);
    ke1_3.set_label(2);
    ke1_3.set_id(2);
    
    Kripke* ktest1 = new Kripke();
    
    vector<Kstate> ksvec1 = vector<Kstate>();
    ksvec1.push_back(kstest1_1);
    ksvec1.push_back(kstest1_2);
    ktest1->set_stateslist(ksvec1);
    
    Edges kevec1 = Edges();
    kevec1.push_back(ke1_1);
    kevec1.push_back(ke1_2);
    kevec1.push_back(ke1_3);
    ktest1->set_edgeslist(kevec1);
    
    ktest1->set_pId(0);
    
    
    //2nd kripke
    Kstate kstest2_1 = Kstate();
    Literals ls2_1 = Literals();
    ls2_1.insert(10);
    ls2_1.insert(4);
    kstest2_1.set_lits(ls2_1);
    kstest2_1.set_id(0);
    EdgeList edge2_1in = EdgeList();
    edge2_1in.push_back(0);
    kstest2_1.set_in(edge2_1in);
    EdgeList edge2_1out = EdgeList();
    edge2_1out.push_back(0);
    edge2_1out.push_back(1);
    edge2_1out.push_back(2);
    kstest2_1.set_out(edge2_1out);
    
    Kstate kstest2_2 = Kstate();
    Literals ls2_2 = Literals();
    ls2_2.insert(100);
    ls2_2.insert(4);
    kstest2_2.set_lits(ls2_2);
    kstest2_2.set_id(1);
    EdgeList edge2_2in = EdgeList();
    edge2_2in.push_back(1);
    edge2_2in.push_back(2);
    kstest2_2.set_in(edge2_2in);
    EdgeList edge2_2out = EdgeList();
    kstest2_2.set_out(edge2_2out);
    
    Kedge ke2_1 = Kedge();
    ke2_1.set_from(0);
    ke2_1.set_to(0);
    ke2_1.set_label(1);
    ke2_1.set_id(0);
    
    Kedge ke2_2 = Kedge();
    ke2_2.set_from(0);
    ke2_2.set_to(1);
    ke2_2.set_label(1);
    ke2_2.set_id(1);
    
    Kedge ke2_3 = Kedge();
    ke2_3.set_from(0);
    ke2_3.set_to(1);
    ke2_3.set_label(2);
    ke2_3.set_id(2);
    
    Kripke* ktest2 = new Kripke();
    
    vector<Kstate> ksvec2 = vector<Kstate>();
    ksvec2.push_back(kstest2_1);
    ksvec2.push_back(kstest2_2);
    ktest2->set_stateslist(ksvec2);
    
    Edges kevec2 = Edges();
    kevec2.push_back(ke2_1);
    kevec2.push_back(ke2_2);
    kevec2.push_back(ke2_3);
    ktest2->set_edgeslist(kevec2);
    
    ktest2->set_pId(0);
    
    //---
    vector<Kripke*> kvec = vector<Kripke*>();
    kvec.push_back(ktest1);
    kvec.push_back(ktest2);
    
    
    
    //create BFNode
    BFNode bnod1 = BFNode();
    bnod1.node_type = fluForm;
    bnod1.fluform = &t;
    
    BFNode bnod2 = BFNode();
    bnod2.node_type = BForm;
    bnod2.agentPro2 = 1;
    bnod2.bfnode1 = &bnod1;
    
    BFNode bnod3 = BFNode();
    bnod3.node_type = propForm;
    bnod3.bfOperator = BFNOT;
    bnod3.bfnode1 = &bnod2;
    
    BFNode bnod4 = BFNode();
    bnod4.node_type = propForm;
    bnod4.bfOperator = BFAND;
    bnod4.bfnode1 = &bnod2;
    bnod4.bfnode2 = &bnod3;
    
    BFNode bnod5 = BFNode();
    bnod5.node_type = EForm;
    Agents as = Agents();
    as.push_back(1);
    as.push_back(2);
    bnod5.agents = as;
    bnod5.bfnode1 = &bnod1;
    
    BFNode bnod6 = BFNode();
    bnod6.node_type = CForm;
    Agents as1 = Agents();
    as1.push_back(1);
    as1.push_back(2);
    bnod6.agents = as1;
    bnod6.bfnode1 = &bnod1;
    
    
    
    //create planninggraph
    PGraphENodeLevel pg = PGraphENodeLevel();
    bool res = pg.entail_combinedKripkes(kvec,bnod6);
    cout << "----TESTING PLANNING GRAPH----" << endl;
    cout << res << endl;
    cout << "----DONE TESTING PLANNING GRAPH---" << endl;
}

    {
        //Tiep: for debugging testing
        /*
         cout << "executing sense -------------->" << endl;
         cout << "--------------executing sense for kripke at 0 ------------------------" << endl;
         int indexact = m_action_map.find("sense")->second;
         KActionList::iterator itacl = ontic.begin();
         itacl = itacl + indexact;
         //
         cout << "state 0 is executable:" << is_executable(m_init_cstates.at(0), *itacl) << endl;
         cout << "state 1 is executable:" << is_executable(m_init_cstates.at(1), *itacl) << endl;
         Kripke* prin0 = execute_sensing(m_init_cstates.at(0), *itacl);
         prin0->print_report();
         m_init_cstates.at(1).print_report();
         Kripke* prin1 = execute_sensing(m_init_cstates.at(1), *itacl);
         prin1->print_report();
         
         cout << "--------------executing shout2 for kripke at 0 ------------------------" << endl;
         indexact = m_action_map.find("shout_2")->second;
         itacl = ontic.begin();
         itacl = itacl + indexact;
         Kripke* prin0_0 = execute_ann(prin0, *itacl);
         prin0_0->print_report();
         cout << "----------done first shout 2" << endl;
         Kripke* prin1_1 = execute_ann(prin1, *itacl);
         prin1_1->print_report();
         //    cout << "----------------------------------" << endl << endl;
         //
         //    cout << "--------------executing sense for kripke at 1 ------------------------" << endl;
         //    indexact = m_action_map.find("right")->second;
         //    itacl = ontic.begin();
         //    itacl = itacl + indexact;
         
         //    cout << "----------------------------------" << endl << endl;
         */
    }
