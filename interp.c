#include <stdio.h>
#include <string.h>
#include "ast.h"
#include "symtab.h"
#include "homer.h"
#include "parser.h"
#include "log.h"
#include "interp.h"

int run(ASTNode* n, Homer* homer)
{
    if (!n)
        return 0;

    switch (n->type) {
        case ASTNodeTypeConstantInteger:
            LOG(("RUN cint %ld", n->cint.value));
            return n->cint.value;

        case ASTNodeTypeConstantDouble:
            LOG(("RUN cdbl %lf", n->cdbl.value));
            // TODO: this just won't do...
            return (int) n->cdbl.value;

        case ASTNodeTypeConstantString:
            LOG(("RUN cstr [%s]", n->cstr.value));
            // TODO: this just won't do...
            return n->cstr.value ? strlen(n->cstr.value) : -1;

        case ASTNodeTypeIdentifier:
            LOG(("RUN identifier %s", n->iden.symbol->name));
            return n->iden.symbol->value;

        case ASTNodeTypeDeclaration:
            LOG(("RUN declaration %s", token_name(n->decl.token)));
            return n->decl.token;

        case ASTNodeTypeOperator:
            LOG(("RUN operator %s", token_name(n->oper.oper)));
            switch (n->oper.oper) {
                case VAR:
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
                    homer_error(homer, "unknown operand %d - %s", n->oper.oper, token_name(n->oper.oper));
                    return 0;
            }
    }

    return 0;
}
