#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include "ast.h"
#include "tabela.h"

TipoDado obterTipoExpressao(NoAST *expr) {
    if (!expr) return -1;
    
    switch (expr->tipo) {
        case NO_NUM:
            return TIPO_NUMBER;
        case NO_STR:
            return TIPO_STRING;
        case NO_BOOL:
            return TIPO_BOOLEAN;
        case NO_ID:
            // Para identificar o tipo de variáveis, precisamos da tabela de símbolos
            return obterTipo(expr->nome);
        default:
            return TIPO_NUMBER; // padrão para outros casos
    }
}

void gerarCodigoC_rec(NoAST *raiz, FILE *out)
{
    if (!raiz)
        return;

    switch (raiz->tipo)
    {
    case NO_DECL:
        // Tipo da variável
        if (raiz->decl.tipo_dado == TIPO_NUMBER)
            fprintf(out, "int ");
        else if (raiz->decl.tipo_dado == TIPO_BOOLEAN)
            fprintf(out, "int ");
        else if (raiz->decl.tipo_dado == TIPO_STRING)
            fprintf(out, "char* ");

        fprintf(out, "%s = ", raiz->decl.nome);
        gerarCodigoC_rec(raiz->decl.expr, out); // expressão inicialização
        fprintf(out, ";\n");
        break;

    case NO_NUM:
        fprintf(out, "%d", raiz->valor);
        break;
    case NO_BOOL:
        fprintf(out, "%d", raiz->valor); // true = 1, false = 0
        break;
    case NO_STR:
        fprintf(out, "\"%s\"", raiz->texto);
        break;
    case NO_ID:
        fprintf(out, "%s", raiz->nome);
        break;
    case NO_OP:
        fprintf(out, "(");
        gerarCodigoC_rec(raiz->esquerda, out);
        fprintf(out, " %c ", (char)raiz->valor);
        gerarCodigoC_rec(raiz->direita, out);
        fprintf(out, ")");
        break;
    case NO_BLOCK:
        fprintf(out, "{\n");
        for (NoAST *s = raiz->body; s != NULL; s = s->prox) {
            gerarCodigoC_rec(s, out);
        }
        fprintf(out, "}\n");
        break;

    case NO_IF:
        fprintf(out, "if (");
        gerarCodigoC_rec(raiz->esquerda, out);
        fprintf(out, ") ");
        gerarCodigoC_rec(raiz->direita, out);
        if (raiz->prox) {
            fprintf(out, " else ");
            gerarCodigoC_rec(raiz->prox, out);
        }
        fprintf(out, "\n");
        break;

    case NO_WHILE:
        fprintf(out, "while (");
        gerarCodigoC_rec(raiz->esquerda, out);
        fprintf(out, ") ");
        if (raiz->body) gerarCodigoC_rec(raiz->body, out);
        fprintf(out, "\n");
        break;

    case NO_FOR:
        fprintf(out, "for (");
        /* init */
        if (raiz->esquerda) gerarCodigoC_rec(raiz->esquerda, out);
        fprintf(out, ";");
        /* cond */
        if (raiz->direita) gerarCodigoC_rec(raiz->direita, out);
        fprintf(out, ";");
        /* update */
        if (raiz->prox) gerarCodigoC_rec(raiz->prox, out);
        fprintf(out, ") ");
        if (raiz->body) gerarCodigoC_rec(raiz->body, out);
        fprintf(out, "\n");
        break;

    case NO_BREAK:
        fprintf(out, "break;\n");
        break;
    case NO_CONTINUE:
        fprintf(out, "continue;\n");
        break;

    case NO_SWITCH:
        fprintf(out, "switch (");
        gerarCodigoC_rec(raiz->esquerda, out);
        fprintf(out, ") {\n");
        /* body is list of NO_CASE nodes chained by prox */
        for (NoAST *c = raiz->body; c != NULL; c = c->prox) {
            if (c->esquerda) {
                fprintf(out, "case ");
                gerarCodigoC_rec(c->esquerda, out);
                fprintf(out, ":\n");
            } else {
                fprintf(out, "default:\n");
            }
            if (c->body) {
                for (NoAST *s = c->body; s != NULL; s = s->prox)
                    gerarCodigoC_rec(s, out);
            }
        }
        fprintf(out, "}\n");
        break;

           case NO_CONSOLE_LOG:
        fprintf(out, "    printf(");
        if (raiz->esquerda) {
            // Se for um identificador, consultar a tabela de símbolos
            if (raiz->esquerda->tipo == NO_ID) {
                TipoDado tipo_variavel = obterTipo(raiz->esquerda->nome);
                switch (tipo_variavel) {
                    case TIPO_STRING:
                        fprintf(out, "\"%%s\\n\", ");
                        gerarCodigoC_rec(raiz->esquerda, out);
                        break;
                    case TIPO_BOOLEAN:
                        fprintf(out, "\"%%s\\n\", ");
                        gerarCodigoC_rec(raiz->esquerda, out);
                        fprintf(out, " ? \"true\" : \"false\"");
                        break;
                    default: // TIPO_NUMBER
                        fprintf(out, "\"%%d\\n\", ");
                        gerarCodigoC_rec(raiz->esquerda, out);
                        break;
                }
            }
            // Para outros tipos (literais)
            else {
                switch (raiz->esquerda->tipo) {
                    case NO_STR:
                        fprintf(out, "\"%%s\\n\", ");
                        gerarCodigoC_rec(raiz->esquerda, out);
                        break;
                    case NO_BOOL:
                        fprintf(out, "\"%%s\\n\", ");
                        gerarCodigoC_rec(raiz->esquerda, out);
                        fprintf(out, " ? \"true\" : \"false\"");
                        break;
                    default: // NO_NUM, NO_OP, etc.
                        fprintf(out, "\"%%d\\n\", ");
                        gerarCodigoC_rec(raiz->esquerda, out);
                        break;
                }
            }
        }
        fprintf(out, ");\n");
        break;
        
    default:
        break;
    }
}


void gerarCodigoC(NoAST *ast_root, const char *nomeArquivo)
{
    FILE *out = fopen(nomeArquivo, "w");
    if (!out)
    {
        perror("Erro ao criar arquivo");
        return;
    }

    fprintf(out, "#include <stdio.h>\n\nint main() {\n");

    for (NoAST *n = ast_root; n != NULL; n = n->prox)
    {
        gerarCodigoC_rec(n, out);
    }

    fprintf(out, "    return 0;\n}\n");
    fclose(out);
}
