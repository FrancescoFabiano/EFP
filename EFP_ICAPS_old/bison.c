/* A Bison parser, made by GNU Bison 2.3.  */

/* Skeleton implementation for Bison's Yacc-like parsers in C

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

/* C LALR(1) parser skeleton written by Richard Stallman, by
   simplifying the original so-called "semantic" parser.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output.  */
#define YYBISON 1

/* Bison version.  */
#define YYBISON_VERSION "2.3"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Using locations.  */
#define YYLSP_NEEDED 0



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




/* Copy the first part of user declarations.  */
#line 2 "lcp.y"

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



/* Enabling traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE 0
#endif

/* Enabling the token table.  */
#ifndef YYTOKEN_TABLE
# define YYTOKEN_TABLE 0
#endif

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
/* Line 193 of yacc.c.  */
#line 181 "lcp.tab.c"
	YYSTYPE;
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
# define YYSTYPE_IS_TRIVIAL 1
#endif



/* Copy the second part of user declarations.  */


/* Line 216 of yacc.c.  */
#line 194 "lcp.tab.c"

#ifdef short
# undef short
#endif

#ifdef YYTYPE_UINT8
typedef YYTYPE_UINT8 yytype_uint8;
#else
typedef unsigned char yytype_uint8;
#endif

#ifdef YYTYPE_INT8
typedef YYTYPE_INT8 yytype_int8;
#elif (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
typedef signed char yytype_int8;
#else
typedef short int yytype_int8;
#endif

#ifdef YYTYPE_UINT16
typedef YYTYPE_UINT16 yytype_uint16;
#else
typedef unsigned short int yytype_uint16;
#endif

#ifdef YYTYPE_INT16
typedef YYTYPE_INT16 yytype_int16;
#else
typedef short int yytype_int16;
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif ! defined YYSIZE_T && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned int
# endif
#endif

#define YYSIZE_MAXIMUM ((YYSIZE_T) -1)

#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(msgid) dgettext ("bison-runtime", msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(msgid) msgid
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YYUSE(e) ((void) (e))
#else
# define YYUSE(e) /* empty */
#endif

/* Identity function, used to suppress warnings about constant conditions.  */
#ifndef lint
# define YYID(n) (n)
#else
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static int
YYID (int i)
#else
static int
YYID (i)
    int i;
#endif
{
  return i;
}
#endif

#if ! defined yyoverflow || YYERROR_VERBOSE

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef YYSTACK_USE_ALLOCA
#  if YYSTACK_USE_ALLOCA
#   ifdef __GNUC__
#    define YYSTACK_ALLOC __builtin_alloca
#   elif defined __BUILTIN_VA_ARG_INCR
#    include <alloca.h> /* INFRINGES ON USER NAME SPACE */
#   elif defined _AIX
#    define YYSTACK_ALLOC __alloca
#   elif defined _MSC_VER
#    include <malloc.h> /* INFRINGES ON USER NAME SPACE */
#    define alloca _alloca
#   else
#    define YYSTACK_ALLOC alloca
#    if ! defined _ALLOCA_H && ! defined _STDLIB_H && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#     ifndef _STDLIB_H
#      define _STDLIB_H 1
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's `empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (YYID (0))
#  ifndef YYSTACK_ALLOC_MAXIMUM
    /* The OS might guarantee only one guard page at the bottom of the stack,
       and a page size can be as small as 4096 bytes.  So we cannot safely
       invoke alloca (N) if N exceeds 4096.  Use a slightly smaller number
       to allow for a few compiler-allocated temporary stack slots.  */
