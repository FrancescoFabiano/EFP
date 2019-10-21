/* Parser for AL language */
%{
#include "reader.h"

int yyerror(char *s);
int yylex(void);

string get_negation(const string*);
bool is_consistent(StringList,StringList);
//StringList2 get_negateFluentForm(StringList2);
StringList2 negate_or(StringList);
StringList2 negate_form(StringList2);
StringList2 join_SL2(StringList2, StringList2);
void printStringList(StringList);
void printStringList2(StringList2);

extern Reader reader;

%}

%union{
  string*	str_val;
  StringList*  str_list; 
  StringList2* str_list2;
  Proposition* prop;
  PropositionList* prop_list;
  BFNode* bfnode;
  Nodes* init_nodes;
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
%token INIT
%token GOAL
%token AGENT
%token B
%token C
%token E

%type <str_val> id
%type <str_val> constant
%type <str_val> param
%type <str_val> param_list

%type <str_val> fluent
%type <str_list> fluent_list
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

%type <str_list> if_part
%type <str_list2> if_part_fluent
%type <bfnode> if_part_bf
%type <bfnode> init
%type <init_nodes> init_spec
%type <bfnode> goal
%type <init_nodes> goal_spec

%type <str_list2> formula
%type <bfnode> belief_formula
%type <str_list2> gd_formula

%type <prop> static_law
%type <prop> dynamic_law
%type <prop> executability
%type <prop> impossibility
%type <prop> proposition
%type <prop> determine
%type <prop> awareness
%type <prop> observance
%type <prop> announcement
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
  reader.m_fluents = *$1;
  reader.m_actions = *$2;
  reader.m_agents = *$3;
  reader.m_propositions = *$4;
  reader.k_init = *$5;
  reader.m_gd = *$6;
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
  $$ = new string(*$1 + "," + *$3);
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
  $$ = new StringList;
  $$->insert(*$1);
};

fluent_list:
fluent {
  $$ = new StringList;
  $$->insert(*$1);
}
|
fluent_list COMMA fluent {
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
  $$ = new StringList;
  $$->insert(*$1);
} 
| 
literal_list COMMA literal {
  $$ = $1;  
  $$->insert(*$3);
}; 

