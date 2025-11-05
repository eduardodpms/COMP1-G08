%{
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>

/* tamanho padrão para strings alocadas */
#define str_size 256

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

/* literais */
%token <ival> BOOLEAN_LITERAL
%token <ival> NULL_LITERAL
%token <ival> NUMBER_LITERAL
%token <sval> STRING_LITERAL
%token <sval> IDENT

/* palavras-chave de controle de fluxo */
%token IF ELSE ELSE_IF
%token DO WHILE FOR CONTINUE
%token SWITCH CASE DEFAULT BREAK
%token FUNCTION RETURN VOID
%token TRY CATCH THROW
%token WITH TYPEOF ENUM

/* variáveis */
%token LET CONST VAR

/* tipos de dados */
%token TYPE_NUMBER TYPE_STRING TYPE_BOOLEAN

/* operadores */
%token PLUS MINUS MULT DIV MOD ASSIGN
%token PLUS_ASSIGN MINUS_ASSIGN MULT_ASSIGN DIV_ASSIGN MOD_ASSIGN
%token EQUAL STRICT_EQUAL NOT_EQUAL STRICT_NOT_EQUAL
%token LESS LESS_EQUAL GREATER GREATER_EQUAL
%token AND OR NOT
%token SEMICOLON COMMA LPAREN RPAREN LBRACE RBRACE COLON
%token CONSOLE_READ CONSOLE_LOG

// Diretivas de precedência e associatividade
%left PLUS MINUS
%left MULT DIV MOD
%left EQUAL NOT_EQUAL LESS LESS_EQUAL GREATER GREATER_EQUAL
%left AND OR
%right NOT
%right ASSIGN
%left SEMICOLON COMMA
%left LPAREN RPAREN LBRACE RBRACE COLON
%left ELSE ELSE_IF

/* não-terminais tipados */
%type <sval> statement
%type <sval> comma_statement
%type <sval> scope_statement
%type <sval> else_statement
%type <sval> keyword
%type <sval> declaration
%type <sval> attribution
%type <sval> output_statement
%type <sval> input_statement
%type <sval> expression_statement
%type <ival> var_kind

%%


program:
    /* vazio */
    | program statement { fprintf(out, "    %s\n", $2); }
    ;

/* obs: todas as statements retornam ponteiros/strings, e essas
   strings são colocadas no arquivo somente no "program" acima. */
statement:
    keyword
    | declaration SEMICOLON {
        $$ = malloc(str_size);
        sprintf($$, "%s;", $1);
    }
    | comma_statement SEMICOLON {
        $$ = malloc(str_size);
        sprintf($$, "%s;", $1);
    }
    | LBRACE scope_statement {
        $$ = malloc(str_size);
        sprintf($$, "{\n%s", $2);
    }
    /* quando ocorre um erro dentro de uma statement, sincroniza até ';' e segue. */
    | error SEMICOLON {
        yyerrok;
        yyclearin;
        $$ = strdup("");
      }
    ;


comma_statement:
    attribution
    | output_statement
    | input_statement
    | comma_statement COMMA comma_statement {
        $$ = malloc(str_size);
        sprintf($$, "%s, %s", $1, $3);
    }
    ;


/* obs: scope_statement trata escopos por meio de
   recursão, analisando chaves "}" e statements. */
scope_statement:
    statement scope_statement {
        $$ = malloc(str_size);
        sprintf($$, "%s\n%s", $1, $2);
    }
    | statement RBRACE {
        $$ = malloc(str_size);
        sprintf($$, "%s\n}", $1);
    }
    ;


else_statement:
    ELSE_IF LPAREN expression_statement RPAREN statement else_statement {
        $$ = malloc(str_size);
        sprintf($$, "else if (%s) %s\n%s", $3, $5, $6);
    }
    | ELSE_IF LPAREN expression_statement RPAREN statement {
        $$ = malloc(str_size);
        sprintf($$, "else if (%s) %s", $3, $5);
    }
    | ELSE statement {
        $$ = malloc(str_size);
        sprintf($$, "else %s", $2);
    }
    ;


keyword:
    IF LPAREN expression_statement RPAREN statement else_statement {
        $$ = malloc(str_size);
        sprintf($$, "if (%s) %s\n%s", $3, $5, $6);
    }
    | IF LPAREN expression_statement RPAREN statement {
        $$ = malloc(str_size);
        sprintf($$, "if (%s) %s", $3, $5);
    }
    | WHILE LPAREN expression_statement RPAREN statement {
        $$ = malloc(str_size);
        sprintf($$, "while (%s) %s", $3, $5);
    }
    | FOR LPAREN declaration SEMICOLON expression_statement SEMICOLON attribution RPAREN statement {
        $$ = malloc(str_size);
        sprintf($$, "for (%s; %s; %s) %s", $3, $5, $7, $9);
    }
    ;

/* declaracoes */
declaration:
    var_kind IDENT COLON TYPE_NUMBER ASSIGN expression_statement {
        $$ = malloc(str_size);
        if ($1 == CONST)
            sprintf($$, "const int %s = %s", $2, $6);
        else
            sprintf($$, "int %s = %s", $2, $6);
    }
    | var_kind IDENT COLON TYPE_STRING ASSIGN STRING_LITERAL {
        $$ = malloc(str_size);  
        if ($1 == CONST)
            sprintf($$, "const char* %s = %s", $2, $6);
        else
            sprintf($$, "char* %s = %s", $2, $6);
    }
    /* casos de erro */
    | var_kind IDENT COLON TYPE_NUMBER ASSIGN STRING_LITERAL {
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


attribution:
    IDENT ASSIGN expression_statement {
        $$ = malloc(str_size);
        sprintf($$, "%s = %s", $1, $3);
    }
    ;


output_statement:
    CONSOLE_LOG LPAREN IDENT RPAREN {
        $$ = malloc(str_size);
        sprintf($$, "printf(\"%%s\\n\", %s)", $3);
    }
    | CONSOLE_LOG LPAREN STRING_LITERAL RPAREN {
        $$ = malloc(str_size);
        sprintf($$, "printf(\"%%s\\n\", %s)", $3);
    }
    /*| CONSOLE_LOG LPAREN expression_statement RPAREN {
        $$ = malloc(str_size);
        sprintf($$, "printf(\"%%d\\n\", %d)", $3);  /* %d para números */
    
    /* casos de erro */
    /*| CONSOLE_LOG LPAREN NUMBER_LITERAL RPAREN {
        int line = (yylineno>0)?yylineno:1;
        report_error(line, "Tentativa de logar número literal '%d'. Use uma variável ou string.", $3);
        yyerrok;
        yyclearin;
    }*/
    ;


input_statement:
    CONSOLE_READ LPAREN IDENT RPAREN {
        $$ = malloc(str_size);
        sprintf($$, "scanf(\"%%d\", &%s)", $3);
    }
;


expression_statement:
    NUMBER_LITERAL                                            { $$ = malloc(str_size); sprintf($$, "%d", $1); }
    | LPAREN expression_statement RPAREN                      { $$ = malloc(str_size); sprintf($$, "(%s)", $2); }
    | expression_statement PLUS expression_statement          { $$ = malloc(str_size); sprintf($$, "%s + %s", $1, $3); }
    | expression_statement MINUS expression_statement         { $$ = malloc(str_size); sprintf($$, "%s - %s", $1, $3); }
    | expression_statement MULT expression_statement          { $$ = malloc(str_size); sprintf($$, "%s * %s", $1, $3); }
    | expression_statement DIV expression_statement           { $$ = malloc(str_size); sprintf($$, "%s / %s", $1, $3); }
    | expression_statement MOD expression_statement           { $$ = malloc(str_size); sprintf($$, "%s %% %s", $1, $3); }
    | expression_statement EQUAL expression_statement         { $$ = malloc(str_size); sprintf($$, "%s == %s", $1, $3); }
    | expression_statement NOT_EQUAL expression_statement     { $$ = malloc(str_size); sprintf($$, "%s != %s", $1, $3); }
    | expression_statement LESS expression_statement          { $$ = malloc(str_size); sprintf($$, "%s < %s", $1, $3); }
    | expression_statement LESS_EQUAL expression_statement    { $$ = malloc(str_size); sprintf($$, "%s <= %s", $1, $3); }
    | expression_statement GREATER expression_statement       { $$ = malloc(str_size); sprintf($$, "%s > %s", $1, $3); }
    | expression_statement GREATER_EQUAL expression_statement { $$ = malloc(str_size); sprintf($$, "%s >= %s", $1, $3); }
    | expression_statement AND expression_statement           { $$ = malloc(str_size); sprintf($$, "%s && %s", $1, $3); }
    | expression_statement OR expression_statement            { $$ = malloc(str_size); sprintf($$, "%s || %s", $1, $3); }
    | NOT expression_statement                                { $$ = malloc(str_size); sprintf($$, "!%s", $2); }
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