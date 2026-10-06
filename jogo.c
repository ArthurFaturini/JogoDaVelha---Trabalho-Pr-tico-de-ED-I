#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <unistd.h>
#include "jogo.h"

void pause()
{
    printf("\nPressione ENTER para continuar...");
    int c;
    while ((c = getchar()) != '\n' && c != EOF)
        ;      // Limpa o buffer até o Enter
    getchar(); // Aguarda o novo Enter do usuário
}

void inicializarTabuleiro(char tabuleiro[3][3])
{
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            tabuleiro[i][j] = ' ';
        }
    }
}

void mostrarTabuleiro(char tabuleiro[3][3])
{
    printf("\n  1   2   3\n");
    for (int i = 0; i < 3; i++)
    {
        printf("%d %c | %c | %c \n", i + 1, tabuleiro[i][0], tabuleiro[i][1], tabuleiro[i][2]);
        if (i < 2)
            printf("  --+---+--\n");
    }
    printf("\n");
}

int decidirQuemComeca()
{
    int escolhaUsuario, numeroUsuario, numeroComputador, soma;

    printf("\n--- Sorteio Inicial: Par ou Impar ---\n");
    printf("Escolha: 0 para PAR ou 1 para IMPAR: ");
    scanf("%d", &escolhaUsuario);

    while (escolhaUsuario != 0 && escolhaUsuario != 1)
    {
        printf("Escolha invalida! Digite 0 para PAR ou 1 para IMPAR: ");
        scanf("%d", &escolhaUsuario);
    }

    printf("Digite um numero de 0 a 5: ");
    scanf("%d", &numeroUsuario);

    numeroComputador = rand() % 6;

    soma = numeroUsuario + numeroComputador;
    printf("\nO computador escolheu o numero %d.\n", numeroComputador);
    printf("Soma = %d (%s)\n", soma, (soma % 2 == 0) ? "PAR" : "IMPAR");

    if (soma % 2 == escolhaUsuario)
    {
        printf("Voce venceu no par ou impar! Voce comeca com o 'X'.\n");
        return 1;
    }
    else
    {
        printf("O computador venceu no par ou impar! Ele comeca com o 'X'.\n");
        return 0;
    }
}

void jogadaUsuario(char tabuleiro[3][3], char marcador, Nodo **listaJogadas)
{
    int linha, coluna;
    int valida = 0;

    while (!valida)
    {
        printf("Sua vez (%c). Digite a posição (ex: 1-3): ", marcador);
        if (scanf("%d-%d", &linha, &coluna) == 2)
        {
            linha--;
            coluna--;

            if (linha >= 0 && linha < 3 && coluna >= 0 && coluna < 3)
            {
                if (tabuleiro[linha][coluna] == ' ')
                {
                    tabuleiro[linha][coluna] = marcador;
                    inserirJogada(listaJogadas, linha, coluna);
                    valida = 1;
                }
                else
                {
                    printf("Posicao ja ocupada! Escolha outra.\n");
                }
            }
            else
            {
                printf("Coordenadas invalidas! Use numeros de 1 a 3.\n");
            }
        }
        else
        {
            while (getchar() != '\n')
                ;
            printf("Entrada invalida! Digite numeros.\n");
        }
    }
}

// Função recursiva do algoritmo Minimax
int minimax(char tabuleiro[3][3], int profundidade, int eMaximizador, char marcadorComp, char marcadorUser, int limiteProfundidade)
{
    // Se atingiu o limite do nível, ele interrompe a visão do futuro e assume neutralidade (0)
    if (profundidade >= limiteProfundidade)
        return 0;

    // Verifica se alguém venceu ou se empatou na simulação atual
    if (verificarVencedor(tabuleiro, marcadorComp))
        return 10 - profundidade; // Prefere vencer em menos jogadas
    if (verificarVencedor(tabuleiro, marcadorUser))
        return profundidade - 10; // Prefere adiar a derrota
    if (verificarEmpate(tabuleiro))
        return 0;

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
                    int pontuacao = minimax(tabuleiro, profundidade + 1, 0, marcadorComp, marcadorUser, limiteProfundidade);
                    tabuleiro[i][j] = ' '; // Desfaz a jogada simulada
                    if (pontuacao > melhorPontuacao)
                        melhorPontuacao = pontuacao;
                }
            }
        }
        return melhorPontuacao;
    }
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
                    int pontuacao = minimax(tabuleiro, profundidade + 1, 1, marcadorComp, marcadorUser, limiteProfundidade);
                    tabuleiro[i][j] = ' '; // Desfaz a jogada simulada
                    if (pontuacao < melhorPontuacao)
                        melhorPontuacao = pontuacao;
                }
            }
        }
        return melhorPontuacao;
    }
}

// Função principal de jogada do Computador usando Minimax
void jogadaComputador(char tabuleiro[3][3], char marcador, Nodo **listaJogadas)
{
    // 9 = Imbatível (Minimax completo, enxerga até o fim do jogo)
    // 1 ou 2 = Burro (Enxerga apenas 1 ou 2 jogadas à frente, fácil de vencer)
    int nivelDificuldade = 9;

    char marcadorUsuario = (marcador == 'X') ? 'O' : 'X';
    int melhorValor = -1000;
    int melhorLinha = -1;
    int melhorColuna = -1;

    printf("\nComputador (%c) jogando", marcador);
    for (int i = 0; i < 3; i++)
    {
        sleep(1);
        printf(".");
    }
    sleep(1);
    printf("\n");

    // Avalia todas as casas vazias e escolhe a jogada com maior pontuação no Minimax
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            if (tabuleiro[i][j] == ' ')
            {
                tabuleiro[i][j] = marcador;
                int valorJogada = minimax(tabuleiro, 0, 0, marcador, marcadorUsuario, nivelDificuldade);
                tabuleiro[i][j] = ' '; // Desfaz a jogada de teste

                if (valorJogada > melhorValor)
                {
                    melhorLinha = i;
                    melhorColuna = j;
                    melhorValor = valorJogada;
                }
            }
        }
    }

    // Executa a melhor jogada encontrada e grava na lista encadeada
    if (melhorLinha != -1 && melhorColuna != -1)
    {
        tabuleiro[melhorLinha][melhorColuna] = marcador;
        inserirJogada(listaJogadas, melhorLinha, melhorColuna);
    }
}

