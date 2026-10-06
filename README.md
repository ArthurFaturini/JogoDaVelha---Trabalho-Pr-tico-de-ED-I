# 🎮 Jogo da Velha em C - IA Imbatível & Histórico de Partidas

Este projeto é uma implementação completa e modularizada do clássico Jogo da Velha desenvolvido em linguagem C. Além da mecânica tradicional, o jogo conta com uma Inteligência Artificial imbatível, armazenamento de histórico de partidas através de Listas Lineares Encadeadas, e persistência de dados em arquivos de texto para a geração de um Ranking de jogadores.

---

## ✨ Funcionalidades

O programa funciona através de um menu interativo com as seguintes opções:

1. **Jogar Partidas:** Inicia uma sessão de Jogo da Velha. O jogador decide quem começa através de um sorteio de Par ou Ímpar contra o computador. O jogo alterna os turnos automaticamente nas partidas seguintes.


2. **Salvar Partidas:** Grava todo o histórico da sessão atual (nome dos jogadores, jogadas realizadas e resultado) no arquivo `partidas_velha.txt` e limpa a memória.


3. **Ranquear Usuários:** Lê o arquivo `partidas_velha.txt`, contabiliza as vitórias (ignorando empates) e exibe um ranking ordenado dos melhores jogadores.


4. **Sair:** Encerra o programa, oferecendo uma última chance de salvar os dados antes de fechar.



---

## 🧠 A Lógica por Trás do Projeto

### 1. Listas Lineares Encadeadas (Estrutura de Dados)

Para armazenar o histórico sem desperdiçar memória, o projeto utiliza **Listas Encadeadas**.

* Cada jogada no formato "linha-coluna" (ex: "1-2") é guardada num `struct Nodo`.


* O contexto inteiro de uma partida (ID, nome do usuário, nome do computador, resultado e os ponteiros para as listas de jogadas) é guardado num `struct Partida`.


* Quando o usuário decide parar de jogar, o programa percorre essa lista encadeada para imprimir o histórico completo da sessão.



### 2. Algoritmo Minimax (Teoria dos Jogos)

A Inteligência Artificial do computador utiliza o algoritmo **Minimax**, o que a torna imbatível (o melhor cenário possível para o jogador humano é um empate).

* **Como funciona:** Antes de fazer uma jogada, o computador simula em memória todas as ramificações possíveis até o final do jogo.


* **Pontuações:** Ele atribui `+10` se a jogada levar à sua vitória, `-10` se levar à vitória do usuário, e `0` para empates. O algoritmo subtrai ou soma a *profundidade* da árvore para priorizar vitórias rápidas e atrasar derrotas.


* **Decisão:** Ele filtra as opções e sempre executa a jogada que maximiza a sua pontuação (Maximizador) enquanto assume que o jogador humano fará a jogada que tenta minimizar essa pontuação (Minimizador).



### 3. Persistência e Ranking

A função de salvar varre a lista encadeada e escreve as informações no arquivo texto separadas por ponto e vírgula (`;`). Na hora de exibir o Ranking, o programa lê esse arquivo linha por linha, extrai o vencedor usando manipulação de strings (`strrchr`), agrupa as vitórias e utiliza o algoritmo *Bubble Sort* para ordenar do maior para o menor.

---

## 📂 Organização dos Arquivos & Como Compilar

O projeto foi preparado em três formatos de entrega para facilitar a avaliação. Certifique-se de usar o compilador **GCC** (via MinGW64 no Windows ou terminal no Linux). Evite usar o bash embutido do MinGW caso encontre erros de permissão; prefira o Prompt de Comando (`cmd`).

### Opção 1: Formato Padrão em `.c` e `.h` (Recomendado)

A estrutura clássica e modularizada com arquivos de cabeçalho:

* `main.c`, `jogo.c`, `jogo.h`, `lista.c`, `lista.h`.



**Comando de compilação:**

```bash
gcc -Wall -Wextra main.c jogo.c lista.c -o jogovelha

```

### Opção 2: Arquivo Único em `.txt`

O código inteiro unificado em um único arquivo de texto, pronto para compilação direta:

* `jogovelha.txt`.



**Comando de compilação:**
*(A flag `-x c` avisa o compilador que o arquivo texto deve ser lido como código C).*

```bash
gcc -x c -Wall -Wextra jogovelha.txt -o jogovelha

```

### Opção 3: Modularizado em `.txt`

O projeto separado de forma estruturada, mas utilizando extensões `.txt` em vez de `.c`:

* `main.txt`, `jogo.txt`, `lista.txt`.



**Comando de compilação:**

```bash
gcc -x c -Wall -Wextra main.txt -o jogovelha

```

---

## ▶️ Como Executar o Jogo

Se qualquer um dos comandos acima não retornar nenhum erro, o arquivo `jogovelha.exe` (ou `./jogovelha` no Linux) foi criado com sucesso. Para iniciar o jogo, basta digitar no terminal:

```bash
jogovelha

```

---

## Referências para o Minimax:
### Explicam de forma mais simples:
- [Minimax Medium](https://medium.com/@mhhassaan.1/mini-max-algorithm-in-artificial-intelligence-e0a0da694b3b)

- [GeeksForGeeks](https://www.geeksforgeeks.org/dsa/minimax-algorithm-in-game-theory-set-1-introduction/)

### Explica de forma mais completa: 
- [NeverStopBuilding](https://www.neverstopbuilding.com/blog/minimax)