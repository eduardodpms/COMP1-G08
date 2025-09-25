%{
#include <stdio.h>
#include <stdlib.h>
#include "parser.tab.h"  /* contém definições geradas pelo Bison (NUM, PLUS, etc.) */
%}

%%

"if"        { return IF; }
"else"      { return ELSE; }


"do"        { return DO; }
"while"     { return WHILE; }
"for"       { return FOR; }
"continue"  { return CONTINUE; }


"switch"    { return SWITCH; }
"case"      { return CASE; }
"default"   { return DEFAULT; }
"break"     { return BREAK; }


"try"       { return TRY; }
"catch"     { return CATCH; }
"throw"     { return THROW; }


"function"  { return FUNCTION; }
"return"    { return RETURN; }
"void"      { return VOID; }


"with"      { return WITH; }
"typeof"    { return TYPEOF; }
"enum"      { return ENUM; }


"let"       { return LET; }
"const"     { return CONST; }
"var"       { return VAR; }


"number"    { return TYPE_NUMBER; }
"string"    { return TYPE_STRING; }
"boolean"   { return TYPE_BOOLEAN; }

"true"      { yylval.ival = 1; return BOOLEAN_LITERAL; }
"false"     { yylval.ival = 0; return BOOLEAN_LITERAL; }
"null"      { yylval.ival = 0; return NULL_LITERAL; }

"+"         { return PLUS; }
"-"         { return MINUS; }
"*"         { return MULT; }
"/"         { return DIV; }
"="         { return ASSIGN; }


";"         { return SEMICOLON; }
":"         { return COLON; }
","         { return COMMA; }
"("         { return LPAREN; }
")"         { return RPAREN; }
"{"         { return LBRACE; }
"}"         { return RBRACE; }


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
