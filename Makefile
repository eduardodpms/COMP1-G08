# Makefile portátil para projeto bison+flex (arquivos em parser/ e lexer/)
UNAME_S := $(shell uname -s)

# Diretórios
SRC_DIR := src
BIN_DIR := bin

# Executável final
EXEC := $(BIN_DIR)/parser

# Fontes estão nas pastas originais
BISON_FILE := parser/parser.y
FLEX_FILE  := lexer/lexer.l

# Arquivos gerados
BISON_C := $(SRC_DIR)/parser.tab.c
BISON_H := $(SRC_DIR)/parser.tab.h
FLEX_C  := $(SRC_DIR)/lex.yy.c

# Compilador e flags
ifeq ($(UNAME_S),Darwin)
	CC ?= clang
	LDFLAGS ?= -ll
else
	CC ?= gcc
	LDFLAGS ?= -lfl
endif

# Compilação padrão (sem símbolos de debug)
CFLAGS ?= -std=c11 -Wall -Wextra -O2

.PHONY: all clean dir debug rebuild

# Build padrão
all: $(EXEC)

# Gera diretórios se necessário
dir:
	@mkdir -p $(SRC_DIR) $(BIN_DIR)

# Bison -> gera parser.tab.c e parser.tab.h em src/
$(BISON_C) $(BISON_H): $(BISON_FILE) | dir
	bison -d -o $(BISON_C) $(BISON_FILE)
	@echo "[bison] gerado: $(BISON_C) $(BISON_H)"

# Flex -> depende do header do bison para tokens
$(FLEX_C): $(FLEX_FILE) $(BISON_H) | dir
	flex -o $(FLEX_C) $(FLEX_FILE)
	@echo "[flex] gerado: $(FLEX_C)"

# Link / compile: bison antes do flex na linha de compilação
$(EXEC): $(BISON_C) $(BISON_H) $(FLEX_C) | dir
	$(CC) $(CFLAGS) $(BISON_C) $(FLEX_C) -o $(EXEC) $(LDFLAGS)
	@echo "[build] executavel: $(EXEC)"
	@rm -rf $(EXEC).dSYM 2>/dev/null || true

# Regra de debug (mantém -g para uso opcional)
debug: $(BISON_C) $(BISON_H) $(FLEX_C) | dir
	$(CC) -std=c11 -Wall -Wextra -g $(BISON_C) $(FLEX_C) -o $(EXEC) $(LDFLAGS)
	@echo "[debug build] executavel: $(EXEC)"

# Rebuild completo
rebuild: clean all

# Limpeza
clean:
	-rm -f $(EXEC) $(BISON_C) $(BISON_H) $(FLEX_C) output.c
	-rm -rf $(EXEC).dSYM
	@echo "[clean] removidos binarios e gerados"
