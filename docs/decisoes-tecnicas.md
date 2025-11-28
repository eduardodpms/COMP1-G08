<style>
  @import url("https://fonts.googleapis.com/css2?family=JetBrains+Mono:wght@400;500;700&family=Roboto+Mono:wght@300;400;700&display=swap");

  :root {
    --compiler-bg: #0a0e14;
    --compiler-text: #e6e6e6;
    --compiler-accent: #ff6d00;
    --compiler-secondary: #00bcd4;
    --compiler-success: #00c853;
    --compiler-float-animation: float 8s ease-in-out infinite;
  }

  body {
    background-color: var(--compiler-bg);
    background-image:
      radial-gradient(circle at 10% 20%, rgba(255, 109, 0, 0.05) 0%, transparent 20%),
      radial-gradient(circle at 90% 80%, rgba(0, 188, 212, 0.05) 0%, transparent 20%);
    color: var(--compiler-text);
    font-family: "JetBrains Mono", "Roboto Mono", monospace;
    margin: 0;
    padding: 0;
    line-height: 1.6;
  }

  /* Container principal no estilo da content-section */
  .adr-container {
    max-width: 1000px;
    margin: 80px auto;
    padding: 0 20px;
  }

  /* Card principal */
  .adr-card {
    background: rgba(30, 35, 48, 0.7);
    border: 1px solid rgba(255, 109, 0, 0.2);
    border-radius: 12px;
    padding: 40px;
    margin-bottom: 40px;
    backdrop-filter: blur(10px);
    position: relative;
    overflow: hidden;
  }

  .adr-card::before {
    content: "";
    position: absolute;
    top: 0;
    left: 0;
    width: 5px;
    height: 100%;
    background: linear-gradient(180deg, var(--compiler-accent), var(--compiler-secondary));
  }

  /* Título igual ao hero-title / section-title */
  .adr-title {
    font-size: 3.2rem;
    font-weight: 900;
    text-align: center;
    color: var(--compiler-accent);
    margin-bottom: 10px;
    text-shadow: 0 0 18px rgba(255, 109, 0, 0.5);
    animation: var(--compiler-float-animation);
  }

  .adr-subtitle {
    text-align: center;
    font-size: 1.3rem;
    color: var(--compiler-secondary);
    opacity: 0.9;
    margin-bottom: 50px;
  }

  /* Seções internas iguais a section-card */
  .adr-section {
    background: rgba(0, 0, 0, 0.3);
    border: 1px solid rgba(255, 109, 0, 0.2);
    border-radius: 10px;
    padding: 35px;
    margin: 40px 0;
    position: relative;
    backdrop-filter: blur(8px);
    overflow: hidden;
  }

  .adr-section::before {
    content: "";
    position: absolute;
    top: 0;
    left: 0;
    width: 4px;
    height: 100%;
    background: linear-gradient(180deg, var(--compiler-accent), var(--compiler-secondary));
  }

  .adr-section h2 {
    color: var(--compiler-accent);
    font-size: 1.9rem;
    margin-bottom: 15px;
    border-bottom: 2px solid rgba(255, 109, 0, 0.3);
    padding-bottom: 10px;
  }

  .adr-section p,
  .adr-section li {
    color: var(--compiler-text);
    line-height: 1.7;
  }

  /* Tabela igual à tech-table */
  .adr-table {
    width: 100%;
    border-collapse: collapse;
    margin: 25px 0;
    background: rgba(0, 0, 0, 0.3);
    border-radius: 8px;
    overflow: hidden;
  }

  .adr-table th {
    background: rgba(255, 109, 0, 0.2);
    color: var(--compiler-accent);
    padding: 15px;
    text-align: left;
    font-weight: bold;
  }

  .adr-table td {
    padding: 15px;
    border-bottom: 1px solid rgba(255, 109, 0, 0.1);
    color: var(--compiler-text);
  }

  .adr-table tr:last-child td {
    border-bottom: none;
  }

  /* Animações usadas na página 2 */
  @keyframes float {
    0%, 100% { transform: translateY(0) rotate(0deg); }
    33% { transform: translateY(-10px) rotate(1deg); }
    66% { transform: translateY(5px) rotate(-1deg); }
  }
</style>

<div class="adr-container">
  <h1 class="adr-title">📘 DECISÕES TÉCNICAS</h1>
  <p class="adr-subtitle">Justificativas e escolhas arquiteturais do compilador</p>

  <div class="adr-card">

    <!-- Contexto -->
    <div class="adr-section">
      <h2>🎯 1. Contexto</h2>
      <p>
        Inicialmente optamos por utilizar TypeScript por aparentar possuir uma sintaxe semelhante à de C. Porém, ao longo do desenvolvimento percebemos que, apesar da familiaridade superficial, existem diferenças estruturais significativas entre as duas linguagens.
      </p>
    </div>

    <!-- Problema -->
    <div class="adr-section">
      <h2>❗ 2. Problema</h2>

      <p>
        Durante o desenvolvimento percebemos que vários conceitos essenciais de C não têm equivalente direto em TypeScript, o que impactou a arquitetura do compilador. Os principais pontos foram:
      </p>

      <p><strong>• Endereçamento</strong><br>
        C trabalha com endereços reais e aritmética de ponteiros; TypeScript não permite acesso à memória. Isso exigiu adaptações em operações como leitura, escrita, manipulação de strings e inferência de tipos.
      </p>

      <p><strong>• Strings</strong><br>
        C usa arrays mutáveis de <code>char</code>; TypeScript usa strings imutáveis. Operações simples, como concatenação, precisaram ser reinterpretadas.
      </p>

      <p><strong>• Orientação a objetos e métodos específicos</strong><br>
        TypeScript possui classes, interfaces e diversos métodos nativos como <code>.split()</code> e <code>.sort()</code>, que não existem em C. Implementar equivalentes exigiria estruturas complexas em baixo nível.
      </p>

      <p><strong>• Assincronicidade & modelo de execução</strong><br>
        TypeScript utiliza <code>async</code>/<code>await</code>, Promises e event loop; C é totalmente síncrono. Replicar esse comportamento exigiria escrever um runtime completo.
      </p>
    </div>

    <!-- Decisão -->
    <div class="adr-section">
      <h2>✅ 3. Decisão</h2>
      <p>
        Optamos por não incluir recursos como orientação a objetos, funções assíncronas (<code>async/await</code>), arrow functions e métodos de alto nível como <code>split()</code> e <code>sort()</code> — este último exigindo a implementação interna de algoritmos como TimSort.  
        Essa escolha permitiu manter o escopo do compilador simples, consistente e compatível com o modelo de execução de C.
      </p>
    </div>

  </div>
</div>
