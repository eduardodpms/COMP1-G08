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

  .md-main {
    background-color: var(--compiler-bg) !important;
    background-image: 
      radial-gradient(circle at 10% 20%, rgba(255, 109, 0, 0.05) 0%, transparent 20%),
      radial-gradient(circle at 90% 80%, rgba(0, 188, 212, 0.05) 0%, transparent 20%);
  }

  .md-content, body {
    background-color: var(--compiler-bg) !important;
    color: var(--compiler-text) !important;
    font-family: "JetBrains Mono", "Roboto Mono", monospace;
    line-height: 1.6;
  }

  /* Header Hero Section */
  .compiler-hero {
    text-align: center;
    padding: 100px 20px 80px;
    background: linear-gradient(135deg, var(--compiler-bg) 0%, #1a1f2e 100%);
    position: relative;
    overflow: hidden;
  }

  .compiler-hero::before {
    content: "";
    position: absolute;
    top: 0;
    left: 0;
    right: 0;
    height: 2px;
    background: linear-gradient(90deg, transparent, var(--compiler-accent) 50%, transparent);
    animation: scanline 3s linear infinite;
  }

  .hero-title {
    font-size: 4rem;
    font-weight: 900;
    color: var(--compiler-accent);
    text-shadow: 0 0 30px rgba(255, 109, 0, 0.5);
    margin-bottom: 20px;
    animation: var(--compiler-float-animation);
  }

  .hero-subtitle {
    font-size: 1.5rem;
    color: var(--compiler-text);
    margin-bottom: 40px;
    opacity: 0.9;
  }

  .badges {
    display: flex;
    justify-content: center;
    gap: 15px;
    margin-bottom: 50px;
    flex-wrap: wrap;
  }

  .badge {
    padding: 8px 16px;
    background: rgba(255, 109, 0, 0.1);
    border: 1px solid var(--compiler-accent);
    border-radius: 20px;
    font-size: 0.9rem;
    color: var(--compiler-accent);
  }

  /* Terminal Demo */
  .terminal-demo {
    background: rgba(0, 0, 0, 0.6);
    border: 1px solid rgba(255, 109, 0, 0.3);
    border-radius: 12px;
    padding: 0;
    margin: 0 auto;
    max-width: 700px;
    text-align: left;
    font-family: "JetBrains Mono", monospace;
    overflow: hidden;
    box-shadow: 0 20px 40px rgba(0, 0, 0, 0.3);
  }

  .terminal-header {
    display: flex;
    align-items: center;
    padding: 15px 20px;
    background: rgba(0, 0, 0, 0.8);
    border-bottom: 1px solid rgba(255, 109, 0, 0.2);
  }

  .terminal-dots {
    display: flex;
    gap: 8px;
  }

  .terminal-dot {
    width: 14px;
    height: 14px;
    border-radius: 50%;
  }

  .terminal-dot:nth-child(1) { background: #ff5f57; }
  .terminal-dot:nth-child(2) { background: #ffbd2e; }
  .terminal-dot:nth-child(3) { background: #28ca42; }

  .terminal-title {
    margin-left: 15px;
    color: var(--compiler-text);
    font-size: 0.9rem;
    opacity: 0.7;
  }

  .terminal-content {
    padding: 25px;
    color: var(--compiler-success);
    line-height: 1.8;
  }

  .terminal-line {
    margin: 8px 0;
  }

  .terminal-prompt {
    color: var(--compiler-secondary);
    font-weight: bold;
  }

  .terminal-command {
    color: var(--compiler-text);
  }

  .terminal-output {
    color: var(--compiler-success);
    opacity: 0.9;
  }

  /* Content Sections */
  .content-section {
    max-width: 1000px;
    margin: 60px auto;
    padding: 0 20px;
  }

  .section-card {
    background: rgba(30, 35, 48, 0.7);
    border: 1px solid rgba(255, 109, 0, 0.2);
    border-radius: 12px;
    padding: 40px;
    margin-bottom: 40px;
    backdrop-filter: blur(10px);
    position: relative;
    overflow: hidden;
  }

  .section-card::before {
    content: "";
    position: absolute;
    top: 0;
    left: 0;
    width: 5px;
    height: 100%;
    background: linear-gradient(180deg, var(--compiler-accent), var(--compiler-secondary));
  }

  .section-title {
    color: var(--compiler-accent) !important;
    font-size: 2rem;
    margin-bottom: 25px;
    border-bottom: 2px solid rgba(255, 109, 0, 0.3);
    padding-bottom: 15px;
  }

  .section-subtitle {
    color: var(--compiler-secondary) !important;
    font-size: 1.3rem;
    margin: 30px 0 15px 0;
  }

  /* Feature Grid */
  .feature-grid {
    display: grid;
    grid-template-columns: repeat(auto-fit, minmax(300px, 1fr));
    gap: 25px;
    margin: 30px 0;
  }

  .feature-card {
    background: rgba(0, 0, 0, 0.3);
    border: 1px solid rgba(255, 109, 0, 0.2);
    border-radius: 8px;
    padding: 25px;
    transition: transform 0.3s ease, border-color 0.3s ease;
  }

  .feature-card:hover {
    transform: translateY(-5px);
    border-color: var(--compiler-accent);
  }

  .feature-icon {
    font-size: 2rem;
    margin-bottom: 15px;
    color: var(--compiler-accent);
  }

  .feature-title {
    color: var(--compiler-text);
    font-size: 1.2rem;
    margin-bottom: 10px;
    font-weight: bold;
  }

  /* Team Section */
  .team-grid {
    display: grid;
    grid-template-columns: repeat(auto-fit, minmax(200px, 1fr));
    gap: 30px;
    margin-top: 40px;
  }

  .team-member {
    text-align: center;
    padding: 20px;
    background: rgba(0, 0, 0, 0.3);
    border-radius: 12px;
    border: 1px solid rgba(255, 109, 0, 0.2);
    transition: transform 0.3s ease;
  }

  .team-member:hover {
    transform: translateY(-5px);
    border-color: var(--compiler-accent);
  }

  .team-avatar {
    width: 120px;
    height: 120px;
    border-radius: 50%;
    margin: 0 auto 15px;
    border: 3px solid var(--compiler-accent);
    object-fit: cover;
  }

  .team-name {
    color: var(--compiler-text);
    font-size: 1.1rem;
    margin-bottom: 5px;
    font-weight: bold;
  }

  .team-role {
    color: var(--compiler-secondary);
    font-size: 0.9rem;
  }

  /* Tables */
  .tech-table {
    width: 100%;
    border-collapse: collapse;
    margin: 25px 0;
    background: rgba(0, 0, 0, 0.3);
    border-radius: 8px;
    overflow: hidden;
  }

  .tech-table th {
    background: rgba(255, 109, 0, 0.2);
    color: var(--compiler-accent);
    padding: 15px;
    text-align: left;
    font-weight: bold;
  }

  .tech-table td {
    padding: 15px;
    border-bottom: 1px solid rgba(255, 109, 0, 0.1);
    color: var(--compiler-text);
  }

  .tech-table tr:last-child td {
    border-bottom: none;
  }

  /* Animations */
  @keyframes float {
    0%, 100% {
      transform: translateY(0) rotate(0deg);
    }
    33% {
      transform: translateY(-10px) rotate(1deg);
    }
    66% {
      transform: translateY(5px) rotate(-1deg);
    }
  }

  @keyframes scanline {
    0% {
      transform: translateX(-100%);
    }
    100% {
      transform: translateX(100%);
    }
  }

  /* Responsive */
  @media (max-width: 768px) {
    .hero-title {
      font-size: 2.5rem;
    }
    
    .hero-subtitle {
      font-size: 1.2rem;
    }
    
    .section-card {
      padding: 25px;
      margin: 20px;
    }
    
    .feature-grid {
      grid-template-columns: 1fr;
    }
    
    .team-grid {
      grid-template-columns: repeat(2, 1fr);
      gap: 20px;
    }
    
    .terminal-content {
      padding: 20px;
    }
  }
</style>

<div class="compiler-hero">
  <h1 class="hero-title">⚡ COMPILADOR TS → C</h1>
  <p class="hero-subtitle">Transformando TypeScript em C com otimizações avançadas</p>
  
  <div class="badges">
    <span class="badge">🔤 TypeScript → C</span>
    <span class="badge">⚡ Strength Reduction</span>
    <span class="badge">🔄 Constant Folding</span>
    <span class="badge">🚀 Performance</span>
  </div>

  <div class="terminal-demo">
    <div class="terminal-header">
      <div class="terminal-dots">
        <div class="terminal-dot"></div>
        <div class="terminal-dot"></div>
        <div class="terminal-dot"></div>
      </div>
      <div class="terminal-title">terminal — compilador — 80×24</div>
    </div>
    <div class="terminal-content">
      <div class="terminal-line">
        <span class="terminal-prompt">$</span>
        <span class="terminal-command">./compilador programa.ts</span>
      </div>
      <div class="terminal-line">
        <span class="terminal-output">✅ Análise léxica e sintática concluída</span>
      </div>
      <div class="terminal-line">
        <span class="terminal-output">🎯 Aplicando Strength Reduction...</span>
      </div>
      <div class="terminal-line">
        <span class="terminal-output">🔄 Aplicando Constant Folding...</span>
      </div>
      <div class="terminal-line">
        <span class="terminal-output">📄 Código C gerado: saida.c</span>
      </div>
      <div class="terminal-line">
        <span class="terminal-prompt">$</span>
        <span class="terminal-command">gcc saida.c -o programa && ./programa</span>
      </div>
      <div class="terminal-line">
        <span class="terminal-output">🚀 Executando código otimizado...</span>
      </div>
    </div>
  </div>
</div>

<div class="content-section">
  <!-- Sobre o Projeto -->
  <div class="section-card">
    <h2 class="section-title">🎯 Sobre o Projeto</h2>
    <p>Desenvolvemos um compilador completo que traduz um subconjunto de <strong>TypeScript</strong> para <strong>C</strong>, implementando técnicas avançadas de otimização em tempo de compilação. Este projeto demonstra na prática os conceitos teóricos de compiladores estudados na disciplina.</p>
    
    <div class="feature-grid">
      <div class="feature-card">
        <div class="feature-icon">🔍</div>
        <h3 class="feature-title">Análise Completa</h3>
        <p>Análise léxica, sintática e semântica robusta com verificação de tipos</p>
      </div>
      <div class="feature-card">
        <div class="feature-icon">⚡</div>
        <h3 class="feature-title">Otimizações Avançadas</h3>
        <p>Strength Reduction e Constant Folding para máximo desempenho</p>
      </div>
      <div class="feature-card">
        <div class="feature-icon">🚀</div>
        <h3 class="feature-title">Código Eficiente</h3>
        <p>Geração de código C otimizado e legível</p>
      </div>
    </div>
  </div>

  <!-- Escopo do Projeto -->
  <div class="section-card">
    <h2 class="section-title">📋 Escopo do Projeto</h2>
    
    <h3 class="section-subtitle">Linguagem Fonte Suportada</h3>
    <div class="feature-grid">
      <div class="feature-card">
        <div class="feature-icon">📝</div>
        <h3 class="feature-title">Declarações</h3>
        <p>let, const, var com tipagem estática</p>
      </div>
      <div class="feature-card">
        <div class="feature-icon">🔄</div>
        <h3 class="feature-title">Estruturas de Controle</h3>
        <p>if/else, while, for, switch/case</p>
      </div>
      <div class="feature-card">
        <div class="feature-icon">🎯</div>
        <h3 class="feature-title">Sistema de Tipos</h3>
        <p>number, string, boolean com verificação</p>
      </div>
    </div>

    <h3 class="section-subtitle">Funcionalidades Implementadas</h3>
    <ul>
      <li>✅ Análise léxica e sintática robusta</li>
      <li>✅ Tabela de símbolos com gerenciamento de escopos</li>
      <li>✅ Verificação de tipos estática</li>
      <li>✅ Geração de código C eficiente</li>
      <li>✅ Sistema de otimizações em tempo de compilação</li>
    </ul>
  </div>

  <!-- Tecnologias -->
  <div class="section-card">
    <h2 class="section-title">🛠️ Tecnologias & Ferramentas</h2>
    
    <table class="tech-table">
      <thead>
        <tr>
          <th>Camada</th>
          <th>Tecnologia</th>
          <th>Propósito</th>
        </tr>
      </thead>
      <tbody>
        <tr>
          <td>🔤 Linguagem Fonte</td>
          <td>TypeScript</td>
          <td>Código de entrada do compilador</td>
        </tr>
        <tr>
          <td>🔄 Análise Léxica</td>
          <td>Flex</td>
          <td>Tokenização do código fonte</td>
        </tr>
        <tr>
          <td>📐 Análise Sintática</td>
          <td>Bison</td>
          <td>Construção da AST</td>
        </tr>
        <tr>
          <td>💻 Implementação</td>
          <td>C</td>
          <td>Linguagem do compilador</td>
        </tr>
        <tr>
          <td>⚙️ Build System</td>
          <td>Make</td>
          <td>Automação da compilação</td>
        </tr>
        <tr>
          <td>🎯 Linguagem Alvo</td>
          <td>C</td>
          <td>Código gerado</td>
        </tr>
      </tbody>
    </table>
  </div>

  <!-- Otimizações -->
  <div class="section-card">
    <h2 class="section-title">⚡ Otimizações Implementadas</h2>
    
    <h3 class="section-subtitle">Strength Reduction</h3>
    <p>Transforma multiplicações por constantes em somas repetidas para melhor performance:</p>
    
    <div class="feature-grid">
      <div class="feature-card">
        <div class="feature-icon">🔢</div>
        <h3 class="feature-title">x * 2</h3>
        <p>→ x + x</p>
      </div>
      <div class="feature-card">
        <div class="feature-icon">🔢</div>
        <h3 class="feature-title">x * 5</h3>
        <p>→ x + x + x + x + x</p>
      </div>
      <div class="feature-card">
        <div class="feature-icon">🎯</div>
        <h3 class="feature-title">Casos Especiais</h3>
        <p>x * 0 → 0<br>x * 1 → x</p>
      </div>
    </div>

    <h3 class="section-subtitle">Constant Folding</h3>
    <p>Calcula expressões constantes em tempo de compilação:</p>
    
    <div class="feature-grid">
      <div class="feature-card">
        <div class="feature-icon">🧮</div>
        <h3 class="feature-title">10 + 5 * 2</h3>
        <p>→ 20</p>
      </div>
      <div class="feature-card">
        <div class="feature-icon">🔍</div>
        <h3 class="feature-title">(10 > 5)</h3>
        <p>→ true</p>
      </div>
      <div class="feature-card">
        <div class="feature-icon">📝</div>
        <h3 class="feature-title">"Hello" + "World"</h3>
        <p>→ "HelloWorld"</p>
      </div>
    </div>
  </div>

  <!-- Como Usar -->
  <div class="section-card">
    <h2 class="section-title">🚀 Como Usar</h2>
    
    <h3 class="section-subtitle">Compilação</h3>
    <div class="terminal-demo" style="max-width: 600px; margin: 20px 0;">
      <div class="terminal-content">
        <div class="terminal-line">
          <span class="terminal-prompt">$</span>
          <span class="terminal-command">make clean && make</span>
        </div>
        <div class="terminal-line">
          <span class="terminal-output">Compilando o compilador...</span>
        </div>
      </div>
    </div>

    <h3 class="section-subtitle">Execução</h3>
    <div class="terminal-demo" style="max-width: 600px; margin: 20px 0;">
      <div class="terminal-content">
        <div class="terminal-line">
          <span class="terminal-prompt">$</span>
          <span class="terminal-command">./parser path/to/typescript.ts</span>
        </div>
        <div class="terminal-line">
          <span class="terminal-output">(Opcional) Código C gerado: saida.c</span>
        </div>
        <div class="terminal-line">
          <span class="terminal-prompt">$</span>
          <span class="terminal-command">gcc saida.c -o programa</span>
        </div>
      </div>
    </div>
  </div>

  <!-- Equipe -->
  <div class="section-card">
    <h2 class="section-title">👥 Nossa Equipe</h2>
    <p style="text-align: center; margin-bottom: 30px; color: var(--compiler-secondary);">Grupo 08 - Compiladores</p>
    
    <div class="team-grid">
      <div class="team-member">
        <img src="https://avatars.githubusercontent.com/eduardodpms" alt="Eduardo" class="team-avatar">
        <h3 class="team-name">Eduardo de Pina</h3>
        <p class="team-role">Desenvolvedor</p>
      </div>
      <div class="team-member">
        <img src="https://avatars.githubusercontent.com/cwtshh" alt="Gustavo" class="team-avatar">
        <h3 class="team-name">Gustavo Costa</h3>
        <p class="team-role">Desenvolvedor</p>
      </div>
      <div class="team-member">
        <img src="https://avatars.githubusercontent.com/JMPNascimento" alt="João" class="team-avatar">
        <h3 class="team-name">João Maurício</h3>
        <p class="team-role">Desenvolvedor</p>
      </div>
      <div class="team-member">
        <img src="https://avatars.githubusercontent.com/AguionStryke" alt="Lucas" class="team-avatar">
        <h3 class="team-name">Lucas Freire</h3>
        <p class="team-role">Desenvolvedor</p>
      </div>
      <div class="team-member">
        <img src="https://avatars.githubusercontent.com/luisa12ll" alt="Luísa" class="team-avatar">
        <h3 class="team-name">Luísa de Souza</h3>
        <p class="team-role">Desenvolvedora</p>
      </div>
    </div>
  </div>
</div>
