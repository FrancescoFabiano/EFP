/* Parser for mAr language */

/* change prefix of symbols from yy to "mar" to avoid
   clashes with any other parsers we may want to link */

%define api.prefix {mar}

%{
#include "../../include/utilities/reader.h"

int marerror(const char *s);
int marlex(void);

std::string get_negation(const std::string*);
bool is_consistent(string_set,string_set);
//string_set_set get_negateFluentForm(string_set_set);
string_set_set negate_or(string_set);
string_set_set negate_form(string_set_set);
string_set_set join_SL2(string_set_set, string_set_set);
void print_string_set(string_set);
void print_string_set_set(string_set_set);

extern std::shared_ptr<reader> domain_reader;
%}

%union{
  std::string*	str_val;
  string_set*  str_list; 
  string_set_set* str_list2;
  proposition* prop;
  proposition_list* prop_list;
  belief_formula* bf;
  formula_list* init_nodes;
}

%start	input 

/* precedence among operators */
%left OR
%left COMMA

%token SEMICOLON
%token COMMA
%token OR
%token LEFT_PAREN
%token RIGHT_PAREN
%token RIGHT_BRAC
%token LEFT_BRAC
%token <str_val> ID
%token <str_val> NEGATION
%token <str_val> NUMBER

%token FLUENT
%token ACTION
%token IF
%token CAUSES
%token EXECUTABLE
%token IMPOSSIBLE
%token DETERMINE
%token AWAREOF
%token OBSERVES
%token ANNOUNCES
%token AGEXEC
%token INIT
%token GOAL
%token AGENT
%token MB
%token MC
%token ME
%token MD

%type <str_val> id
%type <str_val> constant
%type <str_val> param
%type <str_val> param_list

%type <str_val> fluent
%type <str_list> fluent_set
%type <str_list> fluent_det_list

%type <str_val> literal
%type <str_list> literal_list
%type <str_list> fluent_decl
%type <str_list> fluent_decls

%type <str_val> agent
%type <str_list> agent_list
%type <str_list> agent_decl
%type <str_list> agent_decls

%type <str_val> action
%type <str_list> action_list
%type <str_list> action_decl
%type <str_list> action_decls

/*DEBUG_WARNING_REMOVAL %type <str_list> if_ DEBUG_WARNING_REMOVAL*/
%type <str_list2> if_part_fluent
%type <bf> if_part_bf
%type <bf> init
%type <init_nodes> init_spec
%type <bf> goal
%type <init_nodes> goal_spec

%type <str_list2> formula
%type <bf> belief_formula
/* DEBUG_WARNING_REMOVAL %type <str_list2> gd_formula DEBUG_WARNING_REMOVAL*/

//%type <prop> static_law
%type <prop_list> dynamic_law
%type <prop> executability
//%type <prop> impossibility
%type <prop> proposition
%type <prop_list> proposition_list
%type <prop_list> determine
%type <prop> awareness
%type <prop> observance
%type <prop_list> announcement
%type <prop_list> domain

%%

input:		
|
fluent_decls 
action_decls
agent_decls 
domain 
init_spec 
goal_spec
 { 
  domain_reader->m_fluents = *$1;
  domain_reader->m_actions = *$2;
  domain_reader->m_agents = *$3;
  domain_reader->m_propositions = *$4;
  domain_reader->m_bf_initially = *$5;
  domain_reader->m_bf_goal = *$6;
}
;

id:
ID {
  $$ = $1;
};

/* constant */
constant:
NUMBER {
  $$ = $1;
}
|
id {
  $$ = $1;
};

/* parameter */
param:
constant
{
  $$ = $1;
}
/*|
  variable*/
;

/* param list */
param_list:
param {
  $$ = $1;
}
|
param_list COMMA param
{
  $$ = new std::string(*$1 + "," + *$3);
};

/* fluent & fluent list*/
fluent:
id
{
  $$ = $1;
}
|
id LEFT_PAREN param_list RIGHT_PAREN
{
  $$ = new std::string(*$1 + "(" + *$3 + ")");
};

fluent_det_list:
fluent {
  $$ = new string_set;
  $$->insert(*$1);
};

fluent_set:
fluent {
  $$ = new string_set;
  $$->insert(*$1);
}
|
fluent_set COMMA fluent {
  $$ = $1;
  $$->insert(*$3);
};

/* literal list */
literal:
fluent {
  $$ = $1;
}
|
NEGATION fluent
{
  $$ = new std::string(*$1 + *$2);
};

literal_list:
literal
{
  $$ = new string_set;
  $$->insert(*$1);
} 
| 
literal_list COMMA literal {
  $$ = $1;  
  $$->insert(*$3);
}; 

