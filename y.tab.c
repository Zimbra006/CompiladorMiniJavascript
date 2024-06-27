
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

  // Para argumentos, parâmetros e elementos de array
  int contador = 0;     
  
  // Só para valor default de argumento        
  vector<string> valor_default; 

  void clear() {
    c.clear();
    valor_default.clear();
    linha = 0;
    coluna = 0;
    contador = 0;
  }
};

enum TipoDecl { DeclVar = 200, DeclConst, DeclLet };
map<TipoDecl, string> nomeTipoDecl = { 
  { DeclLet, "let" }, 
  { DeclConst, "const" }, 
  { DeclVar, "var" }
};

struct Simbolo {
  TipoDecl tipo;
  int linha;
  int coluna;
};

int in_func = 0;

// Tabela de símbolos - agora é uma pilha
vector< map< string, Simbolo > > ts = { map< string, Simbolo >{} }; 

// Guarda as instruções para a execução de funções
vector<string> funcoes;

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

// Funções dadas pelo professor
// Substitui insere_tabela_de_simbolos
vector<string> declara_var( TipoDecl tipo, string nome, int linha, int coluna ) {

  auto& topo = ts.back();   
       
  if( topo.count( nome ) == 0 ) {
    topo[nome] = Simbolo{ tipo, linha, coluna };
    return vector<string>{ nome, "&" };
  }
  else if( tipo == DeclVar && topo[nome].tipo == DeclVar ) {
    topo[nome] = Simbolo{ tipo, linha, coluna };
    return vector<string>{};
  } 
  else {
    cerr << "Redeclaração de '" << nomeTipoDecl[topo[nome].tipo] << " " << nome 
         << "' declarado na linha: " << topo[nome].linha 
         << ", coluna: " << topo[nome].coluna << endl;
    exit( 1 );     
  }
}

// Checagem de variáveis deve ser desabilitada dentro de funções
bool dentroFuncao = false;

// Substitui ambas as funções checarVariavel
void checa_simbolo( string nome, bool modificavel ) {

  if (dentroFuncao) return;

  for( int i = ts.size() - 1; i >= 0; i-- ) {  
    auto& atual = ts[i];
    
    if( atual.count( nome ) > 0 ) {
      if( modificavel && atual[nome].tipo == DeclConst ) {
        cerr << "Variavel '" << nome << "' não pode ser modificada." << endl;
        exit( 1 );     
      }
      else 
        return;
    }
  }

  cerr << "Variavel '" << nome << "' não declarada." << endl;
  exit( 1 );     
}

void print( vector<string> codigo ) {
  int inc = 0;
  for( int i = 0; i < codigo.size(); i++ ) {
    if (codigo[i] == "") { inc--; continue; } 
    cout << i + inc << ": " << codigo[i] << endl;
  }
    
  cout << endl;  
}


/* Line 189 of yacc.c  */
#line 228 "y.tab.c"

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
     PRINT = 259,
     FOR = 260,
     WHILE = 261,
     ASM = 262,
     ASM_WORD = 263,
     LET = 264,
     VAR = 265,
     CONST = 266,
     FUNCTION = 267,
     RETURN = 268,
     TRUE_TK = 269,
     FALSE_TK = 270,
     CDOUBLE = 271,
     CSTRING = 272,
     CINT = 273,
     PARENTESES_FUNCAO = 274,
     SETA = 275,
     MAIS_IGUAL = 276,
     MAIS_MAIS = 277,
     IGUAL = 278,
     DIF = 279,
     MA_IG = 280,
     ME_IG = 281,
     OR = 282,
     AND = 283,
     IF = 284,
     REDUCE = 285,
     ELSE = 286
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
#line 301 "y.tab.c"

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
#define YYFINAL  74
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   993

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  50
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  36
/* YYNRULES -- Number of rules.  */
#define YYNRULES  117
/* YYNRULES -- Number of states.  */
#define YYNSTATES  228

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   286

#define YYTRANSLATE(YYX)						\
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[YYLEX] -- Bison symbol number corresponding to YYLEX.  */
static const yytype_uint8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,    39,     2,     2,
      41,    46,    37,    35,    47,    36,    42,    38,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,    49,    43,
      23,    20,    24,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,    40,     2,    48,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    44,     2,    45,     2,     2,     2,     2,
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
      15,    16,    17,    18,    19,    21,    22,    25,    26,    27,
      28,    29,    30,    31,    32,    33,    34
};

#if YYDEBUG
/* YYPRHS[YYN] -- Index of the first RHS symbol of rule number YYN in
   YYRHS.  */
static const yytype_uint16 yyprhs[] =
{
       0,     0,     3,     5,     8,    10,    13,    16,    19,    21,
      23,    26,    30,    34,    38,    40,    42,    45,    50,    51,
      55,    58,    59,    60,    70,    72,    74,    75,    79,    82,
      84,    88,    92,    98,   108,   118,   120,   122,   124,   126,
     132,   140,   143,   146,   149,   153,   155,   159,   161,   165,
     167,   169,   173,   177,   179,   183,   187,   191,   195,   200,
     204,   208,   212,   215,   219,   223,   226,   230,   234,   238,
     242,   246,   250,   254,   258,   262,   266,   270,   274,   278,
     280,   283,   286,   288,   291,   294,   296,   299,   302,   304,
     306,   308,   310,   314,   319,   323,   327,   335,   340,   347,
     353,   361,   367,   375,   377,   378,   382,   384,   386,   388,
     392,   396,   398,   402,   406,   410,   412,   414
};

