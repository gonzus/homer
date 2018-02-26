C_FILES = \
	lexer.c \
	parser.c \
	ast.c \
	symtab.c \
	interp.c \

C_MAIN = \
	homer.c \

EXE_MAIN = \
	$(C_MAIN:.c=)

all: $(EXE_MAIN)

CC = cc

# CPPFLAGS  += -DAST_DEBUG # debug AST operations

CFLAGS    += -std=c89 # use ANSI C
CFLAGS    += -Wall # warn a lot
CFLAGS    += -Wno-comment # allow // comments

ALL_FLAGS += -g # compile / link for debugging

# C compiler flags for lexer.c
C_LEXER_FLAGS += \
	-Wno-unused-function \
	-Wno-unneeded-internal-declaration \

O_FILES = $(C_FILES:.c=.o)
O_MAIN  = $(C_MAIN:.c=.o)

%.o : %.c
	$(CC) -c $(ALL_FLAGS) $(CFLAGS) $(CPPFLAGS) -o $@ $<

$(EXE_MAIN): $(O_FILES) $(O_MAIN)
	$(CC) $(ALL_FLAGS) $^ -o $@

lexer.c: lexer.l
	flex -o lexer.c lexer.l

parser.c parser.h: parser.y
	bison -v -d -o parser.c parser.y

# special rule to compile lexer.c
lexer.o: lexer.c parser.h
	$(CC) -c $(ALL_FLAGS) $(CFLAGS) $(CPPFLAGS) $(C_LEXER_FLAGS) -o $@ $<

parser.o: parser.c parser.h
ast.o: ast.c
symtab.o: symtab.c
interp.o: interp.c
homer.o: homer.c

clean:
	rm -f parser.c parser.h parser.dot parser.output
	rm -f lexer.c
	rm -f $(EXE_MAIN) *.o *~
