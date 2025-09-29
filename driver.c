/* driver.c */
#include <stdio.h>
#include "src/parser.tab.h"   /* contém YYSTYPE e defines dos tokens */

extern char *yytext;  /* vem do scanner */
int yylex(void);      /* protótipo do scanner */

YYSTYPE yylval;       /* define a variável global usada pelo scanner */

/* função utilitária para imprimir o valor string com segurança */
static const char* sval_or_null(char *s) {
    return s ? s : "(null)";
}

int main(void) {
    int tok;
    while ((tok = yylex()) != 0) {
        /* imprimimos yylval.sval como ponteiro e também, se não nulo, como string */
        printf("SCANNER -> token=%d  yytext=\"%s\"  yylval.ival=%d  yylval.sval=%p",
               tok,
               yytext ? yytext : "(null)",
               yylval.ival,
               (void*) yylval.sval);
        if (yylval.sval) {
            printf(" \"%s\"", sval_or_null(yylval.sval));
        }
        printf("\n");
    }
    printf("SCANNER -> retornou EOF (0)\n");
    return 0;
}