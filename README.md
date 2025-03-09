# Teoria dos Grafos – COS 242 (2024/2)  

## 🔍 Trabalho de Curso – Parte 3  

Este repositório contém a implementação da terceira parte do trabalho da disciplina **Teoria dos Grafos – COS 242** (2024/2). Nesta etapa, foram adicionadas novas funcionalidades à biblioteca de manipulação de grafos, incluindo suporte a **grafos direcionados com pesos** e a implementação do **algoritmo de Ford-Fulkerson** para cálculo do **fluxo máximo** em redes de fluxo.  

## 📑 Funcionalidades Implementadas  

### 🏗️ Representação de Grafos Direcionados com Pesos  
- Suporte para **grafos direcionados e não direcionados**.  
- Representação via **lista de adjacência** ou **vetor de adjacência** (matriz de adjacência opcional).  
- Formato de entrada compatível com os trabalhos anteriores.  

### 🔄 Algoritmo de Ford-Fulkerson  
- Implementação do algoritmo para cálculo do **fluxo máximo** em redes de fluxo.  
- Representação do **grafo residual** de forma independente.  
- Suporte para armazenamento do resultado em disco (arestas e fluxo).  
- Funções auxiliares:  
  - **Cálculo do gargalo** de um caminho.  
  - **Atualização do fluxo** no grafo original e residual.  

## 📊 Estudos de Caso  

Para cada grafo de entrada, o fluxo máximo foi determinado entre o **vértice 1 (fonte)** e o **vértice 2 (sumidouro)**. O tempo médio de execução foi calculado com base em **10 rodadas** (excluindo operações de E/S).  

Os resultados incluem:  
- **Fluxo máximo encontrado**.  
- **Tempo médio de execução (segundos)**.  
