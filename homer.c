#include <memory.h>
#include <stdio.h>
#include <stdlib.h>
#include "util.h"
#include "ast.h"
#include "parser.h"
#include "interp.h"
#include "homer.h"

// Need to refer to this from lexer.l & parser.y
Homer* homer = 0;

static void init_symtab(SymTab* symtab);

static Homer* homer_build(void)
{
    Homer* homer = (Homer*) malloc(sizeof(Homer));
    memset(homer, 0, sizeof(Homer));
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

    homer = homer_build();
    yyparse();
    homer_destroy(homer);
    homer = 0;
    return 0;
}

int homer_run(void)
{
    run(homer->root, homer->symtab);
    ast_free(homer->root);
    homer->root = 0;
    return 0;
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
        symtab_create(symtab, reserved[j].lexeme, reserved[j].token);
    }
}
