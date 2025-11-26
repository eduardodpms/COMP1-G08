let i: number = 0;
let sum: number = 0;

while (i < 5) {
    sum = sum + i;
    i = i + 1;
}

for (let j: number = 0; j < 3; j = j + 1) {
    if (j == 1) continue;
    sum = sum + j;
}

console.log(sum);