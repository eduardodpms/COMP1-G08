%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "parser.tab.h"   

extern int yylineno;   /* mantido pelo flex quando usamos %option yylineno */
extern int compilation_error_count;  /* contador global definido no parser.y */
%}

%option yylineno
%option noyywrap

%%

"if"          { return IF; }
"else"        { return ELSE; }

"do"          { return DO; }
"while"       { return WHILE; }
"for"         { return FOR; }
"continue"    { return CONTINUE; }

"switch"      { return SWITCH; }
"case"        { return CASE; }
"default"     { return DEFAULT; }
"break"       { return BREAK; }

"function"    { return FUNCTION; }
"return"      { return RETURN; }
"void"        { return VOID; }

"try"         { return TRY; }
"catch"       { return CATCH; }
"throw"       { return THROW; }

"with"        { return WITH; }
"typeof"      { return TYPEOF; }
"enum"        { return ENUM; }

"let"         { return LET; }
"const"       { return CONST; }
"var"         { return VAR; }

"number"      { return TYPE_NUMBER; }
"string"      { return TYPE_STRING; }
"boolean"     { return TYPE_BOOLEAN; }

"true"        { yylval.ival = 1; return BOOLEAN_LITERAL; }
"false"       { yylval.ival = 0; return BOOLEAN_LITERAL; }
"null"        { yylval.ival = 0; return NULL_LITERAL; }

"+"           { return PLUS; }
"-"           { return MINUS; }
"*"           { return MULT; }
"/"           { return DIV; }
"="           { return ASSIGN; }

";"           { return SEMICOLON; }
":"           { return COLON; }
","           { return COMMA; }
"("           { return LPAREN; }
")"           { return RPAREN; }
"{"           { return LBRACE; }
"}"           { return RBRACE; }

"console.read"    { return CONSOLE_READ; } /* token para input */
"console.log" { return CONSOLE_LOG; }  /* token para output */

[0-9]+                 { yylval.ival = atoi(yytext); return NUMBER_LITERAL; }
\"[^\"]*\"             { yylval.sval = strdup(yytext); return STRING_LITERAL; }
[a-zA-Z_][a-zA-Z0-9_]* { yylval.sval = strdup(yytext); return IDENT; }

\n          {  } /* separar \n para atualizar yylineno corretamente e para mensagens de erro precisas */
[ \t]+      {  }  /* ignorar \t */

.   {
        int lineno = (yylineno > 0) ? yylineno : 1;
        fprintf(stderr, "Erro léxico: caractere '%s' não reconhecido na linha %d\n", yytext, lineno);
        compilation_error_count++;
    } /* incrementa contador global, consome o caractere inválido e continuaa o scanning (não retorna token) */
%%
