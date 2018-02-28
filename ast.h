#ifndef AST_H_
#define AST_H_

#include "symtab.h"

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
    Symbol* symbol;
} ASTNodeIdentifier;

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
        ASTNodeConstant   cons;
        ASTNodeIdentifier iden;
        ASTNodeOperator   oper;
    };
} ASTNode;

ASTNode* ast_cons(int value);
ASTNode* ast_iden(Symbol* symbol);
ASTNode* ast_oper(int oper, int nops, ...);
void ast_free(ASTNode* n);
void ast_error(const char* s);

#endif
