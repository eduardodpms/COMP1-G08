# Detecta o sistema operacional
# ("Linux"=Linux, "Darwin"=Mac, "Windows_NT"=Windows)
UNAME_S := $(shell uname -s)

# Nome do executável final
EXEC = bin/parser

# Diretórios a serem gerados
SRC_DIR = src
BIN_DIR = bin

# Arquivos-fonte do Bison e do Flex
BISON_FILE = parser/parser.y
FLEX_FILE  = lexer/lexer.l

# Arquivos que o Bison vai gerar
BISON_C   = src/parser.tab.c
BISON_H   = src/parser.tab.h

# Arquivo gerado pelo Flex
FLEX_C    = src/lex.yy.c

# Arquivo de saída do compilador
OUTPUT_C = output.c

# Parâmetros opcionais ao Bison e Flex
BISON_FLAGS = -d -o # -d gera o arquivo .h (token definitions), -o define o diretório de saída
FLEX_FLAGS  = -o # -o define o diretório de saída

# Parâmetros de compilação
CC      = gcc
CFLAGS  = -o # -o define o diretório de saída

ifeq ($(OS),Linux) # biblioteca do Flex (Linux)
	LDFLAGS = -lfl
endif

ifeq ($(OS),Windows_NT) # biblioteca do Flex (Windows)
	LDFLAGS = -lfl
endif

ifeq ($(OS),Darwin)	# biblioteca do Flex (Mac)
    LDFLAGS = -ll
endif

# Regra padrão (alvo 'all' vai gerar o executável)
all: $(EXEC)

# Regra para gerar o executável: depende dos arquivos gerados por Bison e Flex
$(EXEC): clean dir $(FLEX_C) $(BISON_C)
	$(CC) $(CFLAGS) $@  $(BISON_C) $(FLEX_C) $(LDFLAGS)

# Regra para rodar o Flex: gera lex.yy.c
$(FLEX_C): $(FLEX_FILE)
	flex $(FLEX_FLAGS) $(FLEX_C)  $(FLEX_FILE)

# Regra para rodar o Bison: gera parser.tab.c e parser.tab.h
$(BISON_C) $(BISON_H): $(BISON_FILE)
	bison $(BISON_FLAGS) $(BISON_C)  $(BISON_FILE)

# Cria as pastas src/ e bin/ se não existirem
dir:
	mkdir -p $(SRC_DIR) $(BIN_DIR)

# Regra de limpeza: remove arquivos gerados
clean:
	rm -f $(EXEC) $(OUTPUT_C) $(BISON_C) $(BISON_H) $(FLEX_C)
