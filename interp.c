#include <stdio.h>
#include "ast.h"
#include "parser.h"
#include "symtab.h"
#include "interp.h"

int run(ASTNode* n)
{
    if (!n)
        return 0;

    switch (n->type) {
        case ASTNodeTypeConstant:
            return n->cons.value;

        case ASTNodeTypeIdentifier:
            return symtab_get(n->iden.index);

        case ASTNodeTypeOperator:
            switch (n->oper.oper) {
                case WHILE:
                    while (run(n->oper.op[0]))
                        run(n->oper.op[1]);
                    return 0;

                case IF:
                    if (run(n->oper.op[0]))
                        run(n->oper.op[1]);
                    else if(n->oper.nops > 2)
                        run(n->oper.op[2]);
                    return 0;

                case PRINT:
                    printf("%d\n", run(n->oper.op[0]));
                    return 0;

                case SEMI:
                    run(n->oper.op[0]);
                    return run(n->oper.op[1]);

                case ASS:
                    return symtab_set(n->oper.op[0]->iden.index, run(n->oper.op[1]));

                case UMINUS:
                    return -run(n->oper.op[0]);

                case ADD:
                    return run(n->oper.op[0]) +  run(n->oper.op[1]);

                case SUB:
                    return run(n->oper.op[0]) -  run(n->oper.op[1]);

                case MUL:
                    return run(n->oper.op[0]) *  run(n->oper.op[1]);

                case DIV:
                    return run(n->oper.op[0]) /  run(n->oper.op[1]);

                case LT:
                    return run(n->oper.op[0]) <  run(n->oper.op[1]);

                case GT:
                    return run(n->oper.op[0]) >  run(n->oper.op[1]);

                case GE:
                    return run(n->oper.op[0]) >= run(n->oper.op[1]);

                case LE:
                    return run(n->oper.op[0]) <= run(n->oper.op[1]);

                case EQ:
                    return run(n->oper.op[0]) == run(n->oper.op[1]);

                case NE:
                    return run(n->oper.op[0]) != run(n->oper.op[1]);
            }
    }

    return 0;
}
