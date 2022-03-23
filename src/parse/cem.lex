/* Scanner for mAr language */

/* change prefix of symbols from yy to "cem" to avoid
   clashes with any other parsers we may want to link */

%option prefix="cem"

%option nounput
%option noyywrap

%{
#include "../../include/definitions/define.h"
#include "../../src/actions/proposition.h"
#include "../../src/formulae/belief_formula.h"
#include "cem.tab.hh"

/* seems like a bug that I have to do this, since flex
   should know prefix=mar and match bison's CEMSTYPE */
#define YYSTYPE CEMSTYPE

int cemerror(const char* s);
//int cemlineno = 1;
%}


digit		[0-9]
number	{digit}+
id [a-zA-Z][a-zA-Z_0-9]*
comment %.*$

%%

{number}	{ cemlval.str_val = new std::string(cemtext); return NUMBER; }
";"		{ return SEMICOLON; } 
"fluent" {return FLUENT;}
"action" {return ACTION;}
"agent" {return AGENT;}
"if" {return IF;}
"has_effects" {return HAS_EFFECTS;}
"executable" {return EXECUTABLE;}
"in_group" {return IN_GROUP;}
"of" {return OF;}
"has_type" {return HAS_TYPE;}
"initially" {return INIT;}
"goal" {return GOAL;}
"B" {return MB;}
"C" {return MC;}
"E" {return ME;}
"D" {return MD;}



"[" {return LEFT_BRAC;}
"]" {return RIGHT_BRAC;}
"," {return COMMA;}
"|" {return OR;}
"(" {return LEFT_PAREN;}
")" {return RIGHT_PAREN;}
"-" {cemlval.str_val = new std::string(cemtext); return NEGATION;}

{id} {cemlval.str_val = new std::string(cemtext); return ID;}

[ \t]*		{}
[\n]		{ cemlineno++;	}
{comment} ;

.		{std::cerr << "SCANNER "; cemerror(""); exit(1);	}

