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

static int homer_parse(Homer* homer, FILE* fp);
static int homer_run(Homer* homer);

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

int homer_process(Homer* homer, const char* fn, FILE* fp)
{
    if (!fn && !fp) {
        fprintf(stderr, "Need file name or file stream\n");
        return 1;
    }
    if (fn && fp) {
        fprintf(stderr, "Need only one of file name or file stream\n");
        return 1;
    }

    int close = 0;
    do {
        if (!fp) {
            fp = fopen(fn, "r");
            if (!fp) {
                fprintf(stderr, "Could not open %s for reading\n", fn);
                break;
            }
            close = 1;
        }

        int parse = homer_parse(homer, fp);
        if (parse != 0) {
            fprintf(stderr, "Could not parse input file %s: %d\n", fn, parse);
            break;
        }

        int run = homer_run(homer);
        if (run != 0) {
            fprintf(stderr, "Could not run input file %s: %d\n", fn, run);
            break;
        }
    } while (0);

    if (close && fp) {
        fclose(fp);
        close = 0;
    }

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

static int homer_parse(Homer* homer, FILE* fp)
{
#if 0
    extern int yy_flex_debug;
    yy_flex_debug = 1;
#endif

    yyscan_t scanner;
    yylex_init(&scanner);
    yyset_in(fp, scanner);

    LOG(("=== PARSE START ==="));
    int parse = yyparse(scanner, homer);
    LOG(("=== PARSE END ==="));

    yylex_destroy(scanner);
    return parse;
}

static int homer_run(Homer* homer)
{
    LOG(("=== RUN START ==="));
    run(homer->root, homer);
    ast_free(homer->root);
    homer->root = 0;
    LOG(("=== RUN END ==="));
    return 0;
}
