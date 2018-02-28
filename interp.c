#include <stdio.h>
#include "ast.h"
#include "parser.h"
#include "interp.h"

int run(ASTNode* n, SymTab* symtab)
{
    if (!n)
        return 0;

    switch (n->type) {
        case ASTNodeTypeConstant:
            return n->cons.value;

        case ASTNodeTypeIdentifier:
            return n->iden.symbol->value;

        case ASTNodeTypeOperator:
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
            }
    }

    return 0;
}
