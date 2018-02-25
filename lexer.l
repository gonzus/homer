%{
#include <stdlib.h>
#include "ast.h"
#include "parser.h"
%}

%%

[a-z]              {
                       /* single lowercase letter: variable */
                       yylval.sIndex = yytext[0] - 'a';
                       return VARIABLE;
                   }

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
                       yylval.iValue = strtol(yytext, 0, 8);
                       return INTEGER;
                   }

0x[0-9a-fA-F]+     {
                       /* a hexadecimal constant */
                       yylval.iValue = strtol(yytext + 2, 0, 16);
                       return INTEGER;
                   }

"while"            return WHILE;
"if"               return IF;
"else"             return ELSE;
"print"            return PRINT;
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

[.]                return *yytext;

[ \t\n]            ; /* Ignore whitespace */

.                  ast_error("invalid character");

%%

int yywrap()
{
    return 1;
}
