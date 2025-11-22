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
    if (!novo) { perror("malloc"); exit(1); }
    novo->tipo = NO_NUM;
    novo->valor = valor;
    novo->esquerda = novo->direita = NULL;
    novo->prox = NULL;
    novo->linha = yylineno;
    return novo;
}
// Função para criar um nó string
NoAST *criarNoStr(const char *texto)
{
    NoAST *novo = malloc(sizeof(NoAST));
    if (!novo) { perror("malloc"); exit(1); }
    novo->tipo = NO_STR;
    if (texto) {
        strncpy(novo->texto, texto, sizeof(novo->texto) - 1);
        novo->texto[sizeof(novo->texto) - 1] = '\0'; // Garantir terminação nula
    } else {
        novo->texto[0] = '\0'; // Garantir terminação nula
    }
    novo->esquerda = novo->direita = NULL;
    novo->prox = NULL;
    novo->linha = yylineno;
    return novo;
}

// Função para criar um nó booleano
NoAST *criarNoBool(int valor)
{
    NoAST *novo = malloc(sizeof(NoAST));
    if (!novo) { perror("malloc"); exit(1); }
    novo->tipo = NO_BOOL;
    novo->valor = valor;
    novo->esquerda = novo->direita = NULL;
    novo->prox = NULL;
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
    if (!novo) { perror("malloc"); exit(1); }
    novo->tipo = NO_ID;
    if (nome) {
        strncpy(novo->nome, nome, sizeof(novo->nome) - 1);
        novo->nome[sizeof(novo->nome) - 1] = '\0';
    } else {
        novo->nome[0] = '\0';
    }
    novo->esquerda = novo->direita = NULL;
    novo->prox = NULL;
    novo->linha = yylineno;
    return novo;
}

// Função para criar um nó de declaração
NoAST *criarNoOp(int operador, NoAST *esquerda, NoAST *direita)
{
    NoAST *novo = malloc(sizeof(NoAST));
    if (!novo) { perror("malloc"); exit(1); }
    novo->tipo = NO_OP;
    novo->valor = operador;
    novo->esquerda = esquerda;
    novo->direita = direita;
    novo->prox = NULL;
    novo->linha = yylineno;
    return novo;
}

// Função para criar um nó de declaração
NoAST *criarNoDecl(VarKind var_kind, TipoDado tipo_dado, const char *nome, NoAST *valor)
{
    NoAST *novo = malloc(sizeof(NoAST));
    if (!novo) { perror("malloc"); exit(1); }
    novo->tipo = NO_DECL;
    novo->decl.tipo = var_kind;
    novo->decl.tipo_dado = tipo_dado;
    if (nome) {
        strncpy(novo->decl.nome, nome, sizeof(novo->decl.nome) - 1);
        novo->decl.nome[sizeof(novo->decl.nome) - 1] = '\0';
    } else {
        novo->decl.nome[0] = '\0';
    }

    novo->decl.expr = valor; // <-- aqui guardamos o nó AST inteiro

    novo->esquerda = novo->direita = NULL;
    novo->prox = NULL;
    novo->linha = yylineno;
    return novo;
}

NoAST *adicionarDeclaracao(NoAST *raiz, NoAST *declaracao)
{
    if (!raiz)
        return declaracao;

    NoAST *atual = raiz;
    while (atual->prox)
    {
        atual = atual->prox;
    }
    atual->prox = declaracao;
    return raiz;
}

NoAST *criarNoBlock(NoAST *firstStatement)
{
    NoAST *novo = malloc(sizeof(NoAST));
    if (!novo) { perror("malloc"); exit(1); }
    novo->tipo = NO_BLOCK;
    novo->esquerda = novo->direita = NULL;
    novo->prox = NULL;
    novo->body = firstStatement; // primeiro statement do bloco (pode ser NULL)
    novo->linha = yylineno;
    return novo;
}

NoAST *criarNoIf(NoAST *cond, NoAST *then_branch, NoAST *else_branch) {
    NoAST *node = (NoAST*)malloc(sizeof(NoAST));
    node->tipo = NO_IF;
    node->esquerda = cond;
    node->direita = then_branch;
    node->else_branch = else_branch;  
    node->prox = NULL;  
    return node;
}

