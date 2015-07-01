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
%token TOKEN_LPARENC
%token TOKEN_RPARENC
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
    : id[ID] TOKEN_DEKLERATION expr[EXPR] { $$ = new DeklarationExpression($ID, $EXPR);}
    | id[ID] TOKEN_LPAREN TOKEN_RPAREN TOKEN_LPARENC expr[EXPR] TOKEN_RPARENC { $$ = new FunctionExpression($ID, $EXPR);}
    | expr[L] TOKEN_PLUS expr[R] { $$ = new BiOperationExpression( ePLUS, $L, $R ); }
    | expr[L] TOKEN_MINUS expr[R] { $$ = new BiOperationExpression( eMINUS, $L, $R ); }
    | expr[L] TOKEN_MULTIPLY expr[R] { $$ = new BiOperationExpression( eMULTIPLY, $L, $R ); }
    | expr[L] TOKEN_DIVIDE expr[R] { $$ = new BiOperationExpression( eDIVIDE, $L, $R ); }
    | TOKEN_LPAREN expr[EXPR] TOKEN_RPAREN { $$ = $EXPR; }
    | TOKEN_NUMBER[NUM] { $$ = new IntegerExpression($NUM); }
    | TOKEN_FLOAT[NUM] { $$ = new FloatExpression($NUM); }
    | id[ID] { $$ = $ID; }
    | TOKEN_PRINT TOKEN_LPAREN expr[EXPR] TOKEN_RPAREN { $$ = new PrintExpression($EXPR); }
    ;
id
    : TOKEN_IDENTIFIER[ID] { $$ = new IdentifierExpression($ID); }
    ;

%%
