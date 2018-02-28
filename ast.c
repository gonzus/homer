#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include "ast.h"

#define AST_CHECK(x) \
    if (!x) { \
        ast_error("out of memory"); \
        return 0; \
    }
#define AST_ALLOC(t) \
    ASTNode* n = malloc(sizeof(ASTNode)); \
    AST_CHECK(n); \
    n->type = (t)

ASTNode* ast_cons(int value)
{
    AST_ALLOC(ASTNodeTypeConstant);
    n->cons.value = value;
    return n;
}

ASTNode* ast_iden(Symbol* symbol)
{
    AST_ALLOC(ASTNodeTypeIdentifier);
    n->iden.symbol = symbol;
    return n;
}

ASTNode* ast_oper(int oper, int nops, ...)
{
#ifdef AST_DEBUG
    if (oper <= 0xff) {
        printf("OPR [%c] %d\n", oper, nops);
    } else {
        printf("OPR [%d] %d\n", oper, nops);
    }
#endif
    AST_ALLOC(ASTNodeTypeOperator);

    n->oper.op = malloc(nops * sizeof(ASTNode));
    AST_CHECK(n->oper.op);

    n->oper.oper = oper;
    n->oper.nops = nops;

    va_list ap;
    va_start(ap, nops);
    for (int j = 0; j < nops; j++) {
        n->oper.op[j] = va_arg(ap, ASTNode*);
    }
    va_end(ap);

    return n;
}

void ast_free(ASTNode* n)
{
    if (!n) {
        return;
    }

    if (n->type == ASTNodeTypeOperator) {
        for(int j = 0; j < n->oper.nops; j++)
            ast_free(n->oper.op[j]);
        free(n->oper.op);
    }
    free(n);
}

void ast_error(const char *s)
{
    fprintf(stderr, "%s\n", s);
}
