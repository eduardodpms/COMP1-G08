# Projeto de Compiladores - G08

## Introdução

Projeto do grupo 8 na disciplina de Compiladores 1 do professor Sérgio, na Universidade de Brasília (FCTE).

## Como executar

### > Gerando o binário

1. Clone o repositório e execute o seguinte comando:
```bash
make # Processa os arquivos e gera o binário bin/parser
```

Esse comando irá, automaticamente, executar os seguintes comandos e gerar um binário em `bin/parser`:

```bash
rm -f bin/parser output.c src/parser.tab.c src/parser.tab.h src/lex.yy.c # Apaga os arquivos, caso já existam
mkdir -p src bin # Cria as pasta src/ e bin/
bison -d -o src/parser.tab.c    parser/parser.y # Gera os arquivos do parser
flex -o src/lex.yy.c    lexer/lexer.l # Gera o arquivo do lexer
gcc -o bin/parser    src/parser.tab.c src/lex.yy.c # Compila em um binário em bin/parser
```

2. Depois, execute o binário com o *path* do arquivo *TypeScript* que se deseja compilar:

```bash
./bin/parser  tests/file.ts # Executa o arquivo binário gerado
```

O código em *C* produzido será salvo em `output.c`.

### > Apagando os arquivos

1. Para simplesmente apagar os arquivos gerados, execute o seguinte comando:

```bash
make clean # Apaga os arquivos gerados
```

Essa entrada irá executar o seguinte comando:

```bash
rm -f bin/parser  src/parser.tab.c  src/parser.tab.h  src/lex.yy.c # Apaga os arquivos gerados
```


## Membros da Equipe


<div align="center"><table>
  <tr>
    <td align="center"><a href="https://github.com/eduardodpms"><img src="https://avatars.githubusercontent.com/eduardodpms" width="100px" style="border-radius: 50%;"><br/>Eduardo de Pina<br/></a></td>
    <td align="center"><a href="https://github.com/cwtshh"><img src="https://avatars.githubusercontent.com/cwtshh" width="100px" style="border-radius: 50%;"><br/>Gustavo Costa<br/></a></td>
    <td align="center"><a href="https://github.com/JMPNascimento"><img src="https://avatars.githubusercontent.com/JMPNascimento" width="100px" style="border-radius: 50%;"><br/>João Maurício<br/></a></td>
    <td align="center"><a href="https://github.com/AguionStryke"><img src="https://avatars.githubusercontent.com/AguionStryke" width="100px" style="border-radius: 50%;"><br/>Lucas Freire<br/></a></td>
    <td align="center"><a href="https://github.com/luisa12ll"><img src="https://avatars.githubusercontent.com/luisa12ll" width="100px" style="border-radius: 50%;"><br/>Luísa de Souza<br/></a></td>
  </tr>
</table></div>
