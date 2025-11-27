#!/bin/bash

COMPILADOR="./bin/parser" 
TEST_DIR="./tests"
OUTPUT_BASE="$TEST_DIR/test-output"

GREEN="\033[0;32m"
RED="\033[0;31m"
NC="\033[0m"

PASS_COUNT=0
FAIL_COUNT=0

if [ ! -x "$COMPILADOR" ]; then
    echo -e "${RED}Erro: Compilador não encontrado em '$COMPILADOR'${NC}"
    echo "Rode 'make' primeiro."
    exit 1
fi

# Prepara os diretórios de output
if [ -d "$OUTPUT_BASE" ]; then
    rm -rf "$OUTPUT_BASE"
fi
mkdir -p "$OUTPUT_BASE/valid"
mkdir -p "$OUTPUT_BASE/invalid"

echo "Iniciando testes do compilador..."
echo "Os resultados detalhados estarão em: $OUTPUT_BASE"

# Função auxiliar para organizar o output
processar_teste() {
    local test_file=$1
    local expect_success=$2
    local category=$3
    
    # Extrai o nome do arquivo sem extensão
    local filename=$(basename -- "$test_file")
    local testname="${filename%.*}"
    
    # Cria a pasta específica dentro da categoria
    local test_output_dir="$OUTPUT_BASE/$category/$testname"
    mkdir -p "$test_output_dir"
    
    # Define o nome do arquivo de log baseado no tipo de teste
    local log_filename=""
    if [ "$expect_success" = "true" ]; then
        log_filename="codigo_intermediario.txt"
    else
        log_filename="logs_compilacao.txt"
    fi
    
    # Executa o compilador e salva o output no arquivo correto
    $COMPILADOR "$test_file" > "$test_output_dir/$log_filename" 2>&1
    local exit_code=$?
    
    # Se gerou um arquivo saida.c, move para a pasta do teste
    if [ -f "saida.c" ]; then
        mv "saida.c" "$test_output_dir/"
    fi
    
    # Verifica o resultado
    if [ "$expect_success" = "true" ]; then
        if [ $exit_code -eq 0 ]; then
            echo -e "${GREEN}PASS:${NC} $test_file"
            ((PASS_COUNT++))
        else
            echo -e "${RED}FAIL:${NC} $test_file (Deveria passar, mas falhou)"
            ((FAIL_COUNT++))
        fi
    else
        # Espera falha (testes inválidos)
        if [ $exit_code -ne 0 ]; then
            echo -e "${GREEN}PASS:${NC} $test_file (Falhou como esperado)"
            ((PASS_COUNT++))
        else
            echo -e "${RED}FAIL:${NC} $test_file (Deveria falhar, mas passou)"
            ((FAIL_COUNT++))
        fi
    fi
}

# --- 1. Testes Válidos ---
echo ""
echo "--- Testando casos VÁLIDOS (devem passar) ---"
if ls $TEST_DIR/valid/*.ts 1> /dev/null 2>&1; then
    for test_file in $TEST_DIR/valid/*.ts; do
        processar_teste "$test_file" "true" "valid"
        echo "---------------------" 
    done
else
    echo "Nenhum teste válido encontrado em $TEST_DIR/valid/"
fi

# --- 2. Testes Inválidos ---
echo ""
echo "--- Testando casos INVÁLIDOS (devem falhar) ---"
if ls $TEST_DIR/invalid/*.ts 1> /dev/null 2>&1; then
    for test_file in $TEST_DIR/invalid/*.ts; do
        processar_teste "$test_file" "false" "invalid"
        echo "---------------------" 
    done
else
    echo "Nenhum teste inválido encontrado em $TEST_DIR/invalid/"
fi

# --- Resumo ---
echo ""
echo "--- Resumo dos Testes ---"
echo -e "Total: $((PASS_COUNT + FAIL_COUNT)) testes"
echo -e "${GREEN}Passaram:${NC} $PASS_COUNT"
echo -e "${RED}Falharam:${NC} $FAIL_COUNT"
echo "Outputs gerados em: $OUTPUT_BASE"

if [ $FAIL_COUNT -gt 0 ]; then
    exit 1
else
    exit 0
fi