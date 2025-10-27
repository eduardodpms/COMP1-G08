#include "tabela.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

Simbolo *tabela = NULL;

void inserirSimbolo(const char *nome, const char *tipo)
{
    // evitar duplicatas
    if (buscarSimbolo(nome))
        return;

    Simbolo *novo = malloc(sizeof(Simbolo));
    strncpy(novo->nome, nome, sizeof(novo->nome) - 1);
    novo->nome[sizeof(novo->nome) - 1] = '\0';
    strncpy(novo->tipo, tipo, sizeof(novo->tipo) - 1);
    novo->tipo[sizeof(novo->tipo) - 1] = '\0';
    novo->proximo = NULL;

    if (!tabela)
    {
        tabela = novo;
    }
    else
    {
        Simbolo *atual = tabela;
        while (atual->proximo)
        {
            atual = atual->proximo;
        }
        atual->proximo = novo;
    }
}

Simbolo *buscarSimbolo(const char *nome)
{
    for (Simbolo *s = tabela; s; s = s->proximo)
    {
        if (strcmp(s->nome, nome) == 0)
        {
            return s;
        }
    }
    return NULL;
}

void imprimirTabela()
{
    printf("\nTabela de Símbolos:\n");
    for (Simbolo *s = tabela; s; s = s->proximo)
        printf("Nome: %s, Tipo: %s\n", s->nome, s->tipo);
}