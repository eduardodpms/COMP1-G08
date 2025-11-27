// Teste de Loops
console.log("--- While ---");
let i: number = 0;
while (i < 3) {
  console.log(i);
  i++;
}

console.log("--- For Basico ---");
for (let j: number = 0; j < 3; j = j + 1) {
  console.log(j);
}

console.log("--- Break/Continue ---");
let k: number = 0;
while (k < 10) {
  k++;
  if (k == 2) {
    continue;
  }
  if (k == 5) {
    break;
  }
  console.log(k);
}
// Esperado: 1, 3, 4
