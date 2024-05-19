
/* A Bison parser, made by GNU Bison 2.4.1.  */

/* Skeleton implementation for Bison's Yacc-like parsers in C
   
      Copyright (C) 1984, 1989, 1990, 2000, 2001, 2002, 2003, 2004, 2005, 2006
   Free Software Foundation, Inc.
   
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
#define YYBISON_VERSION "2.4.1"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1

/* Using locations.  */
#define YYLSP_NEEDED 0



/* Copy the first part of user declarations.  */

/* Line 189 of yacc.c  */
#line 1 "java.y"

#include <iostream>
#include <string>
#include <vector>
#include <map>

using namespace std;

int linha = 1, coluna = 1; 

struct Atributos {
  vector<string> c; // Código

  int linha = 0, coluna = 0;

  void clear() {
    c.clear();
    linha = 0;
    coluna = 0;
  }
};

enum TipoDecl { DeclVar = 200, DeclConst, DeclLet };

struct Var {
  int linha, coluna;
  TipoDecl tipo;
};

map<string,Var> ts; // Tabela de Símbolos

#define YYSTYPE Atributos

extern "C" int yylex();
int yyparse();
void yyerror(const char *);

vector<string> concatena( vector<string> a, vector<string> b ) {
  a.insert( a.end(), b.begin(), b.end() );
  return a;
}

vector<string> operator+( vector<string> a, vector<string> b ) {
  return concatena( a, b );
}

vector<string> operator+( vector<string> a, string b ) {
  a.push_back( b );
  return a;
}

vector<string> operator+( string a, vector<string> b ) {
  return vector<string>{ a } + b;
}

vector<string> resolve_enderecos( vector<string> entrada ) {
  map<string,int> label;
  vector<string> saida;
  for( int i = 0; i < entrada.size(); i++ ) 
    if( entrada[i][0] == ':' ) 
        label[entrada[i].substr(1)] = saida.size();
    else
      saida.push_back( entrada[i] );
  
  for( int i = 0; i < saida.size(); i++ )
    if( label.count( saida[i] ) > 0 )
        saida[i] = to_string(label[saida[i]]);
    
  return saida;
}

string gera_label( string prefixo ) {
  static int n = 0;
  return prefixo + "_" + to_string( ++n ) + ":";
}

void insere_tabela_de_simbolos( TipoDecl decl, Atributos att) {

    if (ts.count( att.c[0] ) > 0) {
        if (ts[att.c[0]].tipo == DeclConst) {
            cout << "Erro: tentativa de modificar uma variável constante ('" << att.c[0] << "')." << endl;
            exit( 1 );
        }
        if (ts[att.c[0]].tipo == DeclLet || decl == DeclLet) {
            cout << "Erro: a variável '" << att.c[0] << "' ja foi declarada na linha " << ts[att.c[0]].linha << "." << endl;
            exit( 1 );
        }
        return;
    }

    ts.insert({
            att.c[0],
            Var {
                att.linha,
                att.coluna,
                decl
            }
    });

}


void print( vector<string> codigo ) {
  for( int i = 1; i <= codigo.size(); i++)
    cout << i << ": " << codigo[i] << "\n";
    
  cout << endl;  
}

void checarVariavelExiste(Atributos att) {
  // Checa se uma variável que vai ser modificada existe
  
  if (ts.count( att.c[0] ) < 1) {
    cout << "Erro: a variável '" << att.c[0] << "' não foi declarada." << endl; 
    exit(1);
  }
  
}

void checarVariavelConst(Atributos att) {
  // Checa se uma variável const que já existe está sendo modificada
  if (ts[att.c[0]].tipo == DeclConst) {
    cout << "Erro: tentativa de modificar uma variável constante ('" << att.c[0] << "')." << endl; 
    exit (1);
  }
}


/* Line 189 of yacc.c  */
#line 202 "y.tab.c"

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


/* Tokens.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
   /* Put the tokens into the symbol table, so that GDB and other debuggers
      know about them.  */
   enum yytokentype {
     ID = 258,
     IF = 259,
     ELSE = 260,
     PRINT = 261,
     FOR = 262,
     WHILE = 263,
     OBJ = 264,
     ARRAY = 265,
     LET = 266,
     VAR = 267,
     CONST = 268,
     CDOUBLE = 269,
     CSTRING = 270,
     CINT = 271,
     AND = 272,
     OR = 273,
     ME_IG = 274,
     MA_IG = 275,
     DIF = 276,
     IGUAL = 277,
     MAIS_IGUAL = 278,
     MAIS_MAIS = 279
   };
