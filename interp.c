#include <stdio.h>
#include "ast.h"
#include "symtab.h"
#include "homer.h"
#include "parser.h"
#include "log.h"
#include "interp.h"

static int mark_declared(ASTNode* n)
{
    switch (n->type) {
        case ASTNodeTypeIdentifier:
            n->iden.symbol->decl = 1;
            break;

        case ASTNodeTypeOperator:
            switch (n->oper.oper) {
                case COMMA:
                    // set all members of op 2 to declared
                    mark_declared(n->oper.op[0]);
                    mark_declared(n->oper.op[1]);
                    break;
            }
    }
    return 0;
}

int run(ASTNode* n, Homer* homer)
{
    if (!n)
        return 0;

    switch (n->type) {
        case ASTNodeTypeConstant:
            LOG(("RUN constant %d", n->cons.value));
            return n->cons.value;

        case ASTNodeTypeIdentifier:
            LOG(("RUN identifier %d", n->iden.symbol->value));
            if (!n->iden.symbol->decl) {
                homer_error(homer, "undeclared variable %s", n->iden.symbol->name);
            }
            return n->iden.symbol->value;

        case ASTNodeTypeDeclaration:
            LOG(("RUN declaration %d", n->decl.token));
            return n->decl.token;

        case ASTNodeTypeOperator:
            LOG(("RUN operator %s", token_name(n->oper.oper)));
            switch (n->oper.oper) {
                case VAR:
                    // set all members of op 2 to declared
                    mark_declared(n->oper.op[0]);
                    return 0;

                case WHILE:
                    while (run(n->oper.op[0], homer))
                        run(n->oper.op[1], homer);
                    return 0;

                case IF:
                    if (run(n->oper.op[0], homer))
                        run(n->oper.op[1], homer);
                    else if(n->oper.nops > 2)
                        run(n->oper.op[2], homer);
                    return 0;

                case PRINT:
                    printf("%d\n", run(n->oper.op[0], homer));
                    return 0;

                case SEMI:
                    run(n->oper.op[0], homer);
                    return run(n->oper.op[1], homer);

                case ASS:
                    return n->oper.op[0]->iden.symbol->value = run(n->oper.op[1], homer);

                case UMINUS:
                    return -run(n->oper.op[0], homer);

                case ADD:
                    return run(n->oper.op[0], homer) +  run(n->oper.op[1], homer);

                case SUB:
                    return run(n->oper.op[0], homer) -  run(n->oper.op[1], homer);

                case MUL:
                    return run(n->oper.op[0], homer) *  run(n->oper.op[1], homer);

                case DIV:
                    return run(n->oper.op[0], homer) /  run(n->oper.op[1], homer);

                case LT:
                    return run(n->oper.op[0], homer) <  run(n->oper.op[1], homer);

                case GT:
                    return run(n->oper.op[0], homer) >  run(n->oper.op[1], homer);

                case GE:
                    return run(n->oper.op[0], homer) >= run(n->oper.op[1], homer);

                case LE:
                    return run(n->oper.op[0], homer) <= run(n->oper.op[1], homer);

                case EQ:
                    return run(n->oper.op[0], homer) == run(n->oper.op[1], homer);

                case NE:
                    return run(n->oper.op[0], homer) != run(n->oper.op[1], homer);

                case LAND:
                    return run(n->oper.op[0], homer) && run(n->oper.op[1], homer);

                case LOR:
                    return run(n->oper.op[0], homer) || run(n->oper.op[1], homer);

                case LNOT:
                    return ! run(n->oper.op[0], homer);

                default:
                    // homer_error("unknown operand %d - %s", n->oper.oper, token_name(n->oper.oper));
                    return 0;
            }
    }

    return 0;
}