formula:
literal {
  StringList s1;

  $$ = new StringList2;

  s1.insert(*$1);

  $$->insert(s1);
}
| formula COMMA formula
{
  StringList2::iterator it1;
  StringList2::iterator it2;
  StringList ns;

  $$ = new StringList2;

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
FLUENT fluent_list SEMICOLON {
  $$ = $2;
};

fluent_decls:
/* empty */
{
  $$ = new StringList;
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
  $$ = new string(*$1);
}
|
id LEFT_PAREN param_list RIGHT_PAREN {
  $$ = new string(*$1 + "(" + *$3 + ")");
};

action_list:
action {
  $$ = new StringList;
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
  $$ = new StringList;
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
  $$ = new string(*$1);
}
|
id LEFT_PAREN param_list RIGHT_PAREN {
  $$ = new string(*$1 + "(" + *$3 + ")");
};

agent_list:
agent {
  $$ = new StringList;
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
  $$ = new StringList;
}
|
agent_decls agent_decl
{
  $1->insert($2->begin(),$2->end());
  $$ = $1;
}
;

/* domain description */
if_part: 
/* empty */
{
  $$ = new StringList;
}
|
IF literal_list {
  $$ = $2;
};

/* if part for belief_formula */
if_part_bf:
/* empty */
{
  $$ = new BFNode;
  $$->node_type = BFEmpty;
}
|
IF belief_formula {
  $$ = $2;


};

belief_formula:
formula{  
    $$ = new BFNode;
    $$->node_type = fluForm;
    $$->flu_form = *$1;
}
|
B LEFT_PAREN agent COMMA belief_formula RIGHT_PAREN {
   $$ = new BFNode;
   $$->node_type = BForm;
   $$->agentPro = *$3;
   $$->bfnode1 = $5;
}
|
belief_formula COMMA belief_formula {
   $$ = new BFNode;
   $$->node_type = propForm;
   $$->bfnode1 = $1;
   $$->bfnode2 = $3;
   $$->bfOperator = BFAND;
}
|
belief_formula OR belief_formula {
   $$ = new BFNode;
   $$->node_type = propForm;
   $$->bfnode1 = $1;
   $$->bfnode2 = $3;
   $$->bfOperator = BFOR;
}
|
LEFT_PAREN NEGATION belief_formula RIGHT_PAREN{
   $$ = new BFNode;
   $$->node_type = propForm;
   $$->bfnode1 = $3;
   $$->bfOperator = BFNOT;
}
|
LEFT_PAREN belief_formula RIGHT_PAREN{
    $$ = new BFNode;
    $$->node_type = propForm;
    $$->bfnode1 = $2;
    $$->bfOperator = NONE;
}
|
E LEFT_PAREN LEFT_BRAC agent_list RIGHT_BRAC COMMA belief_formula RIGHT_PAREN {
   $$ = new BFNode;
   $$->node_type = EForm;
   $$->groupAgent = *$4;
   $$->bfnode1 = $7;
}
|
C LEFT_PAREN LEFT_BRAC agent_list RIGHT_BRAC COMMA belief_formula RIGHT_PAREN {
   $$ = new BFNode;
   $$->node_type = CForm;
   $$->groupAgent = *$4;
   $$->bfnode1 = $7;
};


/* if part for fluent_formula */
if_part_fluent: 
/* empty */
{
  $$ = new StringList2;
}
|
IF formula {
  $$ = $2;
};







/* static law */
static_law:
literal_list if_part SEMICOLON
{
  $$ = new Proposition;
  $$->n_type = STATIC;
  $$->precond = *$2;
  $$->effect = *$1;
};

/* dynamic law */
dynamic_law:
action CAUSES literal_list if_part_bf SEMICOLON 
{  
  $$ = new Proposition;
  $$->n_type = DYNAMIC;
  $$->act_name = *$1;
  $$->bel_form = *$4;
  $$->effect = *$3;
};

/* executability condition */
executability:
EXECUTABLE action if_part_bf SEMICOLON
{
  $$ = new Proposition;
  $$->n_type = EXECUTABILITY;
  $$->act_name = *$2;
  $$->bel_form = *$3;
};

/* determines condition */
determine:
action DETERMINE fluent_det_list SEMICOLON
{
  $$ = new Proposition;
  $$->n_type = DETERMINATION;
  $$->act_name = *$1;
  $$->effect = *$3; 
};

/* announcement condition */
announcement:
action ANNOUNCES formula SEMICOLON
{
  $$ = new Proposition;
  $$->n_type = ANNOUNCEMENT;
  $$->act_name = *$1;
  $$->flu_form = *$3;
};

/* awareness condition */
awareness:
agent AWAREOF action if_part_fluent SEMICOLON
{
  $$ = new Proposition;
  $$->n_type = AWARENESS;
  $$->act_name = *$3;
  $$->agentPro = *$1;
  $$->flu_form = *$4;
};

/* observance condition */
observance:
agent OBSERVES action if_part_fluent SEMICOLON
{
  $$ = new Proposition;
  $$->n_type = OBSERVANCE;
  $$->act_name = *$3;				
  $$->agentPro = *$1;
  $$->flu_form = *$4;
};

/* imposibility condition */
impossibility:
IMPOSSIBLE action if_part SEMICOLON
{
  $$ = new Proposition;
  $$->n_type = IMPOSSIBILITY;
  $$->act_name = *$2;
  $$->precond = *$3;
};

/* proposition */
proposition:
static_law {
  $$ = $1;
}
|
dynamic_law
{
  $$ = $1;
}
|
executability
{
  $$ = $1;
}
|
impossibility
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
  $$ = new PropositionList;
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
  $$ = new Nodes;
  //$$->insert(bfnode());
  //$$ = new StringList2;
  //$$->insert(StringList());
}
| init_spec init
{
  $$ = $1;
  $$->push_back(*$2);
};

/* goal */
gd_formula:
literal {
  StringList s1;

  $$ = new StringList2;

  s1.insert(*$1);
  $$->insert(s1);
}
| gd_formula COMMA gd_formula
{
  $$ = $1;
  $$->insert($3->begin(),$3->end());  
}
| 
gd_formula OR gd_formula {
  StringList2::iterator it1;
  StringList2::iterator it2;
  StringList ns;

  $$ = new StringList2;

  for (it2 = $1->begin(); it2 != $1->end(); it2++) {
    for (it1 = $3->begin(); it1 != $3->end(); it1++){
      if (is_consistent(*it1,*it2)) {
	ns = *it2;
	ns.insert(it1->begin(),it1->end());
	$$->insert(ns);
      }
    }
  }  
}
| LEFT_PAREN gd_formula RIGHT_PAREN
{
  $$ = $2;
};

goal:
GOAL belief_formula SEMICOLON
{
  $$ = $2;
};

goal_spec:
/* empty */
{
  $$ = new Nodes;
}
| goal_spec goal
{
  $$ = $1;
  $$->push_back(*$2);
};
%%

