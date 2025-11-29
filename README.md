
<div align="center">

# ⚡ COMPILADOR TS → C

![Status](https://img.shields.io/badge/status-concluído-brightgreen.svg)

[![TypeScript](https://img.shields.io/badge/TypeScript-3178C6?logo=typescript&logoColor=white)](https://www.typescriptlang.org/)
[![C](https://img.shields.io/badge/C-A8B9CC?logo=c&logoColor=black)](https://en.cppreference.com/w/c)
[![Flex](https://img.shields.io/badge/Flex-2C2255?logo=gnu&logoColor=white)](https://github.com/westes/flex)
[![Bison](https://img.shields.io/badge/Bison-2C2255?logo=gnu&logoColor=white)](https://www.gnu.org/software/bison/)

**Projeto da disciplina de Compiladores 1 - Universidade de Brasília**

</div>

-----
## 📋 Sobre o Projeto

Compilador desenvolvido pelo Grupo 08 na disciplina de Compiladores 1 do professor Sérgio. O projeto traduz código TypeScript para linguagem C, implementando todas as fases de um compilador tradicional.

---

## 🎯 Status do Projeto

✅ **PROJETO CONCLUÍDO** - Todas as fases implementadas com sucesso

| Componente | Status |
|------------|--------|
| 🔤 Análise Léxica | ✅ Concluída |
| 📐 Análise Sintática | ✅ Concluída |
| 🎯 Análise Semântica | ✅ Concluída |
| 💻 Geração de Código | ✅ Concluída |

-----

## 📋 Requisitos

- **Sistema**: Ambiente Unix (Linux, WSL, macOS, etc)
- **Flex**: Versão `2.6.4` ou superior
- **Bison**: Versão `3.8.2` ou superior  
- **GCC**: Versão `13.3.0` ou superior
-----

## 💻 Como Executar

```bash
# Compilar o compilador
make

# Executar com arquivo TypeScript
./parser path/to/typescript.ts

# (OPCIONAL) Compilar o código C gerado
gcc output.c -o programa
```

## 🧹 Limpando os arquivos gerados

```bash
# Limpa todos os arquivos gerados
make clean
```

## 🧪 Testes

### Executar Todos os Testes
```bash
# Executar suite completa de testes
./run_tests.sh

# Executar Testes Específicos
./bin/parser tests/valid/nome_do_teste.ts
./bin/parser tests/invalid/nome_do_teste.ts

```
----
## 🔗 Mais Informações

Para documentação completa, exemplos detalhados, estrutura do projeto e informações técnicas:

### [Acesse nossa documentação completa aqui](https://eduardodpms.github.io/COMP1-G08/)

----
## 👥 Membros da Equipe

<div align="center"><table>
  <tr>
    <td align="center"><a href="https://github.com/eduardodpms"><img src="https://avatars.githubusercontent.com/eduardodpms" width="100px" style="border-radius: 50%;"><br/>Eduardo de Pina<br/></a></td>
    <td align="center"><a href="https://github.com/cwtshh"><img src="https://avatars.githubusercontent.com/cwtshh" width="100px" style="border-radius: 50%;"><br/>Gustavo Costa<br/></a></td>
    <td align="center"><a href="https://github.com/JMPNascimento"><img src="https://avatars.githubusercontent.com/JMPNascimento" width="100px" style="border-radius: 50%;"><br/>João Maurício<br/></a></td>
    <td align="center"><a href="https://github.com/AguionStryke"><img src="https://avatars.githubusercontent.com/AguionStryke" width="100px" style="border-radius: 50%;"><br/>Lucas Freire<br/></a></td>
    <td align="center"><a href="https://github.com/luisa12ll"><img src="https://avatars.githubusercontent.com/luisa12ll" width="100px" style="border-radius: 50%;"><br/>Luísa de Souza<br/></a></td>
  </tr>
</table></div>

