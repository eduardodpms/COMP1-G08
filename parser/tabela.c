#include "tabela.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

Simbolo *tabela = NULL;

void inserirSimbolo(const char *nome, const char *tipo_str)
{
    Simbolo *s = malloc(sizeof(Simbolo));
    strncpy(s->nome, nome, sizeof(s->nome));
    s->nome[sizeof(s->nome) - 1] = '\0';

    if (strcmp(tipo_str, "number") == 0)
        s->tipo = TIPO_NUMBER;
    else if (strcmp(tipo_str, "string") == 0)
        s->tipo = TIPO_STRING;
    else if (strcmp(tipo_str, "boolean") == 0)
        s->tipo = TIPO_BOOLEAN;

    s->proximo = tabela;
    tabela = s;
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

TipoDado obterTipo(const char *nome)
{
    for (Simbolo *s = tabela; s != NULL; s = s->proximo)
        if (strcmp(s->nome, nome) == 0)
            return s->tipo;
    return -1; // não encontrado
}