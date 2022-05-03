/* Scanner for mAr language */

/* change prefix of symbols from yy to "mar" to avoid
   clashes with any other parsers we may want to link */

%option prefix="mar"

%option nounput
%option noyywrap

%{
#include "../../src/utilities/reader.h"
#include "../../include/definitions/define.h"
#include "../../src/actions/proposition.h"
#include "../../src/formulae/belief_formula.h"
#include "mar.tab.hh"

/* seems like a bug that I have to do this, since flex
   should know prefix=mar and match bison's MARSTYPE */
#define YYSTYPE MARSTYPE

int marerror(reader *reader, const char* s);
//int marlineno = 1;
%}

digit		[0-9]
number	{digit}+
id [a-zA-Z][a-zA-Z_0-9]*
comment %.*$

%%

{number}	{ marlval.str_val = new std::string(martext); return NUMBER; }
";"		{ return SEMICOLON; } 
"fluent" {return FLUENT;}
"action" {return ACTION;}
"agent" {return AGENT;}
"if" {return IF;}
"executes" {return AGEXEC;}
"causes" {return CAUSES;}
"executable" {return EXECUTABLE;}
"impossible" {return IMPOSSIBLE;}
"determines" {return DETERMINE;}
"aware_of" {return AWAREOF;}
"observes" {return OBSERVES;}
"announces" {return ANNOUNCES;}
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
"-" {marlval.str_val = new std::string(martext); return NEGATION;}

{id} {marlval.str_val = new std::string(martext); return ID;}

[ \t]*		{}
[\n]		{ marlineno++;	}
{comment} ;

.		{std::cerr << "SCANNER "; marerror(nullptr,""); exit(1);	}

