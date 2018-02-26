all: first

C_FILES = \
	lexer.c \
	parser.c \
	ast.c \
	symtab.c \
	interp.c \

C_MAIN = \
	homer.c \

# C pre-processor flags
# CPPFLAGS += -DAST_DEBUG # debug AST operations

# C compiler flags
CFLAGS += -std=c89      # use ANSI C
CFLAGS += -Wall -Wextra # warn a lot
CFLAGS += -Wno-comment  # allow // comments

# C compiler / linker flags
ALL_FLAGS += -g # compile / link for debugging

# special C compiler flags for lexer.c, to avoid these:
# lexer.c:1262:17: warning: unused function 'yyunput' [-Wunused-function]
# lexer.c:1303:16: warning: function 'input' is not needed and will not be emitted [-Wunneeded-internal-declaration]
C_LEXER_FLAGS += -Wno-unused-function
C_LEXER_FLAGS += -Wno-unneeded-internal-declaration

###################################################
# no need to modify below this line
###################################################

CC = cc
EXE_MAIN = $(C_MAIN:.c=)
O_FILES = $(C_FILES:.c=.o)
O_MAIN  = $(C_MAIN:.c=.o)

first: $(EXE_MAIN)

%.o : %.c
	$(CC) -c $(ALL_FLAGS) $(CFLAGS) $(CPPFLAGS) -o $@ $<

$(EXE_MAIN): $(O_FILES) $(O_MAIN)
	$(CC) $(ALL_FLAGS) $^ -o $@

lexer.c: lexer.l
	flex -o lexer.c lexer.l

parser.c parser.h: parser.y
	bison -v -d -o parser.c parser.y

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
