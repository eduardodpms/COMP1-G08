#ifndef AST_H
#define AST_H

typedef enum
{
    NO_NUM,
    NO_STR,
    NO_BOOL,
    NO_ID,
    NO_OP,
    NO_DECL,
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
    VK_LET,
    VK_CONST,
    VK_VAR
} VarKind;

typedef enum
{
    TIPO_NUMBER,
    TIPO_STRING,
    TIPO_BOOLEAN
} TipoDado;

// --- OPERADORES AST ---
typedef enum
{
    AST_OP_ADD = 500,
    AST_OP_SUB,
    AST_OP_MUL,
    AST_OP_DIV,
    AST_OP_MOD,
    AST_OP_ASSIGN,
    AST_OP_EQ,
    AST_OP_NEQ,
    AST_OP_LT,
    AST_OP_GT,
    AST_OP_LE,
    AST_OP_GE,
    AST_OP_INC,
    AST_OP_DEC,
    AST_OP_READ
} AstOperadores;

typedef struct
{
    VarKind tipo;
    TipoDado tipo_dado;
    char nome[100];
    struct NoAST *expr;
} Declaracao;

typedef struct NoAST
{
    NoTipo tipo;
    int valor;
    char texto[256];
    char nome[100];
    Declaracao decl;
    int linha;

    struct NoAST *esquerda;
    struct NoAST *direita;
    struct NoAST *prox;
    struct NoAST *body;
    struct NoAST *else_branch;
    struct NoAST *update;

} NoAST;

NoAST *criarNoNum(int valor);
NoAST *criarNoStr(const char *texto);
NoAST *criarNoBool(int valor);
NoAST *criarNoId(const char *nome);
NoAST *criarNoOp(int operador, NoAST *esquerda, NoAST *direita);
NoAST *criarNoDecl(VarKind var_kind, TipoDado tipo_dado, const char *nome, NoAST *valor);
NoAST *criarNoBlock(NoAST *firstStatement);
NoAST *adicionarDeclaracao(NoAST *raiz, NoAST *declaracao);
NoAST *criarNoIf(NoAST *cond, NoAST *then_branch, NoAST *else_branch);
NoAST *criarNoWhile(NoAST *cond, NoAST *body);
NoAST *criarNoFor(NoAST *init, NoAST *cond, NoAST *update, NoAST *body);
NoAST *criarNoSwitch(NoAST *expr, NoAST *cases);
NoAST *criarNoCase(NoAST *expr, NoAST *body);
NoAST *criarNoBreak();
NoAST *criarNoContinue();
NoAST *criarNoConsoleLog(NoAST *expr);
void ast_free(NoAST *node);

void imprimirAST(NoAST *raiz);
void verificarTiposAST(NoAST *raiz);

int avaliarExpr(NoAST *expr, int *ok);

int obterValor(const char *nome, int *ok);
TipoDado obterTipo(const char *nome);

/* ast.h (adicionar perto das outras declarações/exports) */
int astIsStringLiteral(NoAST *n);
int astIsNumberLiteral(NoAST *n);
int astIsBoolLiteral(NoAST *n);
void liberarNoAST(NoAST *n); /* wrapper para ast_free, para semântica mais clara */

#endif