/* A Bison parser, made by GNU Bison 3.0.4.  */

/* Bison implementation for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015 Free Software Foundation, Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

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
#define YYBISON_VERSION "3.0.4"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1




/* Copy the first part of user declarations.  */
#line 2 "lcp.y" /* yacc.c:339  */

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


#line 85 "lcp.tab.c" /* yacc.c:339  */

# ifndef YY_NULLPTR
#  if defined __cplusplus && 201103L <= __cplusplus
#   define YY_NULLPTR nullptr
#  else
#   define YY_NULLPTR 0
#  endif
# endif

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE 0
#endif

/* In a future release of Bison, this section will be replaced
   by #include "lcp.tab.h".  */
#ifndef YY_YY_LCP_TAB_H_INCLUDED
# define YY_YY_LCP_TAB_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Token type.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
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

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED

union YYSTYPE
{
#line 21 "lcp.y" /* yacc.c:355  */

  string*	str_val;
  StringList*  str_list; 
  StringList2* str_list2;
  Proposition* prop;
  PropositionList* prop_list;
  BFNode* bfnode;
  Nodes* init_nodes;

#line 164 "lcp.tab.c" /* yacc.c:355  */
};

typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (void);

#endif /* !YY_YY_LCP_TAB_H_INCLUDED  */

/* Copy the second part of user declarations.  */

#line 181 "lcp.tab.c" /* yacc.c:358  */

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
#else
typedef signed char yytype_int8;
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
# elif ! defined YYSIZE_T
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
#   define YY_(Msgid) dgettext ("bison-runtime", Msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(Msgid) Msgid
# endif
#endif

#ifndef YY_ATTRIBUTE
# if (defined __GNUC__                                               \
      && (2 < __GNUC__ || (__GNUC__ == 2 && 96 <= __GNUC_MINOR__)))  \
     || defined __SUNPRO_C && 0x5110 <= __SUNPRO_C
#  define YY_ATTRIBUTE(Spec) __attribute__(Spec)
# else
#  define YY_ATTRIBUTE(Spec) /* empty */
# endif
#endif

#ifndef YY_ATTRIBUTE_PURE
# define YY_ATTRIBUTE_PURE   YY_ATTRIBUTE ((__pure__))
#endif

#ifndef YY_ATTRIBUTE_UNUSED
# define YY_ATTRIBUTE_UNUSED YY_ATTRIBUTE ((__unused__))
#endif

#if !defined _Noreturn \
     && (!defined __STDC_VERSION__ || __STDC_VERSION__ < 201112)
# if defined _MSC_VER && 1200 <= _MSC_VER
#  define _Noreturn __declspec (noreturn)
# else
#  define _Noreturn YY_ATTRIBUTE ((__noreturn__))
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YYUSE(E) ((void) (E))
#else
# define YYUSE(E) /* empty */
#endif

#if defined __GNUC__ && 407 <= __GNUC__ * 100 + __GNUC_MINOR__
/* Suppress an incorrect diagnostic about yylval being uninitialized.  */
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN \
    _Pragma ("GCC diagnostic push") \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")\
    _Pragma ("GCC diagnostic ignored \"-Wmaybe-uninitialized\"")
# define YY_IGNORE_MAYBE_UNINITIALIZED_END \
    _Pragma ("GCC diagnostic pop")
#else
# define YY_INITIAL_VALUE(Value) Value
#endif
#ifndef YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_END
#endif
#ifndef YY_INITIAL_VALUE
# define YY_INITIAL_VALUE(Value) /* Nothing. */
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
#    if ! defined _ALLOCA_H && ! defined EXIT_SUCCESS
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
      /* Use EXIT_SUCCESS as a witness for stdlib.h.  */
#     ifndef EXIT_SUCCESS
#      define EXIT_SUCCESS 0
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's 'empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (0)
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
#  if (defined __cplusplus && ! defined EXIT_SUCCESS \
       && ! ((defined YYMALLOC || defined malloc) \
             && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef EXIT_SUCCESS
#    define EXIT_SUCCESS 0
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined EXIT_SUCCESS
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined EXIT_SUCCESS
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
  yytype_int16 yyss_alloc;
  YYSTYPE yyvs_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (sizeof (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (sizeof (yytype_int16) + sizeof (YYSTYPE)) \
      + YYSTACK_GAP_MAXIMUM)

# define YYCOPY_NEEDED 1

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack_alloc, Stack)                           \
    do                                                                  \
      {                                                                 \
        YYSIZE_T yynewbytes;                                            \
        YYCOPY (&yyptr->Stack_alloc, Stack, yysize);                    \
        Stack = &yyptr->Stack_alloc;                                    \
        yynewbytes = yystacksize * sizeof (*Stack) + YYSTACK_GAP_MAXIMUM; \
        yyptr += yynewbytes / sizeof (*yyptr);                          \
      }                                                                 \
    while (0)

#endif

#if defined YYCOPY_NEEDED && YYCOPY_NEEDED
/* Copy COUNT objects from SRC to DST.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(Dst, Src, Count) \
      __builtin_memcpy (Dst, Src, (Count) * sizeof (*(Src)))
#  else
#   define YYCOPY(Dst, Src, Count)              \
      do                                        \
        {                                       \
          YYSIZE_T yyi;                         \
          for (yyi = 0; yyi < (Count); yyi++)   \
            (Dst)[yyi] = (Src)[yyi];            \
        }                                       \
      while (0)
#  endif
# endif
#endif /* !YYCOPY_NEEDED */

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
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  157

/* YYTRANSLATE[YYX] -- Symbol number corresponding to YYX as returned
   by yylex, with out-of-bounds checking.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   283

#define YYTRANSLATE(YYX)                                                \
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, without out-of-bounds checking.  */
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
  /* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
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

#if YYDEBUG || YYERROR_VERBOSE || 0
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
  "proposition", "domain", "init", "init_spec", "goal", "goal_spec", YY_NULLPTR
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[NUM] -- (External) token number corresponding to the
   (internal) symbol number NUM (which must be that of a token).  */
static const yytype_uint16 yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   282,   283
};
# endif

