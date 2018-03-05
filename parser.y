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
    : stmt_list                        { homer->root = $1; }
    ;

stmt
    : SEMI                             { $$ = ast_oper(SEMI, 2, 0, 0); }
    | expr SEMI                        { $$ = $1; }
    | VAR var_list COLON decl SEMI     { $$ = ast_oper(VAR, 2, $2, $4); }
    | PRINT expr SEMI                  { $$ = ast_oper(PRINT, 1, $2); }
    | VARIABLE ASS expr SEMI           { $$ = ast_oper(ASS, 2, var_use(homer, $1), $3); }
    | WHILE LPAR expr RPAR stmt        { $$ = ast_oper(WHILE, 2, $3, $5); }
    | IF LPAR expr RPAR stmt %prec IFX { $$ = ast_oper(IF, 2, $3, $5); }
    | IF LPAR expr RPAR stmt ELSE stmt { $$ = ast_oper(IF, 3, $3, $5, $7); }
    | LBRC {HBN} stmt_list {HBE} RBRC  { $$ = $3; }
    ;

decl
    : INT                              { $$ = ast_decl(INT); }
    ;

var_list
    : VARIABLE                         { $$ = var_decl(homer, $1); }
    | var_list COMMA VARIABLE          { $$ = ast_oper(COMMA, 2, $1, var_decl(homer, $3)); }
    ;

stmt_list
    : stmt                             { $$ = $1; }
    | stmt_list stmt                   { $$ = ast_oper(SEMI, 2, $1, $2); }
    ;

expr
    : INTEGER                          { $$ = ast_cons($1); }
    | VARIABLE                         { $$ = var_use(homer, $1); }
    | SUB expr %prec UMINUS            { $$ = ast_oper(UMINUS, 1, $2); }
    | expr ADD expr                    { $$ = ast_oper(ADD, 2, $1, $3); }
    | expr SUB expr                    { $$ = ast_oper(SUB, 2, $1, $3); }
    | expr MUL expr                    { $$ = ast_oper(MUL, 2, $1, $3); }
    | expr DIV expr                    { $$ = ast_oper(DIV, 2, $1, $3); }
    | expr GT expr                     { $$ = ast_oper(GT, 2, $1, $3); }
    | expr LT expr                     { $$ = ast_oper(LT, 2, $1, $3); }
    | expr GE expr                     { $$ = ast_oper(GE, 2, $1, $3); }
    | expr LE expr                     { $$ = ast_oper(LE, 2, $1, $3); }
    | expr EQ expr                     { $$ = ast_oper(EQ, 2, $1, $3); }
    | expr NE expr                     { $$ = ast_oper(NE, 2, $1, $3); }
    | expr LAND expr                   { $$ = ast_oper(LAND, 2, $1, $3); }
    | expr LOR expr                    { $$ = ast_oper(LOR, 2, $1, $3); }
    | LNOT expr                        { $$ = ast_oper(LNOT, 1, $2); }
    | LPAR expr RPAR                   { $$ = $2; }
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