#   define YYSTACK_ALLOC_MAXIMUM 4032 /* reasonable circa 2006 */
#  endif
# else
#  define YYSTACK_ALLOC YYMALLOC
#  define YYSTACK_FREE YYFREE
#  ifndef YYSTACK_ALLOC_MAXIMUM
#   define YYSTACK_ALLOC_MAXIMUM YYSIZE_MAXIMUM
#  endif
#  if (defined __cplusplus && ! defined _STDLIB_H \
       && ! ((defined YYMALLOC || defined malloc) \
	     && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef _STDLIB_H
#    define _STDLIB_H 1
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined _STDLIB_H && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined _STDLIB_H && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* ! defined yyoverflow || YYERROR_VERBOSE */


#if (! defined yyoverflow \
     && (! defined __cplusplus \
	 || (defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yytype_int16 yyss;
  YYSTYPE yyvs;
  };

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (sizeof (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (sizeof (yytype_int16) + sizeof (YYSTYPE)) \
      + YYSTACK_GAP_MAXIMUM)

/* Copy COUNT objects from FROM to TO.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(To, From, Count) \
      __builtin_memcpy (To, From, (Count) * sizeof (*(From)))
#  else
#   define YYCOPY(To, From, Count)		\
      do					\
	{					\
	  YYSIZE_T yyi;				\
	  for (yyi = 0; yyi < (Count); yyi++)	\
	    (To)[yyi] = (From)[yyi];		\
	}					\
      while (YYID (0))
#  endif
# endif

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack)					\
    do									\
      {									\
	YYSIZE_T yynewbytes;						\
	YYCOPY (&yyptr->Stack, Stack, yysize);				\
	Stack = &yyptr->Stack;						\
	yynewbytes = yystacksize * sizeof (*Stack) + YYSTACK_GAP_MAXIMUM; \
	yyptr += yynewbytes / sizeof (*yyptr);				\
      }									\
    while (YYID (0))

#endif

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  3
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   184

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  29
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  40
/* YYNRULES -- Number of rules.  */
#define YYNRULES  77
/* YYNRULES -- Number of states.  */
#define YYNSTATES  157

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   283

#define YYTRANSLATE(YYX)						\
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[YYLEX] -- Bison symbol number corresponding to YYLEX.  */
static const yytype_uint8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28
};

#if YYDEBUG
/* YYPRHS[YYN] -- Index of the first RHS symbol of rule number YYN in
   YYRHS.  */
static const yytype_uint8 yyprhs[] =
{
       0,     0,     3,     4,    11,    13,    15,    17,    19,    21,
      25,    27,    32,    34,    36,    40,    42,    45,    47,    51,
      53,    57,    61,    65,    69,    70,    73,    75,    80,    82,
      86,    90,    91,    94,    96,   101,   103,   107,   111,   112,
     115,   116,   119,   120,   123,   125,   132,   136,   140,   145,
     149,   158,   167,   168,   171,   175,   181,   186,   191,   196,
     202,   208,   213,   215,   217,   219,   221,   223,   225,   227,
     229,   230,   233,   237,   238,   241,   245,   246
};

/* YYRHS -- A `-1'-separated list of the rules' RHS.  */
static const yytype_int8 yyrhs[] =
{
      30,     0,    -1,    -1,    42,    46,    50,    64,    66,    68,
      -1,    10,    -1,    12,    -1,    31,    -1,    32,    -1,    33,
      -1,    34,     4,    33,    -1,    31,    -1,    31,     6,    34,
       7,    -1,    35,    -1,    35,    -1,    37,     4,    35,    -1,
      35,    -1,    11,    35,    -1,    38,    -1,    39,     4,    38,
      -1,    38,    -1,    40,     4,    40,    -1,    40,     3,    40,
      -1,     6,    40,     7,    -1,    13,    37,     5,    -1,    -1,
      42,    41,    -1,    31,    -1,    31,     6,    34,     7,    -1,
      43,    -1,    44,     4,    43,    -1,    14,    44,     5,    -1,
      -1,    46,    45,    -1,    31,    -1,    31,     6,    34,     7,
      -1,    47,    -1,    48,     4,    47,    -1,    25,    48,     5,
      -1,    -1,    50,    49,    -1,    -1,    15,    39,    -1,    -1,
      15,    53,    -1,    40,    -1,    26,     6,    47,     4,    53,
       7,    -1,    53,     4,    53,    -1,    53,     3,    53,    -1,
       6,    11,    53,     7,    -1,     6,    53,     7,    -1,    28,
       6,     9,    48,     8,     4,    53,     7,    -1,    27,     6,
       9,    48,     8,     4,    53,     7,    -1,    -1,    15,    40,
      -1,    39,    51,     5,    -1,    43,    16,    39,    52,     5,
      -1,    17,    43,    52,     5,    -1,    43,    19,    36,     5,
      -1,    43,    22,    40,     5,    -1,    47,    20,    43,    54,
       5,    -1,    47,    21,    43,    54,     5,    -1,    18,    43,
      51,     5,    -1,    55,    -1,    56,    -1,    57,    -1,    62,
      -1,    58,    -1,    59,    -1,    61,    -1,    60,    -1,    -1,
      64,    63,    -1,    23,    53,     5,    -1,    -1,    66,    65,
      -1,    24,    53,     5,    -1,    -1,    68,    67,    -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   113,   113,   115,   132,   138,   142,   148,   158,   162,
     169,   174,   180,   186,   191,   198,   202,   208,   214,   220,
     229,   247,   251,   258,   264,   268,   277,   281,   286,   291,
     297,   303,   307,   315,   319,   324,   329,   335,   341,   345,
     355,   359,   366,   371,   378,   384,   391,   399,   407,   414,
     421,   428,   439,   443,   455,   465,   476,   486,   496,   506,
     517,   528,   538,   542,   547,   552,   557,   562,   567,   572,
     581,   584,   593,   600,   606,   651,   658,   661
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || YYTOKEN_TABLE
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "OR", "COMMA", "SEMICOLON", "LEFT_PAREN",
  "RIGHT_PAREN", "RIGHT_BRAC", "LEFT_BRAC", "ID", "NEGATION", "NUMBER",
  "FLUENT", "ACTION", "IF", "CAUSES", "EXECUTABLE", "IMPOSSIBLE",
  "DETERMINE", "AWAREOF", "OBSERVES", "ANNOUNCES", "INIT", "GOAL", "AGENT",
  "B", "C", "E", "$accept", "input", "id", "constant", "param",
  "param_list", "fluent", "fluent_det_list", "fluent_list", "literal",
  "literal_list", "formula", "fluent_decl", "fluent_decls", "action",
  "action_list", "action_decl", "action_decls", "agent", "agent_list",
  "agent_decl", "agent_decls", "if_part", "if_part_bf", "belief_formula",
  "if_part_fluent", "static_law", "dynamic_law", "executability",
  "determine", "announcement", "awareness", "observance", "impossibility",
  "proposition", "domain", "init", "init_spec", "goal", "goal_spec", 0
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[YYLEX-NUM] -- Internal token number corresponding to
   token YYLEX-NUM.  */
static const yytype_uint16 yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   282,   283
};
# endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,    29,    30,    30,    31,    32,    32,    33,    34,    34,
      35,    35,    36,    37,    37,    38,    38,    39,    39,    40,
      40,    40,    40,    41,    42,    42,    43,    43,    44,    44,
      45,    46,    46,    47,    47,    48,    48,    49,    50,    50,
      51,    51,    52,    52,    53,    53,    53,    53,    53,    53,
      53,    53,    54,    54,    55,    56,    57,    58,    59,    60,
      61,    62,    63,    63,    63,    63,    63,    63,    63,    63,
      64,    64,    65,    66,    66,    67,    68,    68
};

/* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     0,     6,     1,     1,     1,     1,     1,     3,
       1,     4,     1,     1,     3,     1,     2,     1,     3,     1,
       3,     3,     3,     3,     0,     2,     1,     4,     1,     3,
       3,     0,     2,     1,     4,     1,     3,     3,     0,     2,
       0,     2,     0,     2,     1,     6,     3,     3,     4,     3,
       8,     8,     0,     2,     3,     5,     4,     4,     4,     5,
       5,     4,     1,     1,     1,     1,     1,     1,     1,     1,
       0,     2,     3,     0,     2,     3,     0,     2
};

/* YYDEFACT[STATE-NAME] -- Default rule to reduce with in state
   STATE-NUM when YYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
      24,     0,    31,     1,     0,    25,    38,     4,    10,    13,
       0,     0,    32,    70,     0,     0,    23,    26,    28,     0,
       0,    39,    73,     5,     6,     7,     8,     0,    14,     0,
       0,    30,    33,    35,     0,     0,     0,     0,    10,    15,
      17,    40,     0,     0,    62,    63,    64,    66,    67,    69,
      68,    65,    71,    76,     0,    11,     0,    29,     0,     0,
      37,    16,    42,    40,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    74,     3,     9,    27,     0,    36,
       0,     0,     0,     0,    18,    41,    54,    42,    12,     0,
       0,    19,     0,    52,    52,     0,     0,     0,     0,    44,
       0,     0,    77,    34,    43,    56,    61,    11,     0,    57,
       0,     0,     0,    58,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    72,     0,    55,    22,    21,
      20,    53,    59,    60,    15,     0,    49,     0,     0,     0,
      47,    46,    75,    48,     0,     0,     0,     0,     0,     0,
      45,     0,     0,     0,     0,    51,    50
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int8 yydefgoto[] =
{
      -1,     1,     8,    25,    26,    27,    39,    89,    10,    91,
      41,    99,     5,     2,    18,    19,    12,     6,    33,    34,
      21,    13,    67,    81,   100,   115,    44,    45,    46,    47,
      48,    49,    50,    51,    52,    22,    74,    53,   102,    75
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -74
static const yytype_int16 yypact[] =
{
      23,    29,    25,   -74,    63,   -74,    61,   -74,    71,   -74,
      41,    63,   -74,    81,    -2,    63,   -74,   153,   -74,    59,
      63,   -74,    84,   -74,   -74,   -74,   -74,    58,   -74,    -2,
      63,   -74,   154,   -74,    79,    63,    63,    63,    66,   -74,
     -74,     0,    14,   104,   -74,   -74,   -74,   -74,   -74,   -74,
     -74,   -74,   -74,    97,    -2,   -74,    85,   -74,    -2,    63,
     -74,   -74,   124,   146,    -2,   141,   141,   129,   141,    63,
      93,    63,    63,    31,   -74,   138,   -74,   -74,   101,   -74,
      31,   158,   159,   134,   -74,   161,   -74,    12,   -74,   162,
      93,   -74,   139,   151,   151,    70,   163,   164,   165,   150,
     142,    31,   -74,   -74,   152,   -74,   -74,    91,   167,   -74,
     112,    93,    93,   -74,    93,   168,   169,    31,   112,   114,
      63,   166,   170,    31,    31,   -74,   145,   -74,   -74,   172,
     -74,   150,   -74,   -74,   -74,   119,   -74,   173,    63,    63,
     174,   -74,   -74,   -74,    31,     9,    27,   126,   176,   177,
     -74,    31,    31,   128,   133,   -74,   -74
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
     -74,   -74,   -11,   -74,   130,   -24,    -3,   -74,   -74,     2,
     -14,   -21,   -74,   -74,   -16,   -74,   -74,   -74,   -20,    19,
     -74,   -74,   105,    95,   -73,    89,   -74,   -74,   -74,   -74,
     -74,   -74,   -74,   -74,   -74,   -74,   -74,   -74,   -74,   -74
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If zero, do what YYDEFACT says.
   If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -35
static const yytype_int16 yytable[] =
{
      17,     9,    43,    24,    65,    56,    42,   104,     7,    32,
      23,    38,    28,    59,    57,    66,    65,   148,    24,    17,
      62,    63,   119,    -2,    40,    17,    17,    80,   126,     3,
      68,    59,    61,    69,    78,   149,    70,    95,     4,    79,
      83,     7,    35,    24,   135,    15,    16,    24,    32,    92,
     140,   141,    85,    24,    87,    93,    94,    96,    97,    98,
      17,    17,    54,    30,    31,    55,    88,    84,    40,   110,
      40,   147,    64,     7,   118,    11,    95,    14,   153,   154,
       7,   117,   -26,    59,    60,   -26,   -33,   -33,   -26,    54,
     129,   130,    77,   131,     7,    35,    96,    97,    98,    90,
     137,    36,    37,     7,    35,    54,    20,   -27,   103,    32,
     -27,   -34,   -34,   -27,   134,   111,   112,   123,   124,   128,
      73,   136,   123,   124,    71,    72,   143,    32,    32,   123,
     124,   123,   124,   150,    86,   155,   123,   124,    54,    80,
     156,   107,   111,   112,   113,   123,   124,   125,   123,   124,
     142,     7,    35,   111,   112,   123,   124,   145,   146,    29,
      58,    66,   101,   105,   106,    65,   114,   109,    82,   120,
     121,   122,   127,   132,   133,   138,   112,   144,   124,   139,
     151,   152,   108,   116,    76
};

static const yytype_uint8 yycheck[] =
{
      11,     4,    22,    14,     4,    29,    22,    80,    10,    20,
      12,    22,    15,     4,    30,    15,     4,     8,    29,    30,
      36,    37,    95,     0,    22,    36,    37,    15,   101,     0,
      16,     4,    35,    19,    58,     8,    22,     6,    13,    59,
      64,    10,    11,    54,   117,     4,     5,    58,    59,    70,
     123,   124,    66,    64,    68,    71,    72,    26,    27,    28,
      71,    72,     4,     4,     5,     7,    69,    65,    66,    90,
      68,   144,     6,    10,    95,    14,     6,     6,   151,   152,
      10,    11,    16,     4,     5,    19,    20,    21,    22,     4,
     111,   112,     7,   114,    10,    11,    26,    27,    28,     6,
     120,    17,    18,    10,    11,     4,    25,    16,     7,   120,
      19,    20,    21,    22,   117,     3,     4,     3,     4,     7,
      23,     7,     3,     4,    20,    21,     7,   138,   139,     3,
       4,     3,     4,     7,     5,     7,     3,     4,     4,    15,
       7,     7,     3,     4,     5,     3,     4,     5,     3,     4,
       5,    10,    11,     3,     4,     3,     4,   138,   139,     6,
       6,    15,    24,     5,     5,     4,    15,     5,    63,     6,
       6,     6,     5,     5,     5,     9,     4,     4,     4,     9,
       4,     4,    87,    94,    54
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,    30,    42,     0,    13,    41,    46,    10,    31,    35,
      37,    14,    45,    50,     6,     4,     5,    31,    43,    44,
      25,    49,    64,    12,    31,    32,    33,    34,    35,     6,
       4,     5,    31,    47,    48,    11,    17,    18,    31,    35,
      38,    39,    43,    47,    55,    56,    57,    58,    59,    60,
      61,    62,    63,    66,     4,     7,    34,    43,     6,     4,
       5,    35,    43,    43,     6,     4,    15,    51,    16,    19,
      22,    20,    21,    23,    65,    68,    33,     7,    34,    47,
      15,    52,    51,    34,    38,    39,     5,    39,    35,    36,
       6,    38,    40,    43,    43,     6,    26,    27,    28,    40,
      53,    24,    67,     7,    53,     5,     5,     7,    52,     5,
      40,     3,     4,     5,    15,    54,    54,    11,    40,    53,
       6,     6,     6,     3,     4,     5,    53,     5,     7,    40,
      40,    40,     5,     5,    35,    53,     7,    47,     9,     9,
      53,    53,     5,     7,     4,    48,    48,    53,     8,     8,
       7,     4,     4,    53,    53,     7,     7
};

#define yyerrok		(yyerrstatus = 0)
#define yyclearin	(yychar = YYEMPTY)
#define YYEMPTY		(-2)
#define YYEOF		0

#define YYACCEPT	goto yyacceptlab
#define YYABORT		goto yyabortlab
#define YYERROR		goto yyerrorlab


/* Like YYERROR except do call yyerror.  This remains here temporarily
   to ease the transition to the new meaning of YYERROR, for GCC.
   Once GCC version 2 has supplanted version 1, this can go.  */

#define YYFAIL		goto yyerrlab

#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)					\
do								\
  if (yychar == YYEMPTY && yylen == 1)				\
    {								\
      yychar = (Token);						\
      yylval = (Value);						\
      yytoken = YYTRANSLATE (yychar);				\
      YYPOPSTACK (1);						\
      goto yybackup;						\
    }								\
  else								\
    {								\
      yyerror (YY_("syntax error: cannot back up")); \
      YYERROR;							\
    }								\
while (YYID (0))


#define YYTERROR	1
#define YYERRCODE	256


/* YYLLOC_DEFAULT -- Set CURRENT to span from RHS[1] to RHS[N].
   If N is 0, then set CURRENT to the empty location which ends
   the previous symbol: RHS[0] (always defined).  */

#define YYRHSLOC(Rhs, K) ((Rhs)[K])
#ifndef YYLLOC_DEFAULT
# define YYLLOC_DEFAULT(Current, Rhs, N)				\
    do									\
      if (YYID (N))                                                    \
	{								\
	  (Current).first_line   = YYRHSLOC (Rhs, 1).first_line;	\
	  (Current).first_column = YYRHSLOC (Rhs, 1).first_column;	\
	  (Current).last_line    = YYRHSLOC (Rhs, N).last_line;		\
	  (Current).last_column  = YYRHSLOC (Rhs, N).last_column;	\
	}								\
      else								\
	{								\
	  (Current).first_line   = (Current).last_line   =		\
	    YYRHSLOC (Rhs, 0).last_line;				\
	  (Current).first_column = (Current).last_column =		\
	    YYRHSLOC (Rhs, 0).last_column;				\
	}								\
    while (YYID (0))
#endif


/* YY_LOCATION_PRINT -- Print the location on the stream.
   This macro was not mandated originally: define only if we know
   we won't break user code: when these are the locations we know.  */

#ifndef YY_LOCATION_PRINT
# if defined YYLTYPE_IS_TRIVIAL && YYLTYPE_IS_TRIVIAL
#  define YY_LOCATION_PRINT(File, Loc)			\
     fprintf (File, "%d.%d-%d.%d",			\
	      (Loc).first_line, (Loc).first_column,	\
	      (Loc).last_line,  (Loc).last_column)
# else
#  define YY_LOCATION_PRINT(File, Loc) ((void) 0)
# endif
#endif


/* YYLEX -- calling `yylex' with the right arguments.  */

#ifdef YYLEX_PARAM
# define YYLEX yylex (YYLEX_PARAM)
#else
# define YYLEX yylex ()
#endif

/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)			\
do {						\
  if (yydebug)					\
    YYFPRINTF Args;				\
} while (YYID (0))