#define YYPACT_NINF -74

#define yypact_value_is_default(Yystate) \
  (!!((Yystate) == (-74)))

#define YYTABLE_NINF -35

#define yytable_value_is_error(Yytable_value) \
  0

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
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

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
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

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
     -74,   -74,   -11,   -74,   130,   -24,    -3,   -74,   -74,     2,
     -14,   -21,   -74,   -74,   -16,   -74,   -74,   -74,   -20,    19,
     -74,   -74,   105,    95,   -73,    89,   -74,   -74,   -74,   -74,
     -74,   -74,   -74,   -74,   -74,   -74,   -74,   -74,   -74,   -74
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int8 yydefgoto[] =
{
      -1,     1,     8,    25,    26,    27,    39,    89,    10,    91,
      41,    99,     5,     2,    18,    19,    12,     6,    33,    34,
      21,    13,    67,    81,   100,   115,    44,    45,    46,    47,
      48,    49,    50,    51,    52,    22,    74,    53,   102,    75
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
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

  /* YYR2[YYN] -- Number of symbols on the right hand side of rule YYN.  */
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


#define yyerrok         (yyerrstatus = 0)
#define yyclearin       (yychar = YYEMPTY)
#define YYEMPTY         (-2)
#define YYEOF           0

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab


#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)                                  \
do                                                              \
  if (yychar == YYEMPTY)                                        \
    {                                                           \
      yychar = (Token);                                         \
      yylval = (Value);                                         \
      YYPOPSTACK (yylen);                                       \
      yystate = *yyssp;                                         \
      goto yybackup;                                            \
    }                                                           \
  else                                                          \
    {                                                           \
      yyerror (YY_("syntax error: cannot back up")); \
      YYERROR;                                                  \
    }                                                           \
while (0)

/* Error token number */
#define YYTERROR        1
#define YYERRCODE       256



/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)                        \
do {                                            \
  if (yydebug)                                  \
    YYFPRINTF Args;                             \
} while (0)

/* This macro is provided for backward compatibility. */
#ifndef YY_LOCATION_PRINT
# define YY_LOCATION_PRINT(File, Loc) ((void) 0)
#endif


# define YY_SYMBOL_PRINT(Title, Type, Value, Location)                    \
do {                                                                      \
  if (yydebug)                                                            \
    {                                                                     \
      YYFPRINTF (stderr, "%s ", Title);                                   \
      yy_symbol_print (stderr,                                            \
                  Type, Value); \
      YYFPRINTF (stderr, "\n");                                           \
    }                                                                     \
} while (0)


/*----------------------------------------.
| Print this symbol's value on YYOUTPUT.  |
`----------------------------------------*/

static void
yy_symbol_value_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
{
  FILE *yyo = yyoutput;
  YYUSE (yyo);
  if (!yyvaluep)
    return;
# ifdef YYPRINT
  if (yytype < YYNTOKENS)
    YYPRINT (yyoutput, yytoknum[yytype], *yyvaluep);
# endif
  YYUSE (yytype);
}


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

static void
yy_symbol_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
{
  YYFPRINTF (yyoutput, "%s %s (",
             yytype < YYNTOKENS ? "token" : "nterm", yytname[yytype]);

  yy_symbol_value_print (yyoutput, yytype, yyvaluep);
  YYFPRINTF (yyoutput, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

static void
yy_stack_print (yytype_int16 *yybottom, yytype_int16 *yytop)
{
  YYFPRINTF (stderr, "Stack now");
  for (; yybottom <= yytop; yybottom++)
    {
      int yybot = *yybottom;
      YYFPRINTF (stderr, " %d", yybot);
    }
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)                            \
do {                                                            \
  if (yydebug)                                                  \
    yy_stack_print ((Bottom), (Top));                           \
} while (0)


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

static void
yy_reduce_print (yytype_int16 *yyssp, YYSTYPE *yyvsp, int yyrule)
{
  unsigned long int yylno = yyrline[yyrule];
  int yynrhs = yyr2[yyrule];
  int yyi;
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %lu):\n",
             yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr,
                       yystos[yyssp[yyi + 1 - yynrhs]],
                       &(yyvsp[(yyi + 1) - (yynrhs)])
                                              );
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)          \
do {                                    \
  if (yydebug)                          \
    yy_reduce_print (yyssp, yyvsp, Rule); \
} while (0)

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
#ifndef YYINITDEPTH
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
static YYSIZE_T
yystrlen (const char *yystr)
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
static char *
yystpcpy (char *yydest, const char *yysrc)
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

/* Copy into *YYMSG, which is of size *YYMSG_ALLOC, an error message
   about the unexpected token YYTOKEN for the state stack whose top is
   YYSSP.

   Return 0 if *YYMSG was successfully written.  Return 1 if *YYMSG is
   not large enough to hold the message.  In that case, also set
   *YYMSG_ALLOC to the required number of bytes.  Return 2 if the
   required number of bytes is too large to store.  */
