%{
#include "ast.h"
#include "tabela.h"
#include "codegen.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>

void ast_free(NoAST *root);

// ast
NoAST *ast_root = NULL;

//* ponteiros para gerar arquivos *//
extern FILE *yyin;
FILE *out;

/* Declarações para evitar avisos de função implícita */
int yylex(void);
void yyerror(const char *s);

/* contador global de erros (léxicos, sintáticos, semânticos) */
int compilation_error_count = 0;

/* helper: reporta erro formatado e incrementa contador.
   Não encerra o processo — permite coleta de múltiplos erros.
*/
void report_error(int line, const char *fmt, ...) {
    va_list ap;
    va_start(ap, fmt);
    fprintf(stderr, "Erro (linha %d): ", line);
    vfprintf(stderr, fmt, ap);
    fprintf(stderr, "\n");
    va_end(ap);
    compilation_error_count++;
}

/* obter linha atual via yylineno (mantido pelo flex) */
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



/* palavras-chave de controle de fluxo */
%token IF ELSE
%token DO WHILE FOR CONTINUE
%token SWITCH CASE DEFAULT BREAK
%token FUNCTION RETURN VOID
%token TRY CATCH THROW
%token WITH TYPEOF ENUM

/* variáveis */
%token LET CONST VAR

/* tipos de dados */
%token TYPE_NUMBER TYPE_STRING TYPE_BOOLEAN

/* literais */
%token <ival> BOOLEAN_LITERAL
%token <ival> NULL_LITERAL
%token <ival> NUMBER_LITERAL
%token <sval> STRING_LITERAL
%token <sval> IDENT

/* operadores */
%token PLUS MINUS MULT DIV ASSIGN

/* comparadores */
%token EQ NEQ LT GT LE GE

// precedência e associatividade dos operadores
%nonassoc LOWER_THAN_ELSE
%nonassoc ELSE
%nonassoc EQ NEQ
%nonassoc LT GT LE GE
%left PLUS MINUS
%left MULT DIV

/* símbolos */
%token SEMICOLON COMMA LPAREN RPAREN LBRACE RBRACE COLON


/* funções */
%token CONSOLE_READ CONSOLE_LOG


/* não-terminais tipados */
%type <ival> var_kind
%type <ast_node> declaration statement expr
%type <ast_node> stmt_list
%type <ast_node> block
%type <ast_node> if_stmt while_stmt for_stmt
%type <ast_node> declaration_or_expr
%type <ast_node> switch_stmt case_list case_item
%type <ast_node> console_stmt

%%

program:
    /* vazio */
    | program statement {
        if ($2)  // ignora declarações nulas (erros)
            ast_root = adicionarDeclaracao(ast_root, $2);
    }
;

/* declaracoes */
declaration:
    /* number */
    var_kind IDENT COLON TYPE_NUMBER ASSIGN expr SEMICOLON {
        char *ident_name = $2;
        $$ = criarNoDecl($1, TIPO_NUMBER, ident_name, $6);
        inserirSimbolo(ident_name, TIPO_NUMBER);

        int ok;
        int valor = avaliarExpr($6, &ok);
        if(ok) {
            atualizarValorConstante(ident_name, valor);
        }

        free(ident_name); /* liberar string do lexer */
    }

    /* string */
    | var_kind IDENT COLON TYPE_STRING ASSIGN expr SEMICOLON {
        char *ident_name = $2;
        $$ = criarNoDecl($1, TIPO_STRING, ident_name, $6);
        inserirSimbolo(ident_name, TIPO_STRING);
        free(ident_name);
    }
    /* boolean */
    | var_kind IDENT COLON TYPE_BOOLEAN ASSIGN expr SEMICOLON {
        char *ident_name = $2;
        $$ = criarNoDecl($1, TIPO_BOOLEAN, ident_name, $6);
        inserirSimbolo(ident_name, TIPO_BOOLEAN);
        free(ident_name);
    }
    /* casos de erro */
    | var_kind IDENT COLON TYPE_NUMBER ASSIGN STRING_LITERAL SEMICOLON {
        char *ident_name = $2;
        char *str_lit = $6;
        int line = (yylineno>0)?yylineno:1;
        report_error(line, "Tentativa de atribuir string a variável numérica '%s'.", ident_name);
        free(ident_name);
        free(str_lit);
        yyerrok;
        yyclearin;

    }
    | var_kind IDENT COLON TYPE_STRING ASSIGN NUMBER_LITERAL SEMICOLON {
        char *ident_name = $2;
        int line = (yylineno>0)?yylineno:1;
        report_error(line, "Tentativa de atribuir número a variável string '%s'.", ident_name);
        free(ident_name);
        yyerrok;
        yyclearin;
    }
    | var_kind IDENT COLON TYPE_BOOLEAN ASSIGN STRING_LITERAL SEMICOLON {
        char *ident_name = $2;
        char *str_lit = $6;
        int line = (yylineno>0)?yylineno:1;
        report_error(line, "Tentativa de atribuir string a variável boolean '%s'.", ident_name);
        free(ident_name);
        free(str_lit);
        yyerrok; yyclearin;
    }
    | var_kind IDENT COLON TYPE_BOOLEAN ASSIGN NUMBER_LITERAL SEMICOLON {
        char *ident_name = $2;
        int line = (yylineno>0)?yylineno:1;
        report_error(line, "Tentativa de atribuir número a variável boolean '%s'.", ident_name);
        free(ident_name);
        yyerrok; yyclearin;
    }
    ;

expr:
    expr PLUS expr {
       $$ = criarNoOp('+', $1, $3);
    }
    | expr MINUS expr {
        $$ = criarNoOp('-', $1, $3);
    }
    | expr MULT expr {
        $$ = criarNoOp('*', $1, $3);
    }
    | expr DIV expr {
        $$ = criarNoOp('/', $1, $3);
    }
    | expr EQ expr   { 
    $$ = criarNoOp(OP_EQ, $1, $3);
    }
    | expr NEQ expr  { 
    $$ = criarNoOp(OP_NEQ, $1, $3);
    }
    | expr LT expr   { 
    $$ = criarNoOp(OP_LT, $1, $3);
    }
    | expr GT expr   { 
    $$ = criarNoOp(OP_GT, $1, $3);
    }
    | expr LE expr   { 
    $$ = criarNoOp(OP_LE, $1, $3);
    }
    | expr GE expr   { 
    $$ = criarNoOp(OP_GE, $1, $3);
    }
    | NUMBER_LITERAL {
        $$ = criarNoNum($1);
    } 
    | STRING_LITERAL {
        $$ = criarNoStr($1);
        free($1);
    }
    | BOOLEAN_LITERAL {
        $$ = criarNoBool($1);
    }
    | IDENT {
    char *ident_name = $1;
        int ok;
        int valor = obterValor(ident_name, &ok); /* consultar tabela antes de decidir nó */

        if (ok) {
            /* substituir por número constante */
            $$ = criarNoNum(valor);
        } else {
            $$ = criarNoId(ident_name);
        }

        free(ident_name);
    }
    | '(' expr ')' {
        $$ = $2;
    }
    ;

/* log_statement:
    CONSOLE_LOG LPAREN IDENT RPAREN SEMICOLON {
        fprintf(out, "printf(\"%%s\\n\", %s);\n", $3);
    }
    | CONSOLE_LOG LPAREN STRING_LITERAL RPAREN SEMICOLON {
          fprintf(out, "printf(\"%%s\\n\", %s);\n", $3);
      } */
    /* casos de erro */
    /* | CONSOLE_LOG LPAREN NUMBER_LITERAL RPAREN SEMICOLON {
        int line = (yylineno>0)?yylineno:1;
        report_error(line, "Tentativa de logar número literal '%d'. Use uma variável ou string.", $3);
        yyerrok;
        yyclearin;
    }
    ;
     */

var_kind:
    LET { $$ = VK_LET; }
    | CONST { $$ = VK_CONST; }
    | VAR { $$ = VK_VAR; }
    ;

/* stmt_list: encadeia statements via prox (retorna primeiro) */
stmt_list:
    /* empty */ { $$ = NULL; }
  | stmt_list statement {
        if ($2) {
            if (!$1) $$ = $2;
            else $$ = adicionarDeclaracao($1, $2);
        } else $$ = $1;
    }
;

/* block with scope handling */
block:
    LBRACE { pushScope(); } stmt_list RBRACE { popScope(); $$ = criarNoBlock($3); }
;

/* console.log(expression);  -> por enquanto gera nó nulo (aceita sintaxe) */
console_stmt:
      CONSOLE_LOG LPAREN expr RPAREN SEMICOLON {
          $$ = criarNoConsoleLog($3); /* substituir por criarNoConsoleLog($3) quando existir */
      }
    ;

/* extend statement to accept blocks, control flows and console.log */
statement:
      declaration { $$ = $1; }
    | block       { $$ = $1; }
    | if_stmt     { $$ = $1; }
    | while_stmt  { $$ = $1; }
    | for_stmt    { $$ = $1; }
    | switch_stmt { $$ = $1; }
    | console_stmt { $$ = $1; }   /* <-- adicionado: aceita console.log(...) */
    | error SEMICOLON { yyerrok; yyclearin; $$ = NULL; }
    ;

/* if/else */
if_stmt:
    IF LPAREN expr RPAREN statement {
        $$ = criarNoIf($3, $5, NULL);
    }
    | IF LPAREN expr RPAREN statement ELSE statement {
        $$ = criarNoIf($3, $5, $7);
    }
;

/* while */
while_stmt:
    WHILE LPAREN expr RPAREN statement {
        $$ = criarNoWhile($3, $5);
    }
;

/* for (init ; cond ; update) statement
   where init can be a declaration or an expr or empty
*/
declaration_or_expr:
    declaration { $$ = $1; }
  | expr { $$ = $1; }
  | { $$ = NULL; }
;

for_stmt:
    FOR LPAREN declaration_or_expr SEMICOLON expr SEMICOLON declaration_or_expr RPAREN statement {
        $$ = criarNoFor($3, $5, $7, $9);
    }
;

/* switch/case*/
switch_stmt:
    SWITCH LPAREN expr RPAREN LBRACE case_list RBRACE {
        $$ = criarNoSwitch($3, $6);
    }
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
        /* case with numeric literal (simplified) */
        NoAST *case_expr = criarNoNum($2);
        NoAST *case_body = $4;
        $$ = criarNoCase(case_expr, case_body);
    }
  | DEFAULT COLON stmt_list {
        $$ = criarNoCase(NULL, $3); 
    }
