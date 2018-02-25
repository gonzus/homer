#ifndef AST_H_
#define AST_H_

/* Possible types of node */
typedef enum {
    typeCon,    // constant
    typeId,     // identifier
    typeOpr     // operator
} nodeEnum;

/* Constant nodes */
typedef struct conNodeType {
    int value;
} conNodeType;

/* Identifier nodes */
typedef struct idNodeType {
    int pos;
} idNodeType;

/* Operator nodes */
typedef struct oprNodeType {
    int oper;                // Operator
    int nops;                // Number of operants
    struct nodeType** op; // Operands
} oprNodeType;

typedef struct nodeType {
    nodeEnum type;           // Type of node */
    union {
        conNodeType con;     // constant
        idNodeType  id;      // identifier
        oprNodeType opr;     // operator
    };
} nodeType;

/* Prototypes */
nodeType* opr(int oper, int nops, ...);
nodeType* id(int pos);
nodeType* con(int value);
void freeNode(nodeType* p);
void ast_error(const char* s);

#endif