/* YYRHS -- A `-1'-separated list of the rules' RHS.  */
static const yytype_int8 yyrhs[] =
{
      51,     0,    -1,    52,    -1,    52,    53,    -1,    53,    -1,
      67,    43,    -1,    68,    43,    -1,    69,    43,    -1,    66,
      -1,    57,    -1,    59,    43,    -1,    59,    77,    43,    -1,
      77,    55,    43,    -1,     4,    77,    43,    -1,    63,    -1,
      64,    -1,    77,    43,    -1,    44,    54,    52,    45,    -1,
      -1,     7,    56,    45,    -1,     8,    56,    -1,    -1,    -1,
      12,     3,    58,    41,    60,    46,    44,    52,    45,    -1,
      13,    -1,    61,    -1,    -1,    61,    47,    62,    -1,    62,
      54,    -1,     3,    -1,     3,    20,    77,    -1,     3,    20,
      81,    -1,     6,    41,    77,    46,    53,    -1,     5,    41,
      65,    43,    77,    43,    77,    46,    77,    -1,     5,    41,
      65,    43,    77,    43,    77,    46,    53,    -1,    67,    -1,
      68,    -1,    69,    -1,    77,    -1,    32,    41,    77,    46,
      53,    -1,    32,    41,    77,    46,    53,    34,    53,    -1,
       9,    70,    -1,    10,    71,    -1,    11,    72,    -1,    73,
      47,    70,    -1,    73,    -1,    74,    47,    71,    -1,    74,
      -1,    75,    47,    72,    -1,    75,    -1,     3,    -1,     3,
      20,    77,    -1,     3,    20,    81,    -1,     3,    -1,     3,
      20,    77,    -1,     3,    20,    81,    -1,     3,    20,    77,
      -1,     3,    20,    81,    -1,    77,    40,    77,    48,    -1,
      77,    42,     3,    -1,     3,    20,    77,    -1,     3,    22,
      77,    -1,     3,    25,    -1,    76,    20,    77,    -1,    76,
      22,    77,    -1,    76,    25,    -1,    77,    23,    77,    -1,
      77,    24,    77,    -1,    77,    31,    77,    -1,    77,    30,
      77,    -1,    77,    29,    77,    -1,    77,    28,    77,    -1,
      77,    27,    77,    -1,    77,    26,    77,    -1,    77,    35,
      77,    -1,    77,    36,    77,    -1,    77,    37,    77,    -1,
      77,    38,    77,    -1,    77,    39,    77,    -1,    16,    -1,
      35,    16,    -1,    36,    16,    -1,    18,    -1,    35,    18,
      -1,    36,    18,    -1,    17,    -1,    44,    45,    -1,    40,
      48,    -1,     3,    -1,    76,    -1,    14,    -1,    15,    -1,
      41,    77,    46,    -1,    77,    41,    78,    46,    -1,    41,
      81,    46,    -1,    40,    84,    48,    -1,    12,    41,    60,
      46,    44,    52,    45,    -1,     3,    21,    54,    77,    -1,
       3,    21,    54,    44,    52,    45,    -1,    41,    19,    21,
      54,    77,    -1,    41,    19,    21,    44,    54,    52,    45,
      -1,    41,    60,    19,    21,    77,    -1,    41,    60,    19,
      21,    44,    52,    45,    -1,    79,    -1,    -1,    79,    47,
      80,    -1,    80,    -1,    77,    -1,    81,    -1,    44,    82,
      45,    -1,    82,    47,    83,    -1,    83,    -1,     3,    49,
      77,    -1,     3,    49,    81,    -1,    84,    47,    85,    -1,
      85,    -1,    77,    -1,    81,    -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   176,   176,   179,   180,   183,   184,   185,   186,   187,
     188,   190,   192,   194,   196,   197,   198,   200,   205,   208,
     211,   212,   215,   215,   230,   240,   241,   244,   266,   289,
     294,   300,   308,   324,   338,   354,   355,   356,   357,   361,
     374,   389,   392,   395,   398,   399,   402,   403,   406,   407,
     410,   412,   414,   418,   420,   422,   426,   428,   432,   433,
     436,   441,   446,   451,   453,   455,   457,   459,   461,   463,
     465,   467,   469,   471,   473,   475,   477,   479,   481,   483,
     484,   486,   488,   489,   491,   493,   494,   496,   498,   500,
     502,   503,   504,   505,   509,   513,   517,   527,   537,   547,
     556,   565,   574,   585,   586,   589,   592,   597,   598,   601,
     607,   611,   614,   618,   624,   629,   636,   637
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || YYTOKEN_TABLE
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "ID", "PRINT", "FOR", "WHILE", "ASM",
  "ASM_WORD", "LET", "VAR", "CONST", "FUNCTION", "RETURN", "TRUE_TK",
  "FALSE_TK", "CDOUBLE", "CSTRING", "CINT", "PARENTESES_FUNCAO", "'='",
  "SETA", "MAIS_IGUAL", "'<'", "'>'", "MAIS_MAIS", "IGUAL", "DIF", "MA_IG",
  "ME_IG", "OR", "AND", "IF", "REDUCE", "ELSE", "'+'", "'-'", "'*'", "'/'",
  "'%'", "'['", "'('", "'.'", "';'", "'{'", "'}'", "')'", "','", "']'",
  "':'", "$accept", "S", "CMDs", "CMD", "EMPILHA_TS", "CMD_ASM",
  "ASM_STRING", "CMD_FUNC", "$@1", "CMD_RET", "LISTA_PARAMs", "PARAMs",
  "PARAM", "CMD_WHILE", "CMD_FOR", "PRIM_E", "CMD_IF", "CMD_LET",
  "CMD_VAR", "CMD_CONST", "VARs_LET", "VARs_VAR", "VARs_CONST", "VAR_LET",
  "VAR_VAR", "VAR_CONST", "LVALUEPROP", "E", "LISTA_ARGs", "ARGs", "ARG",
  "OBJ", "LISTA_CAMPOS", "CAMPO", "LISTA_ELEMS", "ELEM", 0
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[YYLEX-NUM] -- Internal token number corresponding to
   token YYLEX-NUM.  */
static const yytype_uint16 yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
      61,   275,   276,    60,    62,   277,   278,   279,   280,   281,
     282,   283,   284,   285,   286,    43,    45,    42,    47,    37,
      91,    40,    46,    59,   123,   125,    41,    44,    93,    58
};
# endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,    50,    51,    52,    52,    53,    53,    53,    53,    53,
      53,    53,    53,    53,    53,    53,    53,    53,    54,    55,
      56,    56,    58,    57,    59,    60,    60,    61,    61,    62,
      62,    62,    63,    64,    64,    65,    65,    65,    65,    66,
      66,    67,    68,    69,    70,    70,    71,    71,    72,    72,
      73,    73,    73,    74,    74,    74,    75,    75,    76,    76,
      77,    77,    77,    77,    77,    77,    77,    77,    77,    77,
      77,    77,    77,    77,    77,    77,    77,    77,    77,    77,
      77,    77,    77,    77,    77,    77,    77,    77,    77,    77,
      77,    77,    77,    77,    77,    77,    77,    77,    77,    77,
      77,    77,    77,    78,    78,    79,    79,    80,    80,    81,
      82,    82,    83,    83,    84,    84,    85,    85
};

/* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     1,     2,     1,     2,     2,     2,     1,     1,
       2,     3,     3,     3,     1,     1,     2,     4,     0,     3,
       2,     0,     0,     9,     1,     1,     0,     3,     2,     1,
       3,     3,     5,     9,     9,     1,     1,     1,     1,     5,
       7,     2,     2,     2,     3,     1,     3,     1,     3,     1,
       1,     3,     3,     1,     3,     3,     3,     3,     4,     3,
       3,     3,     2,     3,     3,     2,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     1,
       2,     2,     1,     2,     2,     1,     2,     2,     1,     1,
       1,     1,     3,     4,     3,     3,     7,     4,     6,     5,
       7,     5,     7,     1,     0,     3,     1,     1,     1,     3,
       3,     1,     3,     3,     3,     1,     1,     1
};

/* YYDEFACT[STATE-NAME] -- Default rule to reduce with in state
   STATE-NUM when YYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       0,    88,     0,     0,     0,     0,     0,     0,     0,    24,
      90,    91,    79,    85,    82,     0,     0,     0,     0,     0,
      18,     0,     2,     4,     9,     0,    14,    15,     8,     0,
       0,     0,    89,     0,     0,    18,     0,    62,     0,     0,
       0,     0,     0,    50,    41,    45,    53,    42,    47,     0,
      43,    49,    22,    26,     0,    80,    83,    81,    84,     0,
      87,   116,   117,     0,   115,    88,     0,     0,    25,    18,
       0,     0,    86,     0,     1,     3,    10,     0,     5,     6,
       7,     0,     0,    65,    21,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   104,
       0,    16,     0,    60,     0,    61,    13,     0,    35,    36,
      37,    38,     0,     0,     0,     0,     0,     0,     0,     0,
      29,     0,     0,     0,     0,   111,     0,    95,     0,    18,
       0,     0,    28,    92,    94,     0,    11,    63,    64,    21,
       0,    66,    67,    73,    72,    71,    70,    69,    68,    74,
      75,    76,    77,    78,     0,   107,     0,   103,   106,   108,
      59,    12,     0,    97,     0,     0,    51,    52,    44,    54,
      55,    46,    56,    57,    48,    26,     0,     0,     0,     0,
     109,     0,   114,    30,    31,    18,     0,     0,    27,    17,
      20,    19,    58,    93,     0,     0,     0,    32,     0,    30,
       0,    39,   112,   113,   110,     0,    99,     0,   101,   105,
      98,     0,     0,     0,     0,     0,     0,     0,     0,    96,
      40,   100,   102,     0,     0,    34,    33,    23
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,    21,    22,    23,    73,   102,   140,    24,   119,    25,
      67,    68,    69,    26,    27,   107,    28,    29,    30,    31,
      44,    47,    50,    45,    48,    51,    32,    33,   156,   157,
     158,    62,   124,   125,    63,    64
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -63
static const yytype_int16 yypact[] =
{
     515,   -13,   605,   -35,   -20,     7,    26,    27,     2,   -63,
     -63,   -63,   -63,   -63,   -63,     1,    19,    31,    10,   561,
       3,    56,   515,   -63,   -63,   595,   -63,   -63,   -63,    14,
      16,    24,    11,   702,   605,   -63,   605,   -63,    25,     3,
     851,   551,   605,    48,   -63,    28,    49,   -63,    29,    50,
     -63,    34,   -63,    75,   605,   -63,   -63,   -63,   -63,    -1,
     -63,   914,   -63,   -33,   -63,    52,    61,    82,    57,   -63,
     746,    59,   -63,   515,   -63,   -63,   -63,   872,   -63,   -63,
     -63,   605,   605,   -63,    95,   605,   605,   605,   605,   605,
     605,   605,   605,   605,   605,   605,   605,   605,   605,   639,
     103,   -63,    64,   914,   649,   914,   -63,    65,   -63,   -63,
     -63,   914,   767,   639,     7,   639,    26,   639,    27,    68,
      90,    66,   788,    69,     8,   -63,   639,   -63,   639,    70,
      96,    75,   -63,   -63,   -63,   214,   -63,   914,   914,    95,
      74,   934,   934,   951,   951,   951,   951,   951,   951,    23,
      23,   -22,   -22,   -22,   723,   914,    76,    73,   -63,   -63,
     -63,   -63,   257,   914,   605,   515,   914,   -63,   -63,   914,
     -63,   -63,   914,   -63,   -63,    75,   639,    81,   515,   639,
     -63,   124,   -63,   809,   -63,   -63,   605,   683,   -63,   -63,
     -63,   -63,   -63,   -63,   639,   300,   893,   -63,    83,   914,
     515,    94,   914,   -63,   -63,   515,   914,   257,   914,   -63,
     -63,   605,    86,   343,   515,   386,   429,   830,   515,   -63,
     -63,   -63,   -63,   515,   472,   -63,   702,   -63
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int8 yypgoto[] =
{
     -63,   -63,   -62,   -19,   -31,   -63,    -8,   -63,   -63,   -63,
     -52,   -63,     5,   -63,   -63,   -63,   -63,    91,    92,    93,
      30,    32,    33,   -63,   -63,   -63,   -63,    -2,   -63,   -63,
     -54,    22,   -63,   -40,   -63,    21
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If zero, do what YYDEFACT says.
   If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -61
static const yytype_int16 yytable[] =
{
      40,   121,   123,    75,   104,    52,    41,    34,    35,    36,
      43,   135,    37,     1,   126,   127,    61,    70,    98,    99,
     100,    42,    38,    77,    10,    11,    12,    13,    14,    46,
      49,    81,   103,    82,   105,    55,    83,    56,   132,   111,
     112,    71,    54,    53,    72,    16,    17,    57,    72,    58,
      18,    19,   122,   180,    59,   181,    74,    78,    60,    79,
      95,    96,    97,    98,    99,   100,    53,    80,   113,   115,
     117,   -29,   128,    35,    36,   114,   116,    37,   120,   137,
     138,   118,   129,   141,   142,   143,   144,   145,   146,   147,
     148,   149,   150,   151,   152,   153,   154,   155,   186,   -29,
     195,   130,   163,   139,   131,   134,   160,   161,   164,   175,
     176,   166,   177,   169,   185,   172,    75,   187,   179,   191,
     194,   159,   193,   198,    61,   200,   183,   123,   214,   212,
     218,   190,   108,   109,   110,   167,   188,   170,   213,   173,
     209,   204,     0,   215,   168,   216,   197,   182,   171,     0,
     184,   174,     0,     0,   205,     0,   224,     0,     0,   201,
       0,     0,   196,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   199,     0,    75,   202,     0,     0,
       0,     0,     0,     0,   206,   208,     0,     0,     0,     0,
       0,     0,   155,     0,    75,   220,    75,    75,   184,     0,
       0,   203,     0,     0,   225,    75,     0,     0,     0,   217,
       0,     0,     0,     0,     0,     0,   159,     1,     2,     3,
       4,   226,     0,     5,     6,     7,     8,     9,    10,    11,
      12,    13,    14,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    15,     0,     0,    16,
      17,     0,     0,     0,    18,    19,     0,     0,    20,   189,
       1,     2,     3,     4,     0,     0,     5,     6,     7,     8,
       9,    10,    11,    12,    13,    14,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    15,
       0,     0,    16,    17,     0,     0,     0,    18,    19,     0,
       0,    20,    72,     1,     2,     3,     4,     0,     0,     5,
       6,     7,     8,     9,    10,    11,    12,    13,    14,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    15,     0,     0,    16,    17,     0,     0,     0,
      18,    19,     0,     0,    20,   210,     1,     2,     3,     4,
       0,     0,     5,     6,     7,     8,     9,    10,    11,    12,
      13,    14,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    15,     0,     0,    16,    17,
       0,     0,     0,    18,    19,     0,     0,    20,   219,     1,
       2,     3,     4,     0,     0,     5,     6,     7,     8,     9,
      10,    11,    12,    13,    14,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    15,     0,
       0,    16,    17,     0,     0,     0,    18,    19,     0,     0,
      20,   221,     1,     2,     3,     4,     0,     0,     5,     6,
       7,     8,     9,    10,    11,    12,    13,    14,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    15,     0,     0,    16,    17,     0,     0,     0,    18,
      19,     0,     0,    20,   222,     1,     2,     3,     4,     0,
       0,     5,     6,     7,     8,     9,    10,    11,    12,    13,
      14,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    15,     0,     0,    16,    17,     0,
       0,     0,    18,    19,     0,     0,    20,   227,     1,     2,
       3,     4,     0,     0,     5,     6,     7,     8,     9,    10,
      11,    12,    13,    14,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    15,     0,     0,
      16,    17,     0,     0,     1,    18,    19,     0,     0,    20,
       5,     6,     7,    38,    65,    10,    11,    12,    13,    14,
       0,     0,     0,    38,     0,    10,    11,    12,    13,    14,
      66,     0,     0,     0,     0,     0,    16,    17,     0,     0,
       0,    18,    19,     0,     0,    39,    16,    17,     1,     0,
       0,    18,    19,     0,     0,    59,     0,    38,     1,    10,
      11,    12,    13,    14,     0,     0,     0,    38,     0,    10,
      11,    12,    13,    14,     0,     0,     0,     0,     0,     0,
      16,    17,     0,     0,     0,    18,    19,     0,    76,    39,
      16,    17,     1,     0,     0,    18,    19,     0,     0,    39,
       0,    38,     1,    10,    11,    12,    13,    14,     0,     0,
       0,    38,     0,    10,    11,    12,    13,    14,     0,     0,
       0,     0,     0,     0,    16,    17,     0,     0,     0,    18,
      19,     0,     0,    59,    16,    17,     1,     0,     0,    18,
      19,     0,     0,   162,     0,    38,     0,    10,    11,    12,
      13,    14,     0,     0,     0,     0,     0,     0,     0,    84,
       0,     0,     0,     0,     0,     0,     0,     0,    16,    17,
       0,     0,     0,    18,    19,    85,    86,   207,    87,    88,
      89,    90,    91,    92,     0,     0,     0,    93,    94,    95,
      96,    97,    98,    99,   100,   101,    85,    86,     0,    87,
      88,    89,    90,    91,    92,     0,     0,     0,    93,    94,
      95,    96,    97,    98,    99,   100,     0,     0,     0,    85,
      86,   192,    87,    88,    89,    90,    91,    92,     0,     0,
       0,    93,    94,    95,    96,    97,    98,    99,   100,     0,
      85,    86,   133,    87,    88,    89,    90,    91,    92,     0,
       0,     0,    93,    94,    95,    96,    97,    98,    99,   100,
       0,    85,    86,   165,    87,    88,    89,    90,    91,    92,
       0,     0,     0,    93,    94,    95,    96,    97,    98,    99,
     100,     0,    85,    86,   178,    87,    88,    89,    90,    91,
      92,     0,     0,     0,    93,    94,    95,    96,    97,    98,
      99,   100,     0,    85,    86,   -60,    87,    88,    89,    90,
      91,    92,     0,     0,     0,    93,    94,    95,    96,    97,
      98,    99,   100,     0,    85,    86,   223,    87,    88,    89,
      90,    91,    92,     0,     0,     0,    93,    94,    95,    96,
      97,    98,    99,   100,   106,    85,    86,     0,    87,    88,
      89,    90,    91,    92,     0,     0,     0,    93,    94,    95,
      96,    97,    98,    99,   100,   136,    85,    86,     0,    87,
      88,    89,    90,    91,    92,     0,     0,     0,    93,    94,
      95,    96,    97,    98,    99,   100,   211,    85,    86,     0,
      87,    88,    89,    90,    91,    92,     0,     0,     0,    93,
      94,    95,    96,    97,    98,    99,   100,   -61,   -61,     0,
      87,    88,    89,    90,    91,    92,     0,     0,     0,    93,
      94,    95,    96,    97,    98,    99,   100,   -61,   -61,   -61,
     -61,   -61,   -61,     0,     0,     0,    93,    94,    95,    96,
      97,    98,    99,   100
};

static const yytype_int16 yycheck[] =
{
       2,    53,     3,    22,    35,     3,    41,    20,    21,    22,
       3,    73,    25,     3,    47,    48,    18,    19,    40,    41,
      42,    41,    12,    25,    14,    15,    16,    17,    18,     3,
       3,    20,    34,    22,    36,    16,    25,    18,    69,    41,
      42,    19,    41,    41,    45,    35,    36,    16,    45,    18,
      40,    41,    54,    45,    44,    47,     0,    43,    48,    43,
      37,    38,    39,    40,    41,    42,    41,    43,    20,    20,
      20,    19,    20,    21,    22,    47,    47,    25,     3,    81,
      82,    47,    21,    85,    86,    87,    88,    89,    90,    91,
      92,    93,    94,    95,    96,    97,    98,    99,   129,    47,
     162,    19,   104,     8,    47,    46,     3,    43,    43,    41,
      20,   113,    46,   115,    44,   117,   135,    21,    49,    45,
      47,    99,    46,   175,   126,    44,   128,     3,    34,    46,
      44,   139,    41,    41,    41,   113,   131,   115,   200,   117,
     194,   181,    -1,   205,   114,   207,   165,   126,   116,    -1,
     128,   118,    -1,    -1,   185,    -1,   218,    -1,    -1,   178,
      -1,    -1,   164,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   176,    -1,   195,   179,    -1,    -1,
      -1,    -1,    -1,    -1,   186,   187,    -1,    -1,    -1,    -1,
      -1,    -1,   194,    -1,   213,   214,   215,   216,   176,    -1,
      -1,   179,    -1,    -1,   223,   224,    -1,    -1,    -1,   211,
      -1,    -1,    -1,    -1,    -1,    -1,   194,     3,     4,     5,
       6,   223,    -1,     9,    10,    11,    12,    13,    14,    15,
      16,    17,    18,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    32,    -1,    -1,    35,
      36,    -1,    -1,    -1,    40,    41,    -1,    -1,    44,    45,
       3,     4,     5,     6,    -1,    -1,     9,    10,    11,    12,
      13,    14,    15,    16,    17,    18,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    32,
      -1,    -1,    35,    36,    -1,    -1,    -1,    40,    41,    -1,
      -1,    44,    45,     3,     4,     5,     6,    -1,    -1,     9,
      10,    11,    12,    13,    14,    15,    16,    17,    18,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    32,    -1,    -1,    35,    36,    -1,    -1,    -1,
      40,    41,    -1,    -1,    44,    45,     3,     4,     5,     6,
      -1,    -1,     9,    10,    11,    12,    13,    14,    15,    16,
      17,    18,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    32,    -1,    -1,    35,    36,
      -1,    -1,    -1,    40,    41,    -1,    -1,    44,    45,     3,
       4,     5,     6,    -1,    -1,     9,    10,    11,    12,    13,
      14,    15,    16,    17,    18,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    32,    -1,
      -1,    35,    36,    -1,    -1,    -1,    40,    41,    -1,    -1,
      44,    45,     3,     4,     5,     6,    -1,    -1,     9,    10,
      11,    12,    13,    14,    15,    16,    17,    18,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    32,    -1,    -1,    35,    36,    -1,    -1,    -1,    40,
      41,    -1,    -1,    44,    45,     3,     4,     5,     6,    -1,
      -1,     9,    10,    11,    12,    13,    14,    15,    16,    17,
      18,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    32,    -1,    -1,    35,    36,    -1,
      -1,    -1,    40,    41,    -1,    -1,    44,    45,     3,     4,
       5,     6,    -1,    -1,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    32,    -1,    -1,
      35,    36,    -1,    -1,     3,    40,    41,    -1,    -1,    44,
       9,    10,    11,    12,     3,    14,    15,    16,    17,    18,
      -1,    -1,    -1,    12,    -1,    14,    15,    16,    17,    18,
      19,    -1,    -1,    -1,    -1,    -1,    35,    36,    -1,    -1,
      -1,    40,    41,    -1,    -1,    44,    35,    36,     3,    -1,
      -1,    40,    41,    -1,    -1,    44,    -1,    12,     3,    14,
      15,    16,    17,    18,    -1,    -1,    -1,    12,    -1,    14,
      15,    16,    17,    18,    -1,    -1,    -1,    -1,    -1,    -1,
      35,    36,    -1,    -1,    -1,    40,    41,    -1,    43,    44,
      35,    36,     3,    -1,    -1,    40,    41,    -1,    -1,    44,
      -1,    12,     3,    14,    15,    16,    17,    18,    -1,    -1,
      -1,    12,    -1,    14,    15,    16,    17,    18,    -1,    -1,
      -1,    -1,    -1,    -1,    35,    36,    -1,    -1,    -1,    40,
      41,    -1,    -1,    44,    35,    36,     3,    -1,    -1,    40,
      41,    -1,    -1,    44,    -1,    12,    -1,    14,    15,    16,
      17,    18,    -1,    -1,    -1,    -1,    -1,    -1,    -1,     7,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    35,    36,
      -1,    -1,    -1,    40,    41,    23,    24,    44,    26,    27,
      28,    29,    30,    31,    -1,    -1,    -1,    35,    36,    37,
      38,    39,    40,    41,    42,    43,    23,    24,    -1,    26,
      27,    28,    29,    30,    31,    -1,    -1,    -1,    35,    36,
      37,    38,    39,    40,    41,    42,    -1,    -1,    -1,    23,
      24,    48,    26,    27,    28,    29,    30,    31,    -1,    -1,
      -1,    35,    36,    37,    38,    39,    40,    41,    42,    -1,
      23,    24,    46,    26,    27,    28,    29,    30,    31,    -1,
      -1,    -1,    35,    36,    37,    38,    39,    40,    41,    42,
      -1,    23,    24,    46,    26,    27,    28,    29,    30,    31,
      -1,    -1,    -1,    35,    36,    37,    38,    39,    40,    41,
      42,    -1,    23,    24,    46,    26,    27,    28,    29,    30,
      31,    -1,    -1,    -1,    35,    36,    37,    38,    39,    40,
      41,    42,    -1,    23,    24,    46,    26,    27,    28,    29,
      30,    31,    -1,    -1,    -1,    35,    36,    37,    38,    39,
      40,    41,    42,    -1,    23,    24,    46,    26,    27,    28,
      29,    30,    31,    -1,    -1,    -1,    35,    36,    37,    38,
      39,    40,    41,    42,    43,    23,    24,    -1,    26,    27,
      28,    29,    30,    31,    -1,    -1,    -1,    35,    36,    37,
      38,    39,    40,    41,    42,    43,    23,    24,    -1,    26,
      27,    28,    29,    30,    31,    -1,    -1,    -1,    35,    36,
      37,    38,    39,    40,    41,    42,    43,    23,    24,    -1,
      26,    27,    28,    29,    30,    31,    -1,    -1,    -1,    35,
      36,    37,    38,    39,    40,    41,    42,    23,    24,    -1,
      26,    27,    28,    29,    30,    31,    -1,    -1,    -1,    35,
      36,    37,    38,    39,    40,    41,    42,    26,    27,    28,
      29,    30,    31,    -1,    -1,    -1,    35,    36,    37,    38,
      39,    40,    41,    42
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,     3,     4,     5,     6,     9,    10,    11,    12,    13,
      14,    15,    16,    17,    18,    32,    35,    36,    40,    41,
      44,    51,    52,    53,    57,    59,    63,    64,    66,    67,
      68,    69,    76,    77,    20,    21,    22,    25,    12,    44,
      77,    41,    41,     3,    70,    73,     3,    71,    74,     3,
      72,    75,     3,    41,    41,    16,    18,    16,    18,    44,
      48,    77,    81,    84,    85,     3,    19,    60,    61,    62,
      77,    81,    45,    54,     0,    53,    43,    77,    43,    43,
      43,    20,    22,    25,     7,    23,    24,    26,    27,    28,
      29,    30,    31,    35,    36,    37,    38,    39,    40,    41,
      42,    43,    55,    77,    54,    77,    43,    65,    67,    68,
      69,    77,    77,    20,    47,    20,    47,    20,    47,    58,
       3,    60,    77,     3,    82,    83,    47,    48,    20,    21,
      19,    47,    54,    46,    46,    52,    43,    77,    77,     8,
      56,    77,    77,    77,    77,    77,    77,    77,    77,    77,
      77,    77,    77,    77,    77,    77,    78,    79,    80,    81,
       3,    43,    44,    77,    43,    46,    77,    81,    70,    77,
      81,    71,    77,    81,    72,    41,    20,    46,    46,    49,
      45,    47,    85,    77,    81,    44,    54,    21,    62,    45,
      56,    45,    48,    46,    47,    52,    77,    53,    60,    77,
      44,    53,    77,    81,    83,    54,    77,    44,    77,    80,
      45,    43,    46,    52,    34,    52,    52,    77,    44,    45,
      53,    45,    45,    46,    52,    53,    77,    45
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
#line 176 "java.y"
    { print( resolve_enderecos( (yyvsp[(1) - (1)]).c + "." + funcoes ) ); ;}
    break;

  case 3:

/* Line 1455 of yacc.c  */
#line 179 "java.y"
    { (yyval).c =  (yyvsp[(1) - (2)]).c + (yyvsp[(2) - (2)]).c ; ;}
    break;

  case 10:

