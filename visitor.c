#include "mem.h"
#include "homer.h"
#include "ast.h"
#include "parser.h"
#include "visitor.h"

Visitor* visitor_create(void)
{
    Visitor* visitor;
    MEM_ALLOC(visitor, Visitor*, sizeof(Visitor));
    memset(visitor, 0, sizeof(Visitor));
    return visitor;
}

void visitor_destroy(Visitor* visitor)
{
    MEM_FREE(visitor, Visitor*, sizeof(Visitor));
}

int visitor_visit(ASTNode* n, Homer* homer)
{
    if (!n) {
        return 0;
    }
    if (!homer) {
        return 0;
    }
    Visitor* visitor = homer->visitor;
    if (!visitor) {
        return 0;
    }

    switch (n->type) {
        case ASTNodeTypeConstantInteger:
            return visitor->visit_constant_integer(n, homer);

        case ASTNodeTypeConstantDouble:
            return visitor->visit_constant_double(n, homer);

        case ASTNodeTypeConstantString:
            return visitor->visit_constant_string(n, homer);

        case ASTNodeTypeIdentifier:
            return visitor->visit_identifier(n, homer);

        case ASTNodeTypeDeclaration:
            return visitor->visit_declaration(n, homer);

        case ASTNodeTypeBlock:
            return visitor->visit_block(n, homer);

        case ASTNodeTypeOperator:
            switch (n->oper.oper) {
                case VAR:
                    return visitor->visit_operator_var(n, homer);

                case WHILE:
                    return visitor->visit_operator_while(n, homer);

                case IF:
                    return visitor->visit_operator_if(n, homer);

                case PRINT:
                    return visitor->visit_operator_print(n, homer);

                case COMMA:
                    return visitor->visit_operator_comma(n, homer);

                case SEMI:
                    return visitor->visit_operator_semi(n, homer);

                case ASS:
                    return visitor->visit_operator_assign(n, homer);

                case UMINUS:
                case LNOT:
                    return visitor->visit_operator_unop(n, homer);

                case ADD:
                case SUB:
                case MUL:
                case DIV:
                case LT:
                case GT:
                case GE:
                case LE:
                case EQ:
                case NE:
                case LAND:
                case LOR:
                    return visitor->visit_operator_binop(n, homer);

                default:
                    return visitor->visit_operator_unknown(n, homer);
            }

        default:
            return visitor->visit_unknown(n, homer);
    }
}
