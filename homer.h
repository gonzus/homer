#ifndef HOMER_H_
#define HOMER_H_

#include "ast.h"
#include "symtab.h"

void yyerror(const char* s);
int yylex(void);
int yyparse(void);
const char* token_name(int token);

typedef struct Homer {
    int lineno;
    ASTNode* root;
    SymTab* symtab;
} Homer;

extern Homer* homer;

int homer_parse(void);
int homer_run(void);
void homer_error(const char *fmt, ...);

#endif
