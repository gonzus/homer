#include "mem.h"
#include "log.h"
#include "table.h"

#define SYMTAB_DEFAULT_SIZE 211

#define SYMTAB_POS(st, str) (hash(str) % (st)->size)

Table* table_build(int size, Table* parent)
{
    if (size <= 0) {
        size = SYMTAB_DEFAULT_SIZE;
    }
    Table* table;
    MEM_ALLOC(table, Table*, sizeof(Table));
    memset(table, 0, sizeof(Table));
    table->size = size;
    MEM_ALLOC(table->table, Symbol**, table->size * sizeof(Symbol*));
    memset(table->table, 0, size * sizeof(Symbol*));
    if (parent) {
        table->parent = parent;
        table->depth = parent->depth + 1;
    }
    LOG(("SYMTAB build %p [%d]", table, table->depth));
    return table;
}

void table_destroy(Table* table)
{
    LOG(("SYMTAB destroy %p [%d]", table, table->depth));
    for (int j = 0; j < table->size; ++j) {
        for (Symbol* s = table->table[j]; s != 0; ) {
            Symbol* q = s;
            s = s->next;
            symbol_destroy(q);
        }
    }
    MEM_FREE(table->table, Symbol**, table->size * sizeof(Symbol*));
    MEM_FREE(table, Table*, sizeof(Table));
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

Symbol* table_lookup(Table* table, const char* name, int local)
{
    int h = hash(name);
    for (Table* st = table; st != 0; st = st->parent) {
        Symbol* s = 0;
        int pos = h % st->size;
        for (s = st->table[pos]; s != 0; s = s->next) {
            if (strcmp(name, s->name) != 0) {
                continue;
            }
            // found it!
            LOG(("SYMTAB %p [%d] found [%s] hash %d pos %d", st, st->depth, name, h, pos));
            return s;
        }

        // not there...
        if (local) {
            break;
        }
    }
    LOG(("SYMTAB %p [%d] did not find [%s], hash %d", table, table->depth, name, h));
    return 0;
}

Symbol* table_create(Table* table, const char* name, SymbolCategory category)
{
    int h = hash(name);
    int pos = h % table->size;
    Symbol* s = symbol_build(name, category);
    s->next = table->table[pos];
    table->table[pos] = s;
    LOG(("SYMTAB %p [%d] created [%s] hash %d pos %d", table, table->depth, name, h, pos));
    return s;
}
