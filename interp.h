#ifndef INTERP_H_
#define INTERP_H_

#include "ast.h"
#include "symtab.h"

int run(ASTNode* node, SymTab* symtab);

#endif
