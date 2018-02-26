#ifndef AST_H_
#define AST_H_

/* Possible types of AST node */
typedef enum {
    ASTNodeTypeConstant,
    ASTNodeTypeIdentifier,
    ASTNodeTypeOperator,
} ASTNodeEnum;

/* Constant AST nodes */
typedef struct ASTNodeConstant {
    int value;
} ASTNodeConstant;

/* Identifier AST nodes */
typedef struct ASTNodeIdentifier {
    // for now our symbol table is just an array
    // and each symbol is a letter a-z
    int index;
} ASTNodeIdentifier;

/* Operator AST nodes */
typedef struct ASTNodeOperator {
    int oper;                // Operator
    int nops;                // Number of operants
    struct ASTNode** op; // Operands
} ASTNodeOperator;

typedef struct ASTNode {
    ASTNodeEnum type;
    union {
        ASTNodeConstant   cons;
        ASTNodeIdentifier iden;
        ASTNodeOperator   oper;
    };
} ASTNode;

ASTNode* ast_cons(int value);
ASTNode* ast_iden(int pos);
ASTNode* ast_oper(int oper, int nops, ...);
void ast_free(ASTNode* n);
void ast_error(const char* s);

#endif
