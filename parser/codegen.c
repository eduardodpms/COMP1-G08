#include <stdio.h>
#include "ast.h"
#include "tabela.h"

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

    for (NoAST *n = ast_root; n != NULL; n = n->direita)
    {
        gerarCodigoC_rec(n, out);
    }

    fprintf(out, "    return 0;\n}\n");
    fclose(out);
}