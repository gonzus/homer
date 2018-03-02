#ifndef SYMTAB_H_
#define SYMTAB_H_

struct Block;

typedef struct Symbol {
    const char* name;
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

Symbol* symtab_lookup(SymTab* symtab, const char* name, int token, struct Block* block);

#endif
