#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include "ast.h"
#include "tabela.h"

// Função recursiva para inferir tipo durante a geração de código
// Necessária para decidir entre %d, %s ou operações de concatenação
TipoDado obterTipoExpressao(NoAST *expr)
{
    if (!expr)
        return -1;
    switch (expr->tipo)
    {
    case NO_NUM:
        return TIPO_NUMBER;
    case NO_STR:
        return TIPO_STRING;
    case NO_BOOL:
        return TIPO_BOOLEAN;
    case NO_ID:
        return obterTipo(expr->nome); // Agora funcionará pois o codegen popula a tabela
    case NO_OP:
        // Comparadores
        if (expr->valor >= AST_OP_EQ && expr->valor <= AST_OP_GE)
            return TIPO_BOOLEAN;

        // Soma/Concatenação
        if (expr->valor == AST_OP_ADD)
        {
            TipoDado tEsq = obterTipoExpressao(expr->esquerda);
            TipoDado tDir = obterTipoExpressao(expr->direita);
            if (tEsq == TIPO_STRING || tDir == TIPO_STRING)
            {
                return TIPO_STRING;
            }
            return TIPO_NUMBER;
        }

        return TIPO_NUMBER;

    default:
        return TIPO_NUMBER;
    }
}

void gerarCodigoC_rec(NoAST *raiz, FILE *out)
{
    if (!raiz)
        return;

    // VARIÁVEIS DE CONTROLE DE ESCOPO
    int escopo_criado = 0;

    switch (raiz->tipo)
    {
    case NO_DECL:
        // [CRÍTICO] Registra a variável na tabela para que usos futuros saibam o tipo
        inserirSimbolo(raiz->decl.nome, raiz->decl.tipo_dado);

        if (raiz->decl.tipo_dado == TIPO_NUMBER || raiz->decl.tipo_dado == TIPO_BOOLEAN)
            fprintf(out, "int ");
        else if (raiz->decl.tipo_dado == TIPO_STRING)
            fprintf(out, "char* ");

        fprintf(out, "%s = ", raiz->decl.nome);
        gerarCodigoC_rec(raiz->decl.expr, out);
        break;

    case NO_NUM:
        fprintf(out, "%d", raiz->valor);
        break;
    case NO_BOOL:
        fprintf(out, "%d", raiz->valor);
        break;
    case NO_STR:
        fprintf(out, "\"%s\"", raiz->texto);
        break;
    case NO_ID:
        fprintf(out, "%s", raiz->nome);
        break;

    case NO_OP:
        if (raiz->valor == AST_OP_READ)
        {
            fprintf(out, "({ int val; scanf(\"%%d\", &val); val; })");
        }
        else if (raiz->valor == AST_OP_ASSIGN)
        {
            gerarCodigoC_rec(raiz->esquerda, out);
            fprintf(out, " = ");
            gerarCodigoC_rec(raiz->direita, out);
        }
        else if (raiz->valor == AST_OP_INC)
        {
            if (raiz->esquerda)
            {
                gerarCodigoC_rec(raiz->esquerda, out);
                fprintf(out, "++");
            }
            else
            {
                fprintf(out, "++");
                gerarCodigoC_rec(raiz->direita, out);
            }
        }
        else if (raiz->valor == AST_OP_DEC)
        {
            if (raiz->esquerda)
            {
                gerarCodigoC_rec(raiz->esquerda, out);
                fprintf(out, "--");
            }
            else
            {
                fprintf(out, "--");
                gerarCodigoC_rec(raiz->direita, out);
            }
        }
        else
        {
            int ehConcatenacao = 0;
            if (raiz->valor == AST_OP_ADD)
            {
                TipoDado tEsq = obterTipoExpressao(raiz->esquerda);
                TipoDado tDir = obterTipoExpressao(raiz->direita);
                if (tEsq == TIPO_STRING || tDir == TIPO_STRING)
                {
                    ehConcatenacao = 1;
                    fprintf(out, "concat_str(");
                    gerarCodigoC_rec(raiz->esquerda, out);
                    fprintf(out, ", ");
                    gerarCodigoC_rec(raiz->direita, out);
                    fprintf(out, ")");
                }
            }

            if (!ehConcatenacao)
            {
                fprintf(out, "(");
                gerarCodigoC_rec(raiz->esquerda, out);
                switch (raiz->valor)
                {
                case AST_OP_ADD:
                    fprintf(out, " + ");
                    break;
                case AST_OP_SUB:
                    fprintf(out, " - ");
                    break;
                case AST_OP_MUL:
                    fprintf(out, " * ");
                    break;
                case AST_OP_DIV:
                    fprintf(out, " / ");
                    break;
                case AST_OP_MOD:
                    fprintf(out, " %% ");
                    break;
                case AST_OP_EQ:
                    fprintf(out, " == ");
                    break;
                case AST_OP_NEQ:
                    fprintf(out, " != ");
                    break;
                case AST_OP_LT:
                    fprintf(out, " < ");
                    break;
                case AST_OP_GT:
                    fprintf(out, " > ");
                    break;
                case AST_OP_LE:
                    fprintf(out, " <= ");
                    break;
                case AST_OP_GE:
                    fprintf(out, " >= ");
                    break;
                default:
                    fprintf(out, " ? ");
                    break;
                }
                gerarCodigoC_rec(raiz->direita, out);
                fprintf(out, ")");
            }
        }
        break;

    case NO_BLOCK:
        // [CRÍTICO] Abre escopo para que variáveis locais existam na tabela
        pushScope();
        escopo_criado = 1;

        fprintf(out, "{\n");
        for (NoAST *s = raiz->body; s != NULL; s = s->prox)
        {
            fprintf(out, "    ");
            gerarCodigoC_rec(s, out);
            if (s->tipo == NO_DECL || s->tipo == NO_OP || s->tipo == NO_CONSOLE_LOG ||
                s->tipo == NO_BREAK || s->tipo == NO_CONTINUE || s->tipo == NO_ID)
            {
                fprintf(out, ";");
            }
            fprintf(out, "\n");
        }
        fprintf(out, "}");

        // Fecha escopo (popScope chamado no final da função para evitar código inalcançável no switch)
        break;

    case NO_IF:
        fprintf(out, "if (");
        gerarCodigoC_rec(raiz->esquerda, out);
        fprintf(out, ") ");
        gerarCodigoC_rec(raiz->direita, out);
        if (raiz->else_branch)
        {
            fprintf(out, " else ");
            gerarCodigoC_rec(raiz->else_branch, out);
        }
        break;

    case NO_WHILE:
        fprintf(out, "while (");
        gerarCodigoC_rec(raiz->esquerda, out);
        fprintf(out, ") ");
        gerarCodigoC_rec(raiz->body, out);
        break;

    case NO_FOR:
        // [CRÍTICO] Abre escopo para o loop FOR (para variáveis declaradas no init)
        pushScope();
        escopo_criado = 1;

        fprintf(out, "for (");
        if (raiz->esquerda)
            gerarCodigoC_rec(raiz->esquerda, out);
        fprintf(out, ";");
        if (raiz->direita)
            gerarCodigoC_rec(raiz->direita, out);
        fprintf(out, ";");
        if (raiz->update)
            gerarCodigoC_rec(raiz->update, out);
        fprintf(out, ") ");
        gerarCodigoC_rec(raiz->body, out);
        break;

    case NO_BREAK:
        fprintf(out, "break");
        break;
    case NO_CONTINUE:
        fprintf(out, "continue");
        break;

    case NO_SWITCH:
        fprintf(out, "switch (");
        gerarCodigoC_rec(raiz->esquerda, out);
        fprintf(out, ") {\n");
        for (NoAST *c = raiz->body; c != NULL; c = c->prox)
        {
            if (c->esquerda)
            {
                fprintf(out, "case ");
                gerarCodigoC_rec(c->esquerda, out);
                fprintf(out, ":\n");
            }
            else
            {
                fprintf(out, "default:\n");
            }
            // Case body é tratado como lista de statements, não necessariamente um bloco
            // Se o usuário usou chaves no case, o NO_BLOCK tratará o escopo.
            if (c->body)
            {
                for (NoAST *s = c->body; s != NULL; s = s->prox)
                {
                    fprintf(out, "    ");
                    gerarCodigoC_rec(s, out);
                    if (s->tipo == NO_DECL || s->tipo == NO_OP || s->tipo == NO_CONSOLE_LOG ||
                        s->tipo == NO_BREAK || s->tipo == NO_CONTINUE)
                    {
                        fprintf(out, ";");
                    }
                    fprintf(out, "\n");
                }
            }
        }
        fprintf(out, "}\n");
        break;

    case NO_CONSOLE_LOG:
        fprintf(out, "    printf(");
        if (raiz->esquerda)
        {
            TipoDado tipo = obterTipoExpressao(raiz->esquerda);

            if (tipo == TIPO_STRING)
            {
                fprintf(out, "\"%%s\\n\", ");
                gerarCodigoC_rec(raiz->esquerda, out);
            }
            else if (tipo == TIPO_BOOLEAN)
            {
                fprintf(out, "\"%%s\\n\", ");
                gerarCodigoC_rec(raiz->esquerda, out);
                fprintf(out, " ? \"true\" : \"false\"");
            }
            else
            {
                fprintf(out, "\"%%d\\n\", ");
                gerarCodigoC_rec(raiz->esquerda, out);
            }
        }
        fprintf(out, ")");
        break;

    default:
        break;
    }

    // [CRÍTICO] Se abriu escopo neste nó, fecha agora.
    if (escopo_criado)
    {
        popScope();
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

    // Limpa qualquer lixo anterior da tabela para garantir geração limpa
    liberarTabelaSimbolos();
    pushScope(); // Escopo Global para Codegen

    fprintf(out, "#include <stdio.h>\n");
    fprintf(out, "#include <stdlib.h>\n");
    fprintf(out, "#include <string.h>\n\n");
    fprintf(out, "char* concat_str(char* s1, char* s2) {\n");
    fprintf(out, "    char* result = malloc(strlen(s1) + strlen(s2) + 1);\n");
    fprintf(out, "    strcpy(result, s1);\n");
    fprintf(out, "    strcat(result, s2);\n");
    fprintf(out, "    return result;\n");
    fprintf(out, "}\n\n");
    fprintf(out, "int main() {\n");

    for (NoAST *n = ast_root; n != NULL; n = n->prox)
    {
        fprintf(out, "    ");
        gerarCodigoC_rec(n, out);
        if (n->tipo == NO_DECL || n->tipo == NO_OP || n->tipo == NO_CONSOLE_LOG ||
            n->tipo == NO_BREAK || n->tipo == NO_CONTINUE)
        {
            fprintf(out, ";");
        }
        fprintf(out, "\n");
    }

    fprintf(out, "    return 0;\n}\n");

    // Limpeza final
    popScope(); // Fecha escopo global
    fclose(out);
}