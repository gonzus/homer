all: homer

homer: lexer.c parser.c ast.c interp.c homer.c main.c
	gcc -Wall -ohomer lexer.c parser.c ast.c interp.c homer.c main.c

lexer.c: lexer.l parser.c
	flex -o lexer.c lexer.l

parser.c: parser.y
	bison -v -d -o parser.c parser.y

clean:
	rm -f parser.c parser.h parser.output
	rm -f lexer.c
	rm -f homer *.o


