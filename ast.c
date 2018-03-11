#include <stdarg.h>
#include "log.h"
#include "mem.h"
#include "homer.h"
#include "ast.h"
#include "parser.h"

#define AST_CHECK(x) \
    if (!x) { \
        return 0; \
    }
#define AST_ALLOC(t) \
    ASTNode* n; \
    MEM_ALLOC(n, ASTNode*, sizeof(ASTNode)); \
    AST_CHECK(n); \
    n->type = (t)

ASTNode* ast_cons_integer(long value)
{
    LOG(("AST cint %ld", value));
    AST_ALLOC(ASTNodeTypeConstantInteger);
    n->cint.value = value;
    return n;
}

ASTNode* ast_cons_double(double value)
{
    LOG(("AST cdbl %lf", value));
    AST_ALLOC(ASTNodeTypeConstantDouble);
    n->cdbl.value = value;
    return n;
}

ASTNode* ast_cons_string(char* value)
{
    LOG(("AST cstr [%s]", value));
    AST_ALLOC(ASTNodeTypeConstantString);
    n->cstr.value = value; /* grab ownership of pointer */
    return n;
}

ASTNode* ast_iden(char* name)
{
    LOG(("AST iden %s", name));
    AST_ALLOC(ASTNodeTypeIdentifier);
    n->iden.name = name; /* grab ownership of pointer */
    return n;
}

ASTNode* ast_oper(int oper, int nops, ...)
{
    AST_ALLOC(ASTNodeTypeOperator);

    MEM_ALLOC(n->oper.op, ASTNode**, nops * sizeof(ASTNode));
    AST_CHECK(n->oper.op);

    n->oper.oper = oper;
    n->oper.nops = nops;

    char label[1024];
    int pos = 0;
    va_list ap;
    va_start(ap, nops);
    for (int j = 0; j < nops; j++) {
        n->oper.op[j] = va_arg(ap, ASTNode*);
        if (pos > 0) {
            label[pos++] = ':';
        }
        switch (n->oper.op[j]->type) {
            case ASTNodeTypeConstantInteger:
                pos += sprintf(label + pos, "KI");
                break;
            case ASTNodeTypeConstantDouble:
                pos += sprintf(label + pos, "KD");
                break;
            case ASTNodeTypeConstantString:
                pos += sprintf(label + pos, "KS");
                break;
            case ASTNodeTypeIdentifier:
                pos += sprintf(label + pos, "ID");
                break;
            case ASTNodeTypeOperator:
                pos += sprintf(label + pos, "OP");
                break;
            case ASTNodeTypeBlock:
                pos += sprintf(label + pos, "BL");
                break;
        }
    }
    va_end(ap);
    label[pos] = '\0';
    LOG(("AST oper %s %d ops [%s])", token_name(oper), nops, label));


    return n;
}

ASTNode* ast_block(ASTNode* stmts)
{
    AST_ALLOC(ASTNodeTypeBlock);
    n->block.stmts = stmts;
    LOG(("AST block"));
    return n;
}

void ast_free(ASTNode* n)
{
    if (!n) {
        return;
    }

    // LOG(("AST free(%p)", n));
    switch (n->type) {
        case ASTNodeTypeOperator:
            for(int j = 0; j < n->oper.nops; j++)
                ast_free(n->oper.op[j]);
            MEM_FREE(n->oper.op, ASTNode**, n->oper.nops * sizeof(ASTNode));
            break;
        case ASTNodeTypeBlock:
            ast_free(n->block.stmts);
            break;
        case ASTNodeTypeConstantString:
            MEM_STRDEL(n->cstr.value, -1);
            break;
        case ASTNodeTypeIdentifier:
            MEM_STRDEL(n->iden.name, -1);
            break;
        default:
            break;
    }
    MEM_FREE(n, ASTNode*, sizeof(ASTNode));
}
