/******************************************************
FGA0003 - Compiladores 1
Curso de Engenharia de Software
Universidade de Brasília (UnB)

Arquivo: parser.y
Descrição: Exemplo de gramática para expressão aritmética
******************************************************/


%{
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>

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

%union {
    int ival;
    char *sval;
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

/* símbolos */
%token SEMICOLON COMMA LPAREN RPAREN LBRACE RBRACE COLON

/* funções */
%token CONSOLE_READ CONSOLE_LOG


/* não-terminais tipados */
%type <ival> var_kind

%%

program:
    /* vazio */
    | program statement
    ;

statement:
    declaration
    | log_statement
    /* quando ocorre um erro dentro de uma statement, sincroniza até ';' e segue. */
    | error SEMICOLON {
        yyerrok;
        yyclearin;
      }
    ;

/* declaracoes */
declaration:
    var_kind IDENT COLON TYPE_NUMBER ASSIGN NUMBER_LITERAL SEMICOLON {
        int val = $6;        /* agora $6 é NUMBER_LITERAL */
        char* name = $2;     /* IDENT */
        int kind = $1;       
        if (kind == CONST)
            fprintf(out, "const int %s = %d;\n", name, val);
        else
            fprintf(out, "int %s = %d;\n", name, val);
    }
    | var_kind IDENT COLON TYPE_STRING ASSIGN STRING_LITERAL SEMICOLON {
        char* val = $6;      
        char* name = $2;     
        int kind = $1;       
        if (kind == CONST)
           fprintf(out, "const char* %s = %s;\n", name, val);
        else
             fprintf(out, "char* %s = %s;\n", name, val);
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
    ;

log_statement:
    CONSOLE_LOG LPAREN IDENT RPAREN SEMICOLON {
        fprintf(out, "printf(\"%%s\\n\", %s);\n", $3);
    }
    | CONSOLE_LOG LPAREN STRING_LITERAL RPAREN SEMICOLON {
          fprintf(out, "printf(\"%%s\\n\", %s);\n", $3);
      }
    /* casos de erro */
    | CONSOLE_LOG LPAREN NUMBER_LITERAL RPAREN SEMICOLON {
        int line = (yylineno>0)?yylineno:1;
        report_error(line, "Tentativa de logar número literal '%d'. Use uma variável ou string.", $3);
        yyerrok;
        yyclearin;
    }
    ;

var_kind: 
    LET { $$ = LET; }
    | CONST { $$ = CONST ; }
    | VAR { $$ = VAR; }
    ;

%%

int main(int argc, char **argv) {
    if (argc < 2) {
        fprintf(stderr, "Uso: %s arquivo.ts\n", argv[0]);
        return EXIT_FAILURE;
    }

    yyin = fopen(argv[1], "r");
    if (yyin == NULL) {
        perror("Erro ao abrir arquivo de entrada");
        return EXIT_FAILURE;
    }

    out = fopen("output.c", "w");
    if (out == NULL) {
        perror("Erro ao abrir arquivo de saída");
        fclose(yyin);
        return EXIT_FAILURE;
    }

    fprintf(out, "#include <stdio.h>\n");
    fprintf(out, "int main() {\n");

    int parse_ret = yyparse();

    /* Se o parser retornou erro FATAL e não houve outros erros reportados, considere fatal. */
    if (parse_ret != 0 && compilation_error_count == 0) {
        fprintf(stderr, "Parsing failed (fatal). Compilation aborted.\n");
        fclose(yyin);
        fclose(out);
        remove("output.c");
        return EXIT_FAILURE;
    }

    /* se encontramos erros (léxicos/sintáticos/semânticos), não geramos output final */
    if (compilation_error_count > 0) {
        fprintf(stderr, "Encontrados %d erro(s). Compilação abortada.\n", compilation_error_count);
        fclose(yyin);
        fclose(out);
        remove("output.c");
        return EXIT_FAILURE;
    }

    fprintf(out, "    return 0;\n}\n");

    fclose(yyin);
    fclose(out);
    printf("Compilação bem-sucedida. output.c gerado.\n");
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