int verificarVencedor(char tabuleiro[3][3], char marcador)
{
    for (int i = 0; i < 3; i++)
    {
        if ((tabuleiro[i][0] == marcador && tabuleiro[i][1] == marcador && tabuleiro[i][2] == marcador) ||
            (tabuleiro[0][i] == marcador && tabuleiro[1][i] == marcador && tabuleiro[2][i] == marcador))
        {
            return 1;
        }
    }
    if ((tabuleiro[0][0] == marcador && tabuleiro[1][1] == marcador && tabuleiro[2][2] == marcador) ||
        (tabuleiro[0][2] == marcador && tabuleiro[1][1] == marcador && tabuleiro[2][0] == marcador))
    {
        return 1;
    }
    return 0;
}

int verificarEmpate(char tabuleiro[3][3])
{
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            if (tabuleiro[i][j] == ' ')
                return 0;
        }
    }
    return 1;
}

void iniciarPartidas(Partida **historico, int *proximoId)
{
    char tabuleiro[3][3];
    char continuar = 's';
    int quemComecaPartida;

    srand((unsigned int)time(NULL));

    printf("\nIniciando nova sessao de partidas!\n");

    while (continuar == 's' || continuar == 'S')
    {
        int numeroPartida = *proximoId;

        printf("\n=============================");
        printf("\n       PARTIDA %d", numeroPartida);
        printf("\n=============================\n");

        inicializarTabuleiro(tabuleiro);

        Partida *partidaCorrente = criarPartida(*proximoId, "Usuario", "Computador");
        (*proximoId)++;

        if (numeroPartida == 1)
        {
            quemComecaPartida = decidirQuemComeca();
        }
        else
        {
            quemComecaPartida = (quemComecaPartida == 1) ? 0 : 1;
            printf("\nRegra de alternancia: Nesta partida quem comeca e %s com o 'X'.\n",
                   (quemComecaPartida == 1) ? "VOCE" : "o COMPUTADOR");
        }

        sleep(1);
        mostrarTabuleiro(tabuleiro);

        int turnoAtual = quemComecaPartida;
        int fimDeJogo = 0;

        while (!fimDeJogo)
        {
            char marcadorAtual = (turnoAtual == quemComecaPartida) ? 'X' : 'O';

            if (turnoAtual == 1)
            {
                jogadaUsuario(tabuleiro, marcadorAtual, &partidaCorrente->jogadasUsuario);
            }
            else
            {
                jogadaComputador(tabuleiro, marcadorAtual, &partidaCorrente->jogadasComputador);
            }

            mostrarTabuleiro(tabuleiro);

            if (verificarVencedor(tabuleiro, marcadorAtual))
            {
                if (turnoAtual == 1)
                {
                    printf("Parabens! Voce venceu a partida %d!\n", numeroPartida);
                    strcpy(partidaCorrente->resultado, "Usuario");
                }
                else
                {
                    printf("O computador venceu a partida %d!\n", numeroPartida);
                    strcpy(partidaCorrente->resultado, "Computador");
                }
                fimDeJogo = 1;
            }
            else if (verificarEmpate(tabuleiro))
            {
                printf("Empate! Ninguem venceu esta partida.\n");
                strcpy(partidaCorrente->resultado, "Empate");
                fimDeJogo = 1;
            }

            turnoAtual = (turnoAtual == 1) ? 0 : 1;
        }

        if (*historico == NULL)
        {
            *historico = partidaCorrente;
        }
        else
        {
            Partida *historicoAtual = *historico;

            while (historicoAtual->prox != NULL)
            {
                historicoAtual = historicoAtual->prox;
            }

            historicoAtual->prox = partidaCorrente;
        }

        printf("\nDeseja jogar mais uma partida? (s/n): ");
        scanf(" %c", &continuar);
    }

    printf("\n=== HISTORICO DA SESSAO ===\n");

    int vitoriasUsuario = 0;
    int vitoriasComputador = 0;

    Partida *p = *historico;
    while (p != NULL)
    {
        printf("\nPartida %d | Vencedor: %s\n", p->id, p->resultado);
        printf("Jogadas Usuario: ");
        imprimirJogadas(p->jogadasUsuario);
        if (strcmp(p->resultado, "Usuario") == 0)
        {
            vitoriasUsuario++;
        }
        else if (strcmp(p->resultado, "Computador") == 0)
        {
            vitoriasComputador++;
        }
        p = p->prox;
    }

    printf("\n=== RESULTADO GERAL ===\n");
    printf("Vitorias do Usuario: %d\n", vitoriasUsuario);
    printf("Vitorias do Computador: %d\n", vitoriasComputador);

    if (vitoriasUsuario > vitoriasComputador)
    {
        printf("Vencedor geral: Usuario\n");
    }
    else if (vitoriasComputador > vitoriasUsuario)
    {
        printf("Vencedor geral: Computador\n");
    }
    else
    {
        printf("Vencedor geral: Empate\n");
    }
    pause();
}