# define YY_SYMBOL_PRINT(Title, Type, Value, Location)			  \
do {									  \
  if (yydebug)								  \
    {									  \
      YYFPRINTF (stderr, "%s ", Title);					  \
      yy_symbol_print (stderr,						  \
		  Type, Value); \
      YYFPRINTF (stderr, "\n");						  \
    }									  \
} while (YYID (0))


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

/*ARGSUSED*/
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_symbol_value_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
#else
static void
yy_symbol_value_print (yyoutput, yytype, yyvaluep)
    FILE *yyoutput;
    int yytype;
    YYSTYPE const * const yyvaluep;
#endif
{
  if (!yyvaluep)
    return;
# ifdef YYPRINT
  if (yytype < YYNTOKENS)
    YYPRINT (yyoutput, yytoknum[yytype], *yyvaluep);
# else
  YYUSE (yyoutput);
# endif
  switch (yytype)
    {
      default:
	break;
    }
}


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_symbol_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
#else
static void
yy_symbol_print (yyoutput, yytype, yyvaluep)
    FILE *yyoutput;
    int yytype;
    YYSTYPE const * const yyvaluep;
#endif
{
  if (yytype < YYNTOKENS)
    YYFPRINTF (yyoutput, "token %s (", yytname[yytype]);
  else
    YYFPRINTF (yyoutput, "nterm %s (", yytname[yytype]);

  yy_symbol_value_print (yyoutput, yytype, yyvaluep);
  YYFPRINTF (yyoutput, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_stack_print (yytype_int16 *bottom, yytype_int16 *top)
#else
static void
yy_stack_print (bottom, top)
    yytype_int16 *bottom;
    yytype_int16 *top;
#endif
{
  YYFPRINTF (stderr, "Stack now");
  for (; bottom <= top; ++bottom)
    YYFPRINTF (stderr, " %d", *bottom);
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)				\
do {								\
  if (yydebug)							\
    yy_stack_print ((Bottom), (Top));				\
} while (YYID (0))


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_reduce_print (YYSTYPE *yyvsp, int yyrule)
#else
static void
yy_reduce_print (yyvsp, yyrule)
    YYSTYPE *yyvsp;
    int yyrule;
#endif
{
  int yynrhs = yyr2[yyrule];
  int yyi;
  unsigned long int yylno = yyrline[yyrule];
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %lu):\n",
	     yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      fprintf (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr, yyrhs[yyprhs[yyrule] + yyi],
		       &(yyvsp[(yyi + 1) - (yynrhs)])
		       		       );
      fprintf (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)		\
do {					\
  if (yydebug)				\
    yy_reduce_print (yyvsp, Rule); \
} while (YYID (0))

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args)
# define YY_SYMBOL_PRINT(Title, Type, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef	YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   YYSTACK_ALLOC_MAXIMUM < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif



#if YYERROR_VERBOSE

# ifndef yystrlen
#  if defined __GLIBC__ && defined _STRING_H
#   define yystrlen strlen
#  else
/* Return the length of YYSTR.  */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static YYSIZE_T
yystrlen (const char *yystr)
#else
static YYSIZE_T
yystrlen (yystr)
    const char *yystr;
#endif
{
  YYSIZE_T yylen;
  for (yylen = 0; yystr[yylen]; yylen++)
    continue;
  return yylen;
}
#  endif
# endif

# ifndef yystpcpy
#  if defined __GLIBC__ && defined _STRING_H && defined _GNU_SOURCE
#   define yystpcpy stpcpy
#  else
/* Copy YYSRC to YYDEST, returning the address of the terminating '\0' in
   YYDEST.  */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static char *
yystpcpy (char *yydest, const char *yysrc)
#else
static char *
yystpcpy (yydest, yysrc)
    char *yydest;
    const char *yysrc;
#endif
{
  char *yyd = yydest;
  const char *yys = yysrc;

  while ((*yyd++ = *yys++) != '\0')
    continue;

  return yyd - 1;
}
#  endif
# endif

# ifndef yytnamerr
/* Copy to YYRES the contents of YYSTR after stripping away unnecessary
   quotes and backslashes, so that it's suitable for yyerror.  The
   heuristic is that double-quoting is unnecessary unless the string
   contains an apostrophe, a comma, or backslash (other than
   backslash-backslash).  YYSTR is taken from yytname.  If YYRES is
   null, do not copy; instead, return the length of what the result
   would have been.  */
static YYSIZE_T
yytnamerr (char *yyres, const char *yystr)
{
  if (*yystr == '"')
    {
      YYSIZE_T yyn = 0;
      char const *yyp = yystr;

      for (;;)
	switch (*++yyp)
	  {
	  case '\'':
	  case ',':
	    goto do_not_strip_quotes;

	  case '\\':
	    if (*++yyp != '\\')
	      goto do_not_strip_quotes;
	    /* Fall through.  */
	  default:
	    if (yyres)
	      yyres[yyn] = *yyp;
	    yyn++;
	    break;

	  case '"':
	    if (yyres)
	      yyres[yyn] = '\0';
	    return yyn;
	  }
    do_not_strip_quotes: ;
    }

  if (! yyres)
    return yystrlen (yystr);

  return yystpcpy (yyres, yystr) - yyres;
}
# endif

/* Copy into YYRESULT an error message about the unexpected token
   YYCHAR while in state YYSTATE.  Return the number of bytes copied,
   including the terminating null byte.  If YYRESULT is null, do not
   copy anything; just return the number of bytes that would be
   copied.  As a special case, return 0 if an ordinary "syntax error"
   message will do.  Return YYSIZE_MAXIMUM if overflow occurs during
   size calculation.  */
static YYSIZE_T
yysyntax_error (char *yyresult, int yystate, int yychar)
{
  int yyn = yypact[yystate];

  if (! (YYPACT_NINF < yyn && yyn <= YYLAST))
    return 0;
  else
    {
      int yytype = YYTRANSLATE (yychar);
      YYSIZE_T yysize0 = yytnamerr (0, yytname[yytype]);
      YYSIZE_T yysize = yysize0;
      YYSIZE_T yysize1;
      int yysize_overflow = 0;
      enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
      char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];
      int yyx;

# if 0
      /* This is so xgettext sees the translatable formats that are
	 constructed on the fly.  */
      YY_("syntax error, unexpected %s");
      YY_("syntax error, unexpected %s, expecting %s");
      YY_("syntax error, unexpected %s, expecting %s or %s");
      YY_("syntax error, unexpected %s, expecting %s or %s or %s");
      YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s");
# endif
      char *yyfmt;
      char const *yyf;
      static char const yyunexpected[] = "syntax error, unexpected %s";
      static char const yyexpecting[] = ", expecting %s";
      static char const yyor[] = " or %s";
      char yyformat[sizeof yyunexpected
		    + sizeof yyexpecting - 1
		    + ((YYERROR_VERBOSE_ARGS_MAXIMUM - 2)
		       * (sizeof yyor - 1))];
      char const *yyprefix = yyexpecting;

      /* Start YYX at -YYN if negative to avoid negative indexes in
	 YYCHECK.  */
      int yyxbegin = yyn < 0 ? -yyn : 0;

      /* Stay within bounds of both yycheck and yytname.  */
      int yychecklim = YYLAST - yyn + 1;
      int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
      int yycount = 1;

      yyarg[0] = yytname[yytype];
      yyfmt = yystpcpy (yyformat, yyunexpected);

      for (yyx = yyxbegin; yyx < yyxend; ++yyx)
	if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR)
	  {
	    if (yycount == YYERROR_VERBOSE_ARGS_MAXIMUM)
	      {
		yycount = 1;
		yysize = yysize0;
		yyformat[sizeof yyunexpected - 1] = '\0';
		break;
	      }
	    yyarg[yycount++] = yytname[yyx];
	    yysize1 = yysize + yytnamerr (0, yytname[yyx]);
	    yysize_overflow |= (yysize1 < yysize);
	    yysize = yysize1;
	    yyfmt = yystpcpy (yyfmt, yyprefix);
	    yyprefix = yyor;
	  }

      yyf = YY_(yyformat);
      yysize1 = yysize + yystrlen (yyf);
      yysize_overflow |= (yysize1 < yysize);
      yysize = yysize1;

      if (yysize_overflow)
	return YYSIZE_MAXIMUM;

      if (yyresult)
	{
	  /* Avoid sprintf, as that infringes on the user's name space.
	     Don't have undefined behavior even if the translation
	     produced a string with the wrong number of "%s"s.  */
	  char *yyp = yyresult;
	  int yyi = 0;
	  while ((*yyp = *yyf) != '\0')
	    {
	      if (*yyp == '%' && yyf[1] == 's' && yyi < yycount)
		{
		  yyp += yytnamerr (yyp, yyarg[yyi++]);
		  yyf += 2;
		}
	      else
		{
		  yyp++;
		  yyf++;
		}
	    }
	}
      return yysize;
    }
}
#endif /* YYERROR_VERBOSE */


