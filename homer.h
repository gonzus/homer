#ifndef HOMER_H_
#define HOMER_H_

#include "ast.h"
#include "symtab.h"

typedef struct Homer {
    int lineno;
    ASTNode* root;
    SymTab* symtab;
} Homer;

const char* token_name(int token);

Homer* homer_build(void);
void homer_destroy(Homer* homer);
int homer_parse(Homer* homer);
int homer_run(Homer* homer);
void homer_error(Homer* homer, const char *fmt, ...);

#endif