int yyerror(string s)
{
  extern int yylineno;	// defined and maintained in lex.c
  extern char *yytext;	// defined and maintained in lex.c
  
  cerr << "ERROR: " << s << " at symbol \"" << yytext;
  cerr << "\" on line " << yylineno << endl;
  exit(1);
  return 0;
}

int yyerror(char *s)
{
  return yyerror(string(s));
}

bool is_consistent(StringList sl1, StringList sl2)
{
  StringList::const_iterator it;
  string nl;

  for (it = sl2.begin(); it != sl2.end(); it++) {
	nl = get_negation(&(*it));
	if (sl1.find(nl) != sl1.end())
	  return false;
  }

  return true;
}

string get_negation(const string* s)
{
  string ns;

  if (s->substr(0,1) == NEGATION_SYMBOL) {
	return s->substr(1);
  }
  ns = NEGATION_SYMBOL;
  return ns.append(*s);
}

/*
StringList2 get_negateFluentForm(StringList2 input){
  
  StringList2 separate;
  StringList2 join;
  StringList2::iterator it1;
  StringList2::iterator it3;
  StringList2 negation;
  string temp;
  StringList::const_iterator it2;

  for(it1 = input.begin(); it1 != input.end(); it1++){
     if(it1->begin() == it1->end())
        join.insert(*it1);
     else
        separate.insert(*it1);
  }//for loop

  //Separate elements in separate
     for(it1 = separate.begin(); it1 != separate.end(); it1++){
        temp = get_negation(&(*(it1->begin())));    //possible pointer problem
        StringList tiep;
	tiep.insert(temp);
	negation.insert(tiep);
     }//for loop
  

  //Join elements in join with all elements in separate
  for(it3 = negation.begin(); it3 != negation.end(); it3++)
     for(it1 = join.begin(); it1 != join.end(); it1++)
        for(it2 = it1->begin(); it2 != it1->end(); it2++)
        {
           temp = get_negation(&(*it2));    //possible pointer problem
           StringList tiep;
           tiep.insert(temp);
           negation.insert(tiep);
	}
  
  return negation;
}
*/

//negate_or: input: String list = list of or. 
//             output: Stringlist 2 = list of and of negation

StringList2 negate_or(StringList input){
   
   StringList::iterator it;
   StringList2 output;
   string element;
   
   for(it = input.begin(); it != input.end(); it++){
      StringList temp;
      element = get_negation(&(*it));
      temp.insert(element);
      output.insert(temp);
   }
   //printStringList2(output);
   return output;
}


// or_2_stringlist2

//negate_and : input: stringlist2 = list of and of or
//		negate_or(each member of input) = a stringlist 2
//                -> n stringlist 2 -> stringlist 3
//                output = first member stirnglist 3 or second member of stringlist 3

StringList2 join_SL2(StringList2 input1, StringList2 input2){
  
  if(input2.size() == 0){
     return input1;
  }

  StringList2::iterator it1;
  StringList2::iterator it2;
  StringList ns;

  StringList2 output;

  for (it2 = input1.begin(); it2 != input1.end(); it2++) {
    for (it1 = input2.begin(); it1 != input2.end(); it1++){
      if (is_consistent(*it1,*it2)) {
	ns = *it2;
	ns.insert(it1->begin(),it1->end());
	output.insert(ns);
      }
    }
  }  
 
  return output;
   
}

StringList2 negate_form(StringList2 input){
   
  typedef set<StringList2> StringList3;
  StringList3 list3;
  StringList2::iterator it1;
  StringList2::iterator it2;
  StringList3::iterator it3;
  StringList ns;
  StringList2 temp;

  StringList2 output;

  //turn all the “or” statements to “and” statements
   for(it1 = input.begin(); it1 != input.end(); it1++){
      temp = negate_or(*it1);
      list3.insert(temp);
   }

   
   output = *list3.begin();
   for(it3 = ++list3.begin(); it3 != list3.end(); it3++){
      output = join_SL2(output, *it3); 
   }

   
   return output;
}

void printStringList(StringList in){
	StringList::iterator it1;
	cout << "[ " ;
        for(it1 = in.begin();it1!=in.end();it1++){
		cout << *it1 << " , ";   
	}
	cout << "] " ;
}

void printStringList2(StringList2 in){
	StringList2::iterator it1;
	cout << "[ "; 
        for(it1 = in.begin();it1!=in.end();it1++){
 		 
		printStringList(*it1);
		cout << " , ";   
	}
	cout << " ] " ;
}

//Planning as Logic
