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
    ASTNodeTypeBlock,
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
    char* name;
} ASTNodeIdentifier;

/* Declaration AST nodes */
typedef struct ASTNodeDeclaration {
    char* name;
} ASTNodeDeclaration;

/* Operator AST nodes */
typedef struct ASTNodeOperator {
    int oper;            // Operator
    int nops;            // Number of operants
    struct ASTNode** op; // Operands
} ASTNodeOperator;

/* Block AST nodes */
typedef struct ASTNodeBlock {
    struct ASTNode* stmts;
} ASTNodeBlock;

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
        ASTNodeBlock           block;
    };
} ASTNode;

ASTNode* ast_cons_integer(long value);
ASTNode* ast_cons_double(double value);
ASTNode* ast_cons_string(char* value);
ASTNode* ast_iden(char* name);
ASTNode* ast_decl(char* name);
ASTNode* ast_oper(int oper, int nops, ...);
ASTNode* ast_block(ASTNode* stmts);
void ast_free(ASTNode* n);

#endif
