#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif

#include <stddef.h>
#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <stdlib.h>

#include "ast.h"
#include "tabela.h"
#include "codegen.h"

extern int yylineno;
extern void report_error(int, const char *, ...);
extern int compilation_error_count;

static const char *type_to_str(TipoDado t)
{
    switch (t)
    {
    case TIPO_NUMBER:
        return "number";
    case TIPO_STRING:
        return "string";
    case TIPO_BOOLEAN:
        return "boolean";
    default:
        return "unknown";
    }
}

const char *tipoNoToString(NoTipo tipo);
TipoDado inferirTipo(NoAST *expr);
void verificarTiposAST(NoAST *raiz);

NoAST *criarNoNum(int valor)
{
    NoAST *novo = calloc(1, sizeof(NoAST));
    if (!novo)
        exit(1);
    novo->tipo = NO_NUM;
    novo->valor = valor;
    novo->linha = yylineno;
    return novo;
}
NoAST *criarNoStr(const char *texto)
{
    NoAST *novo = calloc(1, sizeof(NoAST));
    if (!novo)
        exit(1);
    novo->tipo = NO_STR;
    if (texto)
        snprintf(novo->texto, sizeof(novo->texto), "%s", texto);
    novo->linha = yylineno;
    return novo;
}
NoAST *criarNoBool(int valor)
{
    NoAST *novo = calloc(1, sizeof(NoAST));
    if (!novo)
        exit(1);
    novo->tipo = NO_BOOL;
    novo->valor = valor;
    novo->linha = yylineno;
    return novo;
}
NoAST *criarNoId(const char *nome)
{
    NoAST *novo = calloc(1, sizeof(NoAST));
    if (!novo)
        exit(1);
    novo->tipo = NO_ID;
    if (nome)
        snprintf(novo->nome, sizeof(novo->nome), "%s", nome);
    novo->linha = yylineno;
    return novo;
}
NoAST *criarNoOp(int operador, NoAST *esquerda, NoAST *direita)
{
    NoAST *novo = calloc(1, sizeof(NoAST));
    if (!novo)
        exit(1);
    novo->tipo = NO_OP;
    novo->valor = operador;
    novo->esquerda = esquerda;
    novo->direita = direita;
    novo->linha = yylineno;
    return novo;
}
NoAST *criarNoDecl(VarKind var_kind, TipoDado tipo_dado, const char *nome, NoAST *valor)
{
    NoAST *novo = calloc(1, sizeof(NoAST));
    if (!novo)
        exit(1);
    novo->tipo = NO_DECL;
    novo->decl.tipo = var_kind;
    novo->decl.tipo_dado = tipo_dado;
    if (nome)
        snprintf(novo->decl.nome, sizeof(novo->decl.nome), "%s", nome);
    novo->decl.expr = valor;
    novo->linha = yylineno;
    return novo;
}
NoAST *adicionarDeclaracao(NoAST *raiz, NoAST *declaracao)
{
    if (!raiz)
        return declaracao;
    NoAST *atual = raiz;
    while (atual->prox)
        atual = atual->prox;
    atual->prox = declaracao;
    return raiz;
}
NoAST *criarNoBlock(NoAST *firstStatement)
{
    NoAST *novo = calloc(1, sizeof(NoAST));
    if (!novo)
        exit(1);
    novo->tipo = NO_BLOCK;
    novo->body = firstStatement;
    novo->linha = yylineno;
    return novo;
}
NoAST *criarNoIf(NoAST *cond, NoAST *then_branch, NoAST *else_branch)
{
    NoAST *node = calloc(1, sizeof(NoAST));
    if (!node)
        exit(1);
    node->tipo = NO_IF;
    node->esquerda = cond;
    node->direita = then_branch;
    node->else_branch = else_branch;
    node->linha = yylineno;
    return node;
}
NoAST *criarNoWhile(NoAST *cond, NoAST *body)
{
    NoAST *node = calloc(1, sizeof(NoAST));
    if (!node)
        exit(1);
    node->tipo = NO_WHILE;
    node->esquerda = cond;
    node->body = body;
    node->linha = yylineno;
    return node;
}
NoAST *criarNoFor(NoAST *init, NoAST *cond, NoAST *update, NoAST *body)
{
    NoAST *novo = calloc(1, sizeof(NoAST));
    if (!novo)
        exit(1);
    novo->tipo = NO_FOR;
    novo->esquerda = init;
    novo->direita = cond;
    novo->update = update;
    novo->body = body;
    novo->linha = yylineno;
    return novo;
}
NoAST *criarNoBreak()
{
    NoAST *novo = calloc(1, sizeof(NoAST));
    if (!novo)
        exit(1);
    novo->tipo = NO_BREAK;
    novo->linha = yylineno;
    return novo;
}
NoAST *criarNoContinue()
{
    NoAST *novo = calloc(1, sizeof(NoAST));
    if (!novo)
        exit(1);
    novo->tipo = NO_CONTINUE;
    novo->linha = yylineno;
    return novo;
}
NoAST *criarNoCase(NoAST *caseExpr, NoAST *caseBody)
{
    NoAST *novo = calloc(1, sizeof(NoAST));
    if (!novo)
        exit(1);
    novo->tipo = NO_CASE;
    novo->esquerda = caseExpr;
    novo->body = caseBody;
    novo->linha = yylineno;
    return novo;
}
NoAST *criarNoSwitch(NoAST *expr, NoAST *cases)
{
    NoAST *novo = calloc(1, sizeof(NoAST));
    if (!novo)
        exit(1);
    novo->tipo = NO_SWITCH;
    novo->esquerda = expr;
    novo->body = cases;
    novo->linha = yylineno;
    return novo;
}
NoAST *criarNoConsoleLog(NoAST *expr)
{
    NoAST *novo = calloc(1, sizeof(NoAST));
    if (!novo)
        exit(1);
    novo->tipo = NO_CONSOLE_LOG;
    novo->esquerda = expr;
    novo->linha = yylineno;
    return novo;
}
NoAST *removerBlocosExtras(NoAST *raiz)
{
    if (!raiz)
        return NULL;
    NoAST *atual = raiz;
    while (atual)
    {
        if (atual->tipo == NO_IF && atual->prox && atual->prox->tipo == NO_BLOCK)
        {
            NoAST *proximo = atual->prox;
            if (atual->prox && atual->prox->tipo == NO_BLOCK)
            {
                atual->prox = proximo->prox;
                continue;
            }
        }
        atual = atual->prox;
    }
    return raiz;
}