#endif



#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef int YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
#endif


/* Copy the second part of user declarations.  */


/* Line 264 of yacc.c  */
#line 268 "y.tab.c"

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
# if YYENABLE_NLS
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
YYID (int yyi)
#else
static int
YYID (yyi)
    int yyi;
#endif
{
  return yyi;
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
# define YYSTACK_RELOCATE(Stack_alloc, Stack)				\
    do									\
      {									\
	YYSIZE_T yynewbytes;						\
	YYCOPY (&yyptr->Stack_alloc, Stack, yysize);			\
	Stack = &yyptr->Stack_alloc;					\
	yynewbytes = yystacksize * sizeof (*Stack) + YYSTACK_GAP_MAXIMUM; \
	yyptr += yynewbytes / sizeof (*yyptr);				\
      }									\
    while (YYID (0))

#endif

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  52
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   269

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  42
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  23
/* YYNRULES -- Number of rules.  */
#define YYNRULES  66
/* YYNRULES -- Number of states.  */
#define YYNSTATES  122

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   279

#define YYTRANSLATE(YYX)						\
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[YYLEX] -- Bison symbol number corresponding to YYLEX.  */
static const yytype_uint8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,    32,     2,     2,
      38,    39,    30,    28,    40,    29,    34,    31,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,    35,
      26,    25,    27,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,    33,     2,    41,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    36,     2,    37,     2,     2,     2,     2,
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
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24
};

#if YYDEBUG
/* YYPRHS[YYN] -- Index of the first RHS symbol of rule number YYN in
   YYRHS.  */
static const yytype_uint8 yyprhs[] =
{
       0,     0,     3,     5,     8,    10,    13,    16,    19,    21,
      25,    27,    29,    32,    37,    43,    53,    55,    57,    59,
      61,    64,    67,    70,    74,    76,    80,    82,    86,    88,
      90,    94,    96,   100,   102,   106,   114,   116,   121,   125,
     127,   129,   131,   133,   136,   139,   141,   144,   147,   149,
     151,   153,   155,   157,   161,   165,   169,   172,   176,   180,
     183,   187,   191,   195,   199,   203,   207
};