formula:
literal {
  string_set s1;

  $$ = new string_set_set;

  s1.insert(*$1);

  $$->insert(s1);
}
| formula COMMA formula
{
  string_set_set::iterator it1;
  string_set_set::iterator it2;
  string_set ns;

  $$ = new string_set_set;

  for (it2 = $1->begin(); it2 != $1->end(); it2++) {
    for (it1 = $3->begin(); it1 != $3->end(); it1++){
	  if (is_consistent(*it2,*it1)) {
		ns = *it2;
		ns.insert(it1->begin(),it1->end());
		$$->insert(ns);
	  }
    }
  }  
}
| formula OR formula {
  $$ = $1;
  $$->insert($3->begin(),$3->end());
}
| LEFT_PAREN formula RIGHT_PAREN
{
  $$ = $2;
};

/* fluent declaration */
fluent_decl: 
FLUENT fluent_set SEMICOLON {
  $$ = $2;
};

fluent_decls:
/* empty */
{
  $$ = new string_set;
}
|
fluent_decls fluent_decl
{
  $1->insert($2->begin(),$2->end());
  $$ = $1;
}
;

/* action declaration */
action:
id {
  $$ = new std::string(*$1);
}
|
id LEFT_PAREN param_list RIGHT_PAREN {
  $$ = new std::string(*$1 + "(" + *$3 + ")");
};

action_list:
action {
  $$ = new string_set;
  $$->insert(*$1);
}
|
action_list COMMA action {
  $$ = $1;
  $$->insert(*$3);
};

action_decl: 
ACTION action_list SEMICOLON {
  $$ = $2;
};

action_decls:
/* empty */
{
  $$ = new string_set;
}
|
action_decls action_decl
{
  $1->insert($2->begin(),$2->end());
  $$ = $1;
}
;
/* agent declaration */
agent:
id {
  $$ = new std::string(*$1);
}
|
id LEFT_PAREN param_list RIGHT_PAREN {
  $$ = new std::string(*$1 + "(" + *$3 + ")");
};

agent_list:
agent {
  $$ = new string_set;
  $$->insert(*$1);
}
|
agent_list COMMA agent {
  $$ = $1;
  $$->insert(*$3);
};

agent_decl: 
AGENT agent_list SEMICOLON {
  $$ = $2;
};

agent_decls:
/* empty */
{
  $$ = new string_set;
}
|
agent_decls agent_decl
{
  $1->insert($2->begin(),$2->end());
  $$ = $1;
}
;

/* domain description */
/*DEBUG_WARNING_REMOVAL if_part: DEBUG_WARNING_REMOVAL*/ 
/* empty */
/*DEBUG_WARNING_REMOVAL {
  $$ = new string_set;
}
|
IF literal_list {
  $$ = $2;
};DEBUG_WARNING_REMOVAL*/

/* if part for belief_formula */
if_part_bf:
/* fail */
{
  $$ = new belief_formula;
  $$->set_formula_type(BF_EMPTY);
}
|
IF belief_formula {
  $$ = $2;
};

belief_formula:
formula{  
    $$ = new belief_formula;
    $$->set_formula_type(FLUENT_FORMULA);
    $$->set_string_fluent_formula(*$1);
}
|
MB LEFT_PAREN agent COMMA belief_formula RIGHT_PAREN {
   $$ = new belief_formula;
   $$->set_formula_type(BELIEF_FORMULA);
   $$->set_string_agent(*$3);
   $$->set_bf1(*$5);
}
|
belief_formula COMMA belief_formula {
   $$ = new belief_formula;
   $$->set_formula_type(PROPOSITIONAL_FORMULA);
   $$->set_operator(BF_AND);
   $$->set_bf1(*$1);
   $$->set_bf2(*$3);
}
|
belief_formula OR belief_formula {
   $$ = new belief_formula;
   $$->set_formula_type(PROPOSITIONAL_FORMULA);
   $$->set_operator(BF_OR);
   $$->set_bf1(*$1);
   $$->set_bf2(*$3);
}
|
LEFT_PAREN NEGATION belief_formula RIGHT_PAREN{
   $$ = new belief_formula;
   $$->set_formula_type(PROPOSITIONAL_FORMULA);
   $$->set_operator(BF_NOT);
   $$->set_bf1(*$3);
}
|
LEFT_PAREN belief_formula RIGHT_PAREN{
    $$ = new belief_formula;
    $$->set_formula_type(PROPOSITIONAL_FORMULA);
    $$->set_operator(BF_INPAREN);
    $$->set_bf1(*$2);
}
|
ME LEFT_PAREN LEFT_BRAC agent_list RIGHT_BRAC COMMA belief_formula RIGHT_PAREN {
   $$ = new belief_formula;
   $$->set_formula_type(E_FORMULA);
   $$->set_string_group_agents(*$4);
   $$->set_bf1(*$7);
}
|
MC LEFT_PAREN LEFT_BRAC agent_list RIGHT_BRAC COMMA belief_formula RIGHT_PAREN {
   $$ = new belief_formula;
   $$->set_formula_type(C_FORMULA);
   $$->set_string_group_agents(*$4);
   $$->set_bf1(*$7);
}
|
MD LEFT_PAREN LEFT_BRAC agent_list RIGHT_BRAC COMMA belief_formula RIGHT_PAREN {
   $$ = new belief_formula;
   $$->set_formula_type(D_FORMULA);
   $$->set_string_group_agents(*$4);
   $$->set_bf1(*$7);
};



