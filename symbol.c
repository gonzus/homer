#include "mem.h"
#include "log.h"
#include "symbol.h"

#if defined(HOMER_LOG) && HOMER_LOG > 0
static const char* categories[] = {
    "SymbolCategoryBuiltinType",
    "SymbolCategoryUserType",
    "SymbolCategoryVariable",
    "SymbolCategoryFunction",
};
#endif

Symbol* symbol_build(const char* name, int category)
{
    Symbol* symbol;
    MEM_ALLOC(symbol, Symbol*, sizeof(Symbol));
    memset(symbol, 0, sizeof(Symbol));
    MEM_STRDUP(symbol->name, name, -1);
    symbol->category = category;
    LOG(("SYM [%s] [%s] created", symbol->name, categories[symbol->category]));
    return symbol;
}

void symbol_destroy(Symbol* symbol)
{
    if (!symbol) {
        return;
    }
    LOG(("SYM [%s] [%s] destroyed", symbol->name, categories[symbol->category]));
    MEM_STRDEL(symbol->name, -1);
    MEM_FREE(symbol, Symbol*, sizeof(Symbol));
}

long symbol_assign_integer(Symbol* symbol, long value)
{
    if (!symbol) {
        return 0;
    }
    LOG(("=== SYM ivalue [%s] <- [%d]", symbol->name, value));
    return symbol->value.ivalue = value;
}

double symbol_assign_float(Symbol* symbol, double value)
{
    if (!symbol) {
        return 0.0;
    }
    LOG(("=== SYM fvalue [%s] <- [%lf]", symbol->name, value));
    return symbol->value.fvalue = value;
}

char* symbol_assign_string(Symbol* symbol, char* value)
{
    if (!symbol) {
        return 0;
    }
    LOG(("=== SYM svalue [%s] <- [%s]", symbol->name, value));
    return symbol->value.svalue = value;
}
