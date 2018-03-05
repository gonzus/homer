#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include "log.h"
#include "homer.h"
#include "ast.h"
#include "symtab.h"
#include "parser.h"

        // homer_error("out of memory");
#define AST_CHECK(x) \
    if (!x) { \
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
    LOG(("ast_iden(%p => %s - %s)", symbol, token_name(symbol->token), symbol->name));
    AST_ALLOC(ASTNodeTypeIdentifier);
    n->iden.symbol = symbol;
    return n;
}

ASTNode* ast_decl(int token)
{
    LOG(("ast_decl(%d)", token));
    AST_ALLOC(ASTNodeTypeDeclaration);
    n->decl.token = token;
    return n;
}

ASTNode* ast_oper(int oper, int nops, ...)
{
    LOG(("OPR [%s] with %d operands", token_name(oper), nops));

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

    LOG(("AST DESTROY %p", n));
    if (n->type == ASTNodeTypeOperator) {
        for(int j = 0; j < n->oper.nops; j++)
            ast_free(n->oper.op[j]);
        free(n->oper.op);
    }
    free(n);
}

ASTNode* var_decl(Homer* homer, const char* var)
{
    Symbol* s = symtab_lookup(homer->symtab, var, homer->block, 1);
    if (s) {
        homer_error(homer, "variable %s already declared in this block", var);
        return 0;
    }
    s = symtab_create(homer->symtab, var, homer->block, VARIABLE);
    return ast_iden(s);
}

ASTNode* var_use(Homer* homer, const char* var)
{
    Symbol* s = symtab_lookup(homer->symtab, var, homer->block, 0);
    if (!s) {
        homer_error(homer, "undeclared variable %s", var);
        return 0;
    }
    return ast_iden(s);
}
