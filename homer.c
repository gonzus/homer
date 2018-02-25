#include "ast.h"
#include "parser.h"
#include "homer.h"

// need to declare this somewhere
extern int yyparse (void);

int parse(void)
{
#if 0
    extern int yy_flex_debug;
    yy_flex_debug = 1;
#endif
    return yyparse();
}
