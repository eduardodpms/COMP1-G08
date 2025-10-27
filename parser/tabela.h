#ifndef TABELA_H
#define TABELA_H

typedef struct Simbolo
{
    char nome[64];
    char tipo[16];
    struct Simbolo *proximo;
} Simbolo;

extern Simbolo *tabela;

void inserirSimbolo(const char *nome, const char *tipo);
Simbolo *buscarSimbolo(const char *nome);
void liberarTabelaSimbolos();
void imprimirTabela();

#endif