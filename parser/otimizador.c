#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ast.h"
#include "otimizador.h"

#define MAX_REPEAT_MULT 16


static NoAST *cloneAST(NoAST *n);
static NoAST *criarSomaRepetida(NoAST *expr, int k);

static NoAST *cloneAST(NoAST *n) {
    if (!n) return NULL;

    NoAST *c = malloc(sizeof(NoAST));
    if (!c) { perror("malloc"); exit(1); }

    memcpy(c, n, sizeof(NoAST));

    c->prox  = NULL;
    c->body  = NULL;

    switch (n->tipo) {
        case NO_NUM:
        case NO_BOOL:
        case NO_ID:
        case NO_STR:
            c->esquerda = NULL;
            c->direita  = NULL;
            break;

        case NO_OP:
            c->esquerda = cloneAST(n->esquerda);
            c->direita  = cloneAST(n->direita);
            break;

        case NO_DECL:
            c->decl.expr = cloneAST(n->decl.expr);
            break;

        case NO_BLOCK:
            c->body = cloneAST(n->body);
            break;

        case NO_IF:
            c->esquerda    = cloneAST(n->esquerda);
            c->direita     = cloneAST(n->direita);
            c->else_branch = cloneAST(n->else_branch);
            break;

        case NO_WHILE:
            c->esquerda = cloneAST(n->esquerda);
            c->body     = cloneAST(n->body);
            break;

        case NO_FOR:
            c->esquerda = cloneAST(n->esquerda);
            c->direita  = cloneAST(n->direita);
            c->prox     = cloneAST(n->prox);
            c->body     = cloneAST(n->body);
            break;

        case NO_SWITCH:
            c->esquerda = cloneAST(n->esquerda);
            c->body     = cloneAST(n->body);
            break;

        case NO_CASE:
            c->esquerda = cloneAST(n->esquerda);
            c->body     = cloneAST(n->body);
            break;

        case NO_CONSOLE_LOG:
            c->esquerda = cloneAST(n->esquerda);
            break;

        default:
            c->esquerda = cloneAST(n->esquerda);
            c->direita  = cloneAST(n->direita);
            break;
    }

    return c;
}

static NoAST *criarSomaRepetida(NoAST *expr, int k) {
    if (k <= 0) return criarNoNum(0);
    if (k == 1) return cloneAST(expr);
    if (k == 2) return criarNoOp('+', cloneAST(expr), cloneAST(expr));

    NoAST *res = criarNoOp('+', cloneAST(expr), cloneAST(expr));
    for (int i = 3; i <= k; i++)
        res = criarNoOp('+', res, cloneAST(expr));

    return res;
}

NoAST *otimizarStrengthReduction(NoAST *raiz) {
    if (!raiz) return NULL;

    raiz->esquerda = otimizarStrengthReduction(raiz->esquerda);
    raiz->direita  = otimizarStrengthReduction(raiz->direita);
    raiz->prox     = otimizarStrengthReduction(raiz->prox);

    if (raiz->tipo == NO_DECL)
        raiz->decl.expr = otimizarStrengthReduction(raiz->decl.expr);

    if (raiz->tipo == NO_BLOCK)
        raiz->body = otimizarStrengthReduction(raiz->body);

    if (raiz->tipo == NO_OP) {
        int op = raiz->valor;

        if (raiz->esquerda && raiz->direita &&
            raiz->esquerda->tipo == NO_NUM && raiz->direita->tipo == NO_NUM) {

            int a = raiz->esquerda->valor;
            int b = raiz->direita->valor;

            switch (op) {
                case '+': return criarNoNum(a + b);
                case '-': return criarNoNum(a - b);
                case '*': return criarNoNum(a * b);
                case '/': return criarNoNum(b ? a / b : 0);
            }
        }

        if (op == '*') {
            if (raiz->esquerda && raiz->esquerda->tipo == NO_NUM) {
                int k = raiz->esquerda->valor;
                if (k == 0) return criarNoNum(0);
                if (k == 1) return raiz->direita;
                if (k >= 2 && k <= MAX_REPEAT_MULT)
                    return criarSomaRepetida(raiz->direita, k);
            }
            if (raiz->direita && raiz->direita->tipo == NO_NUM) {
                int k = raiz->direita->valor;
                if (k == 0) return criarNoNum(0);
                if (k == 1) return raiz->esquerda;
                if (k >= 2 && k <= MAX_REPEAT_MULT)
                    return criarSomaRepetida(raiz->esquerda, k);
            }
        }

        if (op == '+') {
            if (raiz->esquerda && raiz->esquerda->tipo == NO_NUM && raiz->esquerda->valor == 0)
                return raiz->direita;
            if (raiz->direita && raiz->direita->tipo == NO_NUM && raiz->direita->valor == 0)
                return raiz->esquerda;
        }

        if (op == '-') {
            if (raiz->direita && raiz->direita->tipo == NO_NUM && raiz->direita->valor == 0)
                return raiz->esquerda;
        }

        if (op == '/') {
            if (raiz->direita && raiz->direita->tipo == NO_NUM && raiz->direita->valor == 1)
                return raiz->esquerda;
        }
    }

    return raiz;
}
