#ifndef CODEGEN_H
#define CODEGEN_H

#include <stdio.h>
#include "ast.h"

// Gera código C recursivamente para a AST
void gerarCodigoC_rec(NoAST *raiz, FILE *out);

// Função principal que gera o arquivo
void gerarCodigoC(NoAST *ast_root, const char *nomeArquivo);

#endif