
<h1 align="center">
  <br>
  Compilador TypeScript → C
  <br>
</h1>

<h4 align="center">Um compilador otimizador desenvolvido em C com suporte a Strength Reduction e Constant Folding</h4>

<p align="center">
  <img src="https://img.shields.io/badge/Linguagem-C-blue.svg" alt="Linguagem">
  <img src="https://img.shields.io/badge/Flex-Bison-orange.svg" alt="Ferramentas">
  <img src="https://img.shields.io/badge/Optimizations-2-brightgreen.svg" alt="Otimizações">
  <img src="https://img.shields.io/badge/Status-Complete-success.svg" alt="Status">
</p>

<div align="center">
    
</div>

## Sobre o Projeto

Somos o grupo 8 e desenvolvemos um compilador que traduz um subconjunto de **TypeScript** para **C**, implementando técnicas avançadas de otimização em tempo de compilação. Este projeto foi desenvolvido para a disciplina de Compiladores, demonstrando na prática os conceitos teóricos estudados.

## Escopo do Projeto

### Linguagem Fonte Suportada
- 

### Funcionalidades Implementadas
- Análise léxica e sintática
- Tabela de símbolos com gerenciamento de escopos
- Verificação de tipos estática
- Geração de código C eficiente
- Sistema de otimizações em tempo de compilação

## Decisões Técnicas

### Arquitetura do Compilador
- **Frontend**: Analisador léxico (Flex) e sintático (Bison)
- **Representação Intermediária**: Árvore Sintática Abstrata (AST)
- **Backend**: Gerador de código C com otimizações

### Ferramentas e Tecnologias

| Camada | Tecnologia | Propósito |
|--------|------------|-----------|
| **Linguagem Fonte** | `TypeScript` | Código de entrada do compilador |
| **Análise Léxica** | `Flex` | Tokenização do código TypeScript |
| **Análise Sintática** | `Bison` | Construção da AST a partir dos tokens |
| **Implementação** | `C` | Linguagem do compilador |
| **Build System** | `Make` | Automação da compilação |
| **Linguagem Alvo** | `C` | Código gerado pelo compilador |

### Estrutura de Dados
- **AST personalizada** para representação do código fonte
- **Tabela de símbolos** com suporte a múltiplos escopos
- **Sistema de tipos** para verificação estática

### Estratégias de Implementação
- **Recuperação de erros** com reporting detalhado
- **Otimizações aplicadas** durante a travessia da AST
- **Geração de código** por visitação recursiva da AST

## Otimizações Implementadas

### Strength Reduction
Transformação de operações matemáticas complexas em operações mais eficientes:

- **Multiplicação por constantes:** convertida para somas repetidas
- **Casos especiais**: multiplicação por 0 e 1 otimizados
- **Limite definido** para evitar expansão excessiva de código

**Por exemplo:**

| Cenário | Entrada | Saída Otimizada |
|---------|---------|-----------------|
| Multiplicação por constante | `x * 5` | `x + x + x + x + x` |
| Casos especiais | `x * 0` | `0` |
| Limite inteligente | `x * 17` | `x * 17` (mantido) |

### Constant Folding
Cálculo de expressões constantes em tempo de compilação:

- **Expressões aritméticas** com operandos constantes
- **Operações de comparação** com resultados booleanos
- **Concatenação de strings** literais

**Por exemplo:**

| Tipo | Antes | Depois |
|------|-------|--------|
| Aritmético | `10 + 5 * 2` | `20` |
| Lógico | `(10 > 5) && true` | `true` |
| String | `"Hello" + "World"` | `"HelloWorld"` |

### Benefícios das Otimizações
- **Performance**: Redução de operações em tempo de execução
- **Código eficiente**: Geração de instruções mais rápidas
- **Tempo de compilação**: Cálculos antecipados

##  Como Utilizar



## Membros da Equipe 👥

<div align="center"><table>
  <tr>
    <td align="center"><a href="https://github.com/eduardodpms"><img src="https://avatars.githubusercontent.com/eduardodpms" width="100px" style="border-radius: 50%;"><br/>Eduardo de Pina<br/></a></td>
    <td align="center"><a href="https://github.com/cwtshh"><img src="https://avatars.githubusercontent.com/cwtshh" width="100px" style="border-radius: 50%;"><br/>Gustavo Costa<br/></a></td>
    <td align="center"><a href="https://github.com/JMPNascimento"><img src="https://avatars.githubusercontent.com/JMPNascimento" width="100px" style="border-radius: 50%;"><br/>João Maurício<br/></a></td>
    <td align="center"><a href="https://github.com/AguionStryke"><img src="https://avatars.githubusercontent.com/AguionStryke" width="100px" style="border-radius: 50%;"><br/>Lucas Freire<br/></a></td>
    <td align="center"><a href="https://github.com/luisa12ll"><img src="https://avatars.githubusercontent.com/luisa12ll" width="100px" style="border-radius: 50%;"><br/>Luísa de Souza<br/></a></td>
  </tr>
</table></div>