TipoDado inferirTipo(NoAST *expr)
{
    if (!expr)
        return (TipoDado)-1;

    switch (expr->tipo)
    {
    case NO_NUM:
        return TIPO_NUMBER;
    case NO_STR:
        return TIPO_STRING;
    case NO_BOOL:
        return TIPO_BOOLEAN;
    case NO_ID:
        if (!buscarSimbolo(expr->nome))
        {
            // Se não achou na tabela (que agora é mantida por verificarTiposAST), erro.
            report_error(expr->linha, "Variavel '%s' nao declarada.", expr->nome);
            return (TipoDado)-1;
        }
        return obterTipo(expr->nome);

    case NO_OP:
    {
        TipoDado esq = inferirTipo(expr->esquerda);
        int op = expr->valor;

        if (op == AST_OP_INC || op == AST_OP_DEC)
        {
            if ((int)esq != -1 && esq != TIPO_NUMBER)
            {
                report_error(expr->linha, "Operador unario espera 'number', recebeu '%s'", type_to_str(esq));
            }
            return TIPO_NUMBER;
        }

        TipoDado dir = inferirTipo(expr->direita);
        if ((int)esq == -1 || (int)dir == -1)
            return (TipoDado)-1;

        if (op >= AST_OP_EQ && op <= AST_OP_GE)
            return TIPO_BOOLEAN;

        if (op == AST_OP_ADD)
        {
            if (esq == TIPO_STRING || dir == TIPO_STRING)
                return TIPO_STRING;
            if (esq == TIPO_NUMBER && dir == TIPO_NUMBER)
                return TIPO_NUMBER;
        }

        if (op == AST_OP_SUB || op == AST_OP_MUL || op == AST_OP_DIV || op == AST_OP_MOD)
        {
            if (esq != TIPO_NUMBER || dir != TIPO_NUMBER)
            {
                report_error(expr->linha, "Operacao aritmetica invalida entre '%s' e '%s'",
                             type_to_str(esq), type_to_str(dir));
            }
            return TIPO_NUMBER;
        }
        return TIPO_NUMBER;
    }
    case NO_DECL:
        return expr->decl.tipo_dado;
    default:
        return (TipoDado)-1;
    }
}

