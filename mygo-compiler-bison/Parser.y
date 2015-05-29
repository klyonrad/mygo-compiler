%{

/*
 * Parser.y file
 * To generate the parser run: "bison Parser.y"
 */

#include "Expression.h"
#include "Parser.h"
#include "Lexer.h"
#include <stdio.h>

int yyerror(SExpression **expression, yyscan_t scanner, const char *msg) {
  printf("%s", msg);
    // Add error handling routine as needed
}

%}

%code requires {

#ifndef YY_TYPEDEF_YY_SCANNER_T
#define YY_TYPEDEF_YY_SCANNER_T
typedef void* yyscan_t;
#endif

}

%output  "Parser.c"
%defines "Parser.h"

%define api.pure
%lex-param   { yyscan_t scanner }
%parse-param { SExpression **expression }
%parse-param { yyscan_t scanner }

%union {
    int value;
    float fvalue;
    char* svalue;
    SExpression *expression;
}

%left '+' TOKEN_PLUS
%left '-' TOKEN_MINUS
%left '*' TOKEN_MULTIPLY
%left '/' TOKEN_DIVIDE

%token <svalue> TOKEN_IDENTIFIER
%token TOKEN_LPAREN
%token TOKEN_RPAREN
%token TOKEN_PLUS
%token TOKEN_MINUS
%token TOKEN_MULTIPLY
%token TOKEN_DIVIDE
%token <value> TOKEN_NUMBER
%token <fvalue> TOKEN_FLOAT
%token TOKEN_DEKLERATION;

%type <expression> expr
%type <expression> id

%%

input
    : expr { *expression = $1; }
    ;

expr
    : id TOKEN_DEKLERATION expr { $$ = createDekleration($1, $3);}
    | expr[L] TOKEN_PLUS expr[R] { $$ = createOperation( ePLUS, $L, $R ); }
    | expr[L] TOKEN_MINUS expr[R] { $$ = createOperation( eMINUS, $L, $R ); }
    | expr[L] TOKEN_MULTIPLY expr[R] { $$ = createOperation( eMULTIPLY, $L, $R ); }
    | expr[L] TOKEN_DIVIDE expr[R] { $$ = createOperation( eDIVIDE, $L, $R ); }
    | TOKEN_LPAREN expr[E] TOKEN_RPAREN { $$ = $E; }
    | TOKEN_NUMBER { $$ = createNumber($1); }
    | TOKEN_FLOAT { $$ = createFloat($1); }
    ;

id
	: TOKEN_IDENTIFIER { $$ = createVar($1); }

%%
