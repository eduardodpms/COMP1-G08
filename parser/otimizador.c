#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ast.h"
#include "otimizador.h"

static void free_node_shallow(NoAST *n)
{
    if (n)
        free(n);
}

static NoAST *cloneAST(NoAST *n)
{
    if (!n)
        return NULL;
    NoAST *c = calloc(1, sizeof(NoAST)); // calloc zera a memória

    // copia dados primitivos (tipo, valor, nome, etc)
    memcpy(c, n, sizeof(NoAST));

    c->prox = NULL;

    // clona filhos recursivamente
    c->esquerda = cloneAST(n->esquerda);
    c->direita = cloneAST(n->direita);
    c->body = cloneAST(n->body);
    c->else_branch = cloneAST(n->else_branch);
    c->update = cloneAST(n->update);

    if (n->tipo == NO_DECL)
    {
        c->decl.expr = cloneAST(n->decl.expr);
    }

    return c;
}

NoAST *otimizarStrengthReduction(NoAST *raiz)
{
    if (!raiz)
        return NULL;

    // Otimiza filhos primeiro
    raiz->esquerda = otimizarStrengthReduction(raiz->esquerda);
    raiz->direita = otimizarStrengthReduction(raiz->direita);
    if (raiz->update)
        raiz->update = otimizarStrengthReduction(raiz->update);
    if (raiz->else_branch)
        raiz->else_branch = otimizarStrengthReduction(raiz->else_branch);
    if (raiz->decl.expr)
        raiz->decl.expr = otimizarStrengthReduction(raiz->decl.expr);
    if (raiz->body)
        raiz->body = otimizarStrengthReduction(raiz->body);

    // Otimiza o próximo statement
    raiz->prox = otimizarStrengthReduction(raiz->prox);

    if (raiz->tipo == NO_OP)
    {
        int op = raiz->valor;

        if (raiz->esquerda && raiz->direita &&
            raiz->esquerda->tipo == NO_NUM && raiz->direita->tipo == NO_NUM)
        {

            int a = raiz->esquerda->valor;
            int b = raiz->direita->valor;
            int res = 0;
            int podeOtimizar = 1;

            switch (op)
            {
            case AST_OP_ADD:
                res = a + b;
                break;
            case AST_OP_SUB:
                res = a - b;
                break;
            case AST_OP_MUL:
                res = a * b;
                break;
            case AST_OP_DIV:
                if (b != 0)
                    res = a / b;
                else
                    podeOtimizar = 0;
                break;
            case AST_OP_MOD:
                if (b != 0)
                    res = a % b;
                else
                    podeOtimizar = 0;
                break;
            default:
                podeOtimizar = 0;
                break;
            }

            if (podeOtimizar)
            {
                NoAST *novo = criarNoNum(res);
                novo->prox = raiz->prox;
                free_node_shallow(raiz);
                return novo;
            }
        }

        if (op == AST_OP_MUL)
        {
            // x * 0 = 0
            if ((raiz->esquerda && raiz->esquerda->tipo == NO_NUM && raiz->esquerda->valor == 0) ||
                (raiz->direita && raiz->direita->tipo == NO_NUM && raiz->direita->valor == 0))
                return criarNoNum(0);

            // x * 1 = x
            if (raiz->direita && raiz->direita->tipo == NO_NUM && raiz->direita->valor == 1)
                return raiz->esquerda;
            if (raiz->esquerda && raiz->esquerda->tipo == NO_NUM && raiz->esquerda->valor == 1)
                return raiz->direita;

            if (raiz->direita && raiz->direita->tipo == NO_NUM)
            {
                int k = raiz->direita->valor;
                if (k >= 2 && k <= 5)
                {
                    NoAST *soma = cloneAST(raiz->esquerda);
                    for (int i = 1; i < k; i++)
                    {
                        // Cria (soma + x)
                        soma = criarNoOp(AST_OP_ADD, soma, cloneAST(raiz->esquerda));
                    }
                    // Mantém o ponteiro prox original
                    soma->prox = raiz->prox;
                    free_node_shallow(raiz);
                    return soma;
                }
            }
        }

        if (op == AST_OP_ADD)
        {
            if (raiz->esquerda && raiz->esquerda->tipo == NO_NUM && raiz->esquerda->valor == 0)
                return raiz->direita;
            if (raiz->direita && raiz->direita->tipo == NO_NUM && raiz->direita->valor == 0)
                return raiz->esquerda;
        }
    }
    return raiz;
}