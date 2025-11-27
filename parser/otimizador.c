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

// Helper para clonar (caso precise reativar somas repetidas, mas agora não estamos usando)
// Mantido caso precise no futuro, ou removível.
static NoAST *cloneAST(NoAST *n)
{
    if (!n)
        return NULL;
    NoAST *c = malloc(sizeof(NoAST));
    memcpy(c, n, sizeof(NoAST));
    c->prox = NULL;
    c->esquerda = cloneAST(n->esquerda);
    c->direita = cloneAST(n->direita);
    c->body = cloneAST(n->body);
    c->else_branch = cloneAST(n->else_branch);
    c->update = cloneAST(n->update);
    if (n->tipo == NO_DECL)
        c->decl.expr = cloneAST(n->decl.expr);
    return c;
}

NoAST *otimizarStrengthReduction(NoAST *raiz)
{
    if (!raiz)
        return NULL;

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
    raiz->prox = otimizarStrengthReduction(raiz->prox);

    if (raiz->tipo == NO_OP)
    {
        int op = raiz->valor;

        // Constant Folding
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

        // Identidades
        if (op == AST_OP_MUL)
        {
            if (raiz->esquerda && raiz->esquerda->tipo == NO_NUM && raiz->esquerda->valor == 0)
                return criarNoNum(0);
            if (raiz->direita && raiz->direita->tipo == NO_NUM && raiz->direita->valor == 0)
                return criarNoNum(0);
            if (raiz->esquerda && raiz->esquerda->tipo == NO_NUM && raiz->esquerda->valor == 1)
                return raiz->direita;
            if (raiz->direita && raiz->direita->tipo == NO_NUM && raiz->direita->valor == 1)
                return raiz->esquerda;
        }
        if (op == AST_OP_ADD)
        {
            if (raiz->esquerda && raiz->esquerda->tipo == NO_NUM && raiz->esquerda->valor == 0)
                return raiz->direita;
            if (raiz->direita && raiz->direita->tipo == NO_NUM && raiz->direita->valor == 0)
                return raiz->esquerda;
        }
        if (op == AST_OP_SUB)
        {
            if (raiz->direita && raiz->direita->tipo == NO_NUM && raiz->direita->valor == 0)
                return raiz->esquerda;
        }
    }
    return raiz;
}