static int
yysyntax_error (YYSIZE_T *yymsg_alloc, char **yymsg,
                yytype_int16 *yyssp, int yytoken)
{
  YYSIZE_T yysize0 = yytnamerr (YY_NULLPTR, yytname[yytoken]);
  YYSIZE_T yysize = yysize0;
  enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
  /* Internationalized format string. */
  const char *yyformat = YY_NULLPTR;
  /* Arguments of yyformat. */
  char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];
  /* Number of reported tokens (one for the "unexpected", one per
     "expected"). */
  int yycount = 0;

  /* There are many possibilities here to consider:
     - If this state is a consistent state with a default action, then
       the only way this function was invoked is if the default action
       is an error action.  In that case, don't check for expected
       tokens because there are none.
     - The only way there can be no lookahead present (in yychar) is if
       this state is a consistent state with a default action.  Thus,
       detecting the absence of a lookahead is sufficient to determine
       that there is no unexpected or expected token to report.  In that
       case, just report a simple "syntax error".
     - Don't assume there isn't a lookahead just because this state is a
       consistent state with a default action.  There might have been a
       previous inconsistent state, consistent state with a non-default
       action, or user semantic action that manipulated yychar.
     - Of course, the expected token list depends on states to have
       correct lookahead information, and it depends on the parser not
       to perform extra reductions after fetching a lookahead from the
       scanner and before detecting a syntax error.  Thus, state merging
       (from LALR or IELR) and default reductions corrupt the expected
       token list.  However, the list is correct for canonical LR with
       one exception: it will still contain any token that will not be
       accepted due to an error action in a later state.
  */
  if (yytoken != YYEMPTY)
    {
      int yyn = yypact[*yyssp];
      yyarg[yycount++] = yytname[yytoken];
      if (!yypact_value_is_default (yyn))
        {
          /* Start YYX at -YYN if negative to avoid negative indexes in
             YYCHECK.  In other words, skip the first -YYN actions for
             this state because they are default actions.  */
          int yyxbegin = yyn < 0 ? -yyn : 0;
          /* Stay within bounds of both yycheck and yytname.  */
          int yychecklim = YYLAST - yyn + 1;
          int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
          int yyx;

          for (yyx = yyxbegin; yyx < yyxend; ++yyx)
            if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR
                && !yytable_value_is_error (yytable[yyx + yyn]))
              {
                if (yycount == YYERROR_VERBOSE_ARGS_MAXIMUM)
                  {
                    yycount = 1;
                    yysize = yysize0;
                    break;
                  }
                yyarg[yycount++] = yytname[yyx];
                {
                  YYSIZE_T yysize1 = yysize + yytnamerr (YY_NULLPTR, yytname[yyx]);
                  if (! (yysize <= yysize1
                         && yysize1 <= YYSTACK_ALLOC_MAXIMUM))
                    return 2;
                  yysize = yysize1;
                }
              }
        }
    }

  switch (yycount)
    {
# define YYCASE_(N, S)                      \
      case N:                               \
        yyformat = S;                       \
      break
      YYCASE_(0, YY_("syntax error"));
      YYCASE_(1, YY_("syntax error, unexpected %s"));
      YYCASE_(2, YY_("syntax error, unexpected %s, expecting %s"));
      YYCASE_(3, YY_("syntax error, unexpected %s, expecting %s or %s"));
      YYCASE_(4, YY_("syntax error, unexpected %s, expecting %s or %s or %s"));
      YYCASE_(5, YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s"));
# undef YYCASE_
    }

  {
    YYSIZE_T yysize1 = yysize + yystrlen (yyformat);
    if (! (yysize <= yysize1 && yysize1 <= YYSTACK_ALLOC_MAXIMUM))
      return 2;
    yysize = yysize1;
  }

  if (*yymsg_alloc < yysize)
    {
      *yymsg_alloc = 2 * yysize;
      if (! (yysize <= *yymsg_alloc
             && *yymsg_alloc <= YYSTACK_ALLOC_MAXIMUM))
        *yymsg_alloc = YYSTACK_ALLOC_MAXIMUM;
      return 1;
    }

  /* Avoid sprintf, as that infringes on the user's name space.
     Don't have undefined behavior even if the translation
     produced a string with the wrong number of "%s"s.  */
  {
    char *yyp = *yymsg;
    int yyi = 0;
    while ((*yyp = *yyformat) != '\0')
      if (*yyp == '%' && yyformat[1] == 's' && yyi < yycount)
        {
          yyp += yytnamerr (yyp, yyarg[yyi++]);
          yyformat += 2;
        }
      else
        {
          yyp++;
          yyformat++;
        }
  }
  return 0;
}
#endif /* YYERROR_VERBOSE */

/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

static void
yydestruct (const char *yymsg, int yytype, YYSTYPE *yyvaluep)
{
  YYUSE (yyvaluep);
  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yytype, yyvaluep, yylocationp);

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YYUSE (yytype);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}




/* The lookahead symbol.  */
int yychar;

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval;
/* Number of syntax errors so far.  */
int yynerrs;


/*----------.
| yyparse.  |
`----------*/

