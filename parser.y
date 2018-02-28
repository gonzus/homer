%{
#include <stdlib.h>
#include "ast.h"
#include "homer.h"

void yyerror(char* s);
%}

%union {
  int iValue;           /* integer value */
  Symbol* symbol;       /* symbol table index */
  ASTNode* nPtr;        /* AST node pointer */
}

%token <iValue> INTEGER
%token <symbol> VARIABLE
%token WHILE IF PRINT
%token SEMI ASS
%token GE LE EQ NE GT LT
%token ADD SUB MUL DIV
%token LPAR RPAR
%token LBRC RBRC
%token LBRK RBRK

%nonassoc IFX           /* trick to manage dangling else */
%nonassoc ELSE

%left GE LE EQ NE GT LT /* comparison operators */
%left ADD SUB           /* addition and subtraction */
%left MUL DIV           /* multiplication and division */

%nonassoc UMINUS        /* unary minus, highest precedence */

%type <nPtr> stmt stmt_list expr

%%

program
    : function
    ;

function
    : /* empty */
    | function stmt                    { homer->root = $2; homer_run(); }
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
    | LPAR expr RPAR                   { $$ = $2; }
    ;

%%

void yyerror(char* s)
{
  ast_error(s);
}
