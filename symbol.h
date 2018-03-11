#ifndef SYMBOL_H_
#define SYMBOL_H_

typedef enum {
    SymbolCategoryType,
    SymbolCategoryVariable,
    SymbolCategoryFunction,
} SymbolCategory;

typedef struct Value {
    union {
        long ivalue;
        double fvalue;
        char* svalue;
    };
} Value;

typedef struct Symbol {
    char* name;
    SymbolCategory category;
    struct Symbol* type;
    Value value;
    struct Symbol* next;
} Symbol;

Symbol* symbol_build(const char* name, int category);
void symbol_destroy(Symbol* symbol);

long symbol_assign_integer(Symbol* symbol, long value);
double symbol_assign_float(Symbol* symbol, double value);
char* symbol_assign_string(Symbol* symbol, char* value);

#endif
