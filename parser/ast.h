#ifndef AST_H
#define AST_H

typedef enum
{
    VK_LET,
    VK_CONST,
    VK_VAR
} VarKind;

// define uma enumeração para os tipos de nós na árvore sintática abstrata
typedef enum
{
    NO_NUM,
    NO_ID,
    NO_OP,
    NO_DECL,
    NO_STR,
    NO_BOOL
} NoTipo;

typedef enum
{
    TIPO_NUMBER,
    TIPO_STRING,
    TIPO_BOOLEAN
} TipoDado;

typedef struct NoAST
{
    NoTipo tipo;
    union
    {
        int valor;       // para NO_NUM
        char nome[32];   // para NO_ID
        char texto[128]; // para NO_STR
        struct
        {
            VarKind tipo;       // tipo de variável
            TipoDado tipo_dado; // tipo de dado
            char nome[32];      // nome da variável
            union
            {
                int valor_num;       // para números
                char valor_str[128]; // para strings
                int valor_bool;      // para booleanos
            };

        } decl;
    };
    struct NoAST *esquerda;
    struct NoAST *direita;
} NoAST;

// Cria um nó numérico
NoAST *criarNoNum(int valor);
// Cria um nó string
NoAST *criarNoStr(const char *texto);
// Cria um nó booleano
NoAST *criarNoBool(int valor);
// Cria um nó de identificador
NoAST *criarNoId(const char *nome);
// Cria um nó de operação
NoAST *criarNoOp(char operador, NoAST *esquerda, NoAST *direita);
// Cria um nó de declaração
NoAST *criarNoDecl(VarKind var_kind, TipoDado tipo_dado, const char *nome, NoAST *valor);
// Adiciona uma declaração à árvore
NoAST *adicionarDeclaracao(NoAST *raiz, NoAST *declaracao);

// Imprime a AST
void imprimirAST(NoAST *raiz);

#endif