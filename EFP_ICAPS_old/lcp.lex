/* Scanner for AL language */

%{
#include "define.h"
#include "tok.h"

int yyerror(char *s);
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
"causes" {return CAUSES;}
"executable" {return EXECUTABLE;}
"impossible" {return IMPOSSIBLE;}
"determines" {return DETERMINE;}
"aware_of" {return AWAREOF;}
"observes" {return OBSERVES;}
"announces" {return ANNOUNCES;}
"initially" {return INIT;}
"goal" {return GOAL;}
"B" {return B;}
"C" {return C;}
"E" {return E;}


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

.		{ std::cerr << "SCANNER "; yyerror(""); exit(1);	}

