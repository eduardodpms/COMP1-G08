#!/bin/bash

# --- Configuração ---
# Verifique se este é o comando correto para executar seu compilador
COMPILADOR="./compilador_final" # <-- Coloque o nome real aqui
TEST_DIR="./tests"

# Cores para facilitar a leitura
GREEN="\033[0;32m"
RED="\033[0;31m"
NC="\033[0m" # Sem Cor

PASS_COUNT=0
FAIL_COUNT=0

# Garante que o compilador existe e é executável
if [ ! -x "$COMPILADOR" ]; then
    echo -e "${RED}Erro: Compilador não encontrado ou não é executável em '$COMPILADOR'${NC}"
    echo "Verifique o caminho e rode 'chmod +x $COMPILADOR' se necessário."
    exit 1
fi

echo "Iniciando testes do compilador..."

# --- 1. Testes Válidos ---
# Devem compilar sem erros (exit code 0)
echo ""
echo "--- Testando casos VÁLIDOS (devem passar) ---"
for test_file in $TEST_DIR/valid/*.ts; do
    # Roda o compilador, joga toda a saída (stdout e stderr) para o buraco negro
    # Estamos interessados apenas no código de saída ($?)
    if $COMPILADOR < "$test_file" > /dev/null 2>&1; then
        echo -e "${GREEN}PASS:${NC} $test_file"
        ((PASS_COUNT++))
    else
        echo -e "${RED}FAIL:${NC} $test_file (Deveria passar, mas falhou)"
        ((FAIL_COUNT++))
    fi
done

# --- 2. Testes Inválidos ---
# NÃO devem compilar (exit code != 0)
echo ""
echo "--- Testando casos INVÁLIDOS (devem falhar) ---"
for test_file in $TEST_DIR/invalid/*.ts; do
    # Usamos '!' para inverter a lógica. O teste passa se o compilador FALHAR.
    if ! $COMPILADOR < "$test_file" > /dev/null 2>&1; then
        echo -e "${GREEN}PASS:${NC} $test_file (Falhou como esperado)"
        ((PASS_COUNT++))
    else
        echo -e "${RED}FAIL:${NC} $test_file (Deveria falhar, mas passou)"
        ((FAIL_COUNT++))
    fi
done

# --- Resumo ---
echo ""
echo "---------------------"
echo "--- Resumo dos Testes ---"
echo -e "Total: $((PASS_COUNT + FAIL_COUNT)) testes"
echo -e "${GREEN}Passaram:${NC} $PASS_COUNT"
echo -e "${RED}Falharam:${NC} $FAIL_COUNT"
echo "---------------------"

# Retorna um código de saída geral (útil para CIs)
if [ $FAIL_COUNT -gt 0 ]; then
    exit 1
else
    exit 0
fi