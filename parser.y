%{
#include <stdlib.h>
#include "ast.h"
#include "interp.h"

extern int yylex (void);

void yyerror(char* s);
%}

%union {
  int iValue;     /* Integer value */
  char sIndex;    /* Symbol table index */
  nodeType* nPtr; /* Node pointer */
}

%token <iValue> INTEGER
%token <sIndex> VARIABLE
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
    : function                         { exit(0);                      }
    ;

function
    : /* empty */
    | function stmt                    { run($2); freeNode($2);        }
    ;

stmt
    : SEMI                             { $$ = opr(SEMI, 2, 0, 0);      }
    | expr SEMI                        { $$ = $1;                      }
    | PRINT expr SEMI                  { $$ = opr(PRINT, 1, $2);       }
    | VARIABLE ASS expr SEMI           { $$ = opr(ASS, 2, id($1), $3); }
    | WHILE LPAR expr RPAR stmt        { $$ = opr(WHILE, 2, $3, $5);   }
    | IF LPAR expr RPAR stmt %prec IFX { $$ = opr(IF, 2, $3, $5);      }
    | IF LPAR expr RPAR stmt ELSE stmt { $$ = opr(IF, 3, $3, $5, $7);  }
    | LBRC stmt_list RBRC              { $$ = $2;                      }
    ;

stmt_list
    : stmt                             { $$ = $1;                      }
    | stmt_list stmt                   { $$ = opr(SEMI, 2, $1, $2);    }
    ;

expr
    : INTEGER                          { $$ = con($1);                 }
    | VARIABLE                         { $$ = id($1);                  }
    | SUB expr %prec UMINUS            { $$ = opr(UMINUS, 1, $2);      }
    | expr ADD expr                    { $$ = opr(ADD, 2, $1, $3);     }
    | expr SUB expr                    { $$ = opr(SUB, 2, $1, $3);     }
    | expr MUL expr                    { $$ = opr(MUL, 2, $1, $3);     }
    | expr DIV expr                    { $$ = opr(DIV, 2, $1, $3);     }
    | expr GT expr                     { $$ = opr(GT, 2, $1, $3);      }
    | expr LT expr                     { $$ = opr(LT, 2, $1, $3);      }
    | expr GE expr                     { $$ = opr(GE, 2, $1, $3);      }
    | expr LE expr                     { $$ = opr(LE, 2, $1, $3);      }
    | expr EQ expr                     { $$ = opr(EQ, 2, $1, $3);      }
    | expr NE expr                     { $$ = opr(NE, 2, $1, $3);      }
    | LPAR expr RPAR                   { $$ = $2;                      }
    ;

%%

void yyerror(char* s)
{
  ast_error(s);
}