/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

/*ARGSUSED*/
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yydestruct (const char *yymsg, int yytype, YYSTYPE *yyvaluep)
#else
static void
yydestruct (yymsg, yytype, yyvaluep)
    const char *yymsg;
    int yytype;
    YYSTYPE *yyvaluep;
#endif
{
  YYUSE (yyvaluep);

  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yytype, yyvaluep, yylocationp);

  switch (yytype)
    {

      default:
	break;
    }
}


/* Prevent warnings from -Wmissing-prototypes.  */

#ifdef YYPARSE_PARAM
#if defined __STDC__ || defined __cplusplus
int yyparse (void *YYPARSE_PARAM);
#else
int yyparse ();
#endif
#else /* ! YYPARSE_PARAM */
#if defined __STDC__ || defined __cplusplus
int yyparse (void);
#else
int yyparse ();
#endif
#endif /* ! YYPARSE_PARAM */



/* The look-ahead symbol.  */
int yychar;

/* The semantic value of the look-ahead symbol.  */
YYSTYPE yylval;

/* Number of syntax errors so far.  */
int yynerrs;



/*----------.
| yyparse.  |
`----------*/

#ifdef YYPARSE_PARAM
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
int
yyparse (void *YYPARSE_PARAM)
#else
int
yyparse (YYPARSE_PARAM)
    void *YYPARSE_PARAM;
