%{
#include "ast.h"
#include "tabela.h"
#include "codegen.h"
#include "otimizador.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>

void ast_free(NoAST *root);

NoAST *ast_root = NULL;
extern FILE *yyin;
FILE *out;

int yylex(void);
void yyerror(const char *s);

int compilation_error_count = 0;

void report_error(int line, const char *fmt, ...) {
    va_list ap;
    va_start(ap, fmt);
    fprintf(stderr, "Erro (linha %d): ", line);
    vfprintf(stderr, fmt, ap);
    fprintf(stderr, "\n");
    va_end(ap);
    compilation_error_count++;
}

extern int yylineno;
%}

%code requires {
    #include "ast.h"
    #include "tabela.h"
    #include "codegen.h"
}

%union {
    int ival;
    char *sval;
    NoAST *ast_node;
}

%token IF ELSE DO WHILE FOR CONTINUE SWITCH CASE DEFAULT BREAK FUNCTION RETURN VOID TRY CATCH THROW WITH TYPEOF ENUM
%token LET CONST VAR
%token TYPE_NUMBER TYPE_STRING TYPE_BOOLEAN
%token <ival> BOOLEAN_LITERAL NULL_LITERAL NUMBER_LITERAL
%token <sval> STRING_LITERAL IDENT
%token PLUS MINUS MULT DIV ASSIGN EQ NEQ LT GT LE GE MOD INCREMENT DECREMENT
%token SEMICOLON COMMA LPAREN RPAREN LBRACE RBRACE COLON
%token CONSOLE_READ CONSOLE_LOG

%right ASSIGN
%nonassoc LOWER_THAN_ELSE
%nonassoc ELSE
%nonassoc EQ NEQ LT GT LE GE
%left PLUS MINUS
%left MULT DIV MOD
%right INCREMENT DECREMENT

%type <ival> var_kind
%type <ast_node> declaration variable_definition statement expr stmt_list block if_stmt while_stmt for_stmt for_init switch_stmt case_list case_item console_stmt

%%

program:
    /* vazio */
    | program statement {
        if ($2) ast_root = adicionarDeclaracao(ast_root, $2);
    }
;

variable_definition:
    var_kind IDENT COLON TYPE_NUMBER ASSIGN expr {
        char *ident_name = $2;
        /* Se $6 for um literal string, reporta erro. 
           Adapte astIsStringLiteral() conforme sua implementação de AST. */
        if (astIsStringLiteral($6)) {
            report_error(yylineno, "Tentativa de atribuir string a variavel numerica '%s'.", ident_name);
            liberarNoAST($6); /* ou free do nó, conforme seu código */
            free(ident_name);
            $$ = NULL;
        } else {
            $$ = criarNoDecl($1, TIPO_NUMBER, ident_name, $6);
            inserirSimbolo(ident_name, TIPO_NUMBER);
            int ok; int valor = avaliarExpr($6, &ok);
            if (ok) atualizarValorConstante(ident_name, valor);
            free(ident_name);
        }
    }
  | var_kind IDENT COLON TYPE_STRING ASSIGN expr {
        char *ident_name = $2;
        if (astIsNumberLiteral($6)) {
            report_error(yylineno, "Tentativa de atribuir numero a variavel string '%s'.", ident_name);
            liberarNoAST($6);
            free(ident_name);
            $$ = NULL;
        } else {
            $$ = criarNoDecl($1, TIPO_STRING, ident_name, $6);
            inserirSimbolo(ident_name, TIPO_STRING);
            free(ident_name);
        }
    }
  | var_kind IDENT COLON TYPE_BOOLEAN ASSIGN expr {
        char *ident_name = $2;
        if (astIsStringLiteral($6) || astIsNumberLiteral($6)) {
            report_error(yylineno, "Tentativa de atribuir valor invalido a variavel boolean '%s'.", ident_name);
            liberarNoAST($6);
            free(ident_name);
            $$ = NULL;
        } else {
            $$ = criarNoDecl($1, TIPO_BOOLEAN, ident_name, $6);
            inserirSimbolo(ident_name, TIPO_BOOLEAN);
            free(ident_name);
        }
    }
;


declaration:
    variable_definition SEMICOLON { $$ = $1; }
;

