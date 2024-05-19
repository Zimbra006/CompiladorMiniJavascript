%{
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
%}

%token ID IF ELSE PRINT FOR WHILE
%token OBJ ARRAY
%token LET VAR CONST
%token CDOUBLE CSTRING CINT
%token AND OR ME_IG MA_IG DIF IGUAL
%token MAIS_IGUAL MAIS_MAIS

%right '=' MAIS_IGUAL
%nonassoc '<' '>' MAIS_MAIS
%left '+' '-'
%left '*' '/' '%'

%left '['
%left '.'


%%

S : CMDs { print( resolve_enderecos( $1.c + "." ) ); }
  ;

CMDs : CMDs CMD  { $$.c =  $1.c + $2.c ; };
     | CMD
     ;
     
CMD : CMD_LET ';'
    | CMD_VAR ';'
    | CMD_CONST ';'
    | CMD_IF 
    | PRINT E ';' 
      { $$.c = $2.c + "println" + "#"; }
    | CMD_WHILE
    | CMD_FOR
    | E ';'
      { $$.c = $1.c + "^"; }
    | '{' CMDs '}' ';'
      { $$.c = $2.c; }
    ;

CMD_WHILE : WHILE '(' E ')' CMD
          { string lbl_fim_while = gera_label( "fim_while" );
            string lbl_condicao_while = gera_label( "condicao_while" );
            string lbl_comando_while = gera_label( "comando_while" );
            string definicao_lbl_fim_while = ":" + lbl_fim_while;
            string definicao_lbl_condicao_while = ":" + lbl_condicao_while;
            string definicao_lbl_comando_while = ":" +lbl_comando_while;

            $$.c = definicao_lbl_condicao_while + $3.c + // Código para a condição
                   lbl_comando_while + "?" + lbl_fim_while + "#" + // Se a condição for true, vai pro comando
                   definicao_lbl_comando_while + $5.c + // Código do comando
                   lbl_condicao_while + "#" + // Volta pra condição
                   definicao_lbl_fim_while;
          }
          ;
 
CMD_FOR : FOR '(' PRIM_E ';' E ';' E ')' CMD 
        { string lbl_fim_for = gera_label( "fim_for" );
          string lbl_condicao_for = gera_label( "condicao_for" );
          string lbl_comando_for = gera_label( "comando_for" );
          string definicao_lbl_fim_for = ":" + lbl_fim_for;
          string definicao_lbl_condicao_for = ":" + lbl_condicao_for;
          string definicao_lbl_comando_for = ":" + lbl_comando_for;
          
          $$.c = $3.c + definicao_lbl_condicao_for +
                 $5.c + lbl_comando_for + "?" + lbl_fim_for + "#" +
                 definicao_lbl_comando_for + $9.c + 
                 $7.c + "^" + lbl_condicao_for + "#" +
                 definicao_lbl_fim_for;
        }
        ;

PRIM_E : CMD_LET 
       | CMD_VAR
       | CMD_CONST
       | E  
         { $$.c = $1.c + "^"; }
       ;

CMD_LET : LET VARs_LET {  $$.c = $2.c; }
        ;

CMD_VAR : VAR VARs_VAR {  $$.c = $2.c;}
        ;

CMD_CONST : CONST VARs_CONST {  $$.c = $2.c;}
          ;

VARs_LET : VAR_R_LET ',' VARs_LET { $$.c = $1.c + $3.c; } 
     | VAR_R_LET
     ;

VARs_VAR : VAR_R_VAR ',' VARs_VAR { $$.c = $1.c + $3.c; } 
     | VAR_R_VAR
     ;

VARs_CONST : VAR_R_CONST ',' VARs_CONST { $$.c = $1.c + $3.c; } 
     | VAR_R_CONST
     ;

VAR_R_LET : ID  
      { $$.c = $1.c + "&"; insere_tabela_de_simbolos(DeclLet, $1);}
    | ID '=' E
      { $$.c = $1.c + "&" + $1.c + $3.c + "=" + "^"; insere_tabela_de_simbolos(DeclLet, $1);}
    ;

