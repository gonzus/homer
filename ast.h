#ifndef AST_H_
#define AST_H_

/* Possible types of AST node */
typedef enum {
    ASTNodeTypeConstantInteger,
    ASTNodeTypeConstantDouble,
    ASTNodeTypeConstantString,
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
typedef struct ASTNodeConstantString {
    char* value;
} ASTNodeConstantString;

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
        ASTNodeConstantDouble  cdbl;
        ASTNodeConstantString  cstr;
        ASTNodeIdentifier      iden;
        ASTNodeDeclaration     decl;
        ASTNodeOperator        oper;
    };
} ASTNode;

ASTNode* ast_cons_integer(long value);
ASTNode* ast_cons_double(double value);
ASTNode* ast_cons_string(char* value);
ASTNode* ast_iden(struct Symbol* symbol);
ASTNode* ast_decl(int token);
ASTNode* ast_oper(int oper, int nops, ...);
void ast_free(ASTNode* n);

ASTNode* var_decl(struct Homer* homer, char* var);
ASTNode* var_use(struct Homer* homer, char* var);

#endif