int
yyparse (void)
{
    int yystate;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus;

    /* The stacks and their tools:
       'yyss': related to states.
       'yyvs': related to semantic values.

       Refer to the stacks through separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* The state stack.  */
    yytype_int16 yyssa[YYINITDEPTH];
    yytype_int16 *yyss;
    yytype_int16 *yyssp;

    /* The semantic value stack.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs;
    YYSTYPE *yyvsp;

    YYSIZE_T yystacksize;

  int yyn;
  int yyresult;
  /* Lookahead token as an internal (translated) token number.  */
  int yytoken = 0;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;

#if YYERROR_VERBOSE
  /* Buffer for error messages, and its allocated size.  */
  char yymsgbuf[128];
  char *yymsg = yymsgbuf;
  YYSIZE_T yymsg_alloc = sizeof yymsgbuf;
#endif

#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  yyssp = yyss = yyssa;
  yyvsp = yyvs = yyvsa;
  yystacksize = YYINITDEPTH;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY; /* Cause a token to be read.  */
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
        YYSTACK_RELOCATE (yyss_alloc, yyss);
        YYSTACK_RELOCATE (yyvs_alloc, yyvs);
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

  if (yystate == YYFINAL)
    YYACCEPT;

  goto yybackup;

/*-----------.
| yybackup.  |
`-----------*/
yybackup:

  /* Do appropriate processing given the current state.  Read a
     lookahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to lookahead token.  */
  yyn = yypact[yystate];
  if (yypact_value_is_default (yyn))
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* YYCHAR is either YYEMPTY or YYEOF or a valid lookahead symbol.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token: "));
      yychar = yylex ();
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
      if (yytable_value_is_error (yyn))
        goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  /* Shift the lookahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);

  /* Discard the shifted token.  */
  yychar = YYEMPTY;

  yystate = yyn;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END

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
     '$$ = $1'.

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
#line 121 "lcp.y" /* yacc.c:1646  */
    { 
  reader.m_fluents = *(yyvsp[-5].str_list);
  reader.m_actions = *(yyvsp[-4].str_list);
  reader.m_agents = *(yyvsp[-3].str_list);
  reader.m_propositions = *(yyvsp[-2].prop_list);
  reader.k_init = *(yyvsp[-1].init_nodes);
  reader.m_gd = *(yyvsp[0].init_nodes);
}
#line 1376 "lcp.tab.c" /* yacc.c:1646  */
    break;

  case 4:
#line 132 "lcp.y" /* yacc.c:1646  */
    {
  (yyval.str_val) = (yyvsp[0].str_val);
}
#line 1384 "lcp.tab.c" /* yacc.c:1646  */
    break;

  case 5:
#line 138 "lcp.y" /* yacc.c:1646  */
    {
  (yyval.str_val) = (yyvsp[0].str_val);
}
#line 1392 "lcp.tab.c" /* yacc.c:1646  */
    break;

  case 6:
#line 142 "lcp.y" /* yacc.c:1646  */
    {
  (yyval.str_val) = (yyvsp[0].str_val);
}
#line 1400 "lcp.tab.c" /* yacc.c:1646  */
    break;

  case 7:
#line 149 "lcp.y" /* yacc.c:1646  */
    {
  (yyval.str_val) = (yyvsp[0].str_val);
}
#line 1408 "lcp.tab.c" /* yacc.c:1646  */
    break;

  case 8:
#line 158 "lcp.y" /* yacc.c:1646  */
    {
  (yyval.str_val) = (yyvsp[0].str_val);
}
#line 1416 "lcp.tab.c" /* yacc.c:1646  */
    break;

  case 9:
#line 163 "lcp.y" /* yacc.c:1646  */
    {
  (yyval.str_val) = new string(*(yyvsp[-2].str_val) + "," + *(yyvsp[0].str_val));
}
#line 1424 "lcp.tab.c" /* yacc.c:1646  */
    break;

  case 10:
#line 170 "lcp.y" /* yacc.c:1646  */
    {
  (yyval.str_val) = (yyvsp[0].str_val);
}
#line 1432 "lcp.tab.c" /* yacc.c:1646  */
    break;

  case 11:
#line 175 "lcp.y" /* yacc.c:1646  */
    {
  (yyval.str_val) = new std::string(*(yyvsp[-3].str_val) + "(" + *(yyvsp[-1].str_val) + ")");
}
#line 1440 "lcp.tab.c" /* yacc.c:1646  */
    break;

  case 12:
#line 180 "lcp.y" /* yacc.c:1646  */
    {
  (yyval.str_list) = new StringList;
  (yyval.str_list)->insert(*(yyvsp[0].str_val));
}
#line 1449 "lcp.tab.c" /* yacc.c:1646  */
    break;

  case 13:
#line 186 "lcp.y" /* yacc.c:1646  */
    {
  (yyval.str_list) = new StringList;
  (yyval.str_list)->insert(*(yyvsp[0].str_val));
}
#line 1458 "lcp.tab.c" /* yacc.c:1646  */
    break;

  case 14:
#line 191 "lcp.y" /* yacc.c:1646  */
    {
  (yyval.str_list) = (yyvsp[-2].str_list);
  (yyval.str_list)->insert(*(yyvsp[0].str_val));
}
#line 1467 "lcp.tab.c" /* yacc.c:1646  */
    break;

  case 15:
#line 198 "lcp.y" /* yacc.c:1646  */
    {
  (yyval.str_val) = (yyvsp[0].str_val);
}
#line 1475 "lcp.tab.c" /* yacc.c:1646  */
    break;

  case 16:
#line 203 "lcp.y" /* yacc.c:1646  */
    {
  (yyval.str_val) = new std::string(*(yyvsp[-1].str_val) + *(yyvsp[0].str_val));
}
#line 1483 "lcp.tab.c" /* yacc.c:1646  */
    break;

  case 17:
#line 209 "lcp.y" /* yacc.c:1646  */
    {
  (yyval.str_list) = new StringList;
  (yyval.str_list)->insert(*(yyvsp[0].str_val));
}
#line 1492 "lcp.tab.c" /* yacc.c:1646  */
    break;

  case 18:
#line 214 "lcp.y" /* yacc.c:1646  */
    {
  (yyval.str_list) = (yyvsp[-2].str_list);  
  (yyval.str_list)->insert(*(yyvsp[0].str_val));
}
#line 1501 "lcp.tab.c" /* yacc.c:1646  */
    break;

  case 19:
#line 220 "lcp.y" /* yacc.c:1646  */
    {
  StringList s1;

  (yyval.str_list2) = new StringList2;

  s1.insert(*(yyvsp[0].str_val));

  (yyval.str_list2)->insert(s1);
}
#line 1515 "lcp.tab.c" /* yacc.c:1646  */
    break;

  case 20:
#line 230 "lcp.y" /* yacc.c:1646  */
    {
  StringList2::iterator it1;
  StringList2::iterator it2;
  StringList ns;

  (yyval.str_list2) = new StringList2;

  for (it2 = (yyvsp[-2].str_list2)->begin(); it2 != (yyvsp[-2].str_list2)->end(); it2++) {
    for (it1 = (yyvsp[0].str_list2)->begin(); it1 != (yyvsp[0].str_list2)->end(); it1++){
	  if (is_consistent(*it2,*it1)) {
		ns = *it2;
		ns.insert(it1->begin(),it1->end());
		(yyval.str_list2)->insert(ns);
	  }
    }
  }  
}
#line 1537 "lcp.tab.c" /* yacc.c:1646  */
    break;

  case 21:
#line 247 "lcp.y" /* yacc.c:1646  */
    {
  (yyval.str_list2) = (yyvsp[-2].str_list2);
  (yyval.str_list2)->insert((yyvsp[0].str_list2)->begin(),(yyvsp[0].str_list2)->end());
}
#line 1546 "lcp.tab.c" /* yacc.c:1646  */
    break;

  case 22:
#line 252 "lcp.y" /* yacc.c:1646  */
    {
  (yyval.str_list2) = (yyvsp[-1].str_list2);
}
#line 1554 "lcp.tab.c" /* yacc.c:1646  */
    break;

  case 23:
#line 258 "lcp.y" /* yacc.c:1646  */
    {
  (yyval.str_list) = (yyvsp[-1].str_list);
}
#line 1562 "lcp.tab.c" /* yacc.c:1646  */
    break;

  case 24:
#line 264 "lcp.y" /* yacc.c:1646  */
    {
  (yyval.str_list) = new StringList;
}
#line 1570 "lcp.tab.c" /* yacc.c:1646  */
    break;

  case 25:
#line 269 "lcp.y" /* yacc.c:1646  */
    {
  (yyvsp[-1].str_list)->insert((yyvsp[0].str_list)->begin(),(yyvsp[0].str_list)->end());
  (yyval.str_list) = (yyvsp[-1].str_list);
}
#line 1579 "lcp.tab.c" /* yacc.c:1646  */
    break;

  case 26:
#line 277 "lcp.y" /* yacc.c:1646  */
    {
  (yyval.str_val) = new string(*(yyvsp[0].str_val));
}
#line 1587 "lcp.tab.c" /* yacc.c:1646  */
    break;

  case 27:
#line 281 "lcp.y" /* yacc.c:1646  */
    {
  (yyval.str_val) = new string(*(yyvsp[-3].str_val) + "(" + *(yyvsp[-1].str_val) + ")");
}
#line 1595 "lcp.tab.c" /* yacc.c:1646  */
    break;

  case 28:
#line 286 "lcp.y" /* yacc.c:1646  */
    {
  (yyval.str_list) = new StringList;
  (yyval.str_list)->insert(*(yyvsp[0].str_val));
}
#line 1604 "lcp.tab.c" /* yacc.c:1646  */
    break;

  case 29:
#line 291 "lcp.y" /* yacc.c:1646  */
    {
  (yyval.str_list) = (yyvsp[-2].str_list);
  (yyval.str_list)->insert(*(yyvsp[0].str_val));
}
#line 1613 "lcp.tab.c" /* yacc.c:1646  */
    break;

  case 30:
#line 297 "lcp.y" /* yacc.c:1646  */
    {
  (yyval.str_list) = (yyvsp[-1].str_list);
}
#line 1621 "lcp.tab.c" /* yacc.c:1646  */
    break;

  case 31:
#line 303 "lcp.y" /* yacc.c:1646  */
    {
  (yyval.str_list) = new StringList;
}
#line 1629 "lcp.tab.c" /* yacc.c:1646  */
    break;

  case 32:
#line 308 "lcp.y" /* yacc.c:1646  */
    {
  (yyvsp[-1].str_list)->insert((yyvsp[0].str_list)->begin(),(yyvsp[0].str_list)->end());
  (yyval.str_list) = (yyvsp[-1].str_list);
}
#line 1638 "lcp.tab.c" /* yacc.c:1646  */
    break;

  case 33:
#line 315 "lcp.y" /* yacc.c:1646  */
    {
  (yyval.str_val) = new string(*(yyvsp[0].str_val));
}
#line 1646 "lcp.tab.c" /* yacc.c:1646  */
    break;

  case 34:
#line 319 "lcp.y" /* yacc.c:1646  */
    {
  (yyval.str_val) = new string(*(yyvsp[-3].str_val) + "(" + *(yyvsp[-1].str_val) + ")");
}
#line 1654 "lcp.tab.c" /* yacc.c:1646  */
    break;

  case 35:
#line 324 "lcp.y" /* yacc.c:1646  */
    {
  (yyval.str_list) = new StringList;
  (yyval.str_list)->insert(*(yyvsp[0].str_val));
}
#line 1663 "lcp.tab.c" /* yacc.c:1646  */
    break;

  case 36:
#line 329 "lcp.y" /* yacc.c:1646  */
    {
  (yyval.str_list) = (yyvsp[-2].str_list);
  (yyval.str_list)->insert(*(yyvsp[0].str_val));
}
#line 1672 "lcp.tab.c" /* yacc.c:1646  */
    break;

  case 37:
#line 335 "lcp.y" /* yacc.c:1646  */
    {
  (yyval.str_list) = (yyvsp[-1].str_list);
}
#line 1680 "lcp.tab.c" /* yacc.c:1646  */
    break;

  case 38:
#line 341 "lcp.y" /* yacc.c:1646  */
    {
  (yyval.str_list) = new StringList;
}
#line 1688 "lcp.tab.c" /* yacc.c:1646  */
    break;

  case 39:
#line 346 "lcp.y" /* yacc.c:1646  */
    {
  (yyvsp[-1].str_list)->insert((yyvsp[0].str_list)->begin(),(yyvsp[0].str_list)->end());
  (yyval.str_list) = (yyvsp[-1].str_list);
}
#line 1697 "lcp.tab.c" /* yacc.c:1646  */
    break;

  case 40:
#line 355 "lcp.y" /* yacc.c:1646  */
    {
  (yyval.str_list) = new StringList;
}
#line 1705 "lcp.tab.c" /* yacc.c:1646  */
    break;

  case 41:
#line 359 "lcp.y" /* yacc.c:1646  */
    {
  (yyval.str_list) = (yyvsp[0].str_list);
}
#line 1713 "lcp.tab.c" /* yacc.c:1646  */
    break;

  case 42:
#line 366 "lcp.y" /* yacc.c:1646  */
    {
  (yyval.bfnode) = new BFNode;
  (yyval.bfnode)->node_type = BFEmpty;
}
#line 1722 "lcp.tab.c" /* yacc.c:1646  */
    break;

  case 43:
#line 371 "lcp.y" /* yacc.c:1646  */
    {
  (yyval.bfnode) = (yyvsp[0].bfnode);


}
#line 1732 "lcp.tab.c" /* yacc.c:1646  */
    break;

  case 44:
#line 378 "lcp.y" /* yacc.c:1646  */
    {  
    (yyval.bfnode) = new BFNode;
    (yyval.bfnode)->node_type = fluForm;
    (yyval.bfnode)->flu_form = *(yyvsp[0].str_list2);
}
#line 1742 "lcp.tab.c" /* yacc.c:1646  */
    break;

  case 45:
#line 384 "lcp.y" /* yacc.c:1646  */
    {
   (yyval.bfnode) = new BFNode;
   (yyval.bfnode)->node_type = BForm;
   (yyval.bfnode)->agentPro = *(yyvsp[-3].str_val);
   (yyval.bfnode)->bfnode1 = (yyvsp[-1].bfnode);
}
#line 1753 "lcp.tab.c" /* yacc.c:1646  */
    break;

  case 46:
#line 391 "lcp.y" /* yacc.c:1646  */
    {
   (yyval.bfnode) = new BFNode;
   (yyval.bfnode)->node_type = propForm;
   (yyval.bfnode)->bfnode1 = (yyvsp[-2].bfnode);
   (yyval.bfnode)->bfnode2 = (yyvsp[0].bfnode);
   (yyval.bfnode)->bfOperator = BFAND;
}
#line 1765 "lcp.tab.c" /* yacc.c:1646  */
    break;

  case 47:
#line 399 "lcp.y" /* yacc.c:1646  */
    {
   (yyval.bfnode) = new BFNode;
   (yyval.bfnode)->node_type = propForm;
   (yyval.bfnode)->bfnode1 = (yyvsp[-2].bfnode);
   (yyval.bfnode)->bfnode2 = (yyvsp[0].bfnode);
   (yyval.bfnode)->bfOperator = BFOR;
}
#line 1777 "lcp.tab.c" /* yacc.c:1646  */
    break;

  case 48:
#line 407 "lcp.y" /* yacc.c:1646  */
    {
   (yyval.bfnode) = new BFNode;
   (yyval.bfnode)->node_type = propForm;
   (yyval.bfnode)->bfnode1 = (yyvsp[-1].bfnode);
   (yyval.bfnode)->bfOperator = BFNOT;
}
#line 1788 "lcp.tab.c" /* yacc.c:1646  */
    break;

  case 49:
#line 414 "lcp.y" /* yacc.c:1646  */
    {
    (yyval.bfnode) = new BFNode;
    (yyval.bfnode)->node_type = propForm;
    (yyval.bfnode)->bfnode1 = (yyvsp[-1].bfnode);
    (yyval.bfnode)->bfOperator = NONE;
}
#line 1799 "lcp.tab.c" /* yacc.c:1646  */
    break;

  case 50:
#line 421 "lcp.y" /* yacc.c:1646  */
    {
   (yyval.bfnode) = new BFNode;
   (yyval.bfnode)->node_type = EForm;
   (yyval.bfnode)->groupAgent = *(yyvsp[-4].str_list);
   (yyval.bfnode)->bfnode1 = (yyvsp[-1].bfnode);
}
#line 1810 "lcp.tab.c" /* yacc.c:1646  */
    break;

  case 51:
#line 428 "lcp.y" /* yacc.c:1646  */
    {
   (yyval.bfnode) = new BFNode;
   (yyval.bfnode)->node_type = CForm;
   (yyval.bfnode)->groupAgent = *(yyvsp[-4].str_list);
   (yyval.bfnode)->bfnode1 = (yyvsp[-1].bfnode);
}
#line 1821 "lcp.tab.c" /* yacc.c:1646  */
    break;

  case 52:
#line 439 "lcp.y" /* yacc.c:1646  */
    {
  (yyval.str_list2) = new StringList2;
}
#line 1829 "lcp.tab.c" /* yacc.c:1646  */
    break;

  case 53:
#line 443 "lcp.y" /* yacc.c:1646  */
    {
  (yyval.str_list2) = (yyvsp[0].str_list2);
}
#line 1837 "lcp.tab.c" /* yacc.c:1646  */
    break;

  case 54:
#line 456 "lcp.y" /* yacc.c:1646  */
    {
  (yyval.prop) = new Proposition;
  (yyval.prop)->n_type = STATIC;
  (yyval.prop)->precond = *(yyvsp[-1].str_list);
  (yyval.prop)->effect = *(yyvsp[-2].str_list);
}
#line 1848 "lcp.tab.c" /* yacc.c:1646  */
    break;

  case 55:
#line 466 "lcp.y" /* yacc.c:1646  */
    {  
  (yyval.prop) = new Proposition;
  (yyval.prop)->n_type = DYNAMIC;
  (yyval.prop)->act_name = *(yyvsp[-4].str_val);
  (yyval.prop)->bel_form = *(yyvsp[-1].bfnode);
  (yyval.prop)->effect = *(yyvsp[-2].str_list);
}
#line 1860 "lcp.tab.c" /* yacc.c:1646  */
    break;

  case 56:
#line 477 "lcp.y" /* yacc.c:1646  */
    {
  (yyval.prop) = new Proposition;
  (yyval.prop)->n_type = EXECUTABILITY;
  (yyval.prop)->act_name = *(yyvsp[-2].str_val);
  (yyval.prop)->bel_form = *(yyvsp[-1].bfnode);
}
#line 1871 "lcp.tab.c" /* yacc.c:1646  */
    break;

  case 57:
#line 487 "lcp.y" /* yacc.c:1646  */
    {
  (yyval.prop) = new Proposition;
  (yyval.prop)->n_type = DETERMINATION;
  (yyval.prop)->act_name = *(yyvsp[-3].str_val);
  (yyval.prop)->effect = *(yyvsp[-1].str_list); 
}
#line 1882 "lcp.tab.c" /* yacc.c:1646  */
    break;

  case 58:
#line 497 "lcp.y" /* yacc.c:1646  */
    {
  (yyval.prop) = new Proposition;
  (yyval.prop)->n_type = ANNOUNCEMENT;
  (yyval.prop)->act_name = *(yyvsp[-3].str_val);
  (yyval.prop)->flu_form = *(yyvsp[-1].str_list2);
}
#line 1893 "lcp.tab.c" /* yacc.c:1646  */
    break;

  case 59:
#line 507 "lcp.y" /* yacc.c:1646  */
    {
  (yyval.prop) = new Proposition;
  (yyval.prop)->n_type = AWARENESS;
  (yyval.prop)->act_name = *(yyvsp[-2].str_val);
  (yyval.prop)->agentPro = *(yyvsp[-4].str_val);
  (yyval.prop)->flu_form = *(yyvsp[-1].str_list2);
}
#line 1905 "lcp.tab.c" /* yacc.c:1646  */
    break;

  case 60:
#line 518 "lcp.y" /* yacc.c:1646  */
    {
  (yyval.prop) = new Proposition;
  (yyval.prop)->n_type = OBSERVANCE;
  (yyval.prop)->act_name = *(yyvsp[-2].str_val);				
  (yyval.prop)->agentPro = *(yyvsp[-4].str_val);
  (yyval.prop)->flu_form = *(yyvsp[-1].str_list2);
}
#line 1917 "lcp.tab.c" /* yacc.c:1646  */
    break;

  case 61:
#line 529 "lcp.y" /* yacc.c:1646  */
    {
  (yyval.prop) = new Proposition;
  (yyval.prop)->n_type = IMPOSSIBILITY;
  (yyval.prop)->act_name = *(yyvsp[-2].str_val);
  (yyval.prop)->precond = *(yyvsp[-1].str_list);
}
#line 1928 "lcp.tab.c" /* yacc.c:1646  */
    break;

  case 62:
#line 538 "lcp.y" /* yacc.c:1646  */
    {
  (yyval.prop) = (yyvsp[0].prop);
}
#line 1936 "lcp.tab.c" /* yacc.c:1646  */
    break;

  case 63:
#line 543 "lcp.y" /* yacc.c:1646  */
    {
  (yyval.prop) = (yyvsp[0].prop);
}
#line 1944 "lcp.tab.c" /* yacc.c:1646  */
    break;

  case 64:
#line 548 "lcp.y" /* yacc.c:1646  */
    {
  (yyval.prop) = (yyvsp[0].prop);
}
#line 1952 "lcp.tab.c" /* yacc.c:1646  */
    break;

  case 65:
#line 553 "lcp.y" /* yacc.c:1646  */
    {
  (yyval.prop) = (yyvsp[0].prop);
}
#line 1960 "lcp.tab.c" /* yacc.c:1646  */
    break;

  case 66:
#line 558 "lcp.y" /* yacc.c:1646  */
    {
  (yyval.prop) = (yyvsp[0].prop);
}
#line 1968 "lcp.tab.c" /* yacc.c:1646  */
    break;

  case 67:
#line 563 "lcp.y" /* yacc.c:1646  */
    {
  (yyval.prop) = (yyvsp[0].prop);
}
#line 1976 "lcp.tab.c" /* yacc.c:1646  */
    break;

  case 68:
#line 568 "lcp.y" /* yacc.c:1646  */
    {
  (yyval.prop) = (yyvsp[0].prop);
}
#line 1984 "lcp.tab.c" /* yacc.c:1646  */
    break;

  case 69:
#line 573 "lcp.y" /* yacc.c:1646  */
    {
  (yyval.prop) = (yyvsp[0].prop);
}
#line 1992 "lcp.tab.c" /* yacc.c:1646  */
    break;

  case 70:
#line 581 "lcp.y" /* yacc.c:1646  */
    {
  (yyval.prop_list) = new PropositionList;
}
#line 2000 "lcp.tab.c" /* yacc.c:1646  */
    break;

  case 71:
#line 585 "lcp.y" /* yacc.c:1646  */
    {
  (yyval.prop_list) = (yyvsp[-1].prop_list);
  (yyvsp[-1].prop_list)->push_back(*(yyvsp[0].prop));
}
#line 2009 "lcp.tab.c" /* yacc.c:1646  */
    break;

  case 72:
#line 594 "lcp.y" /* yacc.c:1646  */
    {
  (yyval.bfnode) = (yyvsp[-1].bfnode);
}
#line 2017 "lcp.tab.c" /* yacc.c:1646  */
    break;

  case 73:
#line 600 "lcp.y" /* yacc.c:1646  */
    {
  (yyval.init_nodes) = new Nodes;
  //$$->insert(bfnode());
  //$$ = new StringList2;
  //$$->insert(StringList());
}
#line 2028 "lcp.tab.c" /* yacc.c:1646  */
    break;

  case 74:
#line 607 "lcp.y" /* yacc.c:1646  */
    {
  (yyval.init_nodes) = (yyvsp[-1].init_nodes);
  (yyval.init_nodes)->push_back(*(yyvsp[0].bfnode));
}
#line 2037 "lcp.tab.c" /* yacc.c:1646  */
    break;

  case 75:
#line 652 "lcp.y" /* yacc.c:1646  */
    {
  (yyval.bfnode) = (yyvsp[-1].bfnode);
}
#line 2045 "lcp.tab.c" /* yacc.c:1646  */
    break;

  case 76:
#line 658 "lcp.y" /* yacc.c:1646  */
    {
  (yyval.init_nodes) = new Nodes;
}
#line 2053 "lcp.tab.c" /* yacc.c:1646  */
    break;

  case 77:
#line 662 "lcp.y" /* yacc.c:1646  */
    {
  (yyval.init_nodes) = (yyvsp[-1].init_nodes);
  (yyval.init_nodes)->push_back(*(yyvsp[0].bfnode));
}
#line 2062 "lcp.tab.c" /* yacc.c:1646  */
    break;


#line 2066 "lcp.tab.c" /* yacc.c:1646  */
      default: break;
    }
  /* User semantic actions sometimes alter yychar, and that requires
     that yytoken be updated with the new translation.  We take the
     approach of translating immediately before every use of yytoken.
     One alternative is translating here after every semantic action,
     but that translation would be missed if the semantic action invokes
     YYABORT, YYACCEPT, or YYERROR immediately after altering yychar or
     if it invokes YYBACKUP.  In the case of YYABORT or YYACCEPT, an
     incorrect destructor might then be invoked immediately.  In the
     case of YYERROR or YYBACKUP, subsequent parser actions might lead
     to an incorrect destructor call or verbose syntax error message
     before the lookahead is translated.  */
  YY_SYMBOL_PRINT ("-> $$ =", yyr1[yyn], &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);

  *++yyvsp = yyval;

  /* Now 'shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */

  yyn = yyr1[yyn];

  yystate = yypgoto[yyn - YYNTOKENS] + *yyssp;
  if (0 <= yystate && yystate <= YYLAST && yycheck[yystate] == *yyssp)
    yystate = yytable[yystate];
  else
    yystate = yydefgoto[yyn - YYNTOKENS];

  goto yynewstate;


