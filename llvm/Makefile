# Makefile

FILES	= Lexer.c Parser.c Expression.c main.c
CC	= clang++
CFLAGS	= -g -O3 -std=c++11 -DDEBUG `llvm-config --cppflags --ldflags --libs core`

all:		$(FILES)
		$(CC) $(CFLAGS) $(FILES)

test:		$(FILES)
		$(CC) $(CFLAGS) $(FILES) -o test

Lexer.c:	Lexer.l
		flex Lexer.l

Parser.c:	Parser.y Lexer.c
		bison -t Parser.y

clean:
		rm -f *.o *~ Lexer.c Lexer.h Parser.c Parser.h test
