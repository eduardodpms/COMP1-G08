%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// ainda nao sei o que e isso aqui nao mas se nao tiver quebra kkkkkkk
extern FILE *yyin;
extern int yylineno;
extern char* yytext;
int yyerror(const char *s);
int yylex(void);

FILE *out; // arquivo de saida
%}

%union {
    int ival; // valor inteiro
    char *sval; // valor string e identificador
}

%token LET
%token TYPE_NUMBER TYPE_STRING
%token NUMBER_LITERAL STRING_LITERAL
%token IDENT
%token SEMI COLON ASSIGN
%token CONSOLE_LOG
%token LPAREN RPAREN


%type <ival> NUMBER_LITERAL
%type <sval> STRING_LITERAL IDENT

%%

program:
    | program statement
    ;

statement: 
    declaration
    | log_statement
    ;

declaration:
    LET IDENT COLON TYPE_NUMBER ASSIGN NUMBER_LITERAL SEMI {
        fprintf(out, "int %s = %d;\n", $2, $6);
    }
  | LET IDENT COLON TYPE_STRING ASSIGN STRING_LITERAL SEMI {
        fprintf(out, "char* %s = %s;\n", $2, $6);
    }
    ;

log_statement:
    CONSOLE_LOG LPAREN IDENT RPAREN SEMI {
          fprintf(out, "printf(\"%%d\\n\", %s);\n", $3);
    }
    | 
    CONSOLE_LOG LPAREN STRING_LITERAL RPAREN SEMI {
          fprintf(out, "printf(\"%%s\\n\", %s);\n", $3);
      }
    ;
%%

int main(int argc, char **argv) {
    if (argc < 2) {
        fprintf(stderr, "Uso: %s <arquivo>\n", argv[0]);
        return 1;
    }

    yyin = fopen(argv[1], "r");
    if (!yyin) {
        perror("Erro ao abrir arquivo");
        return 1;
    }

    out = fopen("output.c", "w");
    fprintf(out, "#include <stdio.h>\n\nint main() {\n");

    yyparse();

    fprintf(out, "    return 0;\n}\n");
    fclose(out);
    return 0;
}

int yyerror(const char *s) {
    fprintf(stderr, "Erro: %s\n", s);
    fprintf(stderr, "'%s'\n", yytext);
    fprintf(stderr, "  Linha: %d\n", yylineno);
    return 0;
}


