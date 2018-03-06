%{
#include "ast.h"
#include "symtab.h"
#include "block.h"
#include "homer.h"
#include "parser.h"
#include "reentrant.h"
#include "lexer.h"
#include "util.h"

#define HBN block_nest(homer->block);
#define HBE block_exit(homer->block);

void yyerror(yyscan_t scanner, Homer* homer, char const *msg);
%}

%code requires {
#ifndef YY_TYPEDEF_YY_SCANNER_T
#define YY_TYPEDEF_YY_SCANNER_T
typedef void* yyscan_t;
#endif
}

%define api.pure full
%param  {yyscan_t scanner} {Homer* homer}

%token-table            /* let's have token names please */

%union {                /* A token returned from the lexer can be: */
  int iValue;           /*   integer value                 */
  char* sValue;         /*   string value                  */
  Symbol* symbol;       /*   pointer to symbol table entry */
  ASTNode* ast;         /*   pointer to AST node           */
}

%token <iValue> INTEGER
%token <sValue> VARIABLE
%token VAR INT
%token WHILE IF PRINT
%token COMMA COLON SEMI ASS
%token LPAR RPAR
%token LBRC RBRC
%token LBRK RBRK
%nonassoc IFX           /* trick to manage dangling else */
%nonassoc ELSE

%left LOR
%left LAND
%nonassoc LNOT          /* logical NOT, highest precedence */

%left GE LE EQ NE GT LT /* comparison operators, equal precedence */

%left ADD SUB
%left MUL DIV
%nonassoc UMINUS        /* unary minus, highest precedence */

%type <ast> program stmt stmt_list expr decl var_list

%%

program
    : stmt_list[L]                               { homer->root = $L; }
    ;

stmt
    : SEMI                                       { $$ = ast_oper(SEMI, 2, 0, 0); }
    | expr[E] SEMI                               { $$ = $E; }
    | VAR var_list[L] COLON decl[D] SEMI         { $$ = ast_oper(VAR, 2, $L, $D); }
    | PRINT expr[E] SEMI                         { $$ = ast_oper(PRINT, 1, $E); }
    | VARIABLE[V] ASS expr[E] SEMI               { $$ = ast_oper(ASS, 2, var_use(homer, $V), $E); }
    | WHILE LPAR expr[E] RPAR stmt[S]            { $$ = ast_oper(WHILE, 2, $E, $S); }
    | IF LPAR expr[C] RPAR stmt[I] %prec IFX     { $$ = ast_oper(IF, 2, $C, $I); }
    | IF LPAR expr[C] RPAR stmt[I] ELSE stmt[E]  { $$ = ast_oper(IF, 3, $C, $I, $E); }
    | LBRC {HBN} stmt_list[L] {HBE} RBRC         { $$ = $L; }
    ;

decl
    : INT                                        { $$ = ast_decl(INT); }
    ;

var_list
    : VARIABLE[V]                                { $$ = var_decl(homer, $V); }
    | var_list[L] COMMA VARIABLE[V]              { $$ = ast_oper(COMMA, 2, $L, var_decl(homer, $V)); }
    ;

stmt_list
    : stmt[S]                                    { $$ = $S; }
    | stmt_list[L] stmt[S]                       { $$ = ast_oper(SEMI, 2, $L, $S); }
    ;

expr
    : INTEGER[I]                                 { $$ = ast_cons_integer($I); }
    | FLOAT[F]                                   { $$ = ast_cons_double($F); }
    | VARIABLE[V]                                { $$ = var_use(homer, $V); }
    | SUB expr[X] %prec UMINUS                   { $$ = ast_oper(UMINUS, 1, $X); }
    | expr[X] ADD expr[Y]                        { $$ = ast_oper(ADD, 2, $X, $Y); }
    | expr[X] SUB expr[Y]                        { $$ = ast_oper(SUB, 2, $X, $Y); }
    | expr[X] MUL expr[Y]                        { $$ = ast_oper(MUL, 2, $X, $Y); }
    | expr[X] DIV expr[Y]                        { $$ = ast_oper(DIV, 2, $X, $Y); }
    | expr[X] GT expr[Y]                         { $$ = ast_oper(GT, 2, $X, $Y); }
    | expr[X] LT expr[Y]                         { $$ = ast_oper(LT, 2, $X, $Y); }
    | expr[X] GE expr[Y]                         { $$ = ast_oper(GE, 2, $X, $Y); }
    | expr[X] LE expr[Y]                         { $$ = ast_oper(LE, 2, $X, $Y); }
    | expr[X] EQ expr[Y]                         { $$ = ast_oper(EQ, 2, $X, $Y); }
    | expr[X] NE expr[Y]                         { $$ = ast_oper(NE, 2, $X, $Y); }
    | expr[X] LAND expr[Y]                       { $$ = ast_oper(LAND, 2, $X, $Y); }
    | expr[X] LOR expr[Y]                        { $$ = ast_oper(LOR, 2, $X, $Y); }
    | LNOT expr[X]                               { $$ = ast_oper(LNOT, 1, $X); }
    | LPAR expr[X] RPAR                          { $$ = $X; }
    ;
%%

/* these functions are used internally or use something internal */

void yyerror(yyscan_t scanner, Homer* homer, char const *msg)
{
    UNUSED_PARAMETER(scanner);
    homer_error(homer, msg);
}

const char* token_name(int token)
{
    return yytname[YYTRANSLATE(token)];
}
