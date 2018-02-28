all: first

NAME = homer

C_FILES = \
	lexer.c \
	parser.c \
	ast.c \
	symtab.c \
	interp.c \
	log.c \
	homer.c \

C_MAIN = \
	main.c \

# C pre-processor flags
# CPPFLAGS += -DHOMER_LOG # debug homer

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
EXE = $(NAME)
O_FILES = $(C_FILES:.c=.o)
O_MAIN  = $(C_MAIN:.c=.o)

first: $(EXE)

%.o : %.c
	$(CC) -c $(ALL_FLAGS) $(CFLAGS) $(CPPFLAGS) -o $@ $<

$(EXE): $(O_FILES) $(O_MAIN)
	$(CC) $(ALL_FLAGS) $^ -o $@

lexer.c lexer.h: lexer.l
	flex --header-file=lexer.h --outfile=lexer.c lexer.l

parser.c parser.h: parser.y
	bison --verbose --output-file=parser.c --defines=parser.h parser.y

lexer.o: lexer.c parser.h
	$(CC) -c $(ALL_FLAGS) $(CFLAGS) $(CPPFLAGS) $(C_LEXER_FLAGS) -o $@ $<

parser.o: parser.c parser.h
ast.o: ast.c
symtab.o: symtab.c
interp.o: interp.c parser.h
log.o: log.c
homer.o: homer.c parser.h

clean:
	rm -f parser.c parser.h parser.dot parser.output
	rm -f lexer.c lexer.h
	rm -f $(EXE) *.o *~
