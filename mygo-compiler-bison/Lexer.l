%{

/*
 * Lexer.l file
 * To generate the lexical analyzer run: "flex Lexer.l"
 */

#include "Expression.h"
#include "Parser.h"

#include <stdio.h>
%}

%option outfile="Lexer.c" header-file="Lexer.h"
%option warn nodefault

%option reentrant noyywrap never-interactive nounistd
%option bison-bridge

LPAREN      "("
RPAREN      ")"
PLUS        "+"
MULTIPLY    "*"
DIVIDE      "/"
MINUS       "-"
DEFINE    ":="

NUMBER      [0-9]+
WS          [ \r\n\t]*
FLOAT       [0-9]*\.[0-9]+
IDENTIFIER  [a-zA-Z]+[a-zA-Z0-9]*

%%

{WS}            { /* Skip blanks. */ }
{NUMBER}        { sscanf(yytext, "%d", &yylval->value); return TOKEN_NUMBER; }
{FLOAT}         { sscanf(yytext, "%f", &yylval->fvalue); return TOKEN_FLOAT; }
{IDENTIFIER}    { sscanf(yytext, "%s", &yylval->svalue); return TOKEN_IDENTIFIER; }

{DEFINE}        { return TOKEN_DEKLERATION; }
{MULTIPLY}      { return TOKEN_MULTIPLY; }
{PLUS}          { return TOKEN_PLUS; }
{MINUS}         { return TOKEN_MINUS; }
{DIVIDE}        { return TOKEN_DIVIDE; }
{LPAREN}        { return TOKEN_LPAREN; }
{RPAREN}        { return TOKEN_RPAREN; }
.               {  }

%%

int yyerror(const char *msg) {
    fprintf(stderr,"Error:%s\n",msg); return 0;
}