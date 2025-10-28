#include "ast.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "tabela.h"

extern int yylineno;                              // linha atual do parser
extern void report_error(int, const char *, ...); // declarada no parser.y

const char *tipoNoToString(NoTipo tipo)
{
    switch (tipo)
    {
    case NO_DECL:
        return "NO_DECL";
    case NO_NUM:
        return "NO_NUM";
    case NO_STR:
        return "NO_STR";
    case NO_BOOL:
        return "NO_BOOL";
    case NO_ID:
        return "NO_ID";
    case NO_OP:
        return "NO_OP";
    default:
        return "NO_DESCONHECIDO";
    }
}

// Função para criar um nó de número
NoAST *criarNoNum(int valor)
{
    NoAST *novo = malloc(sizeof(NoAST));
    novo->tipo = NO_NUM;
    novo->valor = valor;
    novo->esquerda = novo->direita = NULL;
    novo->linha = yylineno;
    return novo;
}
// Função para criar um nó string
NoAST *criarNoStr(const char *texto)
{
    NoAST *novo = malloc(sizeof(NoAST));
    novo->tipo = NO_STR;
    strncpy(novo->texto, texto, sizeof(novo->texto));
    novo->texto[sizeof(novo->texto) - 1] = '\0'; // Garantir terminação nula
    novo->esquerda = novo->direita = NULL;
    novo->linha = yylineno;
    return novo;
}

// Função para criar um nó booleano
NoAST *criarNoBool(int valor)
{
    NoAST *novo = malloc(sizeof(NoAST));
    novo->tipo = NO_BOOL;
    novo->valor = valor;
    novo->esquerda = novo->direita = NULL;
    novo->linha = yylineno;
    return novo;
}

// Função para criar um nó identificador
NoAST *criarNoId(const char *nome)
{
    if (!buscarSimbolo(nome))
    {
        int line = (yylineno > 0) ? yylineno : 1;
        report_error(line, "Uso de variável '%s' não declarada.", nome);
    }
    NoAST *novo = malloc(sizeof(NoAST));
    novo->tipo = NO_ID;
    strcpy(novo->nome, nome);
    novo->esquerda = novo->direita = NULL;
    novo->linha = yylineno;
    return novo;
}

// Função para criar um nó de declaração
NoAST *criarNoOp(char operador, NoAST *esquerda, NoAST *direita)
{
    NoAST *novo = malloc(sizeof(NoAST));
    novo->tipo = NO_OP;
    novo->valor = operador;
    novo->esquerda = esquerda;
    novo->direita = direita;
    novo->linha = yylineno;
    return novo;
}

// Função para criar um nó de declaração
NoAST *criarNoDecl(VarKind var_kind, TipoDado tipo_dado, const char *nome, NoAST *valor)
{
    NoAST *novo = malloc(sizeof(NoAST));
    novo->tipo = NO_DECL;
    novo->decl.tipo = var_kind;
    novo->decl.tipo_dado = tipo_dado;
    strncpy(novo->decl.nome, nome, sizeof(novo->decl.nome) - 1);
    novo->decl.nome[sizeof(novo->decl.nome) - 1] = '\0';

    novo->decl.expr = valor; // <-- aqui guardamos o nó AST inteiro

    novo->esquerda = novo->direita = NULL;
    novo->linha = yylineno;
    return novo;
}

NoAST *adicionarDeclaracao(NoAST *raiz, NoAST *declaracao)
{
    if (!raiz)
        return declaracao;

    NoAST *atual = raiz;
    while (atual->direita)
    {
        atual = atual->direita;
    }
    atual->direita = declaracao;
    return raiz;
}

