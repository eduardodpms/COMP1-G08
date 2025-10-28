#ifndef TABELA_H
#define TABELA_H

#include "ast.h"

typedef struct Simbolo
{
    char nome[64];
    TipoDado tipo;
    int valor_num;
    int is_constante;
    struct Simbolo *proximo;
} Simbolo;

extern Simbolo *tabela;

void inserirSimbolo(const char *nome, TipoDado tipo);
Simbolo *buscarSimbolo(const char *nome);
void liberarTabelaSimbolos();
void imprimirTabela();
TipoDado obterTipo(const char *nome);
int obterValor(const char *nome, int *ok);
void atualizarValorConstante(const char *nome, int valor);

#endif