/* Line 1455 of yacc.c  */
#line 189 "java.y"
    { (yyval).c = "undefined" + ("@" + (yyvsp[(1) - (2)]).c); ;}
    break;

  case 11:

/* Line 1455 of yacc.c  */
#line 191 "java.y"
    { (yyval).c = (yyvsp[(2) - (3)]).c + (yyvsp[(1) - (3)]).c; ;}
    break;

  case 12:

/* Line 1455 of yacc.c  */
#line 193 "java.y"
    { (yyval).c = (yyvsp[(1) - (3)]).c + (yyvsp[(2) - (3)]).c; ;}
    break;

  case 13:

/* Line 1455 of yacc.c  */
#line 195 "java.y"
    { (yyval).c = (yyvsp[(2) - (3)]).c + "println" + "#"; ;}
    break;

  case 16:

/* Line 1455 of yacc.c  */
#line 199 "java.y"
    { (yyval).c = (yyvsp[(1) - (2)]).c + "^"; ;}
    break;

  case 17:

/* Line 1455 of yacc.c  */
#line 201 "java.y"
    { ts.pop_back(); (yyval).c = "<{" + (yyvsp[(3) - (4)]).c + "}>"; ;}
    break;

  case 18:

/* Line 1455 of yacc.c  */
#line 205 "java.y"
    { ts.push_back( map< string, Simbolo >{} ); ;}
    break;

  case 19:

