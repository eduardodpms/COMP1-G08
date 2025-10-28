%{
#include "ast.h"
#include "tabela.h"
#include "codegen.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>

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

// precedência e associatividade dos operadores
%left PLUS MINUS
%left MULT DIV

/* símbolos */
%token SEMICOLON COMMA LPAREN RPAREN LBRACE RBRACE COLON


/* funções */
%token CONSOLE_READ CONSOLE_LOG


/* não-terminais tipados */
%type <ival> var_kind
%type <ast_node> declaration statement expr

%%

program:
    /* vazio */
    | program statement {
        if ($2)  // ignora declarações nulas (erros)
            ast_root = adicionarDeclaracao(ast_root, $2);
    }
;

statement:
    declaration {
        $$ = $1;
    }
    | error SEMICOLON { yyerrok; yyclearin; $$ = NULL; }
    ;

/* declaracoes */
declaration:
    /* number */
    var_kind IDENT COLON TYPE_NUMBER ASSIGN expr SEMICOLON {
        $$ = criarNoDecl($1, TIPO_NUMBER, $2, $6);
        inserirSimbolo($2, TIPO_NUMBER);

        int ok;
        int valor = avaliarExpr($6, &ok);
        if(ok) {
            atualizarValorConstante($2, valor);
        }
    }

    /* string */
    | var_kind IDENT COLON TYPE_STRING ASSIGN expr SEMICOLON {
        $$ = criarNoDecl($1, TIPO_STRING, $2, $6);
        inserirSimbolo($2, TIPO_STRING);
    }
    /* boolean */
    | var_kind IDENT COLON TYPE_BOOLEAN ASSIGN expr SEMICOLON {
        $$ = criarNoDecl($1, TIPO_BOOLEAN, $2, $6);
        inserirSimbolo($2, TIPO_BOOLEAN);
    }
    /* casos de erro */
    | var_kind IDENT COLON TYPE_NUMBER ASSIGN STRING_LITERAL SEMICOLON {
        int line = (yylineno>0)?yylineno:1;
        report_error(line, "Tentativa de atribuir string a variável numérica '%s'.", $2);
        yyerrok;
        yyclearin;
    }
    | var_kind IDENT COLON TYPE_STRING ASSIGN NUMBER_LITERAL SEMICOLON {
        int line = (yylineno>0)?yylineno:1;
        report_error(line, "Tentativa de atribuir número a variável string '%s'.", $2);
        yyerrok;
        yyclearin;
    }
    | var_kind IDENT COLON TYPE_BOOLEAN ASSIGN STRING_LITERAL SEMICOLON {
    int line = (yylineno>0)?yylineno:1;
    report_error(line, "Tentativa de atribuir string a variável boolean '%s'.", $2);
    yyerrok; yyclearin;
    }
    | var_kind IDENT COLON TYPE_BOOLEAN ASSIGN NUMBER_LITERAL SEMICOLON {
        int line = (yylineno>0)?yylineno:1;
        report_error(line, "Tentativa de atribuir número a variável boolean '%s'.", $2);
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
    | NUMBER_LITERAL {
        $$ = criarNoNum($1);
    } 
    | STRING_LITERAL {
        $$ = criarNoStr($1);
    }
    | BOOLEAN_LITERAL {
        $$ = criarNoBool($1);
    }
    | IDENT {
        $$ = criarNoId($1);

        int ok;
        int valor = obterValor($1, &ok);
        if (ok) {
            // Se o identificador tem valor constante, substitui pelo valor
            free($$);
            $$ = criarNoNum(valor);
        }
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

    int parse_ret = yyparse();

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