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

typedef struct NoAST NoAST;

struct NoAST
{
    NoTipo tipo;
    union
    {
        int valor;       // NO_NUM / NO_BOOL
        char nome[32];   // NO_ID
        char texto[128]; // NO_STR
        struct
        {
            VarKind tipo;       // tipo de variável
            TipoDado tipo_dado; // tipo de dado
            char nome[32];      // nome da variável
            NoAST *expr;        // expressão de inicialização
            int is_constante;   // indica se é constante
            int valor_num;      // apenas para números constantes
        } decl;
    };
    NoAST *esquerda;
    NoAST *direita;

    int linha;
};

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

// aplicacao da tabela
TipoDado inferirTipo(NoAST *expr);
void verificarTiposAST(NoAST *raiz);
int avaliarExpr(NoAST *expr, int *ok);
TipoDado verificarTipo(NoAST *raiz);

#endif