/* Line 1455 of yacc.c  */
#line 208 "java.y"
    { (yyval).c = (yyvsp[(2) - (3)]).c + "^"; ;}
    break;

  case 20:

/* Line 1455 of yacc.c  */
#line 211 "java.y"
    { (yyval).c = (yyvsp[(1) - (2)]).c + (yyvsp[(2) - (2)]).c;;}
    break;

  case 21:

/* Line 1455 of yacc.c  */
#line 212 "java.y"
    { (yyval).clear(); ;}
    break;

  case 22:

/* Line 1455 of yacc.c  */
#line 215 "java.y"
    { declara_var( DeclVar, (yyvsp[(2) - (2)]).c[0], (yyvsp[(2) - (2)]).linha, (yyvsp[(2) - (2)]).coluna ); dentroFuncao = true;;}
    break;

  case 23:

/* Line 1455 of yacc.c  */
#line 217 "java.y"
    { 
             string lbl_endereco_funcao = gera_label( "func_" + (yyvsp[(2) - (9)]).c[0] );
             string definicao_lbl_endereco_funcao = ":" + lbl_endereco_funcao;
             
             (yyval).c = (yyvsp[(2) - (9)]).c + "&" + (yyvsp[(2) - (9)]).c + "{}"  + "=" + "'&funcao'" +
                    lbl_endereco_funcao + "[=]" + "^";
             funcoes = funcoes + definicao_lbl_endereco_funcao + (yyvsp[(5) - (9)]).c + (yyvsp[(8) - (9)]).c +
                       "undefined" + "@" + "'&retorno'" + "@"+ "~";
             ts.pop_back(); 
             dentroFuncao = false;
           ;}
    break;

  case 24:

/* Line 1455 of yacc.c  */
#line 231 "java.y"
    { 
          if (!dentroFuncao) {
            cerr << "Erro: Não é permitido 'return' fora de funções." << endl;
            exit( 1 );     
          }
          (yyval).c = vector<string>{"'&retorno'", "@", "~"}; 
        ;}
    break;

  case 26:

/* Line 1455 of yacc.c  */
#line 241 "java.y"
    { (yyval).clear(); ;}
    break;

  case 27:

/* Line 1455 of yacc.c  */
#line 245 "java.y"
    { 
         declara_var( DeclLet, (yyvsp[(3) - (3)]).c[0], (yyvsp[(1) - (3)]).linha, (yyvsp[(1) - (3)]).coluna );
         (yyval).c = (yyvsp[(1) - (3)]).c + (yyvsp[(3) - (3)]).c + "&" + (yyvsp[(3) - (3)]).c + "arguments" + "@" + to_string( (yyvsp[(1) - (3)]).contador )
                + "[@]" + "=" + "^"; 
         if( (yyvsp[(3) - (3)]).valor_default.size() > 0 ) {
            // Teste para usar valor default
            string lbl_true = gera_label( "lbl_true" );
            string lbl_fim_if = gera_label( "lbl_fim_if" );
            string definicao_lbl_true = ":" + lbl_true;
            string definicao_lbl_fim_if = ":" + lbl_fim_if;
                    
            (yyval).c = (yyval).c + (yyvsp[(3) - (3)]).c + "@" + "undefined" + "@" + "==" +
                   lbl_true + "?" +             
                   lbl_fim_if + "#" +           
                   definicao_lbl_true +
                   (yyvsp[(3) - (3)]).c + (yyvsp[(3) - (3)]).valor_default + "=" + "^" +  
                   definicao_lbl_fim_if         
                   ;
         }
         (yyval).contador = (yyvsp[(1) - (3)]).contador + (yyvsp[(3) - (3)]).contador; 
       ;}
    break;

  case 28:

/* Line 1455 of yacc.c  */
#line 267 "java.y"
    {  
         declara_var( DeclLet, (yyvsp[(1) - (2)]).c[0], (yyvsp[(1) - (2)]).linha, (yyvsp[(1) - (2)]).coluna );
         (yyval).c = (yyvsp[(1) - (2)]).c + "&" + (yyvsp[(1) - (2)]).c + "arguments" + "@" + "0" + "[@]" + "=" + "^"; 
         if( (yyvsp[(1) - (2)]).valor_default.size() > 0 ) {
            // Teste para usar valor default
            string lbl_true = gera_label( "lbl_true" );
            string lbl_fim_if = gera_label( "lbl_fim_if" );
            string definicao_lbl_true = ":" + lbl_true;
            string definicao_lbl_fim_if = ":" + lbl_fim_if;
                    
            (yyval).c = (yyval).c + (yyvsp[(1) - (2)]).c + "@" + "undefined" + "@" + "==" +
                   lbl_true + "?" +             
                   lbl_fim_if + "#" +           
                   definicao_lbl_true +
                   (yyvsp[(1) - (2)]).c + (yyvsp[(1) - (2)]).valor_default + "=" + "^" +  
                   definicao_lbl_fim_if        
                   ;
         }
         (yyval).contador = (yyvsp[(1) - (2)]).contador; 
       ;}
    break;

  case 29:

/* Line 1455 of yacc.c  */
#line 290 "java.y"
    { (yyval).c = (yyvsp[(1) - (1)]).c;      
        (yyval).contador = 1;
        (yyval).valor_default.clear();
      ;}
    break;

  case 30:

/* Line 1455 of yacc.c  */
#line 295 "java.y"
    {
        (yyval).c = (yyvsp[(1) - (3)]).c;
        (yyval).contador = 1;
        (yyval).valor_default = (yyvsp[(3) - (3)]).c;          
      ;}
    break;

  case 31:

/* Line 1455 of yacc.c  */
#line 301 "java.y"
    {
        (yyval).c = (yyvsp[(1) - (3)]).c;
        (yyval).contador = 1;
        (yyval).valor_default = (yyvsp[(3) - (3)]).c;          
      ;}
    break;

  case 32:

/* Line 1455 of yacc.c  */
#line 309 "java.y"
    { string lbl_fim_while = gera_label( "fim_while" );
            string lbl_condicao_while = gera_label( "condicao_while" );
            string lbl_comando_while = gera_label( "comando_while" );
            string definicao_lbl_fim_while = ":" + lbl_fim_while;
            string definicao_lbl_condicao_while = ":" + lbl_condicao_while;
            string definicao_lbl_comando_while = ":" +lbl_comando_while;

            (yyval).c = definicao_lbl_condicao_while + (yyvsp[(3) - (5)]).c +
                   lbl_comando_while + "?" + lbl_fim_while + "#" +
                   definicao_lbl_comando_while + (yyvsp[(5) - (5)]).c +
                   lbl_condicao_while + "#" +
                   definicao_lbl_fim_while;
          ;}
    break;

  case 33:

/* Line 1455 of yacc.c  */
#line 325 "java.y"
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

  case 34:

/* Line 1455 of yacc.c  */
#line 339 "java.y"
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

  case 38:

/* Line 1455 of yacc.c  */
#line 358 "java.y"
    { (yyval).c = (yyvsp[(1) - (1)]).c + "^"; ;}
    break;

  case 39:

/* Line 1455 of yacc.c  */
#line 362 "java.y"
    { string lbl_true = gera_label( "lbl_true" );
           string lbl_fim_if = gera_label( "lbl_fim_if" );
           string definicao_lbl_true = ":" + lbl_true;
           string definicao_lbl_fim_if = ":" + lbl_fim_if;
                    
            (yyval).c = (yyvsp[(3) - (5)]).c +                       // Codigo da expressão
                   lbl_true + "?" +             // Código do IF
                   lbl_fim_if + "#" +           // Código do False
                   definicao_lbl_true + (yyvsp[(5) - (5)]).c +  // Código do True
                   definicao_lbl_fim_if         // Fim do IF
                   ;
         ;}
    break;

  case 40:

/* Line 1455 of yacc.c  */
#line 375 "java.y"
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

  case 41:

/* Line 1455 of yacc.c  */
#line 389 "java.y"
    {  (yyval).c = (yyvsp[(2) - (2)]).c; ;}
    break;

  case 42:

/* Line 1455 of yacc.c  */
#line 392 "java.y"
    {  (yyval).c = (yyvsp[(2) - (2)]).c;;}
    break;

  case 43:

/* Line 1455 of yacc.c  */
#line 395 "java.y"
    {  (yyval).c = (yyvsp[(2) - (2)]).c;;}
    break;

  case 44:

/* Line 1455 of yacc.c  */
#line 398 "java.y"
    { (yyval).c = (yyvsp[(1) - (3)]).c + (yyvsp[(3) - (3)]).c; ;}
    break;

  case 46:

/* Line 1455 of yacc.c  */
#line 402 "java.y"
    { (yyval).c = (yyvsp[(1) - (3)]).c + (yyvsp[(3) - (3)]).c; ;}
    break;

  case 48:

/* Line 1455 of yacc.c  */
#line 406 "java.y"
    { (yyval).c = (yyvsp[(1) - (3)]).c + (yyvsp[(3) - (3)]).c; ;}
    break;

  case 50:

/* Line 1455 of yacc.c  */
#line 411 "java.y"
    { (yyval).c = declara_var( DeclLet, (yyvsp[(1) - (1)]).c[0], (yyvsp[(1) - (1)]).linha, (yyvsp[(1) - (1)]).coluna );;}
    break;

  case 51:

/* Line 1455 of yacc.c  */
#line 413 "java.y"
    { (yyval).c = declara_var( DeclLet, (yyvsp[(1) - (3)]).c[0], (yyvsp[(1) - (3)]).linha, (yyvsp[(1) - (3)]).coluna ) + (yyvsp[(1) - (3)]).c + (yyvsp[(3) - (3)]).c + "=" + "^";;}
    break;

  case 52:

/* Line 1455 of yacc.c  */
#line 415 "java.y"
    { (yyval).c = declara_var( DeclLet, (yyvsp[(1) - (3)]).c[0], (yyvsp[(1) - (3)]).linha, (yyvsp[(1) - (3)]).coluna ) + (yyvsp[(1) - (3)]).c + (yyvsp[(3) - (3)]).c + "=" + "^";;}
    break;

  case 53:

/* Line 1455 of yacc.c  */
#line 419 "java.y"
    { (yyval).c = declara_var( DeclVar, (yyvsp[(1) - (1)]).c[0], (yyvsp[(1) - (1)]).linha, (yyvsp[(1) - (1)]).coluna ); ;}
    break;

  case 54:

/* Line 1455 of yacc.c  */
#line 421 "java.y"
    { (yyval).c = declara_var( DeclVar, (yyvsp[(1) - (3)]).c[0], (yyvsp[(1) - (3)]).linha, (yyvsp[(1) - (3)]).coluna ) + (yyvsp[(1) - (3)]).c + (yyvsp[(3) - (3)]).c + "=" + "^";;}
    break;

  case 55:

/* Line 1455 of yacc.c  */
#line 423 "java.y"
    { (yyval).c = declara_var( DeclVar, (yyvsp[(1) - (3)]).c[0], (yyvsp[(1) - (3)]).linha, (yyvsp[(1) - (3)]).coluna ) + (yyvsp[(1) - (3)]).c + (yyvsp[(3) - (3)]).c + "=" + "^";;}
    break;

  case 56:

/* Line 1455 of yacc.c  */
#line 427 "java.y"
    { (yyval).c = declara_var( DeclConst, (yyvsp[(1) - (3)]).c[0], (yyvsp[(1) - (3)]).linha, (yyvsp[(1) - (3)]).coluna ) + (yyvsp[(1) - (3)]).c + (yyvsp[(3) - (3)]).c + "=" + "^"; ;}
    break;

  case 57:

/* Line 1455 of yacc.c  */
#line 429 "java.y"
    { (yyval).c = declara_var( DeclConst, (yyvsp[(1) - (3)]).c[0], (yyvsp[(1) - (3)]).linha, (yyvsp[(1) - (3)]).coluna ) + (yyvsp[(1) - (3)]).c + (yyvsp[(3) - (3)]).c + "=" + "^";;}
    break;

  case 58:

/* Line 1455 of yacc.c  */
#line 432 "java.y"
    { (yyval).c = (yyvsp[(1) - (4)]).c + (yyvsp[(3) - (4)]).c; ;}
    break;

  case 59:

/* Line 1455 of yacc.c  */
#line 433 "java.y"
    { (yyval).c = (yyvsp[(1) - (3)]).c + (yyvsp[(3) - (3)]).c; ;}
    break;

  case 60:

/* Line 1455 of yacc.c  */
#line 437 "java.y"
    { 
      checa_simbolo( (yyvsp[(1) - (3)]).c[0], true );  
      (yyval).c = (yyvsp[(1) - (3)]).c + (yyvsp[(3) - (3)]).c + "="; 
    ;}
    break;

  case 61:

/* Line 1455 of yacc.c  */
#line 442 "java.y"
    {
      checa_simbolo( (yyvsp[(1) - (3)]).c[0], true );  
      (yyval).c = (yyvsp[(1) - (3)]).c + (yyvsp[(1) - (3)]).c + "@" + (yyvsp[(3) - (3)]).c + "+" + "="; 
    ;}
    break;

  case 62:

/* Line 1455 of yacc.c  */
#line 447 "java.y"
    {
      checa_simbolo( (yyvsp[(1) - (2)]).c[0], true );  
      (yyval).c = (yyvsp[(1) - (2)]).c + "@" + (yyvsp[(1) - (2)]).c + (yyvsp[(1) - (2)]).c + "@" + "1" + "+" + "=" + "^"; 
    ;}
    break;

  case 63:

/* Line 1455 of yacc.c  */
#line 452 "java.y"
    { (yyval).c = (yyvsp[(1) - (3)]).c + (yyvsp[(3) - (3)]).c + "[=]"; ;}
    break;

  case 64:

/* Line 1455 of yacc.c  */
#line 454 "java.y"
    { (yyval).c = (yyvsp[(1) - (3)]).c + (yyvsp[(1) - (3)]).c + "[@]" + (yyvsp[(3) - (3)]).c + "+" + "[=]"; ;}
    break;

  case 65:

/* Line 1455 of yacc.c  */
#line 456 "java.y"
    { (yyval).c = (yyvsp[(1) - (2)]).c + "[@]" + (yyvsp[(1) - (2)]).c + (yyvsp[(1) - (2)]).c + "[@]" + "1" + "+" + "=" + "^"; ;}
    break;

  case 66:

/* Line 1455 of yacc.c  */
#line 458 "java.y"
    { (yyval).c = (yyvsp[(1) - (3)]).c + (yyvsp[(3) - (3)]).c + (yyvsp[(2) - (3)]).c; ;}
    break;

  case 67:

/* Line 1455 of yacc.c  */
#line 460 "java.y"
    { (yyval).c = (yyvsp[(1) - (3)]).c + (yyvsp[(3) - (3)]).c + (yyvsp[(2) - (3)]).c; ;}
    break;

  case 68:

/* Line 1455 of yacc.c  */
#line 462 "java.y"
    { (yyval).c = (yyvsp[(1) - (3)]).c + (yyvsp[(3) - (3)]).c + (yyvsp[(2) - (3)]).c; ;}
    break;

  case 69:

/* Line 1455 of yacc.c  */
#line 464 "java.y"
    { (yyval).c = (yyvsp[(1) - (3)]).c + (yyvsp[(3) - (3)]).c + (yyvsp[(2) - (3)]).c; ;}
    break;

  case 70:

/* Line 1455 of yacc.c  */
#line 466 "java.y"
    { (yyval).c = (yyvsp[(1) - (3)]).c + (yyvsp[(3) - (3)]).c + (yyvsp[(2) - (3)]).c; ;}
    break;

  case 71:

/* Line 1455 of yacc.c  */
#line 468 "java.y"
    { (yyval).c = (yyvsp[(1) - (3)]).c + (yyvsp[(3) - (3)]).c + (yyvsp[(2) - (3)]).c; ;}
    break;

  case 72:

/* Line 1455 of yacc.c  */
#line 470 "java.y"
    { (yyval).c = (yyvsp[(1) - (3)]).c + (yyvsp[(3) - (3)]).c + (yyvsp[(2) - (3)]).c; ;}
    break;

  case 73:

/* Line 1455 of yacc.c  */
#line 472 "java.y"
    { (yyval).c = (yyvsp[(1) - (3)]).c + (yyvsp[(3) - (3)]).c + (yyvsp[(2) - (3)]).c; ;}
    break;

  case 74:

/* Line 1455 of yacc.c  */
#line 474 "java.y"
    { (yyval).c = (yyvsp[(1) - (3)]).c + (yyvsp[(3) - (3)]).c + (yyvsp[(2) - (3)]).c; ;}
    break;

  case 75:

/* Line 1455 of yacc.c  */
#line 476 "java.y"
    { (yyval).c = (yyvsp[(1) - (3)]).c + (yyvsp[(3) - (3)]).c + (yyvsp[(2) - (3)]).c; ;}
    break;

  case 76:

/* Line 1455 of yacc.c  */
#line 478 "java.y"
    { (yyval).c = (yyvsp[(1) - (3)]).c + (yyvsp[(3) - (3)]).c + (yyvsp[(2) - (3)]).c; ;}
    break;

  case 77:

/* Line 1455 of yacc.c  */
#line 480 "java.y"
    { (yyval).c = (yyvsp[(1) - (3)]).c + (yyvsp[(3) - (3)]).c + (yyvsp[(2) - (3)]).c; ;}
    break;

  case 78:

/* Line 1455 of yacc.c  */
#line 482 "java.y"
    { (yyval).c = (yyvsp[(1) - (3)]).c + (yyvsp[(3) - (3)]).c + (yyvsp[(2) - (3)]).c; ;}
    break;

  case 80:

/* Line 1455 of yacc.c  */
#line 485 "java.y"
    { (yyval).c = (yyvsp[(2) - (2)]).c; ;}
    break;

  case 81:

/* Line 1455 of yacc.c  */
#line 487 "java.y"
    { (yyval).c = "0" + (yyvsp[(2) - (2)]).c + "-"; ;}
    break;

  case 83:

/* Line 1455 of yacc.c  */
#line 490 "java.y"
    { (yyval).c = (yyvsp[(2) - (2)]).c; ;}
    break;

  case 84:

/* Line 1455 of yacc.c  */
#line 492 "java.y"
    { (yyval).c = "0" + (yyvsp[(2) - (2)]).c + "-"; ;}
    break;

  case 86:

/* Line 1455 of yacc.c  */
#line 495 "java.y"
    { (yyval).c = vector<string>{"{}"}; ;}
    break;

  case 87:

/* Line 1455 of yacc.c  */
#line 497 "java.y"
    { (yyval).c = vector<string>{"[]"}; ;}
    break;

  case 88:

/* Line 1455 of yacc.c  */
#line 499 "java.y"
    { (yyval).c = (yyvsp[(1) - (1)]).c + "@"; ;}
    break;

  case 89:

/* Line 1455 of yacc.c  */
#line 501 "java.y"
    { (yyval).c = (yyvsp[(1) - (1)]).c + "[@]"; ;}
    break;

  case 92:

/* Line 1455 of yacc.c  */
#line 504 "java.y"
    { (yyval).c = (yyvsp[(2) - (3)]).c; ;}
    break;

  case 93:

