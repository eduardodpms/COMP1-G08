// Teste de Lógica e IF/ELSE
let x: number = 10;
let y: number = 5;

let maior: boolean = x > y;
let igual: boolean = x == 10;

if (maior) {
  console.log("X e maior que Y");
  if (igual) {
    console.log("X e igual a 10");
  }
} else {
  console.log("Erro na logica");
}

if (x < y) {
  console.log("Impossivel");
} else {
  console.log("Fluxo else funcionando");
}
