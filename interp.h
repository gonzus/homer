#ifndef INTERP_H_
#define INTERP_H_

struct ASTNode;
struct SymTab;

int run(struct ASTNode* node, struct SymTab* symtab);

#endif
