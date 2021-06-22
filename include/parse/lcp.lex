/* Scanner for AL language */

%option nounput

%{
#include "../include/utilities/define.h"
#include "../include/actions/proposition.h"
#include "../include/formulae/belief_formula.h"

#include "tok.h"

int yyerror(const char* s);
//int yylineno = 1;
%}

digit		[0-9]
number	{digit}+
id [a-zA-Z][a-zA-Z_0-9]*
comment %.*$

%%

{number}	{ yylval.str_val = new std::string(yytext); return NUMBER; }
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
"-" {yylval.str_val = new std::string(yytext); return NEGATION;}

{id} {yylval.str_val = new std::string(yytext); return ID;}

[ \t]*		{}
[\n]		{ yylineno++;	}
{comment} ;

.		{std::cerr << "SCANNER "; yyerror(""); exit(1);	}

