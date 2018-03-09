#ifndef VISITOR_H_
#define VISITOR_H_

typedef int (VisitorFunction)(struct ASTNode* node, struct Homer* homer);

typedef struct Visitor {
    VisitorFunction* visit_constant_integer;
    VisitorFunction* visit_constant_double;
    VisitorFunction* visit_constant_string;
    VisitorFunction* visit_identifier;
    VisitorFunction* visit_declaration;
    VisitorFunction* visit_operator_var;
    VisitorFunction* visit_operator_while;
    VisitorFunction* visit_operator_if;
    VisitorFunction* visit_operator_print;
    VisitorFunction* visit_operator_semi;
    VisitorFunction* visit_operator_assign;
    VisitorFunction* visit_operator_unop;
    VisitorFunction* visit_operator_binop;
    VisitorFunction* visit_operator_unknown;
    VisitorFunction* visit_unknown;
} Visitor;

Visitor* visitor_create(void);
void visitor_destroy(Visitor* visitor);
int visitor_visit(struct ASTNode* node, struct Homer* homer);

#endif
