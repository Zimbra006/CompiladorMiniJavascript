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
         << "' na linha: " << topo[nome].linha 
         << ", coluna: " << topo[nome].coluna << endl;
    exit( 1 );     
  }
}
// Substitui ambas as funções checarVariavel
void checa_simbolo( string nome, bool modificavel ) {

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
%}

%token ID PRINT FOR WHILE ASM ASM_WORD
%token LET VAR CONST FUNCTION RETURN TRUE_TK FALSE_TK
%token CDOUBLE CSTRING CINT

%right '=' MAIS_IGUAL
%nonassoc '<' '>' MAIS_MAIS
%nonassoc  AND OR ME_IG MA_IG DIF IGUAL
  /* Esses três são adicionados para resolver o conflito */
  /* shift-reduce de if no yacc */
%nonassoc IF
%nonassoc REDUCE
%nonassoc ELSE
%left '+' '-'
%left '*' '/' '%'

%right '[' '('
%left '.'


%%

S : CMDs { print( resolve_enderecos( $1.c + "." + funcoes ) ); }
  ;

CMDs : CMDs CMD  { $$.c =  $1.c + $2.c ; };
     | CMD
     ;
     
CMD : CMD_LET OPT_SC
    | CMD_VAR OPT_SC
    | CMD_CONST OPT_SC
    | CMD_IF OPT_SC
    | CMD_FUNC OPT_SC
    | CMD_RET ';' OPT_SC
    { $$.c = "undefined" + ("@" + $1.c); }
    | CMD_RET E ';' OPT_SC
    { $$.c = $2.c + $1.c; }
    | E CMD_ASM ';' OPT_SC
      { $$.c = $1.c + $2.c; }
    | PRINT E ';' OPT_SC
      { $$.c = $2.c + "println" + "#"; }
    | CMD_WHILE OPT_SC
    | CMD_FOR OPT_SC
    | E OPT_SC
      { $$.c = $1.c + "^"; }
    | '{' EMPILHA_TS CMDs '}' OPT_SC
      { ts.pop_back(); $$.c = "<{" + $3.c + "}>"; }
    ;

OPT_SC : ';'
       | { $$.clear();}

// Símbolo apenas para empilhar tabelas de símbolos
EMPILHA_TS : { ts.push_back( map< string, Simbolo >{} ); } 
           ;

CMD_ASM : ASM ASM_STRING '}' { $$.c = $2.c + "^"; }
        ;

ASM_STRING : ASM_WORD ASM_STRING { $$.c = $1.c + $2.c;}
           | { $$.clear(); }
           ;

CMD_FUNC : FUNCTION ID { declara_var( DeclVar, $2.c[0], $2.linha, $2.coluna ); } 
             '(' EMPILHA_TS LISTA_PARAMs ')' '{' CMDs '}'
           { 
             string lbl_endereco_funcao = gera_label( "func_" + $2.c[0] );
             string definicao_lbl_endereco_funcao = ":" + lbl_endereco_funcao;
             
             $$.c = $2.c + "&" + $2.c + "{}"  + "=" + "'&funcao'" +
                    lbl_endereco_funcao + "[=]" + "^";
             funcoes = funcoes + definicao_lbl_endereco_funcao + $6.c + $9.c +
                       "undefined" + "@" + "'&retorno'" + "@"+ "~";
             ts.pop_back(); 
           }
         ;

CMD_RET : RETURN { $$.c = vector<string>{"'&retorno'", "@", "~"}; }
        ;

LISTA_PARAMs : PARAMs
           | { $$.clear(); }
           ;
           
PARAMs : PARAMs ',' PARAM  
       {  
         $$.c = $1.c + $3.c + "&" + $3.c + "arguments" + "@" + to_string( $1.contador )
                + "[@]" + "=" + "^"; 
                
         if( $3.valor_default.size() > 0 ) {
            // Teste para usar valor default
            string lbl_true = gera_label( "lbl_true" );
            string lbl_fim_if = gera_label( "lbl_fim_if" );
            string definicao_lbl_true = ":" + lbl_true;
            string definicao_lbl_fim_if = ":" + lbl_fim_if;
                    
            $$.c = $$.c + $3.c + "@" + "undefined" + "@" + "==" +
                   lbl_true + "?" +             
                   lbl_fim_if + "#" +           
                   definicao_lbl_true +
                   $3.c + $3.valor_default + "=" + "^" +  
                   definicao_lbl_fim_if         
                   ;
         }
         $$.contador = $1.contador + $3.contador; 
       }
     | PARAM 
       {  
         $$.c = $1.c + "&" + $1.c + "arguments" + "@" + "0" + "[@]" + "=" + "^"; 
                
         if( $1.valor_default.size() > 0 ) {
            // Teste para usar valor default
            string lbl_true = gera_label( "lbl_true" );
            string lbl_fim_if = gera_label( "lbl_fim_if" );
            string definicao_lbl_true = ":" + lbl_true;
            string definicao_lbl_fim_if = ":" + lbl_fim_if;
                    
            $$.c = $$.c + $1.c + "@" + "undefined" + "@" + "==" +
                   lbl_true + "?" +             
                   lbl_fim_if + "#" +           
                   definicao_lbl_true +
                   $1.c + $1.valor_default + "=" + "^" +  
                   definicao_lbl_fim_if        
                   ;
         }
         $$.contador = $1.contador; 
       }
     ;
     
