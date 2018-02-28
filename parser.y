%{
#include "ast.h"
#include "homer.h"
%}

%token-table            /* let's have token names please */

%union {
  int iValue;           /* integer value */
  Symbol* symbol;       /* symbol table index */
  ASTNode* ast;         /* AST node pointer */
}

%token <iValue> INTEGER
%token <symbol> VARIABLE
%token WHILE IF PRINT
%token SEMI ASS
%token LPAR RPAR
%token LBRC RBRC
%token LBRK RBRK

%nonassoc IFX           /* trick to manage dangling else */
%nonassoc ELSE

/* operators; the later they are, the higher the precedence */

%left LOR
%left LAND
%nonassoc LNOT

%left GE LE EQ NE GT LT /* comparison operators */

%left ADD SUB           /* addition and subtraction */
%left MUL DIV           /* multiplication and division */
%nonassoc UMINUS        /* unary minus, highest precedence */

%type <ast> program stmt stmt_list expr

%%

program
    : stmt_list                        { homer->root = $1; homer_run(); }
    ;

stmt
    : SEMI                             { $$ = ast_oper(SEMI, 2, 0, 0); }
    | expr SEMI                        { $$ = $1; }
    | PRINT expr SEMI                  { $$ = ast_oper(PRINT, 1, $2); }
    | VARIABLE ASS expr SEMI           { $$ = ast_oper(ASS, 2, ast_iden($1), $3); }
    | WHILE LPAR expr RPAR stmt        { $$ = ast_oper(WHILE, 2, $3, $5); }
    | IF LPAR expr RPAR stmt %prec IFX { $$ = ast_oper(IF, 2, $3, $5); }
    | IF LPAR expr RPAR stmt ELSE stmt { $$ = ast_oper(IF, 3, $3, $5, $7); }
    | LBRC stmt_list RBRC              { $$ = $2; }
    ;

stmt_list
    : stmt                             { $$ = $1; }
    | stmt_list stmt                   { $$ = ast_oper(SEMI, 2, $1, $2); }
    ;

expr
    : INTEGER                          { $$ = ast_cons($1); }
    | VARIABLE                         { $$ = ast_iden($1); }
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

void yyerror(const char* s)
{
    homer_error(s);
}

const char* token_name(int token)
{
    return yytname[YYTRANSLATE(token)];
}
