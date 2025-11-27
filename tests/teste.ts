// test_ok.ts
// Teste completo sem erros — cobre toda a gramática válida do compilador TS->C

/* ---------------------------
   Declarações válidas
   --------------------------- */
let a: number = 10;
const b: number = 3;
var s: string = "hello";
let flag: boolean = a == 10;
let msg: string = "world";
let x: number = a + b * (2 + 1);
let y: number = (a + b) * 2 + (5 % 3);
let z: number = x / (b - 1) - 4;

/* comparadores */
let c1: boolean = a == b;
let c2: boolean = a != b;
let c3: boolean = a < b;
let c4: boolean = x >= y;

/* incremento / decremento */
let i: number = 0;
i++;
i--;

/* reatribuições válidas */
a = a + 1;
s = "text";

/* console.log */
console.log(s);
console.log("literal");

/* ---------------------------
   If / else
   --------------------------- */
if (a > 0) {
  console.log("a positivo");
} else {
  console.log("a nao-positivo");
}

if (flag) {
  s = "flag true";
}

/* ---------------------------
   While
   --------------------------- */
let n: number = 3;
while (n > 0) {
  console.log(n);
  n = n - 1;
}

/* ---------------------------
   For (vários formatos)
   --------------------------- */

/* init como declaração */
for (let j: number = 0; j < 3; j = j + 1) {
  console.log(j);
}

/* init como expr */
let k: number = 0;
for (k = 0; k < 2; k = k + 1) {
  console.log(k);
}

/* init vazio */
let w: number = 0;
for (; w < 2; w = w + 1) {
  console.log("loop vazio init");
}

/* ---------------------------
   Switch / Case / Default
   --------------------------- */
let code: number = 2;

switch (code) {
  case 1:
    console.log("um");
  case 2:
    console.log("dois");
    break;
  default:
    console.log("default");
}

/* ---------------------------
   Bloco simples
   --------------------------- */
{
  let inner: number = 7;
  console.log(inner);
}

/* ---------------------------
   Expressão mais complexa
   --------------------------- */
let complex: number = ((a + b) * (x - y)) % 5;

/* ---------------------------
   Identificador usado como valor
   --------------------------- */
let copy: number = a;

/* ---------------------------
   console.log(expr complexa)
   --------------------------- */
console.log(a + b * 3 - (x % 2));
