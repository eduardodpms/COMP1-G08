#ifndef AST_H
#define AST_H

/* OPERADORES DE COMPARAÇÃO */
#define OP_EQ 256
#define OP_NEQ 257
#define OP_LT 258
#define OP_GT 259
#define OP_LE 260
#define OP_GE 261
#define OP_ASSIGN 262 
#define OP_INCREMENT 266
#define OP_DECREMENT 267

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
    NO_BOOL,
    NO_BLOCK,
    NO_IF,
    NO_WHILE,
    NO_FOR,
    NO_BREAK,
    NO_CONTINUE,
    NO_SWITCH,
    NO_CASE,
    NO_CONSOLE_LOG
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
    NoAST *prox;
    NoAST *body;
    NoAST *else_branch; 
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
NoAST *criarNoOp(int operador, NoAST *esquerda, NoAST *direita);
// Cria um nó de declaração
NoAST *criarNoDecl(VarKind var_kind, TipoDado tipo_dado, const char *nome, NoAST *valor);
// Adiciona uma declaração à árvore
NoAST *adicionarDeclaracao(NoAST *raiz, NoAST *declaracao);
// Cria um nó condicional
NoAST *criarNoIf(NoAST *cond, NoAST *then_branch, NoAST *else_branch);
// Cria um nó de loop "while"
NoAST *criarNoWhile(NoAST *cond, NoAST *body);
// Cria um nó de loop "for"
NoAST *criarNoFor(NoAST *init, NoAST *cond, NoAST *update, NoAST *body);
// Cria um nó "block"
NoAST *criarNoBlock(NoAST *firstStatement);
// Cria um nó "break"
NoAST *criarNoBreak();
// Cria um nó "continue"
NoAST *criarNoContinue();
// Cria um nó "switch"
NoAST *criarNoSwitch(NoAST *expr, NoAST *cases);
// Cria um nó "case"
NoAST *criarNoCase(NoAST *caseExpr, NoAST *caseBody);
//Cria um nó "ConsoleLog"
NoAST *criarNoConsoleLog(NoAST *expr);



// Imprime a AST
void imprimirAST(NoAST *raiz);

// aplicacao da tabela
TipoDado inferirTipo(NoAST *expr);
void verificarTiposAST(NoAST *raiz);
int avaliarExpr(NoAST *expr, int *ok);
TipoDado verificarTipo(NoAST *raiz);

#endif