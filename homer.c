#include <memory.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include "util.h"
#include "ast.h"
#include "symtab.h"
#include "homer.h"
#include "parser.h"
#include "lexer.h"
#include "interp.h"
#include "log.h"
#include "block.h"

Homer* homer_build(void)
{
    Homer* homer = (Homer*) malloc(sizeof(Homer));
    memset(homer, 0, sizeof(Homer));
    homer->lineno = 1;
    homer->symtab = symtab_build(0);
    homer->block = block_create(0);
    return homer;
}

void homer_destroy(Homer* homer)
{
    ast_free(homer->root);
    homer->root = 0;
    block_destroy(homer->block);
    symtab_destroy(homer->symtab);
    free(homer);
}

int homer_parse(Homer* homer, FILE* fp)
{
#if 0
    extern int yy_flex_debug;
    yy_flex_debug = 1;
#endif

    yyscan_t scanner;
    yylex_init(&scanner);
    yyset_in(fp, scanner);

    if (yyparse(scanner, homer) == 0) {
        homer_run(homer);
    } else {
        fprintf(stderr, "Could not parse input file\n");
    }

    yylex_destroy(scanner);
    return 0;
}

int homer_run(Homer* homer)
{
    LOG(("RUNNING root %p", homer->root));
    run(homer->root, homer);
    ast_free(homer->root);
    homer->root = 0;
    return 0;
}

void homer_error(Homer* homer, const char *fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);
    fprintf(stderr, "error on line %d: ", homer->lineno);
    vfprintf(stderr, fmt, ap);
    fprintf(stderr, "\n");
    va_end(ap);
}
