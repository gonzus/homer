#ifndef REENTRANT_H_
#define REENTRANT_H_

/*
 * This bullshit is necessary to pass extra arguments to yylex()
 */

#define YY_DECL int yylex \
        (YYSTYPE * yylval_param, void* yyscanner, Homer *homer)

YY_DECL;

#endif