NoAST *criarNoWhile(NoAST *cond, NoAST *body) {
    NoAST *node = (NoAST*)malloc(sizeof(NoAST));
    node->tipo = NO_WHILE;
    node->esquerda = cond;    
    node->body = body;       
    node->prox = NULL;
    node->linha = yylineno;
    return node;
}

NoAST *criarNoFor(NoAST *init, NoAST *cond, NoAST *update, NoAST *body)
{
    NoAST *novo = malloc(sizeof(NoAST));
    if (!novo) { perror("malloc"); exit(1); }
    novo->tipo = NO_FOR;
    novo->esquerda = init;
    novo->direita = cond;
    novo->prox = update;
    novo->body = body;
    novo->linha = yylineno;
    return novo;
}

NoAST *criarNoBreak()
{
    NoAST *novo = malloc(sizeof(NoAST));
    if (!novo) { perror("malloc"); exit(1); }
    novo->tipo = NO_BREAK;
    novo->esquerda = novo->direita = novo->prox = novo->body = NULL;
    novo->linha = yylineno;
    return novo;
}

NoAST *criarNoContinue()
{
    NoAST *novo = malloc(sizeof(NoAST));
    if (!novo) { perror("malloc"); exit(1); }
    novo->tipo = NO_CONTINUE;
    novo->esquerda = novo->direita = novo->prox = novo->body = NULL;
    novo->linha = yylineno;
    return novo;
}

NoAST *criarNoCase(NoAST *caseExpr, NoAST *caseBody)
{
    NoAST *novo = malloc(sizeof(NoAST));
    if (!novo) { perror("malloc"); exit(1); }
    novo->tipo = NO_CASE;
    novo->esquerda = caseExpr;
    novo->direita = NULL;
    novo->prox = NULL; // next case
    novo->body = caseBody;
    novo->linha = yylineno;
    return novo;
}

NoAST *criarNoSwitch(NoAST *expr, NoAST *cases)
{
    NoAST *novo = malloc(sizeof(NoAST));
    if (!novo) { perror("malloc"); exit(1); }
    novo->tipo = NO_SWITCH;
    novo->esquerda = expr;
    novo->direita = NULL;
    novo->prox = NULL;
    novo->body = cases;
    novo->linha = yylineno;
    return novo;
}

NoAST *criarNoConsoleLog(NoAST *expr) {
    NoAST *novo = malloc(sizeof(NoAST));
    if (!novo) { perror("malloc"); exit(1); }
    novo->tipo = NO_CONSOLE_LOG;
    novo->esquerda = expr;
    novo->direita = NULL;
    novo->prox = NULL;
    novo->body = NULL;
    novo->linha = yylineno;
    return novo;
}