#endif
#else /* ! YYPARSE_PARAM */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
int
yyparse (void)
#else
int
yyparse ()

#endif
#endif
{
  
  int yystate;
  int yyn;
  int yyresult;
  /* Number of tokens to shift before error messages enabled.  */
  int yyerrstatus;
  /* Look-ahead token as an internal (translated) token number.  */
  int yytoken = 0;
#if YYERROR_VERBOSE
  /* Buffer for error messages, and its allocated size.  */
  char yymsgbuf[128];
  char *yymsg = yymsgbuf;
  YYSIZE_T yymsg_alloc = sizeof yymsgbuf;
#endif

  /* Three stacks and their tools:
     `yyss': related to states,
     `yyvs': related to semantic values,
     `yyls': related to locations.

     Refer to the stacks thru separate pointers, to allow yyoverflow
     to reallocate them elsewhere.  */

  /* The state stack.  */
  yytype_int16 yyssa[YYINITDEPTH];
  yytype_int16 *yyss = yyssa;
  yytype_int16 *yyssp;

  /* The semantic value stack.  */
  YYSTYPE yyvsa[YYINITDEPTH];
  YYSTYPE *yyvs = yyvsa;
  YYSTYPE *yyvsp;



#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  YYSIZE_T yystacksize = YYINITDEPTH;

  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;


  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY;		/* Cause a token to be read.  */

  /* Initialize stack pointers.
     Waste one element of value and location stack
     so that they stay on the same level as the state stack.
     The wasted elements are never initialized.  */

  yyssp = yyss;
  yyvsp = yyvs;

  goto yysetstate;

/*------------------------------------------------------------.
| yynewstate -- Push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
 yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;

 yysetstate:
  *yyssp = yystate;

  if (yyss + yystacksize - 1 <= yyssp)
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYSIZE_T yysize = yyssp - yyss + 1;

#ifdef yyoverflow
      {
	/* Give user a chance to reallocate the stack.  Use copies of
	   these so that the &'s don't force the real ones into
	   memory.  */
	YYSTYPE *yyvs1 = yyvs;
	yytype_int16 *yyss1 = yyss;


	/* Each stack pointer address is followed by the size of the
	   data in use in that stack, in bytes.  This used to be a
	   conditional around just the two extra args, but that might
	   be undefined if yyoverflow is a macro.  */
	yyoverflow (YY_("memory exhausted"),
		    &yyss1, yysize * sizeof (*yyssp),
		    &yyvs1, yysize * sizeof (*yyvsp),

		    &yystacksize);

	yyss = yyss1;
	yyvs = yyvs1;
      }
#else /* no yyoverflow */
# ifndef YYSTACK_RELOCATE
      goto yyexhaustedlab;
# else
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
	goto yyexhaustedlab;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
	yystacksize = YYMAXDEPTH;

      {
	yytype_int16 *yyss1 = yyss;
	union yyalloc *yyptr =
	  (union yyalloc *) YYSTACK_ALLOC (YYSTACK_BYTES (yystacksize));
	if (! yyptr)
	  goto yyexhaustedlab;
	YYSTACK_RELOCATE (yyss);
	YYSTACK_RELOCATE (yyvs);

#  undef YYSTACK_RELOCATE
	if (yyss1 != yyssa)
	  YYSTACK_FREE (yyss1);
      }
# endif
#endif /* no yyoverflow */

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;


      YYDPRINTF ((stderr, "Stack size increased to %lu\n",
		  (unsigned long int) yystacksize));

      if (yyss + yystacksize - 1 <= yyssp)
	YYABORT;
    }

  YYDPRINTF ((stderr, "Entering state %d\n", yystate));

  goto yybackup;

/*-----------.
| yybackup.  |
`-----------*/
yybackup:

  /* Do appropriate processing given the current state.  Read a
     look-ahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to look-ahead token.  */
  yyn = yypact[yystate];
  if (yyn == YYPACT_NINF)
    goto yydefault;

  /* Not known => get a look-ahead token if don't already have one.  */

  /* YYCHAR is either YYEMPTY or YYEOF or a valid look-ahead symbol.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token: "));
      yychar = YYLEX;
    }

  if (yychar <= YYEOF)
    {
      yychar = yytoken = YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else
    {
      yytoken = YYTRANSLATE (yychar);
      YY_SYMBOL_PRINT ("Next token is", yytoken, &yylval, &yylloc);
    }

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  yyn += yytoken;
  if (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
    goto yydefault;
  yyn = yytable[yyn];
  if (yyn <= 0)
    {
      if (yyn == 0 || yyn == YYTABLE_NINF)
	goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  if (yyn == YYFINAL)
    YYACCEPT;

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  /* Shift the look-ahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);

  /* Discard the shifted token unless it is eof.  */
  if (yychar != YYEOF)
    yychar = YYEMPTY;

  yystate = yyn;
  *++yyvsp = yylval;

  goto yynewstate;


