# Análise Técnico-Científica: Implementação de Edge AI no Ecossistema Hydrosense

## I. Introdução

O paradigma da Inteligência Artificial de Borda (*Edge AI*) representa uma evolução crítica no desenvolvimento de sistemas embebidos inteligentes. No âmbito do nosso projecto **Hydrosense**, a integração desta tecnologia visa mitigar as limitações inerentes à computação na nuvem convencional, tais como a latência de rede, a dependência de conectividade constante e os riscos de segurança de dados em trânsito. Este documento descreve as bases teóricas, a implementação técnica e os resultados da aplicação de modelos de aprendizagem automática diretamente no microcontrolador ESP32, permitindo uma análise preditiva e autónoma da qualidade da água.

## II. Desenvolvimento

### 2.1 Conceitos Fundamentais: Tensores e Arena de Tensores
Para compreender a execução da IA no nosso hardware, é essencial definir dois conceitos base da biblioteca TensorFlow Lite Micro:

*   **Tensores:** Em termos matemáticos e computacionais, um tensor é uma estrutura de dados multidimensional (uma generalização de escalares, vectores e matrizes). No contexto do nosso projecto, os tensores são os contentores que armazenam todas as informações processadas pela rede neural, incluindo os dados de entrada dos sensores, os pesos matemáticos que definem o conhecimento da IA e os resultados finais da análise.
*   **Arena de Tensores (*Tensor Arena*):** Trata-se de um bloco de memória RAM reservado de forma estática e contígua. Ao contrário dos sistemas convencionais que alocam memória dinamicamente, a Arena de Tensores funciona como um "palco" fixo onde todos os tensores necessários para a inferência são organizados. Esta abordagem garante o determinismo do sistema, assegurando que o microcontrolador terá sempre os recursos necessários para processar a inteligência sem interrupções.

### 2.2 Fundamentação Matemática: Álgebra Linear na Inferência
O funcionamento interno da nossa rede neural é sustentado pelos princípios da Álgebra Linear. Esta disciplina permite quantificar e manipular as relações entre os dados através de três entidades fundamentais:

*   **Escalares:** Representam grandezas numéricas isoladas (tensores de ordem 0). No nosso sistema, cada leitura individual de um sensor (como o valor da temperatura) é, após a normalização, tratada como um escalar.
*   **Vectores:** São conjuntos ordenados de escalares (tensores de ordem 1). No Hydrosense, os dados captados em cada ciclo são agrupados num **vector de características** (ex: `[temperatura, turbidez]`). Este vector representa o estado puntual da água que será submetido à análise.
*   **Matrizes:** São estruturas bidimensionais que organizam dados em linhas e colunas (tensores de ordem 2). Na nossa implementação, os **pesos (weights)** da rede neural — que representam o conhecimento adquirido durante a fase de treino — estão armazenados em matrizes. A inferência da IA consiste num processo contínuo de multiplicação de matrizes pelos vectores de entrada, permitindo ao sistema identificar padrões complexos de forma estatisticamente rigorosa através de operações de álgebra linear.

### 2.3 Arquitectura do Modelo e Implementação Técnica
A arquitectura seleccionada baseia-se em Redes Neurais Densas (*Multilayer Perceptron* - MLP). Optámos por este modelo devido à sua elevada eficácia no processamento de dados tabulares provenientes de sensores ambientais.

O modelo foi implementado através da biblioteca **TensorFlow Lite Micro**, que permite a execução de operações matemáticas complexas com uma utilização de recursos extremamente reduzida:
*   **Optimização de Espaço:** O modelo compilado ocupa meros **2KB** de memória, o que reflecte um trabalho rigoroso de compressão.
*   **Fluxo de Inferência:** O processo inicia-se com a **Normalização Quantitativa** (ajuste dos valores para uma escala de 0 a 1), seguido pela **Invocação Computacional** (`Invoke`), onde a Arena de Tensores é utilizada para realizar os cálculos matriciais que resultam na classificação final da qualidade da água.

## III. Conclusão

A implementação de *Edge AI* no nosso projecto revela-se uma decisão estratégica de sucesso. Conseguimos criar um sistema que não só monitoriza, mas também "compreende" as correlações complexas entre as variáveis da água de forma independente. A robustez da arquitectura MLP, aliada à gestão eficiente da Arena de Tensores e ao rigor matemático da Álgebra Linear, posiciona o Hydrosense como uma solução de monitorização de vanguarda, capaz de operar de forma fiável em cenários críticos.

---

**Hydrosense: Engenharia de Precisão e Inteligência Descentralizada.**