void verificarTiposAST(NoAST *raiz)
{
    if (!raiz)
        return;

    int escopo_criado = 0;
    if (raiz->tipo == NO_BLOCK || raiz->tipo == NO_FOR)
    {
        pushScope();
        escopo_criado = 1;
    }

    if (raiz->tipo == NO_DECL)
    {

        inserirSimbolo(raiz->decl.nome, raiz->decl.tipo_dado);

        if (raiz->decl.expr)
        {
            TipoDado tipoEsperado = raiz->decl.tipo_dado;
            TipoDado tipoReal = inferirTipo(raiz->decl.expr);
            if ((int)tipoReal != -1 && tipoReal != tipoEsperado)
            {
                report_error(raiz->linha,
                             "Tipos incompativeis na declaracao de '%s': esperado '%s', obteve '%s'",
                             raiz->decl.nome, type_to_str(tipoEsperado), type_to_str(tipoReal));
            }
        }
    }

    if (raiz->tipo == NO_OP && raiz->valor == AST_OP_ASSIGN)
    {
        if (raiz->esquerda && raiz->esquerda->tipo == NO_ID)
        {
            // Verifica se variável existe
            if (!buscarSimbolo(raiz->esquerda->nome))
            {
                report_error(raiz->linha, "Atribuicao a variavel nao declarada '%s'", raiz->esquerda->nome);
            }
            else
            {
                TipoDado tipoVar = obterTipo(raiz->esquerda->nome);
                TipoDado tipoValor = inferirTipo(raiz->direita);
                if ((int)tipoValor != -1 && tipoVar != tipoValor)
                {
                    report_error(raiz->linha,
                                 "Atribuicao invalida para '%s': variavel e '%s', valor e '%s'",
                                 raiz->esquerda->nome, type_to_str(tipoVar), type_to_str(tipoValor));
                }
            }
        }
    }

    // Valida operações (ex: 10 + "a")
    if (raiz->tipo == NO_OP)
        inferirTipo(raiz);

    // Se for DECL, verificamos a expressão de inicialização
    if (raiz->tipo == NO_DECL)
    {
        verificarTiposAST(raiz->decl.expr);
    }
    else
    {
        // Init do FOR define variável no escopo novo
        verificarTiposAST(raiz->esquerda);
        verificarTiposAST(raiz->direita);
        verificarTiposAST(raiz->update);
        verificarTiposAST(raiz->else_branch);

        // Body roda dentro do escopo
        verificarTiposAST(raiz->body);
    }

    // DESTRUIR ESCOPO ANTES DE IR PRO PRÓXIMO IRMÃO
    if (escopo_criado)
    {
        popScope();
    }

    // PRÓXIMO COMANDO (mesmo nível de escopo do pai)
    verificarTiposAST(raiz->prox);
}

