#ifndef SYMTAB_H_
#define SYMTAB_H_

typedef struct Symbol {
    char* name;
    int token;
    int value; // TODO: move to env
    struct Block* block;
    struct Symbol* next;
} Symbol;

typedef struct SymTab {
    Symbol** table;
    int size;
} SymTab;

SymTab* symtab_build(int size);
void symtab_destroy(SymTab* symtab);

Symbol* symtab_lookup(SymTab* symtab, const char* name, struct Block* block, int local);

Symbol* symtab_create(SymTab* symtab, const char* name, struct Block* block, int token);

#endif
