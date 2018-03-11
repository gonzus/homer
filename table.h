#ifndef TABLE_H_
#define TABLE_H_

#include "symbol.h"

typedef struct Table {
    Symbol** table;
    int size;
    struct Table* parent;
    int depth;
} Table;

Table* table_build(int size, Table* parent);
void table_destroy(Table* table);

Symbol* table_lookup(Table* table, const char* name, int local);
Symbol* table_create(Table* table, const char* name, SymbolCategory category);

#endif