/* YYRHS -- A `-1'-separated list of the rules' RHS.  */
static const yytype_int8 yyrhs[] =
{
      43,     0,    -1,    44,    -1,    44,    45,    -1,    45,    -1,
      49,    35,    -1,    50,    35,    -1,    51,    35,    -1,    58,
      -1,     6,    61,    35,    -1,    46,    -1,    47,    -1,    61,
      35,    -1,    36,    44,    37,    35,    -1,     8,    38,    61,
      39,    45,    -1,     7,    38,    48,    35,    61,    35,    61,
      39,    45,    -1,    49,    -1,    50,    -1,    51,    -1,    61,
      -1,    11,    52,    -1,    12,    53,    -1,    13,    54,    -1,
      55,    40,    52,    -1,    55,    -1,    56,    40,    53,    -1,
      56,    -1,    57,    40,    54,    -1,    57,    -1,     3,    -1,
       3,    25,    61,    -1,     3,    -1,     3,    25,    61,    -1,
       3,    -1,     3,    25,    61,    -1,     4,    38,    61,    39,
      45,     5,    45,    -1,     3,    -1,    61,    33,    61,    41,
      -1,    61,    34,     3,    -1,    62,    -1,    63,    -1,    64,
      -1,    14,    -1,    28,    14,    -1,    29,    14,    -1,    16,
      -1,    28,    16,    -1,    29,    16,    -1,    15,    -1,     9,
      -1,    10,    -1,    59,    -1,    60,    -1,    38,    61,    39,
      -1,    59,    25,    61,    -1,    59,    23,    61,    -1,    59,
      24,    -1,    60,    25,    61,    -1,    60,    23,    61,    -1,
      60,    24,    -1,    61,    26,    61,    -1,    61,    27,    61,
      -1,    61,    28,    61,    -1,    61,    29,    61,    -1,    61,
      30,    61,    -1,    61,    31,    61,    -1,    61,    32,    61,
      -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   147,   147,   150,   151,   154,   155,   156,   157,   158,
     160,   161,   162,   164,   168,   184,   200,   201,   202,   203,
     207,   210,   213,   216,   217,   220,   221,   224,   225,   228,
     230,   234,   236,   240,   242,   246,   261,   264,   265,   268,
     269,   270,   271,   272,   274,   276,   277,   279,   281,   282,
     284,   286,   288,   290,   295,   301,   307,   313,   315,   317,
     322,   324,   328,   330,   332,   334,   336
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || YYTOKEN_TABLE
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "ID", "IF", "ELSE", "PRINT", "FOR",
  "WHILE", "OBJ", "ARRAY", "LET", "VAR", "CONST", "CDOUBLE", "CSTRING",
  "CINT", "AND", "OR", "ME_IG", "MA_IG", "DIF", "IGUAL", "MAIS_IGUAL",
  "MAIS_MAIS", "'='", "'<'", "'>'", "'+'", "'-'", "'*'", "'/'", "'%'",
  "'['", "'.'", "';'", "'{'", "'}'", "'('", "')'", "','", "']'", "$accept",
  "S", "CMDs", "CMD", "CMD_WHILE", "CMD_FOR", "PRIM_E", "CMD_LET",
  "CMD_VAR", "CMD_CONST", "VARs_LET", "VARs_VAR", "VARs_CONST",
  "VAR_R_LET", "VAR_R_VAR", "VAR_R_CONST", "CMD_IF", "LVALUE",
  "LVALUEPROP", "E", "ATRIB", "BOOL", "MATH", 0
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[YYLEX-NUM] -- Internal token number corresponding to
   token YYLEX-NUM.  */
static const yytype_uint16 yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,    61,    60,    62,    43,    45,
      42,    47,    37,    91,    46,    59,   123,   125,    40,    41,
      44,    93
};
# endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,    42,    43,    44,    44,    45,    45,    45,    45,    45,
      45,    45,    45,    45,    46,    47,    48,    48,    48,    48,
      49,    50,    51,    52,    52,    53,    53,    54,    54,    55,
      55,    56,    56,    57,    57,    58,    59,    60,    60,    61,
      61,    61,    61,    61,    61,    61,    61,    61,    61,    61,
      61,    61,    61,    61,    62,    62,    62,    62,    62,    62,
      63,    63,    64,    64,    64,    64,    64
};

/* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     1,     2,     1,     2,     2,     2,     1,     3,
       1,     1,     2,     4,     5,     9,     1,     1,     1,     1,
       2,     2,     2,     3,     1,     3,     1,     3,     1,     1,
       3,     1,     3,     1,     3,     7,     1,     4,     3,     1,
       1,     1,     1,     2,     2,     1,     2,     2,     1,     1,
       1,     1,     1,     3,     3,     3,     2,     3,     3,     2,
       3,     3,     3,     3,     3,     3,     3
};

/* YYDEFACT[STATE-NAME] -- Default rule to reduce with in state
   STATE-NUM when YYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       0,    36,     0,     0,     0,     0,    49,    50,     0,     0,
       0,    42,    48,    45,     0,     0,     0,     0,     0,     2,
       4,    10,    11,     0,     0,     0,     8,    51,    52,     0,
      39,    40,    41,     0,     0,     0,     0,    29,    20,    24,
      31,    21,    26,    33,    22,    28,    43,    46,    44,    47,
       0,     0,     1,     3,     5,     6,     7,     0,    56,     0,
       0,    59,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    12,     0,     9,     0,    16,    17,    18,    19,
       0,     0,     0,     0,     0,     0,     0,     0,    53,    55,
      54,    58,    57,    60,    61,    62,    63,    64,    65,    66,
       0,    38,     0,     0,     0,    30,    23,    32,    25,    34,
      27,    13,    37,     0,     0,    14,     0,     0,    35,     0,
       0,    15
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int8 yydefgoto[] =
{
      -1,    18,    19,    20,    21,    22,    75,    23,    24,    25,
      38,    41,    44,    39,    42,    45,    26,    27,    28,    29,
      30,    31,    32
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -26
static const yytype_int16 yypact[] =
{
     109,   -26,     1,   155,     2,     4,   -26,   -26,    38,    40,
      41,   -26,   -26,   -26,    15,    20,   109,   155,    48,   109,
     -26,   -26,   -26,    14,    16,    17,   -26,   -22,     3,   196,
     -26,   -26,   -26,   155,   206,   139,   155,    25,   -26,    13,
      30,   -26,    18,    52,   -26,    39,   -26,   -26,   -26,   -26,
       9,    64,   -26,   -26,   -26,   -26,   -26,   155,   -26,   155,
     155,   -26,   155,   155,   155,   155,   155,   155,   155,   155,
     155,    78,   -26,   100,   -26,    49,   -26,   -26,   -26,   226,
     168,   155,    38,   155,    40,   155,    41,    50,   -26,   226,
     226,   226,   226,   235,   235,   -25,   -25,   -23,   -23,   -23,
      42,   -26,   109,   155,   109,   226,   -26,   226,   -26,   226,
     -26,   -26,   -26,    81,   216,   -26,   109,   155,   -26,   182,
     109,   -26
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int8 yypgoto[] =
{
     -26,   -26,    72,   -15,   -26,   -26,   -26,    67,    69,    71,
      26,    23,    24,   -26,   -26,   -26,   -26,   -26,   -26,    -3,
     -26,   -26,   -26
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If zero, do what YYDEFACT says.
   If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -1
static const yytype_int8 yytable[] =
{
      34,    57,    58,    59,    53,    67,    68,    69,    70,    71,
      70,    71,     1,     2,    51,     3,     4,     5,     6,     7,
       8,     9,    10,    11,    12,    13,    60,    61,    62,    46,
      73,    47,    79,    80,    48,    53,    49,    14,    15,    33,
      35,    37,    36,    40,    43,    16,    87,    17,    52,    54,
      81,    55,    56,    82,    89,    83,    90,    91,    84,    92,
      93,    94,    95,    96,    97,    98,    99,   100,    63,    64,
      65,    66,    67,    68,    69,    70,    71,    85,   105,    86,
     107,   101,   109,   112,   103,   111,   116,   113,    50,   115,
      63,    64,    65,    66,    67,    68,    69,    70,    71,     0,
     114,   118,    76,    88,    77,   121,    78,   108,   106,     0,
     110,     0,     1,     2,   119,     3,     4,     5,     6,     7,
       8,     9,    10,    11,    12,    13,    63,    64,    65,    66,
      67,    68,    69,    70,    71,     0,     0,    14,    15,   102,
       0,     0,     1,     0,     0,    16,     0,    17,     6,     7,
       8,     9,    10,    11,    12,    13,     0,     0,     1,     0,
       0,     0,     0,     0,     6,     7,     0,    14,    15,    11,
      12,    13,     0,     0,     0,     0,     0,    17,     0,     0,
       0,     0,     0,    14,    15,     0,     0,     0,     0,     0,
       0,     0,     0,    17,    63,    64,    65,    66,    67,    68,
      69,    70,    71,     0,     0,     0,     0,   104,    63,    64,
      65,    66,    67,    68,    69,    70,    71,     0,     0,     0,
       0,   120,    63,    64,    65,    66,    67,    68,    69,    70,
      71,    72,    63,    64,    65,    66,    67,    68,    69,    70,
      71,    74,    63,    64,    65,    66,    67,    68,    69,    70,
      71,   117,    63,    64,    65,    66,    67,    68,    69,    70,
      71,    -1,    -1,    65,    66,    67,    68,    69,    70,    71
};

static const yytype_int8 yycheck[] =
{
       3,    23,    24,    25,    19,    30,    31,    32,    33,    34,
      33,    34,     3,     4,    17,     6,     7,     8,     9,    10,
      11,    12,    13,    14,    15,    16,    23,    24,    25,    14,
      33,    16,    35,    36,    14,    50,    16,    28,    29,    38,
      38,     3,    38,     3,     3,    36,    37,    38,     0,    35,
      25,    35,    35,    40,    57,    25,    59,    60,    40,    62,
      63,    64,    65,    66,    67,    68,    69,    70,    26,    27,
      28,    29,    30,    31,    32,    33,    34,    25,    81,    40,
      83,     3,    85,    41,    35,    35,     5,   102,    16,   104,
      26,    27,    28,    29,    30,    31,    32,    33,    34,    -1,
     103,   116,    35,    39,    35,   120,    35,    84,    82,    -1,
      86,    -1,     3,     4,   117,     6,     7,     8,     9,    10,
      11,    12,    13,    14,    15,    16,    26,    27,    28,    29,
      30,    31,    32,    33,    34,    -1,    -1,    28,    29,    39,
      -1,    -1,     3,    -1,    -1,    36,    -1,    38,     9,    10,
      11,    12,    13,    14,    15,    16,    -1,    -1,     3,    -1,
      -1,    -1,    -1,    -1,     9,    10,    -1,    28,    29,    14,
      15,    16,    -1,    -1,    -1,    -1,    -1,    38,    -1,    -1,
      -1,    -1,    -1,    28,    29,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    38,    26,    27,    28,    29,    30,    31,
      32,    33,    34,    -1,    -1,    -1,    -1,    39,    26,    27,
      28,    29,    30,    31,    32,    33,    34,    -1,    -1,    -1,
      -1,    39,    26,    27,    28,    29,    30,    31,    32,    33,
      34,    35,    26,    27,    28,    29,    30,    31,    32,    33,
      34,    35,    26,    27,    28,    29,    30,    31,    32,    33,
      34,    35,    26,    27,    28,    29,    30,    31,    32,    33,
      34,    26,    27,    28,    29,    30,    31,    32,    33,    34
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,     3,     4,     6,     7,     8,     9,    10,    11,    12,
      13,    14,    15,    16,    28,    29,    36,    38,    43,    44,
      45,    46,    47,    49,    50,    51,    58,    59,    60,    61,
      62,    63,    64,    38,    61,    38,    38,     3,    52,    55,
       3,    53,    56,     3,    54,    57,    14,    16,    14,    16,
      44,    61,     0,    45,    35,    35,    35,    23,    24,    25,
      23,    24,    25,    26,    27,    28,    29,    30,    31,    32,
      33,    34,    35,    61,    35,    48,    49,    50,    51,    61,
      61,    25,    40,    25,    40,    25,    40,    37,    39,    61,
      61,    61,    61,    61,    61,    61,    61,    61,    61,    61,
      61,     3,    39,    35,    39,    61,    52,    61,    53,    61,
      54,    35,    41,    45,    61,    45,     5,    35,    45,    61,
      39,    45
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
# if YYLTYPE_IS_TRIVIAL
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
yy_stack_print (yytype_int16 *yybottom, yytype_int16 *yytop)
#else
static void
yy_stack_print (yybottom, yytop)
    yytype_int16 *yybottom;
    yytype_int16 *yytop;
#endif
{
  YYFPRINTF (stderr, "Stack now");
  for (; yybottom <= yytop; yybottom++)
    {
      int yybot = *yybottom;
      YYFPRINTF (stderr, " %d", yybot);
    }
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
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr, yyrhs[yyprhs[yyrule] + yyi],
		       &(yyvsp[(yyi + 1) - (yynrhs)])
		       		       );
      YYFPRINTF (stderr, "\n");
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


/* The lookahead symbol.  */
int yychar;

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval;

