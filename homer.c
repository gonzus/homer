#include "ast.h"
#include "parser.h"

// need to declare this somewhere
extern int yyparse (void);

int main(int argc, char* argv[])
{
#if 0
    extern int yy_flex_debug;
    yy_flex_debug = 1;
#endif
    yyparse();
    return 0;
}
