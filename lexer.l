%{
#include <stdlib.h>
#include "ast.h"
#include "parser.h"
#include "homer.h"
%}

%%

0                  {
                       /* just a 0 */
                       yylval.iValue = 0;
                       return INTEGER;
                   }

[1-9][0-9]*        {
                       /* a decimal constant */
                       yylval.iValue = strtol(yytext, 0, 10);
                       return INTEGER;
                   }

0[0-7]*            {
                       /* an octal constant */
                       yylval.iValue = strtol(yytext + 1, 0, 8);
                       return INTEGER;
                   }

0x[0-9a-fA-F]+     {
                       /* a hexadecimal constant */
                       yylval.iValue = strtol(yytext + 2, 0, 16);
                       return INTEGER;
                   }

">="               return GE;
"<="               return LE;
"=="               return EQ;
"!="               return NE;
">"                return GT;
"<"                return LT;
"+"                return ADD;
"-"                return SUB;
"*"                return MUL;
"/"                return DIV;
"("                return LPAR;
")"                return RPAR;
"{"                return LBRC;
"}"                return RBRC;
"["                return LBRK;
"]"                return RBRK;
"="                return ASS;
";"                return SEMI;

[_a-zA-Z][_a-zA-Z0-9]* { /* an identifier */
                       Symbol* s = symtab_lookup(homer->symtab, yytext);
                       if (!s) {
                           s = symtab_create(homer->symtab, yytext, VARIABLE);
                       }
                       yylval.symbol = s;
                       return s->token;
                   }

[ \t\n]            ; /* Ignore whitespace */

.                  {
                       /* anything else is an error */
                       ast_error("invalid character");
                   }

%%

int yywrap()
{
    return 1;
}
