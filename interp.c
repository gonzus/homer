#include <stdio.h>
#include <string.h>
#include "homer.h"
#include "ast.h"
#include "table.h"
#include "parser.h"
#include "log.h"
#include "util.h"
#include "visitor.h"
#include "interp.h"

static int declare(ASTNode* n, Homer* homer);
static Symbol* var_declare(ASTNode* var, Symbol* type, ASTNode* init, Homer* homer);
static Symbol* var_assign(ASTNode* var, ASTNode* init, Homer* homer);
static Symbol* sym_assign(Symbol* symbol, ASTNode* init, Homer* homer);

int interpreter_run(ASTNode* root, Homer* homer)
{
    *homer->visitor = (Visitor) {
        interpreter_constant_integer,
        interpreter_constant_double,
        interpreter_constant_string,
        interpreter_identifier,
        interpreter_block,
        interpreter_operator_var,
        interpreter_operator_while,
        interpreter_operator_if,
        interpreter_operator_print,
        interpreter_operator_comma,
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
    Symbol* symbol = table_lookup(homer->table, n->iden.name, 0);
    if (!symbol) {
        homer_error(homer, "unknown variable %s", n->iden.name);
        return 0;
    }

    // TODO: there has to be a better way than comparing the type name...
    if (strcmp(symbol->type->name, "int") == 0) {
        LOG(("=== VAR ivalue [%s] -> %d", symbol->name, symbol->value.ivalue));
        return symbol->value.ivalue;
    }
    if (strcmp(symbol->type->name, "float") == 0) {
        LOG(("=== VAR fvalue [%s] -> FUCK", symbol->name));
        return 0;
    }
    if (strcmp(symbol->type->name, "string") == 0) {
        LOG(("=== VAR svalue [%s] -> FUCK", symbol->name));
        return 0;
    }
    return 0;
}

int interpreter_block(ASTNode* n, Homer* homer)
{
    Table* current = homer->table;
    homer->table = table_build(0, current);
    visitor_visit(n->block.stmts, homer);
    table_destroy(homer->table);
    homer->table = current;
    return 0;
}

int interpreter_operator_var(ASTNode* n, Homer* homer)
{
    LOG(("RUN operator %s", token_name(n->oper.oper)));
    declare(n, homer);
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

int interpreter_operator_comma(ASTNode* n, Homer* homer)
{
    LOG(("RUN operator %s", token_name(n->oper.oper)));
    int i = 0;
    for (int j = 0; j < n->oper.nops; ++j) {
        visitor_visit(n->oper.op[j], homer);
    }
    return i;
}

int interpreter_operator_semi(ASTNode* n, Homer* homer)
{
    LOG(("RUN operator %s", token_name(n->oper.oper)));
    int i = 0;
    for (int j = 0; j < n->oper.nops; ++j) {
        visitor_visit(n->oper.op[j], homer);
    }
    return i;
}

int interpreter_operator_assign(ASTNode* n, Homer* homer)
{
    LOG(("RUN operator %s", token_name(n->oper.oper)));
    var_assign(n->oper.op[0], n->oper.op[1], homer);
    return 0;
}

int interpreter_operator_unop(ASTNode* n, Homer* homer)
{
    LOG(("RUN unary operator %s", token_name(n->oper.oper)));
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
    LOG(("RUN binary operator %s", token_name(n->oper.oper)));
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

static int declare(ASTNode* n, Homer* homer)
{
    if (n->oper.nops < 2) {
        return 0;
    }

    ASTNode* nlist = n->oper.op[0];
    ASTNode* ntype = n->oper.op[1];
    ASTNode* ninit = 0;
    if (n->oper.nops > 2) {
        ninit = n->oper.op[2];
    }

    // do we already have the type anywhere?
    Symbol* type = table_lookup(homer->table, ntype->iden.name, 0);
    if (!type) {
        homer_error(homer, "type %s not know in this scope", ntype->iden.name);
        return 0;
    }

    int count = 0;
    for (ASTNode* v = nlist; v; ) {
        switch (v->type) {
            case ASTNodeTypeIdentifier:
                ++count;
                var_declare(v, type, ninit, homer);
                v = 0;
                break;
            case ASTNodeTypeOperator:
                if (v->oper.oper != COMMA) {
                    v = 0;
                } else {
                    ++count;
                    var_declare(v->oper.op[1], type, ninit, homer);
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

static Symbol* var_declare(ASTNode* var, Symbol* type, ASTNode* init, Homer* homer)
{
    Symbol* s = 0;

    // do we already have it in our local table?
    s = table_lookup(homer->table, var->iden.name, 1);
    if (s) {
        homer_error(homer, "var %s already exists in this scope", var->iden.name);
        return 0;
    }
#if 0
    // does it exist in outer scopes? warn
    s = table_lookup(homer->table, var->iden.name, 0);
    if (s) {
        homer_warning(homer, "var %s hides previous variable", var->iden.name);
    }
#endif
    s = table_create(homer->table, var->iden.name, SymbolCategoryVariable);
    if (!s) {
        homer_error(homer, "could not create var %s of type %s", var->iden.name, type->name);
        return 0;
    }
    s->type = type;
    return sym_assign(s, init, homer);
}

static Symbol* var_assign(ASTNode* var, ASTNode* init, Homer* homer)
{
    Symbol* s = table_lookup(homer->table, var->iden.name, 0);
    if (!s) {
        homer_error(homer, "var %s does not exist", var->iden.name);
        return 0;
    }
    return sym_assign(s, init, homer);
}

static Symbol* sym_assign(Symbol* symbol, ASTNode* init, Homer* homer)
{
    if (! init) {
        return 0;
    }
    int value = visitor_visit(init, homer);

    // TODO: there has to be a better way than comparing the type name...
    if (strcmp(symbol->type->name, "int") == 0) {
        LOG(("=== VAR ivalue [%s] <- %d", symbol->name, value));
        symbol->value.ivalue = value;
    }
    else if (strcmp(symbol->type->name, "float") == 0) {
        LOG(("=== VAR fvalue [%s] FUCK", symbol->name));
    }
    else if (strcmp(symbol->type->name, "string") == 0) {
        LOG(("=== VAR svalue [%s] FUCK", symbol->name));
    }
    return symbol;
}
