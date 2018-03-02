#include <stdio.h>
#include "ast.h"
#include "symtab.h"
#include "homer.h"
#include "parser.h"
#include "log.h"
#include "interp.h"

int run(ASTNode* n, SymTab* symtab)
{
    if (!n)
        return 0;

    switch (n->type) {
        case ASTNodeTypeConstant:
            LOG(("RUN constant %d", n->cons.value));
            return n->cons.value;

        case ASTNodeTypeIdentifier:
            LOG(("RUN identifier %d", n->iden.symbol->value));
            return n->iden.symbol->value;

        case ASTNodeTypeDeclaration:
            LOG(("RUN declaration %d", n->decl.token));
            return n->decl.token;

        case ASTNodeTypeOperator:
            LOG(("RUN operator %s", token_name(n->oper.oper)));
            switch (n->oper.oper) {
                case WHILE:
                    while (run(n->oper.op[0], symtab))
                        run(n->oper.op[1], symtab);
                    return 0;

                case IF:
                    if (run(n->oper.op[0], symtab))
                        run(n->oper.op[1], symtab);
                    else if(n->oper.nops > 2)
                        run(n->oper.op[2], symtab);
                    return 0;

                case PRINT:
                    printf("%d\n", run(n->oper.op[0], symtab));
                    return 0;

                case SEMI:
                    run(n->oper.op[0], symtab);
                    return run(n->oper.op[1], symtab);

                case ASS:
                    return n->oper.op[0]->iden.symbol->value = run(n->oper.op[1], symtab);

                case UMINUS:
                    return -run(n->oper.op[0], symtab);

                case ADD:
                    return run(n->oper.op[0], symtab) +  run(n->oper.op[1], symtab);

                case SUB:
                    return run(n->oper.op[0], symtab) -  run(n->oper.op[1], symtab);

                case MUL:
                    return run(n->oper.op[0], symtab) *  run(n->oper.op[1], symtab);

                case DIV:
                    return run(n->oper.op[0], symtab) /  run(n->oper.op[1], symtab);

                case LT:
                    return run(n->oper.op[0], symtab) <  run(n->oper.op[1], symtab);

                case GT:
                    return run(n->oper.op[0], symtab) >  run(n->oper.op[1], symtab);

                case GE:
                    return run(n->oper.op[0], symtab) >= run(n->oper.op[1], symtab);

                case LE:
                    return run(n->oper.op[0], symtab) <= run(n->oper.op[1], symtab);

                case EQ:
                    return run(n->oper.op[0], symtab) == run(n->oper.op[1], symtab);

                case NE:
                    return run(n->oper.op[0], symtab) != run(n->oper.op[1], symtab);

                case LAND:
                    return run(n->oper.op[0], symtab) && run(n->oper.op[1], symtab);

                case LOR:
                    return run(n->oper.op[0], symtab) || run(n->oper.op[1], symtab);

                case LNOT:
                    return ! run(n->oper.op[0], symtab);

                default:
                    // homer_error("unknown operand %d - %s", n->oper.oper, token_name(n->oper.oper));
                    return 0;
            }
    }

    return 0;
}
