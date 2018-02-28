#ifndef HOMER_H_
#define HOMER_H_

#include "ast.h"
#include "symtab.h"

extern int yylex(void);
extern int yyparse(void);

typedef struct Homer {
    ASTNode* root;
    SymTab* symtab;
} Homer;

extern Homer* homer;

int homer_parse(void);
int homer_run(void);

#endif
