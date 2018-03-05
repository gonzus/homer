#ifndef AST_H_
#define AST_H_

struct Homer;
struct Symbol;

/* Possible types of AST node */
typedef enum {
    ASTNodeTypeConstant,
    ASTNodeTypeIdentifier,
    ASTNodeTypeDeclaration,
    ASTNodeTypeOperator,
} ASTNodeEnum;

/* Constant AST nodes */
typedef struct ASTNodeConstant {
    int value;
} ASTNodeConstant;

/* Identifier AST nodes */
typedef struct ASTNodeIdentifier {
    struct Symbol* symbol;
} ASTNodeIdentifier;

/* Declaration AST nodes */
typedef struct ASTNodeDeclaration {
    int token; // TODO
} ASTNodeDeclaration;

/* Operator AST nodes */
typedef struct ASTNodeOperator {
    int oper;                // Operator
    int nops;                // Number of operants
    struct ASTNode** op; // Operands
} ASTNodeOperator;

/* AST nodes */
typedef struct ASTNode {
    ASTNodeEnum type;
    union {
        ASTNodeConstant    cons;
        ASTNodeIdentifier  iden;
        ASTNodeDeclaration decl;
        ASTNodeOperator    oper;
    };
} ASTNode;

ASTNode* ast_cons(int value);
ASTNode* ast_iden(struct Symbol* symbol);
ASTNode* ast_decl(int token);
ASTNode* ast_oper(int oper, int nops, ...);
void ast_free(ASTNode* n);

ASTNode* var_decl(struct Homer* homer, const char* var);
ASTNode* var_use(struct Homer* homer, const char* var);

#endif
