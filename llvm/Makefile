# Makefile

FILES	= Lexer.c Parser.c Expression.c main.c
CC	= clang++
CFLAGS	= -g -O3 -std=c++11 -DDEBUG -w
LLVMFLAGS = `llvm-config --cxxflags --ldflags --libs core`

all:		$(FILES)
		$(CC) $(FILES) $(CFLAGS) 

test:		$(FILES)
		$(CC) $(CFLAGS) $(FILES) $(LLVMFLAGS) -o test 

Lexer.c:	Lexer.l
		flex Lexer.l

Parser.c:	Parser.y Lexer.c
		bison -t Parser.y

clean:
		rm -f *.o *~ Lexer.c Lexer.h Parser.c Parser.h test
