// ==========================================
// SUPER TESTE DE INTEGRACAO (TS -> C)
// ==========================================

// 1. Variaveis Globais e Tipos
let executando: boolean = true;
let totalOperacoes: number = 0;
let titulo: string = "--- CALCULADORA ESTATISTICA V1.0 ---";
let contador: number = 0;

console.log(titulo);

// 2. Loop Principal (While)
while (executando) {
  console.log("");
  console.log("Escolha uma opcao:");
  console.log("1. Calcular Fatorial (Testa For e Multiplicacao)");
  console.log("2. Verificar Par ou Impar (Testa If/Else e Modulo)");
  console.log("3. Contagem Regressiva (Testa While e Decremento)");
  console.log("4. Concatenacao de Strings (Testa Strings)");
  console.log("0. Sair");

  // Testa Input
  let opcao: number = console.read();

  // 3. Switch Case com Blocos
  switch (opcao) {
    case 1: {
      console.log("Digite um numero pequeno para fatorial:");
      let n: number = console.read();
      let fat: number = 1;

      // Testa For com variavel local
      for (let i: number = 1; i <= n; i++) {
        fat = fat * i;
      }

      console.log("Resultado do Fatorial:");
      console.log(fat);
      totalOperacoes++;
      break;
    }

    case 2: {
      console.log("Digite um numero para checar:");
      let num: number = console.read();
      let resto: number = num % 2;

      // Testa Comparacao e If/Else
      if (resto == 0) {
        let msg: string = "O numero e PAR";
        console.log(msg);
      } else {
        let msg: string = "O numero e IMPAR";
        console.log(msg);
      }
      totalOperacoes++;
      break;
    }

    case 3: {
      console.log("Iniciando contagem de 5 a 0:");
      let k: number = 5;
      // Testa While interno
      while (k >= 0) {
        console.log(k);
        k--;
      }
      totalOperacoes++;
      break;
    }

    case 4: {
      let s1: string = "Ola";
      let s2: string = "Mundo";
      // Testa precedencia na concatenação: (s1 + space) + s2
      let res: string = s1 + " " + s2 + " - Teste OK";
      console.log(res);
      totalOperacoes++;
      break;
    }

    case 0: {
      console.log("Saindo do sistema...");
      executando = false;
      break;
    }

    default: {
      console.log("Opcao invalida! Tente novamente.");
    }
  }
}

// 4. Relatório Final (Testa conversão implícita no printf se suportada ou lógica simples)
console.log("Sessao Encerrada.");
console.log("Total de operacoes realizadas:");
console.log(totalOperacoes);