NoAST *removerBlocosExtras(NoAST *raiz) {
    if (!raiz) return NULL;
    
    NoAST *atual = raiz;
    NoAST *anterior = NULL;
    
    while (atual) {
        if (atual->tipo == NO_IF && atual->prox && atual->prox->tipo == NO_BLOCK) {
            NoAST *proximo = atual->prox;
            
            if (atual->prox && atual->prox->tipo == NO_BLOCK) {
                atual->prox = proximo->prox;
                
                continue;
            }
        }
        
        anterior = atual;
        atual = atual->prox;
    }
    
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
        if (tipo == (TipoDado)-1)
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
        if (tipo_expr != (TipoDado)-1 && tipo_expr != raiz->decl.tipo_dado)
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
    switch(raiz->valor) {
        case OP_ASSIGN: printf("OP: =\n"); break;
        case OP_EQ: printf("OP: ==\n"); break;
        case OP_NEQ: printf("OP: !=\n"); break;
        case OP_LT: printf("OP: <\n"); break;
        case OP_GT: printf("OP: >\n"); break;
        case OP_LE: printf("OP: <=\n"); break;
        case OP_GE: printf("OP: >=\n"); break;
        case OP_INCREMENT: printf("OP: ++\\n"); break;
        case OP_DECREMENT: printf("OP: --\\n"); break;
        default: printf("OP: %c\n", (char)raiz->valor); break;
    }
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

    case NO_BLOCK:
        printf("BLOCK:\n");
        if (raiz->body)
        {
            imprimirIndentacao(nivel + 1);
            printf("Statements:\n");
            imprimirAST_rec(raiz->body, nivel + 2);
        }
        break;

    case NO_IF:
        printf("IF (line %d):\n", raiz->linha);
        printf("  Cond:\n    ");
        imprimirAST(raiz->esquerda);
        printf("  Then:\n    ");
        imprimirAST(raiz->direita);
        if (raiz->else_branch) {
            printf("  Else:\n    ");
            imprimirAST(raiz->else_branch);
    }
    break;

    case NO_WHILE:
        printf("WHILE (line %d):\n", raiz->linha);
        if (raiz->esquerda) {
            imprimirIndentacao(nivel + 1); printf("Cond:\n");
            imprimirAST_rec(raiz->esquerda, nivel + 2);
        }
        if (raiz->body) {
            imprimirIndentacao(nivel + 1); printf("Body:\n");
            imprimirAST_rec(raiz->body, nivel + 2);
        }
        break;

    case NO_FOR:
        printf("FOR (line %d):\n", raiz->linha);
        if (raiz->esquerda) {
            imprimirIndentacao(nivel + 1); printf("Init:\n");
            imprimirAST_rec(raiz->esquerda, nivel + 2);
        }
        if (raiz->direita) {
            imprimirIndentacao(nivel + 1); printf("Cond:\n");
            imprimirAST_rec(raiz->direita, nivel + 2);
        }
        if (raiz->prox) {
            imprimirIndentacao(nivel + 1); printf("Update:\n");
            imprimirAST_rec(raiz->prox, nivel + 2);
        }
        if (raiz->body) {
            imprimirIndentacao(nivel + 1); printf("Body:\n");
            imprimirAST_rec(raiz->body, nivel + 2);
        }
        break;

    case NO_BREAK:
        printf("BREAK\n");
        break;
    case NO_CONTINUE:
        printf("CONTINUE\n");
        break;

    case NO_SWITCH:
        printf("SWITCH (line %d):\n", raiz->linha);
        if (raiz->esquerda) {
            imprimirIndentacao(nivel + 1); printf("Expr:\n");
            imprimirAST_rec(raiz->esquerda, nivel + 2);
        }
        if (raiz->body) {
            imprimirIndentacao(nivel + 1); printf("Cases:\n");
            imprimirAST_rec(raiz->body, nivel + 2);
        }
        break;

    case NO_CASE:
        if (raiz->esquerda) {
            printf("CASE:\n");
            imprimirIndentacao(nivel + 1); printf("Value:\n");
            imprimirAST_rec(raiz->esquerda, nivel + 2);
        } else {
            printf("DEFAULT:\n");
        }
        if (raiz->body) {
            imprimirIndentacao(nivel + 1); printf("Body:\n");
            imprimirAST_rec(raiz->body, nivel + 2);
        }
        break;


    case NO_CONSOLE_LOG:
        printf("CONSOLE_LOG:\n");
        if (raiz->esquerda) {
            imprimirIndentacao(nivel + 1);
            printf("Expression:\n");
            imprimirAST_rec(raiz->esquerda, nivel + 2);
        }
        break;

    default:
        printf("(NO DESCONHECIDO)\n");
    }

    // Próxima declaração encadeada
    if (raiz->prox)
        imprimirAST_rec(raiz->prox, nivel);
}

void imprimirAST(NoAST *raiz)
{
    imprimirAST_rec(raiz, 0);
}

void ast_free(NoAST *node)
{
    if (!node) return;

    // Liberar filhos específicos do nó
    switch (node->tipo) {
        case NO_IF:
            ast_free(node->esquerda);      // condição
            ast_free(node->direita);       // then branch
            ast_free(node->else_branch);   // else branch (NOVO)
            break;
        case NO_OP:
            ast_free(node->esquerda);
            ast_free(node->direita);
            break;
        case NO_DECL:
            ast_free(node->decl.expr);     // expressão de inicialização
            break;
        case NO_BLOCK:
        case NO_SWITCH:
        case NO_WHILE:
            ast_free(node->esquerda);  
            ast_free(node->body);      
        break;        
        case NO_FOR:
            ast_free(node->body);          // corpo do bloco/loop
            break;
        case NO_CASE:
            ast_free(node->esquerda);      // expressão do case
            ast_free(node->body);          // corpo do case
            break;
        case NO_CONSOLE_LOG:
            ast_free(node->esquerda);      // expressão do console.log
            break;
        default:
            break;
    }
    
    // Liberar próximo nó na lista
    ast_free(node->prox);
    free(node);
}
