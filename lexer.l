%{
#include <ctype.h>
#include "mem.h"
#include "ast.h"
#include "symtab.h"
#include "homer.h"
#include "parser.h"
#include "reentrant.h"

static long parse_integer(const char* s, int base);
%}

%option noyywrap
%option reentrant bison-bridge

BIN                 [_0-1]
OCT                 [_0-7]
DEC                 [_0-9]
HEX                 [_0-9a-fA-F]
SPACE               [ \t]
NEWL                [\n]

IDINIT              [_A-Za-z]
IDREST              [_A-Za-z0-9]

%%

0                   { yylval->iValue = 0; return INTEGER; }
0b{BIN}+            { yylval->iValue = parse_integer(yytext + 2,  2); return INTEGER; }
0{OCT}+             { yylval->iValue = parse_integer(yytext + 1,  8); return INTEGER; }
[1-9]{DEC}*         { yylval->iValue = parse_integer(yytext + 0, 10); return INTEGER; }
0x{HEX}+            { yylval->iValue = parse_integer(yytext + 2, 16); return INTEGER; }

"&&"                return LAND;
"||"                return LOR;
"!"                 return LNOT;

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
":"                 return COLON;
","                 return COMMA;

"var"               { return VAR;   }
"int"               { return INT;   }
"while"             { return WHILE; }
"if"                { return IF;    }
"else"              { return ELSE;  }
"print"             { return PRINT; }

{IDINIT}{IDREST}*   {
                        MEM_STRDUP(yylval->sValue, yytext, -1);
                        return VARIABLE;
                    }

[#].*               { /* ignore comments */ }
{NEWL}              { /* ignore newlines, keep a line count */ ++homer->lineno; }
{SPACE}             { /* ignore whitespace */ }
.                   { homer_error(homer, "invalid character '%c'", yytext[0]); }

%%

static long parse_integer(const char* s, int base)
{
    char tmp[64];
    int k = 0;
    for (int j = 0; s[j] != '\0'; ++j) {
        if (isdigit(s[j])) {
            tmp[k++] = s[j];
        }
    }
    tmp[k] = '\0';
    return strtol(tmp, 0,  base);
}
