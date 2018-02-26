#include "ast.h"
#include "symtab.h"

#define SYMTAB_CHECK(i) \
    do { \
        char name = (i) + 'a'; \
        if (name < 'a' || name > 'z') { \
            ast_error("invalid symbol"); \
            return 0; \
        } \
    } while (0)

// For now our symbol table is just an array of 26 positions,
// because we only support symbols named 'a' to 'z'.
static int sym[26];

int symtab_get(int index)
{
    SYMTAB_CHECK(index);
    return sym[index];
}

int symtab_set(int index, int value)
{
    SYMTAB_CHECK(index);
    return sym[index] = value;
}