/* if part for fluent_formula */
if_part_fluent: 
/* empty */
{
  $$ = new string_set_set;
}
|
IF formula {
  $$ = $2;
};

/* executability condition */
executability:
EXECUTABLE action if_part_bf SEMICOLON
{
  $$ = new proposition;
  $$->set_type(EXECUTABILITY);
  $$->set_action_name(*$2);
  $$->set_conditions(*$3);
};

/* dynamic law */
dynamic_law:
action CAUSES literal_list if_part_bf SEMICOLON
{  
    $$ = new proposition_list;
    proposition p1;
    proposition p2;

    p1.set_type(EFFECTS);
    p1.set_action_name(*$1);
    p1.add_action_effect(*$3);
    p1.set_conditions(*$4);

    $$->push_back(p1);

    p2.set_type(TYPE);
    p2.set_action_name(*$1);
    p2.set_action_type("ontic");

    $$->push_back(p2);
};


/* determines condition */
determine:
action DETERMINE literal_list SEMICOLON
{
  $$ = new proposition_list;
  proposition p1;
  proposition p2;

  p1.set_type(EFFECTS);
  p1.set_action_name(*$1);
  p1.add_action_effect(*$3);

  $$->push_back(p1);

  p2.set_type(TYPE);
  p2.set_action_name(*$1);
  p2.set_action_type("sensing");

  $$->push_back(p2);
};

/* announcement condition */
announcement:
action ANNOUNCES literal_list if_part_bf SEMICOLON
{
  $$ = new proposition_list;
  proposition p1;
  proposition p2;

  p1.set_type(EFFECTS);
  p1.set_action_name(*$1);
  p1.add_action_effect(*$3);
  p1.set_conditions(*$4);

  $$->push_back(p1);

  p2.set_type(TYPE);
  p2.set_action_name(*$1);
  p2.set_action_type("announcement");

  $$->push_back(p2);

};

/* awareness condition */
awareness:
agent AWAREOF action if_part_bf SEMICOLON
{
  $$ = new proposition;
  $$->set_type(OBSERVABILITY);
  $$->set_agent_group("partially");
  $$->set_action_name(*$3);
  $$->set_agent(*$1);
  $$->set_conditions(*$4);
};

/* observance condition */
observance:
agent OBSERVES action if_part_bf SEMICOLON
{
   $$ = new proposition;
   $$->set_type(OBSERVABILITY);
   $$->set_agent_group("fully");
   $$->set_action_name(*$3);
   $$->set_agent(*$1);
   $$->set_conditions(*$4);
};


proposition_list:
dynamic_law
{
  $$ = $1;
}
|
determine
{
  $$ = $1;
}
|
announcement
{
  $$ = $1;
};

/* proposition */
proposition:
executability
{
  $$ = $1;
}
|
observance
{
  $$ = $1;
}
|
awareness
{
  $$ = $1;
}
;



/* domain */
domain:
/* empty */
{
  $$ = new proposition_list;
}
| domain proposition
{
  $$ = $1;
  $1->push_back(*$2);
}
;

/* init */
init:
INIT belief_formula SEMICOLON
{
  $$ = $2;
};

init_spec:
/* empty */
{
  $$ = new formula_list;
}
| init_spec init
{
  $$ = $1;
  $$->push_back(*$2);
};


goal:
GOAL belief_formula SEMICOLON
{
  $$ = $2;
};

goal_spec:
/* empty */
{
  $$ = new formula_list;
}
| goal_spec goal
{
  $$ = $1;
  $$->push_back(*$2);
};
%%

int marerror(std::string s)
{
  extern int marlineno;	// defined and maintained in lex.c
  extern char *martext;	// defined and maintained in lex.c
  
  std::cerr << "ERROR: " << s << " at symbol \"" << martext;
  std::cerr << "\" on line " << marlineno << std::endl;
  exit(1);
  return 0;
}

