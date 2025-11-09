# Makefile para a estrutura:
#  lexer/lexer.l
#  parser/*.c, parser/*.h, parser/parser.y
#  src/  <- saída do bison/flex (parser.tab.c, parser.tab.h, lex.yy.c)
#  bin/parser <- executável final

UNAME_S := $(shell uname -s)

# compilador padrão (override via: make CC=clang)
CC       := gcc
INCDIRS  := -I./parser -I./src
CFLAGS   := -g -Wall -Wextra $(INCDIRS) -fno-common

# Diretório opcional onde uma libfl pode existir (ex: Homebrew)
# Use: make FLEX_LIBDIR=/opt/homebrew/opt/flex/lib
FLEX_LIBDIR ?=

# LDLIBS: só linkamos -lfl em Linux por padrão.
LDLIBS :=
ifeq ($(UNAME_S),Linux)
	LDLIBS += -lfl
endif

# Se usuário forneceu FLEX_LIBDIR e existe libfl lá, force link via -L
ifeq ($(wildcard $(FLEX_LIBDIR)/libfl.*),$(FLEX_LIBDIR)/libfl.*)
	LDLIBS += -L$(FLEX_LIBDIR) -lfl
endif

LEXER    := lexer/lexer.l
BISON    := parser/parser.y
BISON_C  := src/parser.tab.c
BISON_H  := src/parser.tab.h
FLEX_C   := src/lex.yy.c

# fontes "manuais" na pasta parser/
PARSER_SRCS := $(wildcard parser/*.c)
# fontes gerados e outros fontes em src/
SRC_SRCS := $(wildcard src/*.c)

# objetos (um .o para cada .c)
OBJS := $(patsubst %.c,%.o,$(PARSER_SRCS) $(SRC_SRCS))

BIN_DIR := bin
EXEC := $(BIN_DIR)/parser

.PHONY: all clean dir debug rebuild

all: $(EXEC)

# linka todos os objetos para o executável
$(EXEC): dir $(BISON_C) $(FLEX_C) $(OBJS)
	$(CC) $(CFLAGS) -o $@ $(OBJS) $(LDLIBS)

# regra genérica .c -> .o (coloca dependência no header gerado do bison)
%.o: %.c $(BISON_H)
	$(CC) $(CFLAGS) -c -o $@ $<

# gerar lex.yy.c via flex (coloca em src/)
$(FLEX_C): $(LEXER)
	mkdir -p src
	flex -o $(FLEX_C) $(LEXER)

# gerar parser.tab.c/h via bison (coloca em src/)
$(BISON_C) $(BISON_H): $(BISON)
	mkdir -p src
	bison -d -o $(BISON_C) $(BISON)

dir:
	mkdir -p $(BIN_DIR) src

# build com debug (mantém -g)
debug: CFLAGS := -g -Wall -Wextra $(INCDIRS) -fno-common
debug: clean all

rebuild: clean all

clean:
	rm -rf $(BIN_DIR)/*
	rm -f src/*.o parser/*.o parser/*.o src/parser.tab.c src/parser.tab.h src/lex.yy.c
	@echo "[clean] removidos binarios e gerados"
