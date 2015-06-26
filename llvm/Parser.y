%{

/*
 * Parser.y file
 * To generate the parser run: "bison Parser.y"
 */

#include "Expression.h"
#include "Parser.h"
#include "Lexer.h"
#include <stdio.h>

int yyerror(Expression **expression, yyscan_t scanner, const char *msg) {
  std::cout << msg << endl;
  //printf("%s", msg);
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

%define api.pure full
%define parse.error verbose
%lex-param   { yyscan_t scanner }
%parse-param { Expression **expression }
%parse-param { yyscan_t scanner }

%union {
    int value;
    float fvalue;
    char* svalue;
    Expression* expression;
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
%token TOKEN_SEMICOLON;
%token TOKEN_PRINT;

%type <expression> expr
%type <expression> id
%type <expression> line
%type <expression> input

%%

input
    : line { *expression = $1; }
    ;
line
    : expr TOKEN_SEMICOLON line { $$ = new LineExpression($1, $3); }
    | { $$ = new EOFExpression(); }
    ;	
expr
    : id TOKEN_DEKLERATION expr { $$ = new DeklarationExpression($1, $3);}
    | expr[L] TOKEN_PLUS expr[R] { $$ = new BiOperationExpression( ePLUS, $L, $R ); }
    | expr[L] TOKEN_MINUS expr[R] { $$ = new BiOperationExpression( eMINUS, $L, $R ); }
    | expr[L] TOKEN_MULTIPLY expr[R] { $$ = new BiOperationExpression( eMULTIPLY, $L, $R ); }
    | expr[L] TOKEN_DIVIDE expr[R] { $$ = new BiOperationExpression( eDIVIDE, $L, $R ); }
    | TOKEN_LPAREN expr[E] TOKEN_RPAREN { $$ = $E; }
    | TOKEN_NUMBER { $$ = new IntegerExpression($1); }
    | TOKEN_FLOAT { $$ = new FloatExpression($1); }
    | id { $$ = $1; }
    | TOKEN_PRINT TOKEN_LPAREN expr TOKEN_RPAREN { $$ = new PrintExpression($3); }
	;

id
        : TOKEN_IDENTIFIER { $$ = new IdentifierExpression($1); }
	;

%%
