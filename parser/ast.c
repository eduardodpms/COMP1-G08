#include "ast.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

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
    return novo;
}

// Função para criar um nó booleano
NoAST *criarNoBool(int valor)
{
    NoAST *novo = malloc(sizeof(NoAST));
    novo->tipo = NO_BOOL;
    novo->valor = valor;
    novo->esquerda = novo->direita = NULL;
    return novo;
}

// Função para criar um nó identificador
NoAST *criarNoId(const char *nome)
{
    NoAST *novo = malloc(sizeof(NoAST));
    novo->tipo = NO_ID;
    strcpy(novo->nome, nome);
    novo->esquerda = novo->direita = NULL;
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
    return novo;
}

// Função para criar um nó de declaração
NoAST *criarNoDecl(VarKind var_kind, TipoDado tipo_dado, const char *nome, NoAST *valor)
{
    NoAST *novo = malloc(sizeof(NoAST));
    novo->tipo = NO_DECL;
    novo->decl.tipo = var_kind;
    novo->decl.tipo_dado = tipo_dado;
    strncpy(novo->decl.nome, nome, sizeof(novo->decl.nome));
    novo->decl.nome[sizeof(novo->decl.nome) - 1] = '\0';

    switch (tipo_dado)
    {
    case TIPO_NUMBER:
        novo->decl.valor_num = valor->valor;
        break;
    case TIPO_STRING:
        strncpy(novo->decl.valor_str, valor->texto, sizeof(novo->decl.valor_str));
        novo->decl.valor_str[sizeof(novo->decl.valor_str) - 1] = '\0';
        break;
    case TIPO_BOOLEAN:
        novo->decl.valor_bool = valor->valor;
        break;
    }

    novo->esquerda = novo->direita = NULL;
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

static void imprimirIndentacao(int nivel)
{
    for (int i = 0; i < nivel; i++)
        printf("  ");
}

void imprimirAST_rec(NoAST *raiz, int nivel)
{
    if (!raiz)
        return;

    // Indentação
    for (int i = 0; i < nivel; i++)
        printf("  ");

    // Imprime o nó atual
    switch (raiz->tipo)
    {
    case NO_DECL:
        if (raiz->decl.tipo == VK_CONST)
            printf("const ");
        else if (raiz->decl.tipo == VK_LET)
            printf("let ");
        else
            printf("var ");

        switch (raiz->decl.tipo_dado)
        {
        case TIPO_NUMBER:
            printf("%s: number = %d\n", raiz->decl.nome, raiz->decl.valor_num);
            break;
        case TIPO_STRING:
            printf("%s: string = \"%s\"\n", raiz->decl.nome, raiz->decl.valor_str);
            break;
        case TIPO_BOOLEAN:
            printf("%s: boolean = %s\n", raiz->decl.nome, raiz->decl.valor_bool ? "true" : "false");
            break;
        }
        break;

    case NO_NUM:
        printf("Número: %d\n", raiz->valor);
        break;
    case NO_STR:
        printf("String: \"%s\"\n", raiz->texto);
        break;
    case NO_BOOL:
        printf("Boolean: %s\n", raiz->valor ? "true" : "false");
        break;
    case NO_ID:
        printf("Identificador: %s\n", raiz->nome);
        break;
    case NO_OP:
        printf("Operador: %c\n", (char)raiz->valor);
        imprimirAST_rec(raiz->esquerda, nivel + 1);
        imprimirAST_rec(raiz->direita, nivel + 1);
        break;
    default:
        printf("(nó desconhecido)\n");
    }

    // Percorre a próxima declaração encadeada
    if (raiz->direita)
        imprimirAST_rec(raiz->direita, nivel);
}

void imprimirAST(NoAST *raiz)
{
    imprimirAST_rec(raiz, 0);
}