/*-----------------------------------------------------------.
| yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
yydefault:
  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;
  goto yyreduce;


/*-----------------------------.
| yyreduce -- Do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     `$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];


  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
        case 3:
#line 121 "lcp.y"
    { 
  reader.m_fluents = *(yyvsp[(1) - (6)].str_list);
  reader.m_actions = *(yyvsp[(2) - (6)].str_list);
  reader.m_agents = *(yyvsp[(3) - (6)].str_list);
  reader.m_propositions = *(yyvsp[(4) - (6)].prop_list);
  reader.k_init = *(yyvsp[(5) - (6)].init_nodes);
  reader.m_gd = *(yyvsp[(6) - (6)].init_nodes);
;}
    break;

  case 4:
#line 132 "lcp.y"
    {
  (yyval.str_val) = (yyvsp[(1) - (1)].str_val);
;}
    break;

  case 5:
#line 138 "lcp.y"
    {
  (yyval.str_val) = (yyvsp[(1) - (1)].str_val);
;}
    break;

  case 6:
#line 142 "lcp.y"
    {
  (yyval.str_val) = (yyvsp[(1) - (1)].str_val);
;}
    break;

  case 7:
#line 149 "lcp.y"
    {
  (yyval.str_val) = (yyvsp[(1) - (1)].str_val);
;}
    break;

  case 8:
#line 158 "lcp.y"
    {
  (yyval.str_val) = (yyvsp[(1) - (1)].str_val);
;}
    break;

  case 9:
#line 163 "lcp.y"
    {
  (yyval.str_val) = new string(*(yyvsp[(1) - (3)].str_val) + "," + *(yyvsp[(3) - (3)].str_val));
;}
    break;

  case 10:
#line 170 "lcp.y"
    {
  (yyval.str_val) = (yyvsp[(1) - (1)].str_val);
;}
    break;

  case 11:
#line 175 "lcp.y"
    {
  (yyval.str_val) = new std::string(*(yyvsp[(1) - (4)].str_val) + "(" + *(yyvsp[(3) - (4)].str_val) + ")");
;}
    break;

  case 12:
#line 180 "lcp.y"
    {
  (yyval.str_list) = new StringList;
  (yyval.str_list)->insert(*(yyvsp[(1) - (1)].str_val));
;}
    break;

  case 13:
#line 186 "lcp.y"
    {
  (yyval.str_list) = new StringList;
  (yyval.str_list)->insert(*(yyvsp[(1) - (1)].str_val));
;}
    break;

  case 14:
#line 191 "lcp.y"
    {
  (yyval.str_list) = (yyvsp[(1) - (3)].str_list);
  (yyval.str_list)->insert(*(yyvsp[(3) - (3)].str_val));
;}
    break;

  case 15:
#line 198 "lcp.y"
    {
  (yyval.str_val) = (yyvsp[(1) - (1)].str_val);
;}
    break;

  case 16:
#line 203 "lcp.y"
    {
  (yyval.str_val) = new std::string(*(yyvsp[(1) - (2)].str_val) + *(yyvsp[(2) - (2)].str_val));
;}
    break;

  case 17:
#line 209 "lcp.y"
    {
  (yyval.str_list) = new StringList;
  (yyval.str_list)->insert(*(yyvsp[(1) - (1)].str_val));
;}
    break;

  case 18:
#line 214 "lcp.y"
    {
  (yyval.str_list) = (yyvsp[(1) - (3)].str_list);  
  (yyval.str_list)->insert(*(yyvsp[(3) - (3)].str_val));
;}
    break;

  case 19:
#line 220 "lcp.y"
    {
  StringList s1;

  (yyval.str_list2) = new StringList2;

  s1.insert(*(yyvsp[(1) - (1)].str_val));

  (yyval.str_list2)->insert(s1);
;}
    break;

  case 20:
#line 230 "lcp.y"
    {
  StringList2::iterator it1;
  StringList2::iterator it2;
  StringList ns;

  (yyval.str_list2) = new StringList2;

  for (it2 = (yyvsp[(1) - (3)].str_list2)->begin(); it2 != (yyvsp[(1) - (3)].str_list2)->end(); it2++) {
    for (it1 = (yyvsp[(3) - (3)].str_list2)->begin(); it1 != (yyvsp[(3) - (3)].str_list2)->end(); it1++){
	  if (is_consistent(*it2,*it1)) {
		ns = *it2;
		ns.insert(it1->begin(),it1->end());
		(yyval.str_list2)->insert(ns);
	  }
    }
  }  
;}
    break;

  case 21:
#line 247 "lcp.y"
    {
  (yyval.str_list2) = (yyvsp[(1) - (3)].str_list2);
  (yyval.str_list2)->insert((yyvsp[(3) - (3)].str_list2)->begin(),(yyvsp[(3) - (3)].str_list2)->end());
;}
    break;

  case 22:
#line 252 "lcp.y"
    {
  (yyval.str_list2) = (yyvsp[(2) - (3)].str_list2);
;}
    break;

  case 23:
#line 258 "lcp.y"
    {
  (yyval.str_list) = (yyvsp[(2) - (3)].str_list);
;}
    break;

  case 24:
#line 264 "lcp.y"
    {
  (yyval.str_list) = new StringList;
;}
    break;

  case 25:
#line 269 "lcp.y"
    {
  (yyvsp[(1) - (2)].str_list)->insert((yyvsp[(2) - (2)].str_list)->begin(),(yyvsp[(2) - (2)].str_list)->end());
  (yyval.str_list) = (yyvsp[(1) - (2)].str_list);
;}
    break;

  case 26:
#line 277 "lcp.y"
    {
  (yyval.str_val) = new string(*(yyvsp[(1) - (1)].str_val));
;}
    break;

  case 27:
#line 281 "lcp.y"
    {
  (yyval.str_val) = new string(*(yyvsp[(1) - (4)].str_val) + "(" + *(yyvsp[(3) - (4)].str_val) + ")");
;}
    break;

  case 28:
#line 286 "lcp.y"
    {
  (yyval.str_list) = new StringList;
  (yyval.str_list)->insert(*(yyvsp[(1) - (1)].str_val));
;}
    break;

  case 29:
#line 291 "lcp.y"
    {
  (yyval.str_list) = (yyvsp[(1) - (3)].str_list);
  (yyval.str_list)->insert(*(yyvsp[(3) - (3)].str_val));
;}
    break;

  case 30:
#line 297 "lcp.y"
    {
  (yyval.str_list) = (yyvsp[(2) - (3)].str_list);
;}
    break;

  case 31:
#line 303 "lcp.y"
    {
  (yyval.str_list) = new StringList;
;}
    break;

  case 32:
#line 308 "lcp.y"
    {
  (yyvsp[(1) - (2)].str_list)->insert((yyvsp[(2) - (2)].str_list)->begin(),(yyvsp[(2) - (2)].str_list)->end());
  (yyval.str_list) = (yyvsp[(1) - (2)].str_list);
;}
    break;

  case 33:
#line 315 "lcp.y"
    {
  (yyval.str_val) = new string(*(yyvsp[(1) - (1)].str_val));
;}
    break;

  case 34:
#line 319 "lcp.y"
    {
  (yyval.str_val) = new string(*(yyvsp[(1) - (4)].str_val) + "(" + *(yyvsp[(3) - (4)].str_val) + ")");
;}
    break;

  case 35:
#line 324 "lcp.y"
    {
  (yyval.str_list) = new StringList;
  (yyval.str_list)->insert(*(yyvsp[(1) - (1)].str_val));
;}
    break;

  case 36:
#line 329 "lcp.y"
    {
  (yyval.str_list) = (yyvsp[(1) - (3)].str_list);
  (yyval.str_list)->insert(*(yyvsp[(3) - (3)].str_val));
;}
    break;

  case 37:
#line 335 "lcp.y"
    {
  (yyval.str_list) = (yyvsp[(2) - (3)].str_list);
;}
    break;

  case 38:
#line 341 "lcp.y"
    {
  (yyval.str_list) = new StringList;
;}
    break;

  case 39:
#line 346 "lcp.y"
    {
  (yyvsp[(1) - (2)].str_list)->insert((yyvsp[(2) - (2)].str_list)->begin(),(yyvsp[(2) - (2)].str_list)->end());
  (yyval.str_list) = (yyvsp[(1) - (2)].str_list);
;}
    break;

  case 40:
#line 355 "lcp.y"
    {
  (yyval.str_list) = new StringList;
;}
    break;

  case 41:
#line 359 "lcp.y"
    {
  (yyval.str_list) = (yyvsp[(2) - (2)].str_list);
;}
    break;

  case 42:
#line 366 "lcp.y"
    {
  (yyval.bfnode) = new BFNode;
  (yyval.bfnode)->node_type = BFEmpty;
;}
    break;

  case 43:
#line 371 "lcp.y"
    {
  (yyval.bfnode) = (yyvsp[(2) - (2)].bfnode);


;}
    break;

  case 44:
#line 378 "lcp.y"
    {  
    (yyval.bfnode) = new BFNode;
    (yyval.bfnode)->node_type = fluForm;
    (yyval.bfnode)->flu_form = *(yyvsp[(1) - (1)].str_list2);
;}
    break;

  case 45:
#line 384 "lcp.y"
    {
   (yyval.bfnode) = new BFNode;
   (yyval.bfnode)->node_type = BForm;
   (yyval.bfnode)->agentPro = *(yyvsp[(3) - (6)].str_val);
   (yyval.bfnode)->bfnode1 = (yyvsp[(5) - (6)].bfnode);
;}
    break;

  case 46:
#line 391 "lcp.y"
    {
   (yyval.bfnode) = new BFNode;
   (yyval.bfnode)->node_type = propForm;
   (yyval.bfnode)->bfnode1 = (yyvsp[(1) - (3)].bfnode);
   (yyval.bfnode)->bfnode2 = (yyvsp[(3) - (3)].bfnode);
   (yyval.bfnode)->bfOperator = BFAND;
;}
    break;

  case 47:
#line 399 "lcp.y"
    {
   (yyval.bfnode) = new BFNode;
   (yyval.bfnode)->node_type = propForm;
   (yyval.bfnode)->bfnode1 = (yyvsp[(1) - (3)].bfnode);
   (yyval.bfnode)->bfnode2 = (yyvsp[(3) - (3)].bfnode);
   (yyval.bfnode)->bfOperator = BFOR;
;}
    break;

  case 48:
#line 407 "lcp.y"
    {
   (yyval.bfnode) = new BFNode;
   (yyval.bfnode)->node_type = propForm;
   (yyval.bfnode)->bfnode1 = (yyvsp[(3) - (4)].bfnode);
   (yyval.bfnode)->bfOperator = BFNOT;
;}
    break;

  case 49:
#line 414 "lcp.y"
    {
    (yyval.bfnode) = new BFNode;
    (yyval.bfnode)->node_type = propForm;
    (yyval.bfnode)->bfnode1 = (yyvsp[(2) - (3)].bfnode);
    (yyval.bfnode)->bfOperator = NONE;
;}
    break;

  case 50:
#line 421 "lcp.y"
    {
   (yyval.bfnode) = new BFNode;
   (yyval.bfnode)->node_type = EForm;
   (yyval.bfnode)->groupAgent = *(yyvsp[(4) - (8)].str_list);
   (yyval.bfnode)->bfnode1 = (yyvsp[(7) - (8)].bfnode);
;}
    break;

  case 51:
#line 428 "lcp.y"
    {
   (yyval.bfnode) = new BFNode;
   (yyval.bfnode)->node_type = CForm;
   (yyval.bfnode)->groupAgent = *(yyvsp[(4) - (8)].str_list);
   (yyval.bfnode)->bfnode1 = (yyvsp[(7) - (8)].bfnode);
;}
    break;

  case 52:
#line 439 "lcp.y"
    {
  (yyval.str_list2) = new StringList2;
;}
    break;

  case 53:
#line 443 "lcp.y"
    {
  (yyval.str_list2) = (yyvsp[(2) - (2)].str_list2);
;}
    break;

  case 54:
#line 456 "lcp.y"
    {
  (yyval.prop) = new Proposition;
  (yyval.prop)->n_type = STATIC;
  (yyval.prop)->precond = *(yyvsp[(2) - (3)].str_list);
  (yyval.prop)->effect = *(yyvsp[(1) - (3)].str_list);
;}
    break;

  case 55:
#line 466 "lcp.y"
    {  
  (yyval.prop) = new Proposition;
  (yyval.prop)->n_type = DYNAMIC;
  (yyval.prop)->act_name = *(yyvsp[(1) - (5)].str_val);
  (yyval.prop)->bel_form = *(yyvsp[(4) - (5)].bfnode);
  (yyval.prop)->effect = *(yyvsp[(3) - (5)].str_list);
;}
    break;

  case 56:
#line 477 "lcp.y"
    {
  (yyval.prop) = new Proposition;
  (yyval.prop)->n_type = EXECUTABILITY;
  (yyval.prop)->act_name = *(yyvsp[(2) - (4)].str_val);
  (yyval.prop)->bel_form = *(yyvsp[(3) - (4)].bfnode);
;}
    break;

  case 57:
#line 487 "lcp.y"
    {
  (yyval.prop) = new Proposition;
  (yyval.prop)->n_type = DETERMINATION;
  (yyval.prop)->act_name = *(yyvsp[(1) - (4)].str_val);
  (yyval.prop)->effect = *(yyvsp[(3) - (4)].str_list); 
;}
    break;

  case 58:
#line 497 "lcp.y"
    {
  (yyval.prop) = new Proposition;
  (yyval.prop)->n_type = ANNOUNCEMENT;
  (yyval.prop)->act_name = *(yyvsp[(1) - (4)].str_val);
  (yyval.prop)->flu_form = *(yyvsp[(3) - (4)].str_list2);
;}
    break;

  case 59:
#line 507 "lcp.y"
    {
  (yyval.prop) = new Proposition;
  (yyval.prop)->n_type = AWARENESS;
  (yyval.prop)->act_name = *(yyvsp[(3) - (5)].str_val);
  (yyval.prop)->agentPro = *(yyvsp[(1) - (5)].str_val);
  (yyval.prop)->flu_form = *(yyvsp[(4) - (5)].str_list2);
;}
    break;

  case 60:
#line 518 "lcp.y"
    {
  (yyval.prop) = new Proposition;
  (yyval.prop)->n_type = OBSERVANCE;
  (yyval.prop)->act_name = *(yyvsp[(3) - (5)].str_val);				
  (yyval.prop)->agentPro = *(yyvsp[(1) - (5)].str_val);
  (yyval.prop)->flu_form = *(yyvsp[(4) - (5)].str_list2);
;}
    break;

  case 61:
#line 529 "lcp.y"
    {
  (yyval.prop) = new Proposition;
  (yyval.prop)->n_type = IMPOSSIBILITY;
  (yyval.prop)->act_name = *(yyvsp[(2) - (4)].str_val);
  (yyval.prop)->precond = *(yyvsp[(3) - (4)].str_list);
;}
    break;

  case 62:
#line 538 "lcp.y"
    {
  (yyval.prop) = (yyvsp[(1) - (1)].prop);
;}
    break;

  case 63:
#line 543 "lcp.y"
    {
  (yyval.prop) = (yyvsp[(1) - (1)].prop);
;}
    break;

  case 64:
#line 548 "lcp.y"
    {
  (yyval.prop) = (yyvsp[(1) - (1)].prop);
;}
    break;

  case 65:
#line 553 "lcp.y"
    {
  (yyval.prop) = (yyvsp[(1) - (1)].prop);
;}
    break;

  case 66:
#line 558 "lcp.y"
    {
  (yyval.prop) = (yyvsp[(1) - (1)].prop);
;}
    break;

  case 67:
#line 563 "lcp.y"
    {
  (yyval.prop) = (yyvsp[(1) - (1)].prop);
;}
    break;

  case 68:
#line 568 "lcp.y"
    {
  (yyval.prop) = (yyvsp[(1) - (1)].prop);
;}
    break;

  case 69:
#line 573 "lcp.y"
    {
  (yyval.prop) = (yyvsp[(1) - (1)].prop);
;}
    break;

  case 70:
#line 581 "lcp.y"
    {
  (yyval.prop_list) = new PropositionList;
;}
    break;

  case 71:
#line 585 "lcp.y"
    {
  (yyval.prop_list) = (yyvsp[(1) - (2)].prop_list);
  (yyvsp[(1) - (2)].prop_list)->push_back(*(yyvsp[(2) - (2)].prop));
;}
    break;

  case 72:
#line 594 "lcp.y"
    {
  (yyval.bfnode) = (yyvsp[(2) - (3)].bfnode);
;}
    break;

  case 73:
#line 600 "lcp.y"
    {
  (yyval.init_nodes) = new Nodes;
  //$$->insert(bfnode());
  //$$ = new StringList2;
  //$$->insert(StringList());
;}
    break;

  case 74:
#line 607 "lcp.y"
    {
  (yyval.init_nodes) = (yyvsp[(1) - (2)].init_nodes);
  (yyval.init_nodes)->push_back(*(yyvsp[(2) - (2)].bfnode));
;}
    break;

  case 75:
#line 652 "lcp.y"
    {
  (yyval.bfnode) = (yyvsp[(2) - (3)].bfnode);
;}
    break;

  case 76:
#line 658 "lcp.y"
    {
  (yyval.init_nodes) = new Nodes;
;}
    break;

  case 77:
#line 662 "lcp.y"
    {
  (yyval.init_nodes) = (yyvsp[(1) - (2)].init_nodes);
  (yyval.init_nodes)->push_back(*(yyvsp[(2) - (2)].bfnode));
;}
    break;


/* Line 1267 of yacc.c.  */
#line 2151 "lcp.tab.c"
      default: break;
    }
  YY_SYMBOL_PRINT ("-> $$ =", yyr1[yyn], &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);

  *++yyvsp = yyval;


  /* Now `shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */

  yyn = yyr1[yyn];

  yystate = yypgoto[yyn - YYNTOKENS] + *yyssp;
  if (0 <= yystate && yystate <= YYLAST && yycheck[yystate] == *yyssp)
    yystate = yytable[yystate];
  else
    yystate = yydefgoto[yyn - YYNTOKENS];

  goto yynewstate;


