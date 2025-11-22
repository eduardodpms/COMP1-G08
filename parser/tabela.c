#include "tabela.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

extern int yylineno;
extern void report_error(int, const char *, ...);

typedef struct ScopeFrame {
    Simbolo *head;
    struct ScopeFrame *next;
} ScopeFrame;

Simbolo *tabela = NULL;

static ScopeFrame *scope_stack = NULL;

void pushScope() {
    ScopeFrame *f = malloc(sizeof(ScopeFrame));
    if (!f) { perror("malloc"); exit(1); }
    f->head = NULL;
    f->next = scope_stack;
    scope_stack = f;
}

void popScope() {
    if (!scope_stack) return;
    Simbolo *s = scope_stack->head;
    while (s) {
        Simbolo *n = s->proximo;
        free(s);
        s = n;
    }
    ScopeFrame *old = scope_stack;
    scope_stack = old->next;
    free(old);
}

void inserirSimbolo(const char *nome, TipoDado tipo)
{
    if (!scope_stack) pushScope(); /* ensure there is at least a global scope */
    /* check if symbol already exists in current scope (shadowing detection) */
    for (Simbolo *it = scope_stack->head; it; it = it->proximo) {
        if (strcmp(it->nome, nome) == 0) {
            int line = (yylineno > 0) ? yylineno : 1;
            report_error(line, "Redeclaração da variável '%s' no mesmo escopo.", nome);
            return;
        }
    }

    Simbolo *s = malloc(sizeof(Simbolo));
    if (!s) { perror("malloc"); exit(1); }
    strncpy(s->nome, nome, sizeof(s->nome) - 1);
    s->nome[sizeof(s->nome)-1] = '\0';
    s->tipo = tipo;
    s->valor_num = 0;
    s->is_constante = 0;
    s->proximo = scope_stack->head;
    scope_stack->head = s;
}


// Buscar símbolo
Simbolo *buscarSimbolo(const char *nome)
{
    for (ScopeFrame *f = scope_stack; f; f = f->next) {
        for (Simbolo *s = f->head; s; s = s->proximo) {
            if (strcmp(s->nome, nome) == 0)
                return s;
        }
    }
    return NULL;
}

void imprimirTabela()
{
    printf("\nTabela de Símbolos:\n");
    int level = 0;
    for (ScopeFrame *f = scope_stack; f; f = f->next) {
        printf(" Scope level %d:\n", level++);
        for (Simbolo *s = f->head; s; s = s->proximo) {
            const char *tipo_str = "";
            switch (s->tipo)
            {
            case TIPO_NUMBER:
                tipo_str = "number";
                break;
            case TIPO_STRING:
                tipo_str = "string";
                break;
            case TIPO_BOOLEAN:
                tipo_str = "boolean";
                break;
            }
            printf("  Nome: %s, Tipo: %s", s->nome, tipo_str);
            if (s->tipo == TIPO_NUMBER && s->is_constante) {
                printf(", Valor Constante: %d", s->valor_num);
            }
            printf("\n");
        }
    }
}

void liberarTabelaSimbolos()
{
    while (scope_stack) popScope();
}


TipoDado obterTipo(const char *nome)
{
    Simbolo *s = buscarSimbolo(nome);
    if (s)
        return s->tipo;
    return TIPO_NUMBER; // não encontrado
}

int obterValor(const char *nome, int *ok)
{
    Simbolo *s = buscarSimbolo(nome);
    if (s && s->is_constante && s->tipo == TIPO_NUMBER)
    {
        *ok = 1;
        return s->valor_num;
    }
    *ok = 0;
    return 0;
}

void atualizarValorConstante(const char *nome, int valor)
{
    Simbolo *s = buscarSimbolo(nome);
    if (s && s->tipo == TIPO_NUMBER)
    {
        s->valor_num = valor;
        s->is_constante = 1;
    }
}