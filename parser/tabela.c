#include "tabela.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

Simbolo *tabela = NULL;

void inserirSimbolo(const char *nome, TipoDado tipo)
{
    Simbolo *s = malloc(sizeof(Simbolo));
    strncpy(s->nome, nome, sizeof(s->nome));
    s->nome[sizeof(s->nome) - 1] = '\0';
    s->tipo = tipo;

    // Inicializa valor constante como desconhecido
    s->valor_num = 0;
    s->is_constante = 0;

    s->proximo = tabela;
    tabela = s;
}

// Buscar símbolo
Simbolo *buscarSimbolo(const char *nome)
{
    for (Simbolo *s = tabela; s; s = s->proximo)
    {
        if (strcmp(s->nome, nome) == 0)
            return s;
    }
    return NULL;
}

void imprimirTabela()
{
    printf("\nTabela de Símbolos:\n");
    for (Simbolo *s = tabela; s; s = s->proximo)
    {
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

        printf("Nome: %s, Tipo: %s", s->nome, tipo_str);

        if (s->tipo == TIPO_NUMBER && s->is_constante)
        {
            printf(", Valor Constante: %d", s->valor_num);
        }

        printf("\n");
    }
}

TipoDado obterTipo(const char *nome)
{
    Simbolo *s = buscarSimbolo(nome);
    if (s)
        return s->tipo;
    return -1; // não encontrado
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