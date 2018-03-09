#ifndef INTERP_H_
#define INTERP_H_

int interpreter_run(struct ASTNode* root, struct Homer* homer);

int interpreter_constant_integer(struct ASTNode* n, struct Homer* homer);
int interpreter_constant_double(struct ASTNode* n, struct Homer* homer);
int interpreter_constant_string(struct ASTNode* n, struct Homer* homer);
int interpreter_identifier(struct ASTNode* n, struct Homer* homer);
int interpreter_declaration(struct ASTNode* n, struct Homer* homer);
int interpreter_operator_var(struct ASTNode* n, struct Homer* homer);
int interpreter_operator_while(struct ASTNode* n, struct Homer* homer);
int interpreter_operator_if(struct ASTNode* n, struct Homer* homer);
int interpreter_operator_print(struct ASTNode* n, struct Homer* homer);
int interpreter_operator_semi(struct ASTNode* n, struct Homer* homer);
int interpreter_operator_assign(struct ASTNode* n, struct Homer* homer);
int interpreter_operator_unop(struct ASTNode* n, struct Homer* homer);
int interpreter_operator_binop(struct ASTNode* n, struct Homer* homer);
int interpreter_operator_unknown(struct ASTNode* n, struct Homer* homer);
int interpreter_unknown(struct ASTNode* n, struct Homer* homer);

#endif
