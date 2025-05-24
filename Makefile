
all: parser

parser: lex.yy.c parser.tab.c ast.c symbol_table.c
	gcc -o parser lex.yy.c parser.tab.c ast.c symbol_table.c -lfl

lex.yy.c: scanner.l
	flex -o lex.yy.c scanner.l

parser.tab.c parser.tab.h: parser.y
	bison -d parser.y -o parser.tab.c

clean:
	rm -f parser lex.yy.c parser.tab.c parser.tab.h
