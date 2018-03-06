#ifndef AST_H_
#define AST_H_

struct Homer;
struct Symbol;

/* Possible types of AST node */
typedef enum {
    ASTNodeTypeConstantInteger,
    ASTNodeTypeConstantDouble,
    ASTNodeTypeIdentifier,
    ASTNodeTypeDeclaration,
    ASTNodeTypeOperator,
} ASTNodeEnum;

/* Constant AST nodes */
typedef struct ASTNodeConstantInteger {
    long value;
} ASTNodeConstantInteger;
typedef struct ASTNodeConstantDouble {
    double value;
} ASTNodeConstantDouble;

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
    int oper;            // Operator
    int nops;            // Number of operants
    struct ASTNode** op; // Operands
} ASTNodeOperator;

/* AST nodes */
typedef struct ASTNode {
    ASTNodeEnum type;
    union {
        ASTNodeConstantInteger cint;
        ASTNodeConstantDouble  cdouble;
        ASTNodeIdentifier      iden;
        ASTNodeDeclaration     decl;
        ASTNodeOperator        oper;
    };
} ASTNode;

ASTNode* ast_cons_integer(long value);
ASTNode* ast_cons_double(double value);
ASTNode* ast_iden(struct Symbol* symbol);
ASTNode* ast_decl(int token);
ASTNode* ast_oper(int oper, int nops, ...);
void ast_free(ASTNode* n);

ASTNode* var_decl(struct Homer* homer, char* var);
ASTNode* var_use(struct Homer* homer, char* var);

#endif
