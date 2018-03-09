#include <stdio.h>
#include <string.h>
#include "homer.h"
#include "ast.h"
#include "symtab.h"
#include "parser.h"
#include "log.h"
#include "util.h"
#include "visitor.h"
#include "interp.h"

static int initialize(ASTNode* n, Homer* homer);

int interpreter_run(ASTNode* root, Homer* homer)
{
    *homer->visitor = (Visitor) {
        interpreter_constant_integer,
        interpreter_constant_double,
        interpreter_constant_string,
        interpreter_identifier,
        interpreter_declaration,
        interpreter_operator_var,
        interpreter_operator_while,
        interpreter_operator_if,
        interpreter_operator_print,
        interpreter_operator_semi,
        interpreter_operator_assign,
        interpreter_operator_unop,
        interpreter_operator_binop,
        interpreter_operator_unknown,
        interpreter_unknown,
    };
    return visitor_visit(root, homer);
}

int interpreter_constant_integer(ASTNode* n, Homer* homer)
{
    UNUSED_PARAMETER(homer);
    LOG(("RUN cint %ld", n->cint.value));
    return n->cint.value;
}

int interpreter_constant_double(ASTNode* n, Homer* homer)
{
    UNUSED_PARAMETER(homer);
    LOG(("RUN cdbl %lf", n->cdbl.value));
    // TODO: this just won't do...
    return (int) n->cdbl.value;
}

int interpreter_constant_string(ASTNode* n, Homer* homer)
{
    UNUSED_PARAMETER(homer);
    LOG(("RUN cstr [%s]", n->cstr.value));
    // TODO: this just won't do...
    return n->cstr.value ? strlen(n->cstr.value) : -1;
}

int interpreter_identifier(ASTNode* n, Homer* homer)
{
    UNUSED_PARAMETER(homer);
    LOG(("RUN identifier %s", n->iden.symbol->name));
    return n->iden.symbol->value;
}

int interpreter_declaration(ASTNode* n, Homer* homer)
{
    UNUSED_PARAMETER(homer);
    LOG(("RUN declaration %s", token_name(n->decl.token)));
    return n->decl.token;
}

int interpreter_operator_var(ASTNode* n, Homer* homer)
{
    LOG(("RUN operator %s", token_name(n->oper.oper)));
    initialize(n, homer);
    return 0;
}

int interpreter_operator_while(ASTNode* n, Homer* homer)
{
    LOG(("RUN operator %s", token_name(n->oper.oper)));
    while (visitor_visit(n->oper.op[0], homer)) {
        visitor_visit(n->oper.op[1], homer);
    }
    return 0;
}

int interpreter_operator_if(ASTNode* n, Homer* homer)
{
    LOG(("RUN operator %s", token_name(n->oper.oper)));
    if (visitor_visit(n->oper.op[0], homer)) {
        visitor_visit(n->oper.op[1], homer);
    } else if(n->oper.nops >= 3) {
        visitor_visit(n->oper.op[2], homer);
    }
    return 0;
}

int interpreter_operator_print(ASTNode* n, Homer* homer)
{
    LOG(("RUN operator %s", token_name(n->oper.oper)));
    int i0 = visitor_visit(n->oper.op[0], homer);
    printf("%d\n", i0);
    return 0;
}

int interpreter_operator_semi(ASTNode* n, Homer* homer)
{
    LOG(("RUN operator %s", token_name(n->oper.oper)));
    int i0 = visitor_visit(n->oper.op[0], homer);
    int i1 = visitor_visit(n->oper.op[1], homer);
    UNUSED_PARAMETER(i0);
    return i1;
}

int interpreter_operator_assign(ASTNode* n, Homer* homer)
{
    LOG(("RUN operator %s", token_name(n->oper.oper)));
    // TODO: this just won't do...
    Symbol* s0 = n->oper.op[0]->iden.symbol;
    int i1 = visitor_visit(n->oper.op[1], homer);
    return s0->value = i1;
}

int interpreter_operator_unop(ASTNode* n, Homer* homer)
{
    LOG(("RUN operator %s", token_name(n->oper.oper)));
    int i0 = visitor_visit(n->oper.op[0], homer);
    switch (n->oper.oper) {
        case UMINUS:
            return - i0;
        case LNOT:
            return ! i0;
    }
    return 0;
}

int interpreter_operator_binop(ASTNode* n, Homer* homer)
{
    LOG(("RUN operator %s", token_name(n->oper.oper)));
    int i0 = visitor_visit(n->oper.op[0], homer);
    int i1 = visitor_visit(n->oper.op[1], homer);
    switch (n->oper.oper) {
        case ADD:
            return i0 + i1;

        case SUB:
            return i0 - i1;

        case MUL:
            return i0 * i1;

        case DIV:
            return i0 / i1;

        case LT:
            return i0 < i1;

        case GT:
            return i0 > i1;

        case GE:
            return i0 >= i1;

        case LE:
            return i0 <= i1;

        case EQ:
            return i0 == i1;

        case NE:
            return i0 != i1;

        case LAND:
            return i0 && i1;

        case LOR:
            return i0 || i1;
    }
    return 0;
}

int interpreter_operator_unknown(ASTNode* n, Homer* homer)
{
    homer_error(homer, "unknown operator %d - %s", n->oper.oper, token_name(n->oper.oper));
    return 0;
}

int interpreter_unknown(ASTNode* n, Homer* homer)
{
    homer_error(homer, "unknown %d - %s", n->oper.oper, token_name(n->oper.oper));
    return 0;
}

static int initialize(ASTNode* n, Homer* homer)
{
    if (n->oper.nops < 3) {
        return 0;
    }

    int count = 0;
    int val = interpreter_run(n->oper.op[2], homer);
    ASTNode* v = n->oper.op[0];
    while (1) {
        if (!v) {
            break;
        }
        switch (v->type) {
            case ASTNodeTypeIdentifier:
                v->iden.symbol->value = val;
                ++count;
                v = 0;
                break;
            case ASTNodeTypeOperator:
                if (v->oper.oper != COMMA) {
                    v = 0;
                } else {
                    v->oper.op[1]->iden.symbol->value = val;
                    ++count;
                    v = v->oper.op[0];
                }
                break;
            default:
                v = 0;
                break;
        }
    }
    return count;
}
