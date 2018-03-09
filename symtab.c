#include "mem.h"
#include "log.h"
#include "homer.h"
#include "ast.h"
#include "block.h"
#include "symtab.h"
#include "parser.h"

#define SYMTAB_DEFAULT_SIZE 211

static Symbol* symbol_build(const char* name, int token, Block* block)
{
    Symbol* symbol;
    MEM_ALLOC(symbol, Symbol*, sizeof(Symbol));
    memset(symbol, 0, sizeof(Symbol));
    MEM_STRDUP(symbol->name, name, -1);
    symbol->token = token;
    symbol->block = block_clone(block);
    if (symbol->token != VARIABLE) {
        LOG(("SYM created (%s) [%s] => %p", token_name(token), name, symbol));
    } else {
        char wanted[256];
        block_format(block, wanted);
        LOG(("SYM created [%s] (%s) => %p", name, wanted, symbol));
    }
    return symbol;
}

static void symbol_destroy(Symbol* symbol)
{
    if (!symbol) {
        return;
    }
    block_destroy(symbol->block);
    MEM_STRDEL(symbol->name, -1);
    MEM_FREE(symbol, Symbol*, sizeof(Symbol));
}

SymTab* symtab_build(int size)
{
    if (size <= 0) {
        size = SYMTAB_DEFAULT_SIZE;
    }
    SymTab* symtab;
    MEM_ALLOC(symtab, SymTab*, sizeof(SymTab));
    MEM_ALLOC(symtab->table, Symbol**, size * sizeof(Symbol*));
    memset(symtab->table, 0, size * sizeof(Symbol*));
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
    MEM_FREE(symtab->table, Symbol**, symtab->size * sizeof(Symbol*));
    MEM_FREE(symtab, SymTab*, sizeof(SymTab));
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

Symbol* symtab_lookup(SymTab* symtab, const char* name, struct Block* block, int local)
{
    Symbol* s = 0;
    int h = hash(name) % symtab->size;
    for (s = symtab->table[h]; s != 0; s = s->next) {
        if (strcmp(name, s->name) != 0) {
            continue;
        }
        int contained = local ? block_equals(s->block, block) : block_contains(s->block, block);
        if (!contained) {
            continue;
        }
        // found it!
        if (s->token != VARIABLE) {
            LOG(("SYM found (%s) [%s] %d", token_name(s->token), name, h));
        } else {
            char found[256];
            block_format(s->block, found);
            char wanted[256];
            block_format(block, wanted);
            LOG(("SYM found [%s] %d -> [%s] / [%s]", name, h, found, wanted));
        }
        return s;
    }

    // not there...
    LOG(("SYM not found [%s] %d", name, h));
    return 0;
}

Symbol* symtab_create(SymTab* symtab, const char* name, struct Block* block, int token)
{
    int h = hash(name) % symtab->size;
    Symbol* s = symbol_build(name, token, block);
    s->next = symtab->table[h];
    symtab->table[h] = s;
    return s;
}
