let contador: number = 1;
let resultado: string = "";

while (contador <= 5) {
    if (contador % 2 == 0) {
        resultado = "par";
        console.log(resultado);
    } else {
        resultado = "ímpar";
        console.log(resultado);
    }
    contador = contador + 1;
}

console.log("Fim!");