/* Number of syntax errors so far.  */
int yynerrs;



/*-------------------------.
| yyparse or yypush_parse.  |
`-------------------------*/

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
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus;

    /* The stacks and their tools:
       `yyss': related to states.
       `yyvs': related to semantic values.

       Refer to the stacks thru separate pointers, to allow yyoverflow
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
  int yytoken;
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

  yytoken = 0;
  yyss = yyssa;
  yyvs = yyvsa;
  yystacksize = YYINITDEPTH;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY; /* Cause a token to be read.  */

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
  if (yyn == YYPACT_NINF)
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* YYCHAR is either YYEMPTY or YYEOF or a valid lookahead symbol.  */
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

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  /* Shift the lookahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);

  /* Discard the shifted token.  */
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
        case 2:

/* Line 1455 of yacc.c  */
#line 147 "java.y"
    { print( resolve_enderecos( (yyvsp[(1) - (1)]).c + "." ) ); ;}
    break;

  case 3:

/* Line 1455 of yacc.c  */
#line 150 "java.y"
    { (yyval).c =  (yyvsp[(1) - (2)]).c + (yyvsp[(2) - (2)]).c ; ;}
    break;

  case 9:

/* Line 1455 of yacc.c  */
#line 159 "java.y"
    { (yyval).c = (yyvsp[(2) - (3)]).c + "println" + "#"; ;}
    break;

  case 12:

/* Line 1455 of yacc.c  */
#line 163 "java.y"
    { (yyval).c = (yyvsp[(1) - (2)]).c + "^"; ;}
    break;

  case 13:

/* Line 1455 of yacc.c  */
#line 165 "java.y"
    { (yyval).c = (yyvsp[(2) - (4)]).c; ;}
    break;

  case 14:

/* Line 1455 of yacc.c  */
#line 169 "java.y"
    { string lbl_fim_while = gera_label( "fim_while" );
            string lbl_condicao_while = gera_label( "condicao_while" );
            string lbl_comando_while = gera_label( "comando_while" );
            string definicao_lbl_fim_while = ":" + lbl_fim_while;
            string definicao_lbl_condicao_while = ":" + lbl_condicao_while;
            string definicao_lbl_comando_while = ":" +lbl_comando_while;

            (yyval).c = definicao_lbl_condicao_while + (yyvsp[(3) - (5)]).c + // Código para a condição
                   lbl_comando_while + "?" + lbl_fim_while + "#" + // Se a condição for true, vai pro comando
                   definicao_lbl_comando_while + (yyvsp[(5) - (5)]).c + // Código do comando
                   lbl_condicao_while + "#" + // Volta pra condição
                   definicao_lbl_fim_while;
          ;}
    break;

  case 15:

/* Line 1455 of yacc.c  */
#line 185 "java.y"
    { string lbl_fim_for = gera_label( "fim_for" );
          string lbl_condicao_for = gera_label( "condicao_for" );
          string lbl_comando_for = gera_label( "comando_for" );
          string definicao_lbl_fim_for = ":" + lbl_fim_for;
          string definicao_lbl_condicao_for = ":" + lbl_condicao_for;
          string definicao_lbl_comando_for = ":" + lbl_comando_for;
          
          (yyval).c = (yyvsp[(3) - (9)]).c + definicao_lbl_condicao_for +
                 (yyvsp[(5) - (9)]).c + lbl_comando_for + "?" + lbl_fim_for + "#" +
                 definicao_lbl_comando_for + (yyvsp[(9) - (9)]).c + 
                 (yyvsp[(7) - (9)]).c + "^" + lbl_condicao_for + "#" +
                 definicao_lbl_fim_for;
        ;}
    break;

  case 19:

/* Line 1455 of yacc.c  */
#line 204 "java.y"
    { (yyval).c = (yyvsp[(1) - (1)]).c + "^"; ;}
    break;

  case 20:

/* Line 1455 of yacc.c  */
#line 207 "java.y"
    {  (yyval).c = (yyvsp[(2) - (2)]).c; ;}
    break;

  case 21:

/* Line 1455 of yacc.c  */
#line 210 "java.y"
    {  (yyval).c = (yyvsp[(2) - (2)]).c;;}
    break;

  case 22:

/* Line 1455 of yacc.c  */
#line 213 "java.y"
    {  (yyval).c = (yyvsp[(2) - (2)]).c;;}
    break;

  case 23:

/* Line 1455 of yacc.c  */
#line 216 "java.y"
    { (yyval).c = (yyvsp[(1) - (3)]).c + (yyvsp[(3) - (3)]).c; ;}
    break;

  case 25:

/* Line 1455 of yacc.c  */
#line 220 "java.y"
    { (yyval).c = (yyvsp[(1) - (3)]).c + (yyvsp[(3) - (3)]).c; ;}
    break;

  case 27:

/* Line 1455 of yacc.c  */
#line 224 "java.y"
    { (yyval).c = (yyvsp[(1) - (3)]).c + (yyvsp[(3) - (3)]).c; ;}
    break;

  case 29:

/* Line 1455 of yacc.c  */
#line 229 "java.y"
    { (yyval).c = (yyvsp[(1) - (1)]).c + "&"; insere_tabela_de_simbolos(DeclLet, (yyvsp[(1) - (1)]));;}
    break;

  case 30:

/* Line 1455 of yacc.c  */
#line 231 "java.y"
    { (yyval).c = (yyvsp[(1) - (3)]).c + "&" + (yyvsp[(1) - (3)]).c + (yyvsp[(3) - (3)]).c + "=" + "^"; insere_tabela_de_simbolos(DeclLet, (yyvsp[(1) - (3)]));;}
    break;

  case 31:

