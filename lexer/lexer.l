%{
#include <stdio.h>
#include <stdlib.h>
#include "parser.tab.h"   
%}

%option yylineno

%%

"let"       { return LET; }
"number"    { return TYPE_NUMBER; }
"string"    { return TYPE_STRING; }
";"         { return SEMI; }
"="         { return ASSIGN; }
":"         { return COLON; }
"console.log" { return CONSOLE_LOG; }
"("         { return LPAREN; }
")"         { return RPAREN; }

[0-9]+      { yylval.ival = atoi(yytext); return NUMBER_LITERAL; }
\"[^\"]*\"  { yylval.sval = strdup(yytext); return STRING_LITERAL; }
[a-zA-Z_][a-zA-Z0-9_]* { yylval.sval = strdup(yytext); return IDENT; }

[ \t\n]     { /* ignora espaços em branco */ }
.           { printf("Caractere não reconhecido: %s\n", yytext); }

%%

int yywrap(void) {
    return 1;
}