PARAM : ID 
      { $$.c = $1.c;      
        $$.contador = 1;
        $$.valor_default.clear();
        declara_var( DeclLet, $1.c[0], $1.linha, $1.coluna ); 
      }
    | ID '=' E
      { // Código do IF
        $$.c = $1.c;
        $$.contador = 1;
        $$.valor_default = $3.c;         
        declara_var( DeclLet, $1.c[0], $1.linha, $1.coluna ); 
      }
    ;

CMD_WHILE : WHILE '(' E ')' CMD
          { string lbl_fim_while = gera_label( "fim_while" );
            string lbl_condicao_while = gera_label( "condicao_while" );
            string lbl_comando_while = gera_label( "comando_while" );
            string definicao_lbl_fim_while = ":" + lbl_fim_while;
            string definicao_lbl_condicao_while = ":" + lbl_condicao_while;
            string definicao_lbl_comando_while = ":" +lbl_comando_while;

            $$.c = definicao_lbl_condicao_while + $3.c +
                   lbl_comando_while + "?" + lbl_fim_while + "#" +
                   definicao_lbl_comando_while + $5.c +
                   lbl_condicao_while + "#" +
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

CMD_IF : IF '(' E ')' CMD %prec REDUCE
         { string lbl_true = gera_label( "lbl_true" );
           string lbl_fim_if = gera_label( "lbl_fim_if" );
           string definicao_lbl_true = ":" + lbl_true;
           string definicao_lbl_fim_if = ":" + lbl_fim_if;
                    
            $$.c = $3.c +                       // Codigo da expressão
                   lbl_true + "?" +             // Código do IF
                   lbl_fim_if + "#" +           // Código do False
                   definicao_lbl_true + $5.c +  // Código do True
                   definicao_lbl_fim_if         // Fim do IF
                   ;
         }
       | IF '(' E ')' CMD ELSE CMD
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

CMD_LET : LET VARs_LET {  $$.c = $2.c; }
        ;

CMD_VAR : VAR VARs_VAR {  $$.c = $2.c;}
        ;

CMD_CONST : CONST VARs_CONST {  $$.c = $2.c;}
          ;

VARs_LET : VAR_LET ',' VARs_LET { $$.c = $1.c + $3.c; } 
     | VAR_LET
     ;

VARs_VAR : VAR_VAR ',' VARs_VAR { $$.c = $1.c + $3.c; } 
     | VAR_VAR
     ;

VARs_CONST : VAR_CONST ',' VARs_CONST { $$.c = $1.c + $3.c; } 
     | VAR_CONST
     ;

VAR_LET : ID  
      { $$.c = declara_var( DeclLet, $1.c[0], $1.linha, $1.coluna );}
    | ID '=' E
      { $$.c = declara_var( DeclLet, $1.c[0], $1.linha, $1.coluna ) + $1.c + $3.c + "=" + "^";}
    ;

VAR_VAR : ID  
      { $$.c = declara_var( DeclVar, $1.c[0], $1.linha, $1.coluna ); }
    | ID '=' E
      { $$.c = declara_var( DeclVar, $1.c[0], $1.linha, $1.coluna ) + $1.c + $3.c + "=" + "^";}
    ;

VAR_CONST : ID '=' E
      { $$.c = declara_var( DeclConst, $1.c[0], $1.linha, $1.coluna ) + $1.c + $3.c + "=" + "^"; }
      ;

LVALUE : ID
       ;
       
LVALUEPROP : E '[' E ']' { $$.c = $1.c + $3.c; }
           | E '.' ID { $$.c = $1.c + $3.c; }
           ;

E : LVALUE '=' E 
    { 
      checa_simbolo( $1.c[0], true );  
      $$.c = $1.c + $3.c + "="; 
    }
  | LVALUE MAIS_IGUAL E
    {
      checa_simbolo( $1.c[0], true );  
      $$.c = $1.c + $1.c + "@" + $3.c + "+" + "="; 
    }
  | LVALUE MAIS_MAIS
    {
      checa_simbolo( $1.c[0], true );  
      $$.c = $1.c + "@" + $1.c + $1.c + "@" + "1" + "+" + "=" + "^"; 
    }
  | LVALUEPROP '=' E 
    { $$.c = $1.c + $3.c + "[=]"; }
  | LVALUEPROP MAIS_IGUAL E 
    { $$.c = $1.c + $1.c + "[@]" + $3.c + "+" + "[=]"; }
  | LVALUEPROP MAIS_MAIS 
    { $$.c = $1.c + "[@]" + $1.c + $1.c + "[@]" + "1" + "+" + "=" + "^"; }
  | E '<' E
    { $$.c = $1.c + $3.c + $2.c; }
  | E '>' E
    { $$.c = $1.c + $3.c + $2.c; }
  | E AND E
    { $$.c = $1.c + $3.c + $2.c; }
  | E OR E
    { $$.c = $1.c + $3.c + $2.c; }
  | E ME_IG E
    { $$.c = $1.c + $3.c + $2.c; }
  | E MA_IG E
    { $$.c = $1.c + $3.c + $2.c; }
  | E DIF E
    { $$.c = $1.c + $3.c + $2.c; }
  | E IGUAL E
    { $$.c = $1.c + $3.c + $2.c; }
  | E '+' E
    { $$.c = $1.c + $3.c + $2.c; }
  | E '-' E
    { $$.c = $1.c + $3.c + $2.c; }
  | E '*' E
    { $$.c = $1.c + $3.c + $2.c; }
  | E '/' E
    { $$.c = $1.c + $3.c + $2.c; }
  | E '%' E
    { $$.c = $1.c + $3.c + $2.c; }
  | CDOUBLE
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
  | '{' '}'
    { $$.c = vector<string>{"{}"}; }
  | '[' ']'
    { $$.c = vector<string>{"[]"}; }
  | LVALUE 
    { $$.c = $1.c + "@"; } 
  | LVALUEPROP
    { $$.c = $1.c + "[@]"; }
  | TRUE_TK
  | FALSE_TK
  | '(' E ')' { $$.c = $2.c; }
  | E '(' LISTA_ARGs ')'
    {
      $$.c = $3.c + to_string( $3.contador ) + $1.c + "$";
    }
  | '{' LISTA_CAMPOS '}'
      {
        $$.c = "{}" + $2.c;
      }
  | '[' LISTA_ELEMS ']'
      {
        $$.c = "[]" + $2.c;
      }
  | FUNCTION '(' EMPILHA_TS LISTA_PARAMs ')' '{' CMDs '}'
           { 
             string lbl_endereco_funcao = gera_label( "func_" + $2.c[0] );
             string definicao_lbl_endereco_funcao = ":" + lbl_endereco_funcao;
             
             $$.c = vector<string>{"{}"} + "'&funcao'" + lbl_endereco_funcao + "[<=]";
             funcoes = funcoes + definicao_lbl_endereco_funcao + $4.c + $7.c +
                       "undefined" + "@" + "'&retorno'" + "@"+ "~";
             ts.pop_back(); 
           }
  ;

LISTA_ARGs : ARGs
           | { $$.clear(); }
           ;
             
ARGs : ARGs ',' E
       { $$.c = $1.c + $3.c;
         $$.contador = $1.contador + 1; }
     | E
       { $$.c = $1.c;
         $$.contador = 1; }
     ;

LISTA_CAMPOS : LISTA_CAMPOS ',' CAMPO
              {
                $$.c = $1.c + $3.c;
              }
             | CAMPO
             ;

CAMPO : ID ':' E 
      { 
        $$.c = $1.c + $3.c + "[<=]";
      }

LISTA_ELEMS : LISTA_ELEMS ',' E
              {
                $$.c = $1.c + to_string($1.contador) + $3.c + "[<=]";
                $$.contador = $1.contador + 1;
              }
             | E
              {
                $$.c = "0" + $1.c + "[<=]";
                $$.contador = 1;
              }
             ;

%%

#include "lex.yy.c"

void yyerror( const char* st ) {
   cerr << st << endl; 
   cerr << "Proximo a: " << yytext << endl <<"Linha: " << yylval.linha << endl;
   exit( 1 );
}

int main( int argc, char* argv[] ) {
  yyparse();
  
  return 0;
}