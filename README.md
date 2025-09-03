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
mkdir -p src # Cria a pasta src/
bison -d -o src/parser.tab.c    parser/parser.y # Gera os arquivos do parser
flex -o src/lex.yy.c    lexer/lexer.l # Gera o arquivo do lexer
mkdir -p bin # Cria a pasta bin/
gcc  -o bin/parser src/parser.tab.c src/lex.yy.c # Compila em um binário em bin/parser
```

2. Depois, execute o binário com o seguinte comando:

```bash
./bin/parser # Executa o arquivo binário gerado
```

3. Por fim, digite os casos de teste e finalize com `EOF`.

### > Resetando os arquivos

1. Para apagar os arquivos gerados, execute o seguinte comando:

```bash
make clean # Apaga os arquivos gerados
```

Essa entrada irá executar o seguinte comando:

```bash
rm -f bin/parser  src/parser.tab.c  src/parser.tab.h  src/lex.yy.c # Apaga os arquivos gerados
```

2. Se quiser gerar os arquivos novamente, basta executar:

```bash
make # Processa os arquivos e gera o binário bin/parser
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
