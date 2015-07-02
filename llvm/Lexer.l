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
%option warn default

%option reentrant noyywrap never-interactive nounistd
%option bison-bridge

LPAREN      "("
RPAREN      ")"
LPARENC     "{"
RPARENC     "}"
PLUS        "+"
MULTIPLY    "*"
DIVIDE      "/"
MINUS       "-"
DEFINE    ":="
SEMICOLON	";"
PRINTLN       "println"
FUNC		"func"
MAINFUNC	"func main"

NUMBER      [0-9]+
WS          [ \r\n\t]*
FLOAT       [0-9]*\.[0-9]+
IDENTIFIER  [a-zA-Z]+[a-zA-Z0-9]*

%%
{PRINTLN}       { return TOKEN_PRINT; } /* needs to be ABOVE identifier rule, so that println is not confused with an identifier */
{FUNC}			{ return TOKEN_FUNCDEF; }
{MAINFUNC}			{ return TOKEN_MAINFUNC; }

{WS}            { /* Skip blanks. */ }
{NUMBER}        { sscanf(yytext, "%d", &yylval->value); return TOKEN_NUMBER; }
{FLOAT}         { sscanf(yytext, "%f", &yylval->fvalue); return TOKEN_FLOAT; }
{IDENTIFIER}    { /*sscanf(yytext, "%s", &yylval->svalue);*/ yylval->svalue = strdup(yytext);
					/* memory leak! http://flex.sourceforge.net/manual/A-Note-About-yytext-And-Memory.html*/ 
					return TOKEN_IDENTIFIER; 
				}
{SEMICOLON}		{ return TOKEN_SEMICOLON; }
{DEFINE}        { return TOKEN_DEKLERATION; }
{MULTIPLY}      { return TOKEN_MULTIPLY; }
{PLUS}          { return TOKEN_PLUS; }
{MINUS}         { return TOKEN_MINUS; }
{DIVIDE}        { return TOKEN_DIVIDE; }
{LPAREN}        { return TOKEN_LPAREN; }
{RPAREN}        { return TOKEN_RPAREN; }
{LPARENC}       { return TOKEN_LPARENC; }
{RPARENC}       { return TOKEN_RPARENC; }
.               {  }

%%

int yyerror(const char *msg) {
    fprintf(stderr,"Error:%s\n",msg); return 0;
}
