#ifndef HOMER_H_
#define HOMER_H_

#include <stdio.h>

struct ASTNode;
struct SymTab;

typedef struct Homer {
    int lineno;
    struct ASTNode* root;
    struct SymTab* symtab;
    struct Block* block;
} Homer;

Homer* homer_build(void);
void homer_destroy(Homer* homer);
int homer_process(Homer* homer, const char* fn, FILE* fp);
void homer_error(Homer* homer, const char *fmt, ...);

// this function is defined in parser.y and need bison internals
const char* token_name(int token);

#endif