/*------------------------------------.
| yyerrlab -- here on detecting error |
`------------------------------------*/
yyerrlab:
  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
#if ! YYERROR_VERBOSE
      yyerror (YY_("syntax error"));
#else
      {
	YYSIZE_T yysize = yysyntax_error (0, yystate, yychar);
	if (yymsg_alloc < yysize && yymsg_alloc < YYSTACK_ALLOC_MAXIMUM)
	  {
	    YYSIZE_T yyalloc = 2 * yysize;
	    if (! (yysize <= yyalloc && yyalloc <= YYSTACK_ALLOC_MAXIMUM))
	      yyalloc = YYSTACK_ALLOC_MAXIMUM;
	    if (yymsg != yymsgbuf)
	      YYSTACK_FREE (yymsg);
	    yymsg = (char *) YYSTACK_ALLOC (yyalloc);
	    if (yymsg)
	      yymsg_alloc = yyalloc;
	    else
	      {
		yymsg = yymsgbuf;
		yymsg_alloc = sizeof yymsgbuf;
	      }
	  }

	if (0 < yysize && yysize <= yymsg_alloc)
	  {
	    (void) yysyntax_error (yymsg, yystate, yychar);
	    yyerror (yymsg);
	  }
	else
	  {
	    yyerror (YY_("syntax error"));
	    if (yysize != 0)
	      goto yyexhaustedlab;
	  }
      }