int marerror(const char *s)
{
  return marerror(std::string(s));
}
//
//bool is_consistent(string_set sl1, string_set sl2)
//{
//  string_set::const_iterator it;
//  std::string nl;
//
//  for (it = sl2.begin(); it != sl2.end(); it++) {
//	nl = get_negation(&(*it));
//	if (sl1.find(nl) != sl1.end())
//	  return false;
//  }
//
//  return true;
//}
//
//std::string get_negation(const std::string* s)
//{
//  std::string ns;
//
//  if (s->substr(0,1) == NEGATION_SYMBOL) {
//	return s->substr(1);
//  }
//  ns = NEGATION_SYMBOL;
//  return ns.append(*s);
//}
//
///*
//string_set_set get_negateFluentForm(string_set_set input){
//
//  string_set_set separate;
//  string_set_set join;
//  string_set_set::iterator it1;
//  string_set_set::iterator it3;
//  string_set_set negation;
//  std::string temp;
//  string_set::const_iterator it2;
//
//  for(it1 = input.begin(); it1 != input.end(); it1++){
//     if(it1->begin() == it1->end())
//        join.insert(*it1);
//     else
//        separate.insert(*it1);
//  }//for loop
//
//  //Separate elements in separate
//     for(it1 = separate.begin(); it1 != separate.end(); it1++){
//        temp = get_negation(&(*(it1->begin())));    //possible pointer problem
//        string_set tiep;
//	tiep.insert(temp);
//	negation.insert(tiep);
//     }//for loop
//
//
//  //Join elements in join with all elements in separate
//  for(it3 = negation.begin(); it3 != negation.end(); it3++)
//     for(it1 = join.begin(); it1 != join.end(); it1++)
//        for(it2 = it1->begin(); it2 != it1->end(); it2++)
//        {
//           temp = get_negation(&(*it2));    //possible pointer problem
//           string_set tiep;
//           tiep.insert(temp);
//           negation.insert(tiep);
//	}
//
//  return negation;
//}
//*/
//
////negate_or: input: String list = list of or.
////             output: Stringlist 2 = list of and of negation
//
//string_set_set negate_or(string_set input){
//
//   string_set::iterator it;
//   string_set_set output;
//   std::string element;
//
//   for(it = input.begin(); it != input.end(); it++){
//      string_set temp;
//      element = get_negation(&(*it));
//      temp.insert(element);
//      output.insert(temp);
//   }
//   //print_string_set_set(output);
//   return output;
//}
//
//
//// or_2_stringlist2
//
////negate_and : input: std::stringlist2 = list of and of or
////		negate_or(each member of input) = a std::stringlist 2
////                -> n std::stringlist 2 -> std::stringlist 3
////                output = first member stirnglist 3 or second member of std::stringlist 3
//
//string_set_set join_SL2(string_set_set input1, string_set_set input2){
//
//  if(input2.size() == 0){
//     return input1;
//  }
//
//  string_set_set::iterator it1;
//  string_set_set::iterator it2;
//  string_set ns;
//
//  string_set_set output;
//
//  for (it2 = input1.begin(); it2 != input1.end(); it2++) {
//    for (it1 = input2.begin(); it1 != input2.end(); it1++){
//      if (is_consistent(*it1,*it2)) {
//	ns = *it2;
//	ns.insert(it1->begin(),it1->end());
//	output.insert(ns);
//      }
//    }
//  }
//
//  return output;
//
//}
//
//string_set_set negate_form(string_set_set input){
//
//  typedef std::set<string_set_set> string_set3;
//  string_set3 list3;
//  string_set_set::iterator it1;
//  string_set_set::iterator it2;
//  string_set3::iterator it3;
//  string_set ns;
//  string_set_set temp;
//
//  string_set_set output;
//
//  //turn all the otr statements to and statements
//   for(it1 = input.begin(); it1 != input.end(); it1++){
//      temp = negate_or(*it1);
//      list3.insert(temp);
//   }
//
//
//   output = *list3.begin();
//   for(it3 = ++list3.begin(); it3 != list3.end(); it3++){
//      output = join_SL2(output, *it3);
//   }
//
//
//   return output;
//}
//
//void print_string_set(string_set in){
//	string_set::iterator it1;
//	std::cout << "[ " ;
//        for(it1 = in.begin();it1!=in.end();it1++){
//		std::cout << *it1 << " , ";
//	}
//	std::cout << "] " ;
//}
//
//void print_string_set_set(string_set_set in){
//	string_set_set::iterator it1;
//	std::cout << "[ ";
//        for(it1 = in.begin();it1!=in.end();it1++){
//
//		print_string_set(*it1);
//		std::cout << " , ";
//	}
//	std::cout << " ] " ;
//}
//
////Planning as Logic
