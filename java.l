%{
int token( int tk );
%}

D  	    [0-9]
L   	  [A-Za-z_]

D_ASPAS_S   "''"
B_ASPAS_S  "\\'"
D_ASPAS_D   "\"\""
B_ASPAS_D  "\\\""


INT	      {D}+
DOUBLE    {D}+"."{D}+([Ee][+\-]?{D}+)?
STRING_S  '([^'\n]|{D_ASPAS_S}|{B_ASPAS_S})*'
STRING_D  \"([^"\n]|{D_ASPAS_D}|{B_ASPAS_D})*\"
ID        {L}({L}|{D})*
ASM       "asm{"
SETA      "=>"

%x asm

%%

"\t"       { coluna += 4; }
" "        { coluna++; }
"\n"	     { linha++; coluna = 1; }

{DOUBLE}   { return token( CDOUBLE ); }
{INT}	     { return token( CINT ); } 
{STRING_S} { return token( CSTRING ); }
{STRING_D} { return token( CSTRING ); }

"&&"	   { return token( AND ); }
"||"	   { return token( OR ); }
"<="	   { return token( ME_IG ); }
">="	   { return token( MA_IG ); }
"!="	   { return token( DIF ); }
"=="	   { return token( IGUAL ); }

"+="       { return token( MAIS_IGUAL ); }
"++"       { return token( MAIS_MAIS ); }

{SETA}             { dentroFuncao = true; return token( SETA ); }
")"/[ \t\n]*{SETA} { return token( PARENTESES_FUNCAO ); }

"let"      { return token( LET ); }
"var"      { return token( VAR ); }
"const"    { return token( CONST ); }
"if"       { return token( IF ); }
"else"     { return token( ELSE ); }
"for"      { return token( FOR ); }
"while"    { return token( WHILE ); }
"function" { dentroFuncao = true; return token( FUNCTION ); }
"return"   { return token( RETURN ); }
"true"     { return token( TRUE_TK );}
"false"     { return token( FALSE_TK );}

{ASM}      { BEGIN asm; return token( ASM ); }

{ID}       { return token( ID ); }

.          { return token( *yytext ); }

  /* Regras exclusivas para asm */

<asm>[ \t\n] ;

<asm>"{}" |
<asm>"}>" |
<asm>[^} \t\n]*   { return token( ASM_WORD ); }

<asm>"}"  { BEGIN INITIAL; return token( *yytext ); }


%%

int token( int tk ) {  
  yylval.c = vector<string>{ yytext };
  
  yylval.linha = linha;
  yylval.coluna = coluna;

  coluna += strlen( yytext ); 

  return tk;
}

int yywrap() {
    return 1;
}