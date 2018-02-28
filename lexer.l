%{
#include <stdlib.h>
#include "ast.h"
#include "parser.h"
#include "log.h"
#include "homer.h"
%}

BIN                 [0-1]
OCT                 [0-7]
DEC                 [0-9]
HEX                 [0-9a-fA-F]
SPACE               [ \t\n]

IDINIT              [_A-Za-z]
IDREST              [_A-Za-z0-9]

%%

0                   {
                        /* a 0 is 0 in any base */
                        yylval.iValue = 0;
                        return INTEGER;
                    }
0b{BIN}+            {
                        /* a binary constant */
                        yylval.iValue = strtol(yytext + 2, 0, 2);
                        return INTEGER;
                    }
0{OCT}+             {
                        /* an octal constant */
                        yylval.iValue = strtol(yytext + 1, 0, 8);
                        return INTEGER;
                    }
[1-9]{DEC}*         {
                        /* a decimal (non-zero) constant */
                        yylval.iValue = strtol(yytext, 0, 10);
                        return INTEGER;
                    }
0x{HEX}+            {
                        /* a hexadecimal constant */
                        yylval.iValue = strtol(yytext + 2, 0, 16);
                        return INTEGER;
                    }

">="                return GE;
"<="                return LE;
"=="                return EQ;
"!="                return NE;
">"                 return GT;
"<"                 return LT;
"+"                 return ADD;
"-"                 return SUB;
"*"                 return MUL;
"/"                 return DIV;
"("                 return LPAR;
")"                 return RPAR;
"{"                 return LBRC;
"}"                 return RBRC;
"["                 return LBRK;
"]"                 return RBRK;
"="                 return ASS;  // :-)
";"                 return SEMI;

[_a-zA-Z][_a-zA-Z0-9]* { /* an identifier */
                       Symbol* s = symtab_lookup(homer->symtab, yytext);
                       if (!s) {
                           s = symtab_create(homer->symtab, yytext, VARIABLE);
                       }
                       yylval.symbol = s;
                       return s->token;
                   }

{SPACE}             {
                        /* Ignore whitespace */
                    }

.                   {
                        /* anything else is an error */
                        ast_error("invalid character");
                    }

%%

int yywrap()
{
    return 1;
}
