#ifndef HOMER_H_
#define HOMER_H_

#include "ast.h"
#include "symtab.h"

typedef struct Homer {
    int lineno;
    ASTNode* root;
    SymTab* symtab;
} Homer;

extern Homer* homer_global;

const char* token_name(int token);

int homer_parse(void);
int homer_run(void);
void homer_error(const char *fmt, ...);

#endif