;
%%

int main(int argc, char **argv) {
    if (argc < 2) {
        fprintf(stderr, "Uso: %s arquivo.ts\n", argv[0]);
        return EXIT_FAILURE;
    }
    yyin = fopen(argv[1], "r");
    if (!yyin) {
        perror("Erro ao abrir arquivo");
        return EXIT_FAILURE;
    }

    pushScope();
    yyparse();

    verificarTipo(ast_root);
    
    if (compilation_error_count > 0) {
        fprintf(stderr, "Encontrados %d erro(s). Abortando.\n", compilation_error_count);
        fclose(yyin);
        return EXIT_FAILURE;
    }

    printf("AST:\n");
    imprimirAST(ast_root);

    imprimirTabela();

    // === GERAR C ===
    gerarCodigoC(ast_root, "saida.c");
    printf("\nCódigo C gerado em 'saida.c'\n");

    ast_free(ast_root);
    ast_root = NULL;
    liberarTabelaSimbolos();

    fclose(yyin);
    return EXIT_SUCCESS;
}

/* yyerror: reporta sintaxe e incrementa contador — não exit() */
void yyerror(const char *s) {
    int line = (yylineno>0)?yylineno:1;
    /* Se bison já passar "syntax error" como s, imprimimos só "syntax error" uma vez.
       Caso s contenha uma mensagem específica, imprimimos ela. */
    if (s == NULL || strcmp(s, "syntax error") == 0) {
        report_error(line, "syntax error");
    } else {
        report_error(line, "%s", s);
    }
} /* não chamar exit() — o parser tentará recuperar via regras com 'error' */