UNAME_S := $(shell uname -s)

CC       := gcc
INCDIRS  := -I./parser -I./src
CFLAGS   := -g -Wall -Wextra $(INCDIRS) -fno-common

FLEX_LIBDIR ?=

LDLIBS :=
ifeq ($(UNAME_S),Linux)
	LDLIBS += -lfl
endif

ifeq ($(wildcard $(FLEX_LIBDIR)/libfl.*),$(FLEX_LIBDIR)/libfl.*)
	LDLIBS += -L$(FLEX_LIBDIR) -lfl
endif

LEXER    := lexer/lexer.l
BISON    := parser/parser.y
BISON_C  := src/parser.tab.c
BISON_H  := src/parser.tab.h
FLEX_C   := src/lex.yy.c

# fontes manuais
PARSER_SRCS := parser/ast.c parser/codegen.c parser/tabela.c parser/otimizador.c
# fontes gerados pelo flex/bison
SRC_SRCS := src/parser.tab.c src/lex.yy.c

OBJS := $(PARSER_SRCS:.c=.o) $(SRC_SRCS:.c=.o)

BIN_DIR := bin
EXEC := $(BIN_DIR)/parser

.PHONY: all clean dir debug rebuild

all: $(EXEC)

$(EXEC): dir $(BISON_C) $(FLEX_C) $(OBJS)
	$(CC) $(CFLAGS) -o $@ $(OBJS) $(LDLIBS)

%.o: %.c $(BISON_H)
	$(CC) $(CFLAGS) -c -o $@ $<

$(FLEX_C): $(LEXER)
	mkdir -p src
	flex -o $(FLEX_C) $(LEXER)

$(BISON_C) $(BISON_H): $(BISON)
	mkdir -p src
	bison -d -o $(BISON_C) $(BISON)

dir:
	mkdir -p $(BIN_DIR) src

debug: CFLAGS := -g -Wall -Wextra $(INCDIRS) -fno-common
debug: clean all

rebuild: clean all

clean:
	rm -rf $(BIN_DIR)/*
	rm -f src/*.o parser/*.o parser/*.o src/parser.tab.c src/parser.tab.h src/lex.yy.c
	@echo "[clean] removidos binarios e gerados"
