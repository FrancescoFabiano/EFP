/* A Bison parser, made by GNU Bison 2.3.  */

/* Skeleton interface for Bison's Yacc-like parsers in C

   Copyright (C) 1984, 1989, 1990, 2000, 2001, 2002, 2003, 2004, 2005, 2006
   Free Software Foundation, Inc.

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2, or (at your option)
   any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

/* Tokens.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
   /* Put the tokens into the symbol table, so that GDB and other debuggers
      know about them.  */
   enum yytokentype {
     OR = 258,
     COMMA = 259,
     SEMICOLON = 260,
     LEFT_PAREN = 261,
     RIGHT_PAREN = 262,
     RIGHT_BRAC = 263,
     LEFT_BRAC = 264,
     ID = 265,
     NEGATION = 266,
     NUMBER = 267,
     FLUENT = 268,
     ACTION = 269,
     IF = 270,
     CAUSES = 271,
     EXECUTABLE = 272,
     IMPOSSIBLE = 273,
     DETERMINE = 274,
     AWAREOF = 275,
     OBSERVES = 276,
     ANNOUNCES = 277,
     INIT = 278,
     GOAL = 279,
     AGENT = 280,
     B = 281,
     C = 282,
     E = 283
   };
#endif
/* Tokens.  */
#define OR 258
#define COMMA 259
#define SEMICOLON 260
#define LEFT_PAREN 261
#define RIGHT_PAREN 262
#define RIGHT_BRAC 263
#define LEFT_BRAC 264
#define ID 265
#define NEGATION 266
#define NUMBER 267
#define FLUENT 268
#define ACTION 269
#define IF 270
#define CAUSES 271
#define EXECUTABLE 272
#define IMPOSSIBLE 273
#define DETERMINE 274
#define AWAREOF 275
#define OBSERVES 276
#define ANNOUNCES 277
#define INIT 278
#define GOAL 279
#define AGENT 280
#define B 281
#define C 282
#define E 283




#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE
#line 21 "lcp.y"
{
  string*	str_val;
  StringList*  str_list; 
  StringList2* str_list2;
  Proposition* prop;
  PropositionList* prop_list;
  BFNode* bfnode;
  Nodes* init_nodes;
}
/* Line 1529 of yacc.c.  */
#line 115 "lcp.tab.h"
	YYSTYPE;
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
# define YYSTYPE_IS_TRIVIAL 1
#endif

extern YYSTYPE yylval;

