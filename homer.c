#include <stdarg.h>
#include <stdio.h>
#include "homer.h"
#include "ast.h"
#include "table.h"
#include "parser.h"
#include "lexer.h"
#include "visitor.h"
#include "interp.h"
#include "log.h"

static int parse_file(Homer* homer, FILE* fp);
static int run_interpreter(Homer* homer);
static void populate_table(Table* table);

Homer* homer_build(void)
{
    Homer* homer = (Homer*) malloc(sizeof(Homer));
    memset(homer, 0, sizeof(Homer));
    homer->visitor = visitor_create();
    return homer;
}

void homer_destroy(Homer* homer)
{
    ast_free(homer->root);
    homer->root = 0;
    visitor_destroy(homer->visitor);
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
    homer->lineno = 1;
    do {
        if (!fp) {
            fp = fopen(fn, "r");
            if (!fp) {
                fprintf(stderr, "Could not open %s for reading\n", fn);
                break;
            }
            close = 1;
        }

        int parse = parse_file(homer, fp);
        if (parse != 0) {
            fprintf(stderr, "Could not parse input file %s: %d\n", fn, parse);
            break;
        }

        int run = run_interpreter(homer);
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

static void homer_message(Homer* homer, const char* type, const char* fmt, va_list ap)
{
    // TODO: lineno is always the last line, because we already parsed the
    // whole file by now...
    fprintf(stderr, "%s line %d: ", type, homer->lineno);
    vfprintf(stderr, fmt, ap);
    fprintf(stderr, "\n");
}

void homer_error(Homer* homer, const char *fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);
    homer_message(homer, "ERROR", fmt, ap);
    va_end(ap);
}

void homer_warning(Homer* homer, const char *fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);
    homer_message(homer, "WARNING", fmt, ap);
    va_end(ap);
}

static int parse_file(Homer* homer, FILE* fp)
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

static int run_interpreter(Homer* homer)
{
    LOG(("=== RUN START ==="));
    homer->table = table_build(0, 0);
    populate_table(homer->table);
#if 1
    interpreter_run(homer->root, homer);
#endif
    ast_free(homer->root);
    homer->root = 0;
    table_destroy(homer->table);
    homer->table = 0;
    LOG(("=== RUN END ==="));
    return 0;
}

static void populate_table(Table* table)
{
    struct Data {
        int type;
        const char* name;
    } data[] = {
        { SymbolCategoryBuiltinType, "int"   },
        { SymbolCategoryBuiltinType, "float" },
    };

    for (unsigned long j = 0; j < sizeof(data) / sizeof(data[0]); ++j) {
        table_create(table, data[j].name, data[j].type);
    }
}