VAR_R_VAR : ID  
      { $$.c = $1.c + "&"; insere_tabela_de_simbolos(DeclVar, $1); }
    | ID '=' E
      { $$.c = $1.c + "&" + $1.c + $3.c + "=" + "^"; insere_tabela_de_simbolos(DeclVar, $1); }
    ;

VAR_R_CONST : ID  
      { $$.c = $1.c + "&"; insere_tabela_de_simbolos(DeclConst, $1);}
    | ID '=' E
      { $$.c = $1.c + "&" + $1.c + $3.c + "=" + "^"; insere_tabela_de_simbolos(DeclConst, $1);}
    ;
     
CMD_IF : IF '(' E ')' CMD ELSE CMD
         { string lbl_true = gera_label( "lbl_true" );
           string lbl_fim_if = gera_label( "lbl_fim_if" );
           string definicao_lbl_true = ":" + lbl_true;
           string definicao_lbl_fim_if = ":" + lbl_fim_if;
                    
            $$.c = $3.c +                       // Codigo da expressão
                   lbl_true + "?" +             // Código do IF
                   $7.c + lbl_fim_if + "#" +    // Código do False
                   definicao_lbl_true + $5.c +  // Código do True
                   definicao_lbl_fim_if         // Fim do IF
                   ;
         }
       ;
        
LVALUE : ID 
       ;
       
LVALUEPROP : E '[' E ']' { $$.c = $1.c + $3.c; }
           | E '.' ID { $$.c = $1.c + $3.c; }
           ;

E : ATRIB
  | BOOL
  | MATH
  | VALUES
  | '(' E ')'
    { $$.c = $2.c; }
  ;
  
// Lidam apenas com atribuições
ATRIB: LVALUE '=' E 
      { 
        checarVariavelExiste($1);  
        checarVariavelConst($1);  
        $$.c = $1.c + $3.c + "="; 
      }
     | LVALUE MAIS_IGUAL E
      {
        checarVariavelExiste($1);  
        checarVariavelConst($1);  
        $$.c = $1.c + $1.c + "@" + $3.c + "+ ="; 
      }
     | LVALUE MAIS_MAIS
      {
        checarVariavelExiste($1);  
        checarVariavelConst($1);  
        $$.c = $1.c + "@" + $1.c + $1.c + "@ 1 + = ^"; 
      }
     | LVALUEPROP '=' E 
      { $$.c = $1.c + $3.c + "[=]"; }
     | LVALUEPROP MAIS_IGUAL E 
      { $$.c = $1.c + $1.c + "[@]" + $3.c + "+ [=]"; }
     | LVALUEPROP MAIS_MAIS
      { $$.c = $1.c + "@" + $1.c + $1.c + "@ 1 + [=] ^"; }
     ;

// Lidam apenas com comparações booleanas
BOOL: E '<' E
     { $$.c = $1.c + $3.c + $2.c; }
    | E '>' E
     { $$.c = $1.c + $3.c + $2.c; }
    ;

// Lidam apenas com expressões matemáticas
MATH: E '+' E
      { $$.c = $1.c + $3.c + $2.c; }
    | E '-' E
      { $$.c = $1.c + $3.c + $2.c; }
    | E '*' E
      { $$.c = $1.c + $3.c + $2.c; }
    | E '/' E
      { $$.c = $1.c + $3.c + $2.c; }
    | E '%' E
      { $$.c = $1.c + $3.c + $2.c; }
    ;

// Lidam apenas com valores constantes ou variáveis
VALUES: CDOUBLE
      | '+' CDOUBLE
        { $$.c = $2.c; }
      | '-' CDOUBLE
        { $$.c = "0" + $2.c + "-"; }
      | CINT
      | '+' CINT
        { $$.c = $2.c; }
      | '-' CINT
        { $$.c = "0" + $2.c + "-"; }
      | CSTRING
      | OBJ 
        { $$.c = $1.c; }
      | ARRAY 
        { $$.c = $1.c; }
      | LVALUE 
        { $$.c = $1.c + "@"; } 
      | LVALUEPROP
        { $$.c = $1.c + "[@]"; }
      ;
%%

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