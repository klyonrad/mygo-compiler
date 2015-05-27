# mygo-compiler

##Wie kompiliere ich den Lexer?
 
    flex mygo.lex
    gcc lex.yy.c -lfl

(getestet unter Ubuntu 14.04 mit gcc 4.8.2 und flex 2.5.35)
