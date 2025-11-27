// Teste de Aritmética e Precedência
let a: number = 10;
let b: number = 20;
let c: number = 5;

// Esperado: 10 + 100 = 110
let res1: number = a + b * c;
console.log(res1);

// Esperado: (30) * 5 = 150
let res2: number = (a + b) * c;
console.log(res2);

// Esperado: 4 + 1 = 5
let res3: number = 24 / (c + 1) + 1;
console.log(res3);

// Teste de módulo
let res4: number = 10 % 3; // Esperado: 1
console.log(res4);
