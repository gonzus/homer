#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ast.h"
#include "log.h"
#include "homer.h"
#include "block.h"
#include "symtab.h"
#include "parser.h"

#define SYMTAB_DEFAULT_SIZE 211

static Symbol* symbol_build(const char* name, int token, Block* block)
{
    Symbol* symbol = (Symbol*) malloc(sizeof(Symbol));
    memset(symbol, 0, sizeof(Symbol));
    symbol->name = strdup(name);
    symbol->token = token;
    symbol->block = block_clone(block);
    if (symbol->token != VARIABLE) {
        LOG(("SYMBOL CREATED (%s) [%s] => %p", token_name(token), name, symbol));
    } else {
        char wanted[256];
        block_format(block, wanted);
        LOG(("SYMBOL CREATED [%s] (%s) => %p", name, wanted, symbol));
    }
    return symbol;
}

static void symbol_destroy(Symbol* symbol)
{
    if (!symbol) {
        return;
    }
    block_destroy(symbol->block);
    free((void*) symbol->name);
    free((void*) symbol);
}

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
            symbol_destroy(q);
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

Symbol* symtab_lookup(SymTab* symtab, const char* name, int token, struct Block* block)
{
    Symbol* s = 0;
    int h = hash(name) % symtab->size;
    for (s = symtab->table[h]; s != 0; s = s->next) {
        if (strcmp(name, s->name) == 0 && block_contains(s->block, block)) {
            // found it!
            if (s->token != VARIABLE) {
                LOG(("SYMBOL FOUND (%s) [%s] %d", token_name(s->token), name, h));
            } else {
                char found[256];
                block_format(s->block, found);
                char wanted[256];
                block_format(block, wanted);
                LOG(("SYMBOL FOUND [%s] %d -> [%s] / [%s]", name, h, found, wanted));
            }
            return s;
        }
    }

    // not there...
    LOG(("SYMBOL NOT FOUND [%s] %d", name, h));

    if (!token) {
        // ... and we were not asked to create it
        return 0;
    }

    // create it then, at the head of its list
    s = symbol_build(name, token, block);
    s->next = symtab->table[h];
    symtab->table[h] = s;
    return s;
}
