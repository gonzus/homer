%{
#include "log.h"
#include "mem.h"
#include "homer.h"
#include "ast.h"
#include "parser.h"
#include "reentrant.h"

static void parse_integer(const char* str, int base, long* ans);
static void parse_float(const char* str, double* ans);
static void parse_string(const char* str, int len, char** ans);
%}

%option reentrant bison-bridge
%option warn nodefault
%option noyywrap never-interactive nounistd


SGN                 [-+]
BIN                 [_0-1]
OCT                 [_0-7]
DEC                 [_0-9]
HEX                 [_0-9a-fA-F]
SPACE               [ \t]
NEWL                [\n]
Q                   ["]
NQ                  [^"]

IDINIT              [_A-Za-z]
IDREST              [_A-Za-z0-9]

%%

0             { yylval->iValue = 0; return INTEGER; }
0[bB]{BIN}+   { parse_integer(yytext + 2,  2, &yylval->iValue); return INTEGER; }
0{OCT}+       { parse_integer(yytext + 1,  8, &yylval->iValue); return INTEGER; }
[1-9]{DEC}*   { parse_integer(yytext + 0, 10, &yylval->iValue); return INTEGER; }
0[xX]{HEX}+   { parse_integer(yytext + 2, 16, &yylval->iValue); return INTEGER; }

[1-9]{DEC}*\.{DEC}* { parse_float(yytext + 0, &yylval->fValue); return FLOAT; }

{Q}{NQ}*{Q}         { parse_string(yytext, yyleng, &yylval->sValue); return STRING; }

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

static void parse_integer(const char* str, int base, long* ans)
{
    char tmp[100];
    int k = 0;
    for (int j = 0; str[j] != '\0'; ++j) {
        if (str[j] == '_') {
            continue;
        }
        tmp[k++] = str[j];
    }
    tmp[k] = '\0';
    *ans = strtol(tmp, 0,  base);
    LOG(("int [%s] (%d) -> [%s] => %ld", str, base, tmp, *ans, base));
}

static void parse_float(const char* str, double* ans)
{
    char tmp[100];
    int k = 0;
    for (int j = 0; str[j] != '\0'; ++j) {
        if (str[j] == '_') {
            continue;
        }
        tmp[k++] = str[j];
    }
    tmp[k] = '\0';
    *ans = strtod(tmp, 0);
    LOG(("dbl [%s] -> [%s] => %lf", str, tmp, *ans));
}

static void parse_string(const char* str, int len, char** ans)
{
    MEM_STRDUP(*ans, str + 1, len - 2);
    LOG(("str [%s] (%d) => [%s]", str, len, *ans));
}