#endif
    }



  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse look-ahead token after an
	 error, discard it.  */

      if (yychar <= YYEOF)
	{
	  /* Return failure if at end of input.  */
	  if (yychar == YYEOF)
	    YYABORT;
	}
      else
	{
	  yydestruct ("Error: discarding",
		      yytoken, &yylval);
	  yychar = YYEMPTY;
	}
    }

  /* Else will try to reuse look-ahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
yyerrorlab:

  /* Pacify compilers like GCC when the user code never invokes
     YYERROR and the label yyerrorlab therefore never appears in user
     code.  */
  if (/*CONSTCOND*/ 0)
     goto yyerrorlab;

  /* Do not reclaim the symbols of the rule which action triggered
     this YYERROR.  */
  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);
  yystate = *yyssp;
  goto yyerrlab1;


/*-------------------------------------------------------------.
| yyerrlab1 -- common code for both syntax error and YYERROR.  |
`-------------------------------------------------------------*/
yyerrlab1:
  yyerrstatus = 3;	/* Each real token shifted decrements this.  */

  for (;;)
    {
      yyn = yypact[yystate];
      if (yyn != YYPACT_NINF)
	{
	  yyn += YYTERROR;
	  if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYTERROR)
	    {
	      yyn = yytable[yyn];
	      if (0 < yyn)
		break;
	    }
	}

      /* Pop the current state because it cannot handle the error token.  */
      if (yyssp == yyss)
	YYABORT;


      yydestruct ("Error: popping",
		  yystos[yystate], yyvsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  if (yyn == YYFINAL)
    YYACCEPT;

  *++yyvsp = yylval;


  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", yystos[yyn], yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturn;

/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturn;

#ifndef yyoverflow
/*-------------------------------------------------.
| yyexhaustedlab -- memory exhaustion comes here.  |
`-------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  /* Fall through.  */
#endif

yyreturn:
  if (yychar != YYEOF && yychar != YYEMPTY)
     yydestruct ("Cleanup: discarding lookahead",
		 yytoken, &yylval);
  /* Do not reclaim the symbols of the rule which action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
		  yystos[*yyssp], yyvsp);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif
#if YYERROR_VERBOSE
  if (yymsg != yymsgbuf)
    YYSTACK_FREE (yymsg);
#endif
  /* Make sure YYID is used.  */
  return YYID (yyresult);
}


#line 666 "lcp.y"


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

