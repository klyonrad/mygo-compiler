%{
  float yylfval = 0.0f;
  int yylval = 0;
  typedef enum{ DEFINEOP=0, EQUALS, PLUS, MINUS, TIMES, DIVIDE, SEMICOLON, PARENTHESISLEFT, PARENTHESISRIGHT,
    IDENTIFIER = 100,
    INTEGER=200, FLOAT, STRING,
    UNKNOWN=999, FINISHED=999999
  } tokenType;
%}

digit                       [0-9]
letter                      [a-zA-Z]

%%
"("                         {return PARENTHESISLEFT;}
")"                         {return PARENTHESISRIGHT;}
":="                        {return DEFINEOP;}
"="                         {return EQUALS;}
"+"                         {return PLUS;}
"-"                         {return MINUS;}
"*"                         {return TIMES;}
"/"                         {return DIVIDE;}
";"                         {return SEMICOLON;}
\".*\"                      {return STRING;}
{letter}({letter}|{digit})* {return IDENT;}
[-+]?[0-9]+                 {yylval = atoi(yytext); return INTEGER;}
[-+]?[0-9]*\.?[0-9]+        {yylfval = atof(yytext); return FLOAT;}
[ \t\n\r]
.                           {printf("Unknown character '%c' ", yytext[0]); return UNKNOWN;}
<<EOF>>                     {return FINISHED;}
%%
int main(int argc, char *argv[]){
  //yyin = ((argc == 2)? fopen(argv[1], "r"):stdin);

  int tok = -1;
  while(tok != FINISHED){
    tok = yylex();
    if(tok == INTEGER)
      printf("%d, val: %d\n", tok, yylval);
    else if(tok == FLOAT)
      printf("%d, val: %f\n", tok, yylfval);
    else
      printf("%d \n", tok);
    //cout << "Token = " << tok << ",\t last value of yylval = " << yyval << endl;
  }
  return 0;
}