expr:
    NUMBER_LITERAL { $$ = criarNoNum($1); } 
    | STRING_LITERAL { $$ = criarNoStr($1); free($1); }
    | BOOLEAN_LITERAL { $$ = criarNoBool($1); }
    | IDENT {
        $$ = criarNoId($1);
        free($1);
    }
    | LPAREN expr RPAREN { $$ = $2; }
    | CONSOLE_READ LPAREN RPAREN {
        $$ = criarNoOp(AST_OP_READ, NULL, NULL); 
    }
    | expr INCREMENT { $$ = criarNoOp(AST_OP_INC, $1, NULL); }
    | expr DECREMENT { $$ = criarNoOp(AST_OP_DEC, $1, NULL); }
    | expr MULT expr { $$ = criarNoOp(AST_OP_MUL, $1, $3); }
    | expr DIV expr { $$ = criarNoOp(AST_OP_DIV, $1, $3); }
    | expr MOD expr { $$ = criarNoOp(AST_OP_MOD, $1, $3); }
    | expr PLUS expr { $$ = criarNoOp(AST_OP_ADD, $1, $3); }
    | expr MINUS expr { $$ = criarNoOp(AST_OP_SUB, $1, $3); }
    | expr LT expr { $$ = criarNoOp(AST_OP_LT, $1, $3); }
    | expr GT expr { $$ = criarNoOp(AST_OP_GT, $1, $3); }
    | expr LE expr { $$ = criarNoOp(AST_OP_LE, $1, $3); }
    | expr GE expr { $$ = criarNoOp(AST_OP_GE, $1, $3); }
    | expr EQ expr { $$ = criarNoOp(AST_OP_EQ, $1, $3); }
    | expr NEQ expr { $$ = criarNoOp(AST_OP_NEQ, $1, $3); }
    | IDENT ASSIGN expr %prec ASSIGN {
        $$ = criarNoOp(AST_OP_ASSIGN, criarNoId($1), $3);
        free($1);
    }
;

var_kind:
    LET { $$ = VK_LET; } | CONST { $$ = VK_CONST; } | VAR { $$ = VK_VAR; }
;

stmt_list:
    /* empty */ { $$ = NULL; }
  | stmt_list statement {
        if ($2) {
            if (!$1) $$ = $2;
            else $$ = adicionarDeclaracao($1, $2);
        } else $$ = $1;
    }
;

block:
    LBRACE { pushScope(); } stmt_list RBRACE { popScope(); $$ = criarNoBlock($3); }
;

console_stmt:
      CONSOLE_LOG LPAREN expr RPAREN SEMICOLON { $$ = criarNoConsoleLog($3); }
;

statement:
      declaration { $$ = $1; }
    | block       { $$ = $1; }
    | if_stmt     { $$ = $1; }
    | while_stmt  { $$ = $1; }
    | for_stmt    { $$ = $1; }
    | switch_stmt { $$ = $1; }
    | console_stmt { $$ = $1; } 
    | BREAK SEMICOLON { $$ = criarNoBreak(); }
    | CONTINUE SEMICOLON { $$ = criarNoContinue(); }
    | expr SEMICOLON { $$ = $1; }
    | error SEMICOLON { yyerrok; yyclearin; $$ = NULL; }
;

if_stmt:
    IF LPAREN expr RPAREN statement %prec LOWER_THAN_ELSE { $$ = criarNoIf($3, $5, NULL); }
    | IF LPAREN expr RPAREN statement ELSE statement { $$ = criarNoIf($3, $5, $7); }
;

while_stmt:
    WHILE LPAREN expr RPAREN statement { $$ = criarNoWhile($3, $5); }
;

for_init:
      variable_definition { $$ = $1; }
    | expr { $$ = $1; }
    | /* empty */ { $$ = NULL; }
;

for_stmt:
    FOR LPAREN for_init SEMICOLON expr SEMICOLON for_init RPAREN statement {
        $$ = criarNoFor($3, $5, $7, $9);
    }
;

switch_stmt:
    SWITCH LPAREN expr RPAREN LBRACE case_list RBRACE { $$ = criarNoSwitch($3, $6); }
;

case_list:
    /* empty */ { $$ = NULL; }
  | case_list case_item {
        if (!$1) $$ = $2;
        else $$ = adicionarDeclaracao($1, $2);
    }
;

case_item:
    CASE NUMBER_LITERAL COLON stmt_list {
        NoAST *case_expr = criarNoNum($2);
        NoAST *case_body = $4;
        $$ = criarNoCase(case_expr, case_body);
    }
  | DEFAULT COLON stmt_list { $$ = criarNoCase(NULL, $3); }
;

%%

int main(int argc, char **argv) {
    if (argc < 2) {
        fprintf(stderr, "Uso: %s arquivo.ts\n", argv[0]);
        return EXIT_FAILURE;
    }
    yyin = fopen(argv[1], "r");
    if (!yyin) { perror("Erro ao abrir arquivo"); return EXIT_FAILURE; }

    pushScope(); 
    yyparse();

    if (compilation_error_count > 0) {
        fprintf(stderr, "Encontrados %d erro(s). Abortando.\n", compilation_error_count);
        fclose(yyin); return EXIT_FAILURE;
    }
    
    liberarTabelaSimbolos(); 
    pushScope();

    ast_root = otimizarStrengthReduction(ast_root);
    verificarTiposAST(ast_root);
    
    if (compilation_error_count > 0) {
        fprintf(stderr, "Erros semanticos encontrados. Abortando.\n");
        fclose(yyin); return EXIT_FAILURE;
    }

    printf("AST:\n");
    imprimirAST(ast_root);
    imprimirTabela();

    gerarCodigoC(ast_root, "saida.c");
    printf("\nCodigo C gerado em 'saida.c'\n");

    ast_free(ast_root);
    ast_root = NULL;
    liberarTabelaSimbolos();
    fclose(yyin);
    return EXIT_SUCCESS;
}

void yyerror(const char *s) {
    if (s == NULL || strcmp(s, "syntax error") == 0) report_error(yylineno, "syntax error");
    else report_error(yylineno, "%s", s);
}