int avaliarExpr(NoAST *expr, int *ok)
{
    if (!expr)
    {
        *ok = 0;
        return 0;
    }
    switch (expr->tipo)
    {
    case NO_NUM:
        *ok = 1;
        return expr->valor;
    case NO_BOOL:
        *ok = 1;
        return expr->valor;
    case NO_ID:
        return obterValor(expr->nome, ok);
    case NO_OP:
    {
        int ok1 = 0, ok2 = 0;
        int v1 = avaliarExpr(expr->esquerda, &ok1);
        int v2 = avaliarExpr(expr->direita, &ok2);
        if (!ok1 || !ok2)
        {
            *ok = 0;
            return 0;
        }
        *ok = 1;
        switch (expr->valor)
        {
        case AST_OP_ADD:
            return v1 + v2;
        case AST_OP_SUB:
            return v1 - v2;
        case AST_OP_MUL:
            return v1 * v2;
        case AST_OP_DIV:
            return (v2 != 0) ? v1 / v2 : 0;
        case AST_OP_MOD:
            return (v2 != 0) ? v1 % v2 : 0;
        default:
            *ok = 0;
            return 0; // Simplificado
        }
    }
    default:
        *ok = 0;
        return 0;
    }
}

static void imprimirIndentacao(int nivel)
{
    for (int i = 0; i < nivel; i++)
        printf("  ");
}

void imprimirAST_rec(NoAST *raiz, int nivel)
{
    if (!raiz)
        return;
    imprimirIndentacao(nivel);
    switch (raiz->tipo)
    {
    case NO_DECL:
        printf("DECLARACAO (%s): %s\n", raiz->decl.nome, type_to_str(raiz->decl.tipo_dado));
        if (raiz->decl.expr)
        {
            imprimirIndentacao(nivel + 1);
            printf("Inicializacao:\n");
            imprimirAST_rec(raiz->decl.expr, nivel + 2);
        }
        break;
    case NO_NUM:
        printf("NUM: %d\n", raiz->valor);
        break;
    case NO_STR:
        printf("STRING: \"%s\"\n", raiz->texto);
        break;
    case NO_BOOL:
        printf("BOOLEAN: %s\n", raiz->valor ? "true" : "false");
        break;
    case NO_ID:
        printf("ID: %s\n", raiz->nome);
        break;
    case NO_OP:
        switch (raiz->valor)
        {
        case AST_OP_ASSIGN:
            printf("OP: =\n");
            break;
        case AST_OP_ADD:
            printf("OP: +\n");
            break;
        case AST_OP_SUB:
            printf("OP: -\n");
            break;
        case AST_OP_MUL:
            printf("OP: *\n");
            break;
        case AST_OP_DIV:
            printf("OP: /\n");
            break;
        case AST_OP_MOD:
            printf("OP: %%\n");
            break;
        case AST_OP_EQ:
            printf("OP: ==\n");
            break;
        case AST_OP_NEQ:
            printf("OP: !=\n");
            break;
        case AST_OP_LT:
            printf("OP: <\n");
            break;
        case AST_OP_GT:
            printf("OP: >\n");
            break;
        case AST_OP_LE:
            printf("OP: <=\n");
            break;
        case AST_OP_GE:
            printf("OP: >=\n");
            break;
        case AST_OP_INC:
            printf("OP: ++\n");
            break;
        case AST_OP_DEC:
            printf("OP: --\n");
            break;
        case AST_OP_READ:
            printf("OP: read\n");
            break;
        default:
            printf("OP: %d\n", raiz->valor);
            break;
        }
        if (raiz->esquerda)
        {
            imprimirIndentacao(nivel + 1);
            printf("Esq:\n");
            imprimirAST_rec(raiz->esquerda, nivel + 2);
        }
        if (raiz->direita)
        {
            imprimirIndentacao(nivel + 1);
            printf("Dir:\n");
            imprimirAST_rec(raiz->direita, nivel + 2);
        }
        break;
    case NO_BLOCK:
        printf("BLOCK:\n");
        if (raiz->body)
            imprimirAST_rec(raiz->body, nivel + 1);
        break;
    case NO_IF:
        printf("IF:\n");
        imprimirAST_rec(raiz->esquerda, nivel + 1);
        printf("  THEN:\n");
        imprimirAST_rec(raiz->direita, nivel + 1);
        if (raiz->else_branch)
        {
            printf("  ELSE:\n");
            imprimirAST_rec(raiz->else_branch, nivel + 1);
        }
        break;
    case NO_WHILE:
        printf("WHILE:\n");
        imprimirAST_rec(raiz->esquerda, nivel + 1);
        imprimirAST_rec(raiz->body, nivel + 1);
        break;
    case NO_FOR:
        printf("FOR:\n");
        if (raiz->esquerda)
        {
            imprimirIndentacao(nivel + 1);
            printf("Init:\n");
            imprimirAST_rec(raiz->esquerda, nivel + 2);
        }
        if (raiz->direita)
        {
            imprimirIndentacao(nivel + 1);
            printf("Cond:\n");
            imprimirAST_rec(raiz->direita, nivel + 2);
        }
        if (raiz->update)
        {
            imprimirIndentacao(nivel + 1);
            printf("Update:\n");
            imprimirAST_rec(raiz->update, nivel + 2);
        }
        imprimirAST_rec(raiz->body, nivel + 1);
        break;
    case NO_BREAK:
        printf("BREAK\n");
        break;
    case NO_CONTINUE:
        printf("CONTINUE\n");
        break;
    case NO_SWITCH:
        printf("SWITCH:\n");
        imprimirAST_rec(raiz->esquerda, nivel + 1);
        imprimirAST_rec(raiz->body, nivel + 1);
        break;
    case NO_CASE:
        if (raiz->esquerda)
        {
            printf("CASE:\n");
            imprimirAST_rec(raiz->esquerda, nivel + 1);
        }
        else
        {
            printf("DEFAULT:\n");
        }
        imprimirAST_rec(raiz->body, nivel + 1);
        break;
    case NO_CONSOLE_LOG:
        printf("CONSOLE_LOG:\n");
        imprimirAST_rec(raiz->esquerda, nivel + 1);
        break;
    default:
        printf("(NO DESCONHECIDO: %d)\n", raiz->tipo);
    }
    if (raiz->prox)
        imprimirAST_rec(raiz->prox, nivel);
}

