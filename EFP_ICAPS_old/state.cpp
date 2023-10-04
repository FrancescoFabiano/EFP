#include "state.h"
#include "planner.h"

/*************************************************************************
 * State Implementation
 *************************************************************************/
State::State(const Planner* planner)
{
  m_planner = planner;
  m_hvalue = -1;
  m_gsatisfied = -1;
}
  
State::~State()
{
  
}

void State::clear()
{
  m_hvalue = -1;
  m_gsatisfied = -1;
  m_literals.clear();
}

/* operators */
int State::operator<(const State& s) const {
  return m_literals < s.m_literals;
}

bool State::operator==(const State& s) const {
  return m_literals == s.m_literals;
}

State State::operator=(const State& s) {
  m_literals = s.m_literals;
  m_planner = s.m_planner;
  m_hvalue = s.m_hvalue;
  m_gsatisfied = s.m_gsatisfied;
  
  return *this;
}

/* main functions */
void State::closure()
{
  StaticLaws::const_iterator it;
  unsigned int n;

  do {
    n = m_literals.size();
    for (it = m_planner->m_statics.begin(); it != m_planner->m_statics.end(); ++it) {
      if (includes(it->get_body())) {
		m_literals.insert(it->get_head()->begin(),
				  it->get_head()->end());
      }
    }	
  } while (m_literals.size() != n);
}

/* other functions */
bool State::is_consistent() const
{
  Literals::const_iterator it;
  Literal l;

  for (it = m_literals.begin(); it != m_literals.end(); it++) {
    NEGATE(*it,l);
    if (m_literals.count(l))
      return false;
  }

  return true;
}

int State::hvalue() 
{ 
  if (m_hvalue == -1) {
	Literals::iterator it;
	m_hvalue = 0;
	for (it = m_literals.begin(); it != m_literals.end(); it++) {
	  m_hvalue += m_planner->m_goal.count(*it);
	}
  }
  
  return m_hvalue;
}

bool State::satisfies(const Literals* x)
{
  Literals::const_iterator it;
  for (it = x->begin(); it != x->end(); it++) {
    if (m_literals.find(*it) != m_literals.end())
      return true;
  }
  return false;
}

bool State::goal_satisfied() 
{
  set<Literals>::const_iterator it;

  if (m_gsatisfied != -1)
    return m_gsatisfied == 1;

  // goal satisfaction has not been checked
  for (it = (m_planner->m_gd).begin();
       it != (m_planner->m_gd).end(); it++) {
    if (!satisfies(&(*it))) {
      m_gsatisfied = 0;
      return false;
    }
  }

  m_gsatisfied = 1;
  return true;
}

bool State::intersect(const Literals* x) const
{
  Literals::const_iterator it_x;

  for (it_x = x->begin(); it_x != x->end(); it_x++) {
    if (m_literals.count((*it_x)))
      return true;
  }
  return false;
}

bool State::intersect(const State* s) const
{
  return intersect(&s->m_literals);
}

bool State::includes(const Literals* x) const //includes(const list of list of short int)
{
  return std::includes(m_literals.begin(),m_literals.end(),
					   x->begin(),x->end());
}

bool State::includes(const State* s) const
{
  return this->includes(&s->m_literals);
}

void State::print() const
{
  cout << "{";
  m_planner->print(m_literals);
  cout << "}";
}

/*************************************************************************
 * CState Implementation
 *************************************************************************/
/* constructor and destructor */
CState::CState() 
{
  m_hvalue = -1;
  m_plan_length = 0;
  m_action = NULL;
  sensing = NULL;		//Ben
  ontic = NULL;			//Ben
  ann = NULL;			//Ben
  m_previous_cstate = NULL;
}

CState::~CState() 
{
	
}

void CState::clear()
{
  m_hvalue = -1;
  m_plan_length = 0;
  m_states.clear();
}

bool CState::operator<(const CState& cs) const 
{
  return m_states < cs.m_states;
}

CState CState::operator=(const CState& cs) 
{
  m_states = cs.m_states;
  m_hvalue = cs.m_hvalue;

  m_action = cs.m_action;
  sensing = cs.sensing;		//Ben
  ontic = cs.ontic;		//Ben
  ann = cs.ann;			//Ben
  m_previous_cstate = cs.m_previous_cstate;
  m_plan_length = cs.m_plan_length;

  return *this;
}

bool CState::goal_satisfied()
{
  set<State*>::iterator it;
	
  for (it = m_states.begin(); it != m_states.end(); it++) {
    if (!(*it)->goal_satisfied()) {
      return false;
    }
  }
  
  return true;
}

int CState::hvalue() 
{
  if (m_hvalue == -1) {
    m_hvalue = (*m_states.begin())->hvalue();
  }
  return m_hvalue;
}

void CState::set_plan_length(const int& len)
{
  m_plan_length = len;  
}

int CState::get_plan_length() const
{
  return m_plan_length;
}

void CState::set_action(const Action* act)
{
  m_action = act;
}


/* Ben *////////////////////////////////////

void CState::set_sensing(const Action* act)
{
  sensing = act;
}

void CState::set_ontic(const Action* act)
{
  ontic = act;
}

void CState::set_ann(const Action* act)
{
  ann = act;
}

/////////////////////////////////////////////



const Action* CState::get_action() const
{
  return m_action;
}


/* Ben *////////////////////////////////////

const Action* CState::get_sensing() const
{
  return sensing;
}

const Action* CState::get_ontic() const
{
  return ontic;
}

const Action* CState::get_ann() const
{
  return ann;
}


/////////////////////////////////////////////


void CState::set_previous_cstate(const CState* cs)
{
  m_previous_cstate = cs;
}

const CState* CState::get_previous_cstate() const
{
  return m_previous_cstate;
}

void CState::print() const
{
  bool comma = false;
  set<State*>::const_iterator it;

  cout << "<{";
  for (it = m_states.begin(); it != m_states.end(); it++) {
    if (comma) {
      cout << ",";
    }
    else 
      comma = true;
    (*it)->print();
  }
  cout << "}," << m_hvalue << ">";
}

void CState::print_plan() const
{
  if (get_previous_cstate() != NULL) {
	get_previous_cstate()->print_plan();
  }

  if (m_action != NULL) {
	cout << get_plan_length() << ": " << m_action->get_name() << endl;
  }

  /* Ben *////////////////////////////////////

  if (sensing != NULL) {
	cout << get_plan_length() << ": " << sensing->get_name() << endl;
  }

  if (ontic != NULL) {
	cout << get_plan_length() << ": " << ontic->get_name() << endl;
  }

  if (ann != NULL) {
	cout << get_plan_length() << ": " << ann->get_name() << endl;
  }

  /////////////////////////////////////////////
}
