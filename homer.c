#include <memory.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include "util.h"
#include "ast.h"
#include "homer.h"
#include "parser.h"
#include "lexer.h"
#include "interp.h"
#include "log.h"

// Need to refer to this from lexer.l & parser.y
Homer* homer_global = 0;

static void init_symtab(SymTab* symtab);

static Homer* homer_build(void)
{
    Homer* homer = (Homer*) malloc(sizeof(Homer));
    memset(homer, 0, sizeof(Homer));
    homer->lineno = 1;
    homer->symtab = symtab_build(0);
    init_symtab(homer->symtab);
    return homer;
}

static void homer_destroy(Homer* homer)
{
    ast_free(homer->root);
    homer->root = 0;
    symtab_destroy(homer->symtab);
    free(homer);
    fflush(stdout);
}

int homer_parse(void)
{
#if 0
    extern int yy_flex_debug;
    yy_flex_debug = 1;
#endif

    yyscan_t scanner;
    yylex_init(&scanner);
    yyset_in(stdin, scanner);

    homer_global = homer_build();
    if (yyparse(scanner, homer_global) == 0) {
        homer_run();
    } else {
        fprintf(stderr, "Could not parse input\n");
    }
    homer_destroy(homer_global);
    homer_global = 0;

    yylex_destroy(scanner);
    return 0;
}

int homer_run(void)
{
    LOG(("RUNNING root %p", homer_global->root));
    run(homer_global->root, homer_global->symtab);
    ast_free(homer_global->root);
    homer_global->root = 0;
    return 0;
}

void homer_error(const char *fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);
    fprintf(stderr, "error on line %d: ", homer_global->lineno);
    vfprintf(stderr, fmt, ap);
    fprintf(stderr, "\n");
    va_end(ap);
}

static void init_symtab(SymTab* symtab)
{
    static struct Reserved {
        const char* lexeme;
        int token;
    } reserved[] = {
        { "while", WHILE },
        { "if"   , IF    },
        { "else" , ELSE  },
        { "print", PRINT },
    };
    for (unsigned long j = 0; j < sizeof(reserved) / sizeof(reserved[0]); ++j) {
        symtab_lookup(symtab, reserved[j].lexeme, reserved[j].token);
    }
}
