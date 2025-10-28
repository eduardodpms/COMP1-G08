#ifndef TABELA_H
#define TABELA_H

#include "ast.h"

typedef struct Simbolo
{
    char nome[64];
    TipoDado tipo;
    struct Simbolo *proximo;
} Simbolo;

extern Simbolo *tabela;

void inserirSimbolo(const char *nome, const char *tipo);
Simbolo *buscarSimbolo(const char *nome);
void liberarTabelaSimbolos();
void imprimirTabela();
TipoDado obterTipo(const char *nome);

#endif