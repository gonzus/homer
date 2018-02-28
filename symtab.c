#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ast.h"
#include "log.h"
#include "homer.h"
#include "symtab.h"

#define SYMTAB_DEFAULT_SIZE 211

SymTab* symtab_build(int size)
{
    if (size <= 0) {
        size = SYMTAB_DEFAULT_SIZE;
    }
    SymTab* symtab = (SymTab*) malloc(sizeof(SymTab));
    symtab->table = (Symbol**) calloc(size, sizeof(Symbol*));
    symtab->size = size;
    return symtab;
}

void symtab_destroy(SymTab* symtab)
{
    for (int j = 0; j < symtab->size; ++j) {
        for (Symbol* s = symtab->table[j]; s != 0; ) {
            Symbol* q = s;
            s = s->next;
            free((void*) q->name);
            free((void*) q);
        }
    }
    free(symtab->table);
}

// djb2 by Dan Bernstein - http://www.cse.yorku.ca/~oz/hash.html
static unsigned long hash(const char *s)
{
    unsigned long h = 5381;
    unsigned const char* str = (unsigned const char*) s;
    int c;
    while ((c = *str++)) {
        h = ((h << 5) + h) + c; /* h * 33 + c */
    }

    return h;
}

Symbol* symtab_lookup(SymTab* symtab, const char* name)
{
    int h = hash(name) % symtab->size;
    for (Symbol* s = symtab->table[h]; s != 0; s = s->next) {
        if (strcmp(name, s->name) == 0) {
            LOG(("LOOKUP %p [%s] => %d %s - %p", symtab, name, h, token_name(s->token), s));
            return s;
        }
    }
    LOG(("LOOKUP %p [%s] => %d NIL", symtab, name, h));
    return 0;
}

Symbol* symtab_create(SymTab* symtab, const char* name, int token)
{
    int h = hash(name) % symtab->size;
    Symbol* s = (Symbol*) malloc(sizeof(Symbol));
    memset(s, 0, sizeof(Symbol));
    s->name = strdup(name);
    s->token = token;
    s->next = symtab->table[h];
    symtab->table[h] = s;
    LOG(("CREATE %p %s - [%s] => %d %p", symtab, token_name(token), name, h, s));
    return s;
}