/* Line 1455 of yacc.c  */
#line 506 "java.y"
    {
      (yyval).c = (yyvsp[(3) - (4)]).c + to_string( (yyvsp[(3) - (4)]).contador ) + (yyvsp[(1) - (4)]).c + "$";
    ;}
    break;

  case 94:

/* Line 1455 of yacc.c  */
#line 510 "java.y"
    { 
      (yyval).c = (yyvsp[(2) - (3)]).c;
    ;}
    break;

  case 95:

/* Line 1455 of yacc.c  */
#line 514 "java.y"
    {
      (yyval).c = "[]" + (yyvsp[(2) - (3)]).c;
    ;}
    break;

  case 96:

/* Line 1455 of yacc.c  */
#line 518 "java.y"
    { 
             string lbl_endereco_funcao = gera_label( "func_anon" );
             string definicao_lbl_endereco_funcao = ":" + lbl_endereco_funcao;
             
             (yyval).c = vector<string>{"{}"} + "'&funcao'" + lbl_endereco_funcao + "[<=]";
             funcoes = funcoes + definicao_lbl_endereco_funcao + (yyvsp[(3) - (7)]).c + (yyvsp[(6) - (7)]).c +
                       "undefined" + "@" + "'&retorno'" + "@"+ "~";
             ts.pop_back(); 
           ;}
    break;

  case 97:

/* Line 1455 of yacc.c  */
#line 528 "java.y"
    {
        string lbl_endereco_funcao = gera_label( "func_anon" );
        string definicao_lbl_endereco_funcao = ":" + lbl_endereco_funcao;
             
        (yyval).c = vector<string>{"{}"} + "'&funcao'" + lbl_endereco_funcao + "[<=]";
        funcoes = funcoes + definicao_lbl_endereco_funcao + declara_var( DeclLet, (yyvsp[(1) - (4)]).c[0], (yyvsp[(1) - (4)]).linha, (yyvsp[(1) - (4)]).coluna ) +
        (yyvsp[(4) - (4)]).c + "'&retorno'" + "@"+ "~";
        ts.pop_back(); 
      ;}
    break;

  case 98:

/* Line 1455 of yacc.c  */
#line 538 "java.y"
    {
        string lbl_endereco_funcao = gera_label( "func_anon" );
        string definicao_lbl_endereco_funcao = ":" + lbl_endereco_funcao;
             
        (yyval).c = vector<string>{"{}"} + "'&funcao'" + lbl_endereco_funcao + "[<=]";
        funcoes = funcoes + definicao_lbl_endereco_funcao + declara_var( DeclLet, (yyvsp[(1) - (6)]).c[0], (yyvsp[(1) - (6)]).linha, (yyvsp[(1) - (6)]).coluna ) +
        (yyvsp[(5) - (6)]).c + "undefined" + "@" + "'&retorno'" + "@"+ "~";
        ts.pop_back(); 
      ;}
    break;

  case 99:

/* Line 1455 of yacc.c  */
#line 548 "java.y"
    {
        string lbl_endereco_funcao = gera_label( "func_anon" );
        string definicao_lbl_endereco_funcao = ":" + lbl_endereco_funcao;
             
        (yyval).c = vector<string>{"{}"} + "'&funcao'" + lbl_endereco_funcao + "[<=]";
        funcoes = funcoes + definicao_lbl_endereco_funcao + (yyvsp[(4) - (5)]).c + "'&retorno'" + "@"+ "~";
        ts.pop_back();
      ;}
    break;

  case 100:

/* Line 1455 of yacc.c  */
#line 557 "java.y"
    {
        string lbl_endereco_funcao = gera_label( "func_anon" );
        string definicao_lbl_endereco_funcao = ":" + lbl_endereco_funcao;
             
        (yyval).c = vector<string>{"{}"} + "'&funcao'" + lbl_endereco_funcao + "[<=]";
        funcoes = funcoes + definicao_lbl_endereco_funcao + (yyvsp[(6) - (7)]).c + "undefined" + "@" + "'&retorno'" + "@"+ "~";
        ts.pop_back();
      ;}
    break;

  case 101:

/* Line 1455 of yacc.c  */
#line 566 "java.y"
    {
        string lbl_endereco_funcao = gera_label( "func_anon" );
        string definicao_lbl_endereco_funcao = ":" + lbl_endereco_funcao;
             
        (yyval).c = vector<string>{"{}"} + "'&funcao'" + lbl_endereco_funcao + "[<=]";
        funcoes = funcoes + definicao_lbl_endereco_funcao + (yyvsp[(2) - (5)]).c + (yyvsp[(5) - (5)]).c + "'&retorno'" + "@"+ "~";
        ts.pop_back();
      ;}
    break;

  case 102:

/* Line 1455 of yacc.c  */
#line 575 "java.y"
    {
        string lbl_endereco_funcao = gera_label( "func_anon" );
        string definicao_lbl_endereco_funcao = ":" + lbl_endereco_funcao;
             
        (yyval).c = vector<string>{"{}"} + "'&funcao'" + lbl_endereco_funcao + "[<=]";
        funcoes = funcoes + definicao_lbl_endereco_funcao + (yyvsp[(2) - (7)]).c + (yyvsp[(6) - (7)]).c + "undefined" + "@" + "'&retorno'" + "@"+ "~";
        ts.pop_back();
      ;}
    break;

  case 104:

/* Line 1455 of yacc.c  */
#line 586 "java.y"
    { (yyval).clear(); ;}
    break;

  case 105:

/* Line 1455 of yacc.c  */
#line 590 "java.y"
    { (yyval).c = (yyvsp[(1) - (3)]).c + (yyvsp[(3) - (3)]).c;
         (yyval).contador = (yyvsp[(1) - (3)]).contador + 1; ;}
    break;

  case 106:

/* Line 1455 of yacc.c  */
#line 593 "java.y"
    { (yyval).c = (yyvsp[(1) - (1)]).c;
         (yyval).contador = 1; ;}
    break;

  case 109:

/* Line 1455 of yacc.c  */
#line 602 "java.y"
    {
        (yyval).c = "{}" + (yyvsp[(2) - (3)]).c;
      ;}
    break;

  case 110:

/* Line 1455 of yacc.c  */
#line 608 "java.y"
    {
                (yyval).c = (yyvsp[(1) - (3)]).c + (yyvsp[(3) - (3)]).c;
              ;}
    break;

  case 112:

/* Line 1455 of yacc.c  */
#line 615 "java.y"
    { 
        (yyval).c = (yyvsp[(1) - (3)]).c + (yyvsp[(3) - (3)]).c + "[<=]";
      ;}
    break;

  case 113:

/* Line 1455 of yacc.c  */
#line 619 "java.y"
    { 
        (yyval).c = (yyvsp[(1) - (3)]).c + (yyvsp[(3) - (3)]).c + "[<=]";
      ;}
    break;

  case 114:

/* Line 1455 of yacc.c  */
#line 625 "java.y"
    {
                (yyval).c = (yyvsp[(1) - (3)]).c + to_string((yyvsp[(1) - (3)]).contador) + (yyvsp[(3) - (3)]).c + "[<=]";
                (yyval).contador = (yyvsp[(1) - (3)]).contador + 1;
              ;}
    break;

  case 115:

/* Line 1455 of yacc.c  */
#line 630 "java.y"
    {
                (yyval).c = "0" + (yyvsp[(1) - (1)]).c + "[<=]";
                (yyval).contador = 1;
              ;}
    break;



/* Line 1455 of yacc.c  */
#line 2687 "y.tab.c"
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
#line 640 "java.y"


#include "lex.yy.c"

void yyerror( const char* st ) {
   cerr << st << endl; 
   cerr << "Proximo a: " << yytext << endl 
        << "Linha: " << yylval.linha << endl
        << "Coluna: " <<yylval.coluna << endl;
   exit( 1 );
}

int main( int argc, char* argv[] ) {
  yyparse();
  
  return 0;
}