void imprimirAST(NoAST *raiz) { imprimirAST_rec(raiz, 0); }

void ast_free(NoAST *node)
{
    if (!node)
        return;
    switch (node->tipo)
    {
    case NO_IF:
        ast_free(node->esquerda);
        ast_free(node->direita);
        ast_free(node->else_branch);
        break;
    case NO_FOR:
        ast_free(node->esquerda);
        ast_free(node->direita);
        ast_free(node->update);
        ast_free(node->body);
        break;
    case NO_OP:
        ast_free(node->esquerda);
        ast_free(node->direita);
        break;
    case NO_DECL:
        ast_free(node->decl.expr);
        break;
    case NO_BLOCK:
    case NO_WHILE:
    case NO_SWITCH:
    case NO_CASE:
        ast_free(node->esquerda);
        ast_free(node->body);
        break;
    case NO_CONSOLE_LOG:
        ast_free(node->esquerda);
        break;
    default:
        break;
    }
    ast_free(node->prox);
    free(node);
}

/* ast.c (adicionar depois de ast_free) */

int astIsStringLiteral(NoAST *n)
{
    return n != NULL && n->tipo == NO_STR;
}

int astIsNumberLiteral(NoAST *n)
{
    return n != NULL && n->tipo == NO_NUM;
}

int astIsBoolLiteral(NoAST *n)
{
    return n != NULL && n->tipo == NO_BOOL;
}

/* Wrapper semântico para liberar um nó AST (chama ast_free já existente) */
void liberarNoAST(NoAST *n)
{
    ast_free(n);
}