/* Line 1455 of yacc.c  */
#line 235 "java.y"
    { (yyval).c = (yyvsp[(1) - (1)]).c + "&"; insere_tabela_de_simbolos(DeclVar, (yyvsp[(1) - (1)])); ;}
    break;

  case 32:

/* Line 1455 of yacc.c  */
#line 237 "java.y"
    { (yyval).c = (yyvsp[(1) - (3)]).c + "&" + (yyvsp[(1) - (3)]).c + (yyvsp[(3) - (3)]).c + "=" + "^"; insere_tabela_de_simbolos(DeclVar, (yyvsp[(1) - (3)])); ;}
    break;

  case 33:

/* Line 1455 of yacc.c  */
#line 241 "java.y"
    { (yyval).c = (yyvsp[(1) - (1)]).c + "&"; insere_tabela_de_simbolos(DeclConst, (yyvsp[(1) - (1)]));;}
    break;

  case 34:

/* Line 1455 of yacc.c  */
#line 243 "java.y"
    { (yyval).c = (yyvsp[(1) - (3)]).c + "&" + (yyvsp[(1) - (3)]).c + (yyvsp[(3) - (3)]).c + "=" + "^"; insere_tabela_de_simbolos(DeclConst, (yyvsp[(1) - (3)]));;}
    break;

  case 35:

/* Line 1455 of yacc.c  */
#line 247 "java.y"
    { string lbl_true = gera_label( "lbl_true" );
           string lbl_fim_if = gera_label( "lbl_fim_if" );
           string definicao_lbl_true = ":" + lbl_true;
           string definicao_lbl_fim_if = ":" + lbl_fim_if;
                    
            (yyval).c = (yyvsp[(3) - (7)]).c +                       // Codigo da expressão
                   lbl_true + "?" +             // Código do IF
                   (yyvsp[(7) - (7)]).c + lbl_fim_if + "#" +    // Código do False
                   definicao_lbl_true + (yyvsp[(5) - (7)]).c +  // Código do True
                   definicao_lbl_fim_if         // Fim do IF
                   ;
         ;}
    break;

  case 37:

/* Line 1455 of yacc.c  */
#line 264 "java.y"
    { (yyval).c = (yyvsp[(1) - (4)]).c + (yyvsp[(3) - (4)]).c; ;}
    break;

  case 38:

/* Line 1455 of yacc.c  */
#line 265 "java.y"
    { (yyval).c = (yyvsp[(1) - (3)]).c + (yyvsp[(3) - (3)]).c; ;}
    break;

  case 43:

/* Line 1455 of yacc.c  */
#line 273 "java.y"
    { (yyval).c = (yyvsp[(2) - (2)]).c; ;}
    break;

  case 44:

/* Line 1455 of yacc.c  */
#line 275 "java.y"
    { (yyval).c = "0" + (yyvsp[(2) - (2)]).c + "-"; ;}
    break;

  case 46:

/* Line 1455 of yacc.c  */
#line 278 "java.y"
    { (yyval).c = (yyvsp[(2) - (2)]).c; ;}
    break;

  case 47:

/* Line 1455 of yacc.c  */
#line 280 "java.y"
    { (yyval).c = "0" + (yyvsp[(2) - (2)]).c + "-"; ;}
    break;

  case 49:

/* Line 1455 of yacc.c  */
#line 283 "java.y"
    { (yyval).c = (yyvsp[(1) - (1)]).c; ;}
    break;

  case 50:

/* Line 1455 of yacc.c  */
#line 285 "java.y"
    { (yyval).c = (yyvsp[(1) - (1)]).c; ;}
    break;

  case 51:

/* Line 1455 of yacc.c  */
#line 287 "java.y"
    { (yyval).c = (yyvsp[(1) - (1)]).c + "@"; ;}
    break;

  case 52:

/* Line 1455 of yacc.c  */
#line 289 "java.y"
    { (yyval).c = (yyvsp[(1) - (1)]).c + "[@]"; ;}
    break;

  case 53:

/* Line 1455 of yacc.c  */
#line 291 "java.y"
    { (yyval).c = (yyvsp[(2) - (3)]).c; ;}
    break;

  case 54:

/* Line 1455 of yacc.c  */
#line 296 "java.y"
    { 
        checarVariavelExiste((yyvsp[(1) - (3)]));  
        checarVariavelConst((yyvsp[(1) - (3)]));  
        (yyval).c = (yyvsp[(1) - (3)]).c + (yyvsp[(3) - (3)]).c + "="; 
      ;}
    break;

  case 55:

/* Line 1455 of yacc.c  */
#line 302 "java.y"
    {
        checarVariavelExiste((yyvsp[(1) - (3)]));  
        checarVariavelConst((yyvsp[(1) - (3)]));  
        (yyval).c = (yyvsp[(1) - (3)]).c + (yyvsp[(1) - (3)]).c + "@" + (yyvsp[(3) - (3)]).c + "+ ="; 
      ;}
    break;

  case 56:

/* Line 1455 of yacc.c  */
#line 308 "java.y"
    {
        checarVariavelExiste((yyvsp[(1) - (2)]));  
        checarVariavelConst((yyvsp[(1) - (2)]));  
        (yyval).c = (yyvsp[(1) - (2)]).c + "@" + (yyvsp[(1) - (2)]).c + (yyvsp[(1) - (2)]).c + "@ 1 + = ^"; 
      ;}
    break;

  case 57:

/* Line 1455 of yacc.c  */
#line 314 "java.y"
    { (yyval).c = (yyvsp[(1) - (3)]).c + (yyvsp[(3) - (3)]).c + "[=]"; ;}
    break;

  case 58:

/* Line 1455 of yacc.c  */
#line 316 "java.y"
    { (yyval).c = (yyvsp[(1) - (3)]).c + (yyvsp[(1) - (3)]).c + "[@]" + (yyvsp[(3) - (3)]).c + "+ [=]"; ;}
    break;

  case 59:

/* Line 1455 of yacc.c  */
#line 318 "java.y"
    { (yyval).c = (yyvsp[(1) - (2)]).c + "@" + (yyvsp[(1) - (2)]).c + (yyvsp[(1) - (2)]).c + "@ 1 + [=] ^"; ;}
    break;

  case 60:

/* Line 1455 of yacc.c  */
#line 323 "java.y"
    { (yyval).c = (yyvsp[(1) - (3)]).c + (yyvsp[(3) - (3)]).c + (yyvsp[(2) - (3)]).c; ;}
    break;

  case 61:

/* Line 1455 of yacc.c  */
#line 325 "java.y"
    { (yyval).c = (yyvsp[(1) - (3)]).c + (yyvsp[(3) - (3)]).c + (yyvsp[(2) - (3)]).c; ;}
    break;

  case 62:

/* Line 1455 of yacc.c  */
#line 329 "java.y"
    { (yyval).c = (yyvsp[(1) - (3)]).c + (yyvsp[(3) - (3)]).c + (yyvsp[(2) - (3)]).c; ;}
    break;

  case 63:

/* Line 1455 of yacc.c  */
#line 331 "java.y"
    { (yyval).c = (yyvsp[(1) - (3)]).c + (yyvsp[(3) - (3)]).c + (yyvsp[(2) - (3)]).c; ;}
    break;

  case 64:

/* Line 1455 of yacc.c  */
#line 333 "java.y"
    { (yyval).c = (yyvsp[(1) - (3)]).c + (yyvsp[(3) - (3)]).c + (yyvsp[(2) - (3)]).c; ;}
    break;

  case 65:

/* Line 1455 of yacc.c  */
#line 335 "java.y"
    { (yyval).c = (yyvsp[(1) - (3)]).c + (yyvsp[(3) - (3)]).c + (yyvsp[(2) - (3)]).c; ;}
    break;

  case 66:

/* Line 1455 of yacc.c  */
#line 337 "java.y"
    { (yyval).c = (yyvsp[(1) - (3)]).c + (yyvsp[(3) - (3)]).c + (yyvsp[(2) - (3)]).c; ;}
    break;



/* Line 1455 of yacc.c  */
#line 1957 "y.tab.c"
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

#if !defined(yyoverflow) || YYERROR_VERBOSE
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



/* Line 1675 of yacc.c  */
#line 340 "java.y"


#include "lex.yy.c"

void yyerror( const char* st ) {
   puts( st ); 
   printf( "Proximo a: %s\n", yytext );
   exit( 0 );
}

int main( int argc, char* argv[] ) {
  yyparse();
  
  return 0;
}
