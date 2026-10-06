Aqui está uma explicação detalhada e estruturada do algoritmo **Minimax**, conectada passo a passo com as funções que implementámos no teu código em C. Essa explicação serve como excelente base teórica para a tua defesa do trabalho.

---

### 1. O que é o Algoritmo Minimax?

O **Minimax** é um algoritmo clássico da **Teoria dos Jogos** utilizado em jogos de soma zero (onde o ganho de um jogador representa a perda exata do outro) para dois jogadores com informação perfeita, como o Jogo da Velha, Xadrez ou Damas.

O seu princípio fundamental é:

* **Maximizador (Computador):** Tenta tomar a decisão que **maximiza** a sua pontuação (busca a vitória).
* **Minimizador (Usuário):** Tenta tomar a decisão que **minimiza** a pontuação do computador (busca a vitória do utilizador ou impede a vitória do computador).

O algoritmo explora todas as ramificações de jogadas futuras a partir do tabuleiro atual em formato de **árvore de decisão**, prevendo todos os cenários até o final do jogo antes de realizar a jogada real.

---

### 2. Relação com o Código Criado

#### A. A Função Principal (`jogadaComputador`)

Esta função atua como o **ponto de entrada** da Inteligência Artificial quando chega a vez do computador jogar:

```c
void jogadaComputador(char tabuleiro[3][3], char marcador, Nodo **listaJogadas)
{
    char marcadorUsuario = (marcador == 'X') ? 'O' : 'X';
    int melhorValor = -1000;
    int melhorLinha = -1;
    int melhorColuna = -1;

```

1. **Avaliação das Opções Disponíveis:**
O computador percorre todas as casas da matriz $3 \times 3$ à procura de células vazias (`' '`).
2. **Simulação da Jogada:**
Para cada casa vazia, ele coloca temporariamente o seu símbolo (`tabuleiro[i][j] = marcador`).
3. **Chamada ao Minimax:**
Ele chama a função `minimax(tabuleiro, 0, 0, marcador, marcadorUsuario)` para calcular a pontuação dessa jogada.
4. **Desfazer a Simulação:**
Logo a seguir, restaura a célula para vazia (`tabuleiro[i][j] = ' '`), pois está apenas a testar hipóteses em memória.
5. **Decisão do Melhor Movimento:**
Se a pontuação retornada for superior ao `melhorValor` atual, ele guarda as coordenadas `(melhorLinha, melhorColuna)`.
6. **Execução e Registo:**
No final do ciclo, ele efetua a jogada definitiva no tabuleiro real e regista as coordenadas na **lista encadeada** com `inserirJogada(listaJogadas, melhorLinha, melhorColuna)`.



---

#### B. A Função Recursiva (`minimax`)

A magia do algoritmo acontece dentro da função recursiva `minimax`:

##### 1. Os Casos Base (Condições de Paragem da Recursão)

Antes de continuar a simular mais jogadas, o algoritmo verifica se a simulação atual atingiu um estado terminal do jogo:

```c
if (verificarVencedor(tabuleiro, marcadorComp))
    return 10 - profundidade; 
if (verificarVencedor(tabuleiro, marcadorUser))
    return profundidade - 10; 
if (verificarEmpate(tabuleiro))
    return 0;

```

* **$+10 - \text{profundidade}$ (Vitória do Computador):** Retorna um valor positivo. A subtração da `profundidade` garante que o computador dê preferência a vencer no **menor número de jogadas possível**.
* **$\text{profundidade} - 10$ (Vitória do Usuário):** Retorna um valor negativo. O algoritmo tenta evitar este cenário a todo o custo.
* **$0$ (Empate):** Se o tabuleiro encher sem vencedor, a pontuação atribuída é neutra.

##### 2. O Turno do Maximizador (`eMaximizador == 1`)

Representa a simulação das respostas do **Computador**:

```c
if (eMaximizador)
{
    int melhorPontuacao = -1000;
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            if (tabuleiro[i][j] == ' ')
            {
                tabuleiro[i][j] = marcadorComp;
                int pontuacao = minimax(tabuleiro, profundidade + 1, 0, marcadorComp, marcadorUser);
                tabuleiro[i][j] = ' '; 
                if (pontuacao > melhorPontuacao)
                    melhorPontuacao = pontuacao;
            }
        }
    }
    return melhorPontuacao;
}

```

* O computador testa todas as posições possíveis e escolhe o valor **máximo** (`pontuacao > melhorPontuacao`).
* Passa `eMaximizador = 0` na chamada recursiva para simular que o próximo a jogar será o utilizador.

##### 3. O Turno do Minimizador (`eMaximizador == 0`)

Representa a simulação das respostas do **Usuário**:

```c
else
{
    int melhorPontuacao = 1000;
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            if (tabuleiro[i][j] == ' ')
            {
                tabuleiro[i][j] = marcadorUser;
                int pontuacao = minimax(tabuleiro, profundidade + 1, 1, marcadorComp, marcadorUser);
                tabuleiro[i][j] = ' '; 
                if (pontuacao < melhorPontuacao)
                    melhorPontuacao = pontuacao;
            }
        }
    }
    return melhorPontuacao;
}

```

* Assume que o utilizador vai jogar com inteligência perfeita, escolhendo a jogada que mais prejudica o computador (valor **mínimo**, `pontuacao < melhorPontuacao`).
* Passa `eMaximizador = 1` na chamada recursiva para simular a resposta seguinte do computador.

---

### 3. Exemplo Prático de Funcionamento

Imagine que o utilizador está prestes a fazer uma linha de 3 peças e vencer no próximo turno:

1. O computador simula a sua jogada numa casa qualquer que não bloqueia o jogador.
2. Na chamada recursiva seguinte, o utilizador joga na casa de vitória.
3. O caso base deteta a vitória do utilizador e retorna uma pontuação negativa baixa (ex: $-9$).
4. Em alternativa, o computador simula jogar **na casa que bloqueia o utilizador**.
5. Na chamada recursiva seguinte, o utilizador já não consegue vencer imediatamente. O caso base retorna uma pontuação de empate ($0$) ou de vitória futura do computador ($+8$).
6. O computador compara as pontuações ($+8$ ou $0$ é maior que $-9$) e decide obrigatoriamente fazer a jogada de **bloqueio**.

---

### 4. Por que razão este algoritmo é perfeito para a defesa do trabalho?

1. **Garantia Teórica:** No Jogo da Velha, o Minimax torna o computador **imbatível**. O melhor resultado que um jogador humano consegue obter contra ele é o empate.
2. **Uso de Recursão e Estruturas de Dados:** Demonstra o domínio de funções recursivas e integração com a **lista encadeada** exigida no enunciado.


3. **Complexidade Adequada:** Como o espaço de estados do Jogo da Velha é pequeno ($9! = 362.880$ combinações no máximo), a recursão executa em frações de segundo, mantendo a fluidez do programa.