/*--------------------------------------.
| yyerrlab -- here on detecting error.  |
`--------------------------------------*/
yyerrlab:
  /* Make sure we have latest lookahead translation.  See comments at
     user semantic actions for why this is necessary.  */
  yytoken = yychar == YYEMPTY ? YYEMPTY : YYTRANSLATE (yychar);

  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
#if ! YYERROR_VERBOSE
      yyerror (YY_("syntax error"));
#else
# define YYSYNTAX_ERROR yysyntax_error (&yymsg_alloc, &yymsg, \
                                        yyssp, yytoken)
      {
        char const *yymsgp = YY_("syntax error");
        int yysyntax_error_status;
        yysyntax_error_status = YYSYNTAX_ERROR;
        if (yysyntax_error_status == 0)
          yymsgp = yymsg;
        else if (yysyntax_error_status == 1)
          {
            if (yymsg != yymsgbuf)
              YYSTACK_FREE (yymsg);
            yymsg = (char *) YYSTACK_ALLOC (yymsg_alloc);
            if (!yymsg)
              {
                yymsg = yymsgbuf;
                yymsg_alloc = sizeof yymsgbuf;
                yysyntax_error_status = 2;
              }
            else
              {
                yysyntax_error_status = YYSYNTAX_ERROR;
                yymsgp = yymsg;
              }
          }
        yyerror (yymsgp);
        if (yysyntax_error_status == 2)
          goto yyexhaustedlab;
      }
# undef YYSYNTAX_ERROR
#endif
    }



  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse lookahead token after an
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

  /* Else will try to reuse lookahead token after shifting the error
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

  /* Do not reclaim the symbols of the rule whose action triggered
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
  yyerrstatus = 3;      /* Each real token shifted decrements this.  */

  for (;;)
    {
      yyn = yypact[yystate];
      if (!yypact_value_is_default (yyn))
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

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END


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

#if !defined yyoverflow || YYERROR_VERBOSE
/*-------------------------------------------------.
| yyexhaustedlab -- memory exhaustion comes here.  |
`-------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  /* Fall through.  */
#endif

yyreturn:
  if (yychar != YYEMPTY)
    {
      /* Make sure we have latest lookahead translation.  See comments at
         user semantic actions for why this is necessary.  */
      yytoken = YYTRANSLATE (yychar);
      yydestruct ("Cleanup: discarding lookahead",
                  yytoken, &yylval);
    }
  /* Do not reclaim the symbols of the rule whose action triggered
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
  return yyresult;
}
#line 666 "lcp.y" /* yacc.c:1906  */


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
