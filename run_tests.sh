COMPILADOR="./bin/parser" 
TEST_DIR="./tests"

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

echo "Iniciando testes do compilador..."

# --- 1. Testes Válidos ---
echo ""
echo "--- Testando casos VÁLIDOS (devem passar) ---"
for test_file in $TEST_DIR/valid/*.ts; do
    
    if $COMPILADOR "$test_file"; then
        echo -e "${GREEN}PASS:${NC} $test_file"
        ((PASS_COUNT++))
    else
        echo -e "${RED}FAIL:${NC} $test_file (Deveria passar, mas falhou)"
        ((FAIL_COUNT++))
    fi
    echo "---------------------" 
done

# --- 2. Testes Inválidos ---
echo ""
echo "--- Testando casos INVÁLIDOS (devem falhar) ---"
for test_file in $TEST_DIR/invalid/*.ts; do
    
    if ! $COMPILADOR "$test_file"; then
        echo -e "${GREEN}PASS:${NC} $test_file (Falhou como esperado)"
        ((PASS_COUNT++))
    else
        echo -e "${RED}FAIL:${NC} $test_file (Deveria falhar, mas passou)"
        ((FAIL_COUNT++))
    fi
    echo "---------------------" 
done

# --- Resumo ---
echo ""
echo "--- Resumo dos Testes ---"
echo -e "Total: $((PASS_COUNT + FAIL_COUNT)) testes"
echo -e "${GREEN}Passaram:${NC} $PASS_COUNT"
echo -e "${RED}Falharam:${NC} $FAIL_COUNT"

if [ $FAIL_COUNT -gt 0 ]; then
    exit 1
else
    exit 0
fi