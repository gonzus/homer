#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include "ast.h"

nodeType* con(int value)
{
    nodeType* p = malloc(sizeof(nodeType));
    if (!p) {
        ast_error("out of memory");
        return 0;
    }

    p->type = typeCon;
    p->con.value = value;
    return p;
}

nodeType* id(int pos)
{
    nodeType* p = malloc(sizeof(nodeType));
    if (!p) {
        ast_error("out of memory");
        return 0;
    }

    p->type = typeId;
    p->id.pos = pos;
    return p;
}

nodeType* opr(int oper, int nops, ...)
{
#ifdef AST_DEBUG
    if (oper <= 0xff) {
        printf("OPR [%c] %d\n", oper, nops);
    } else {
        printf("OPR [%d] %d\n", oper, nops);
    }
#endif
    nodeType *p = malloc(sizeof(nodeType));
    if (!p) {
        ast_error("out of memory");
        return 0;
    }
    p->opr.op = malloc(nops * sizeof(nodeType));
    if (!p->opr.op) {
        ast_error("out of memory");
        return 0;
    }

    p->type = typeOpr;
    p->opr.oper = oper;
    p->opr.nops = nops;

    va_list ap;
    va_start(ap, nops);
    for (int i = 0; i < nops; i++)
        p->opr.op[i] = va_arg(ap, nodeType*);
    va_end(ap);

    return p;
}

void freeNode(nodeType* p)
{
    if (!p)
        return;

    if (p->type == typeOpr) {
        for(int i = 0; i < p->opr.nops; i++)
            freeNode(p->opr.op[i]);
        free(p->opr.op);
    }

    free(p);
}

void ast_error(const char *s)
{
    fprintf(stderr, "%s\n", s);
}

