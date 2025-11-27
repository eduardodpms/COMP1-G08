let sum: number = 0;
let i: number = 0;
let temp: number = 0;

for (i = 0; i < 5; i = i + 1) {
    temp = i * 4;
    sum = sum + temp;
}

console.log(sum);