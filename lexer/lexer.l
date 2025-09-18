/******************************************************
FGA0003 - Compiladores 1
Curso de Engenharia de Software
Universidade de Brasília (UnB)

Arquivo: lexer.l
Descrição: Exemplo de analisador léxico (tokens aritméticos)
******************************************************/

%{
#include <stdio.h>
#include <stdlib.h>
#include "parser.tab.h"  /* contém definições geradas pelo Bison (NUM, PLUS, etc.) */
%}

%%


"let" { return LET; }
"const" { return CONST; }
"var" { return VAR; }


"number" { return TYPE_NUMBER; }
"string" { return TYPE_STRING; }
"boolean" { return TYPE_BOOLEAN; }


"+"      { return PLUS; }
"-"      { return MINUS; }
"*"      { return MULT; }
"/"      { return DIV; }
"="      { return ASSIGN; }


";"      { return SEMICOLON; }
":"      { return COLON; }
","      { return COMMA; }
"("      { return LPAREN; }
")"      { return RPAREN; }
"{"      { return LBRACE; }
"}"      { return RBRACE; }

"console.log" { return CONSOLE_LOG; }

[0-9]+   { yylval.ival = atoi(yytext); return NUMBER_LITERAL; }
\"[^\"]*\"  { yylval.sval = strdup(yytext); return STRING_LITERAL; }
[a-zA-Z_][a-zA-Z0-9_]* { yylval.sval = strdup(yytext); return IDENT; }


[ \t\n]+  { /* não faz nada */ }

.        { printf("Caractere não reconhecido: %s\n", yytext); }

%%

int yywrap(void) {
    return 1;
}