TipoDado inferirTipo(NoAST *expr)
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
    {
        TipoDado tipo = obterTipo(expr->nome);
        if (tipo == -1)
            report_error(yylineno, "Uso de variável '%s' não declarada", expr->nome);
        return tipo;
    }
    case NO_OP:
    {
        TipoDado esq = inferirTipo(expr->esquerda);
        TipoDado dir = inferirTipo(expr->direita);
        if (esq != TIPO_NUMBER || dir != TIPO_NUMBER)
            report_error(yylineno, "Operação inválida entre tipos diferentes");
        return TIPO_NUMBER;
    }
    case NO_DECL:
        return expr->decl.tipo_dado;
    default:
        return -1;
    }
}

void verificarTiposAST(NoAST *raiz)
{
    if (!raiz)
        return;

    if (raiz->tipo == NO_DECL)
    {
        TipoDado tipo_expr = inferirTipo(raiz->decl.expr);
        if (tipo_expr != -1 && tipo_expr != raiz->decl.tipo_dado)
            report_error(yylineno, "Atribuição inválida: variável '%s' recebe tipo diferente do declarado", raiz->decl.nome);

        verificarTiposAST(raiz->decl.expr);
    }

    verificarTiposAST(raiz->esquerda);
    verificarTiposAST(raiz->direita);
}

TipoDado verificarTipo(NoAST *raiz)
{
    if (!raiz)
        return -1;

    switch (raiz->tipo)
    {
    case NO_NUM:
        return TIPO_NUMBER;
    case NO_BOOL:
        return TIPO_BOOLEAN;
    case NO_STR:
        return TIPO_STRING;
    case NO_ID:
    {
        Simbolo *s = buscarSimbolo(raiz->nome);
        if (!s)
        {
            report_error(raiz->linha, "Uso de variável '%s' não declarada.", raiz->nome);
            return -1;
        }
        return s->tipo;
    }
    case NO_OP:
    {
        TipoDado t1 = verificarTipo(raiz->esquerda);
        TipoDado t2 = verificarTipo(raiz->direita);
        if (t1 != t2)
        {
            report_error(raiz->linha, "Operação inválida entre tipos diferentes");
            return -1;
        }
        return t1; // tipo do resultado
    }
    case NO_DECL:
        return raiz->decl.tipo_dado;
    default:
        return -1;
    }
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
    {
        int sucesso = 0;
        int valor = obterValor(expr->nome, &sucesso);
        *ok = sucesso;
        return valor;
    }

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
        case '+':
            return v1 + v2;
        case '-':
            return v1 - v2;
        case '*':
            return v1 * v2;
        case '/':
            return v2 != 0 ? v1 / v2 : 0;
        default:
            *ok = 0;
            return 0;
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
        printf("DECLARACAO (%s):\n", raiz->decl.nome);
        imprimirIndentacao(nivel + 1);
        printf("Tipo variavel: %s\n",
               raiz->decl.tipo == VK_LET ? "let" : raiz->decl.tipo == VK_CONST ? "const"
                                                                               : "var");
        imprimirIndentacao(nivel + 1);
        printf("Tipo dado: %s\n",
               raiz->decl.tipo_dado == TIPO_NUMBER ? "number" : raiz->decl.tipo_dado == TIPO_STRING ? "string"
                                                                                                    : "boolean");
        // Expressão de inicialização
        if (raiz->decl.expr)
        {
            imprimirIndentacao(nivel + 1);
            printf("Expressao inicializacao:\n");
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
        printf("OP: %c\n", (char)raiz->valor);
        if (raiz->esquerda)
        {
            imprimirIndentacao(nivel + 1);
            printf("Esquerda:\n");
            imprimirAST_rec(raiz->esquerda, nivel + 2);
        }
        if (raiz->direita)
        {
            imprimirIndentacao(nivel + 1);
            printf("Direita:\n");
            imprimirAST_rec(raiz->direita, nivel + 2);
        }
        break;

    default:
        printf("(NO DESCONHECIDO)\n");
    }

    // Próxima declaração encadeada
    if (raiz->direita)
        imprimirAST_rec(raiz->direita, nivel);
}

void imprimirAST(NoAST *raiz)
{
    imprimirAST_rec(raiz, 0);
}
