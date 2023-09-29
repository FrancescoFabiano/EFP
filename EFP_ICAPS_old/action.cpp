#include "action.h"
#include "planner.h"
#include "define.h"
/*********************************************************************
 Conditional effect implementation
**********************************************************************/
Effect::Effect()
{

}
Effect::~Effect()
{

}

void Effect::set_body(Literals bd) { 
  Literals::iterator it;
  Literal l;
  
  m_body = bd;

  m_neg_body.clear();

  for (it = bd.begin(); it != bd.end(); ++it) {
    NEGATE(*it,l);
    m_neg_body.insert(l);
  }  
}

void Effect::set_head(Literals hd) {
  m_head = hd;    
}

const Literals* Effect::get_body() const {
  return &m_body;
}

const Literals* Effect::get_neg_body() const {
  return &m_neg_body;
}

const Literals* Effect::get_head() const {
  return &m_head;
}

/*********************************************************************
 Fluents BF effect implementation
 **********************************************************************/
Fluents_BF::Fluents_BF()
{
    
}
Fluents_BF::~Fluents_BF()
{
    
}

void Fluents_BF::set_l_head(Literals bd) {
    l_head = bd;
}

void Fluents_BF::set_bn_body(const BFNode& node) {
    b_body = node;
}

const Literals* Fluents_BF::get_head() const {
    return &l_head;
}

const BFNode* Fluents_BF::get_body() const {
    return &b_body;
}


/*********************************************************************
 Fluents BF effect implementation
 **********************************************************************/
Agent_FF::Agent_FF()
{
    
}
Agent_FF::~Agent_FF()
{
    
}

void Agent_FF::set_a_head(Agent ag) {
    a_head = ag;
}

void Agent_FF::set_ff_body(FluentFormula ff) {
    ff_body = ff;
}

Agent Agent_FF::get_head() const {
    return a_head;
}

FluentFormula Agent_FF::get_body() const {
    return ff_body;
}


/*********************************************************************
 Action implementation
**********************************************************************/
/* constructor and destructor */
KAction::KAction(const Planner* planner, const string& name, const PropositionType proptype)
{
  m_planner = planner;
  m_name = name;
  m_type = proptype;
  //m_agents =  set<Agent>;
}

KAction::~KAction()
{
  
}

/* get & set functions */

void KAction::set_name(const string& name) {
  m_name = name;
}

void KAction::set_type(const PropositionType pt) {
    m_type = pt;
}

string KAction::get_name() const
{
  return m_name;
}

/* main functions */
void KAction::add_execs(const BFNode& ex)
{
    m_execs.push_back(ex);
}

void KAction::add_effect(Literals hd, const BFNode& bd)
{
    Fluents_BF ef;
    
    ef.set_l_head(hd);
    ef.set_bn_body(bd);
    
    m_effects.push_back(ef);
}

void KAction::add_observes(const Agent& ag, const FluentFormula& ff)
{
    Agent_FF af;
    af.set_a_head(ag);
    af.set_ff_body(ff);
    m_observes.push_back(af);
}

void KAction::add_awares(const Agent& ag, const FluentFormula& ff)
{
    Agent_FF af;
    af.set_a_head(ag);
    af.set_ff_body(ff);
    m_awares.push_back(af);
}

void KAction::add_sensedFluents(const Literals& f){
    sensedFluents.push_back(f);
}
void KAction::add_ann_ff(const FluentFormula& ff){
    annFFs.push_back(ff);
}

const Nodes* KAction::get_execs() const
{
  return &m_execs;
}

const Fluents_BF_list* KAction::get_effects() const
{
  return &m_effects;
}

Agent_FF_list KAction::get_m_observes() const
{
  return m_observes;
}
Agent_FF_list KAction::get_m_awares() const
{
    return m_awares;
}

const PropositionType KAction::get_type() const
{
    return m_type;
}
const Literals KAction::get_sensedFluents() const
{
	Literals out;
	list<Literals>::const_iterator it1;
	Literals::iterator it2;
	for(it1 = sensedFluents.begin(); it1 != sensedFluents.end(); it1++){
		for(it2 = it1->begin(); it2 != it1->end(); it2++){
			out.insert(*it2);
		} 
	}
    return out;
}
const list<FluentFormula> KAction::get_ann_ff() const
{
    return annFFs;
}

//bool Action::is_executable(const State* s) const
//{
//  list<Literals>::const_iterator it;
//  Literals x;
//
//  // check impossibility conditions
//  for (it = get_imposs()->begin(); 
//       it != get_imposs()->end(); ++it) {
//    // if the body of an impossiblity is empty
//    // then the action is non-executable
//    if (it->empty()) {
//      return false;
//    }
//
//    x = m_planner->negate(*it);
//    if (!s->intersect(&x)) {
//      return false;
//    }
//  }
//
//  // default is executable
//  if (get_execs()->empty()) {
//    return true;
//  }
//
//  for (it = get_execs()->begin(); 
//       it != get_execs()->end(); ++it) {
//    if (s->includes(&(*it))) {
//      return true;
//    }
//  }
//  return false;
//}
//
//bool Action::is_executable(const CState* cs) const
//{
//  set<State*>::const_iterator it;
//
//  for (it = cs->m_states.begin(); it != cs->m_states.end(); it++) {
//	if (!is_executable(*it))
//	  return false;
//  }
//
//  return true;
//}
void KAction::print() const
{
    cout << "DONE TIEP" << endl;
//  list<Literals>::const_iterator it1;	
//  EffectList::const_iterator it2;
//
//  cout << get_name() << endl;
//  // print executability condition
//  cout << "    Executability:\n";
//  for (it1 = get_execs()->begin(); it1 != get_execs()->end(); ++it1) {
//    cout << "\t";
//    m_planner->print(*it1);
//    cout << endl;
//  }	
//
//  cout << "    Imposibility:\n";
//  for (it1 = get_imposs()->begin(); 
//       it1 != get_imposs()->end(); ++it1) {
//    cout << "\t";
//    m_planner->print(*it1);
//    cout << endl;
//  }
//
//  // print conditional effects
//  cout << "    Conditional Effects\n";	
//  for (it2 = get_effects()->begin(); 
//       it2 != get_effects()->end(); ++it2) {
//    cout << "\t";
//    m_planner->print(*it2->get_head());
//    cout << " <- ";
//    m_planner->print(*it2->get_body());
//    cout << endl;	  
//  }
}


