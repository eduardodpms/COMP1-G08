// ==========================================
// TESTE DE CAOS SEMANTICO (Deve falhar!)
// ==========================================

// Cenario 1: Mistura de Tipos na Declaracao
let numero: number = 10;
let texto: string = "Ola";
let boleano: boolean = true;

// ERRO 1: Atribuir string para boolean
let teste1: boolean = "nao pode";

// ERRO 2: Atribuir number para string
let teste2: string = 500;

// Cenario 2: Operacoes Invalidas
// ERRO 3: Multiplicacao de string (so + é permitido para string)
let resultado: number = texto * 5;

// ERRO 4: Subtracao de booleano
let erroMath: number = boleano - 10;

// ERRO 5: Incremento em string
texto++;

// Cenario 3: Escopo e Declaracao
if (numero > 0) {
  let escopoLocal: number = 99;
  escopoLocal = escopoLocal + 1;
}

// ERRO 6: Usar variavel fora do seu escopo (escopoLocal morreu no fecha chaves acima)
escopoLocal = 100;

// ERRO 7: Variavel que nunca foi declarada
fantasma = 50;

// ERRO 8: Atribuicao incompativel em reatribuicao
numero = "agora virei texto";

// ERRO 9: Divisao de string
let div: number = "A" / "B";
