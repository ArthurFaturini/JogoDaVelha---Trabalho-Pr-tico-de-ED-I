#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <windows.h>
#include <string.h>
#include "jogo.h"

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
        printf("Sua vez (%c). Digite a linha e coluna (1-3 1-3): ", marcador);
        if (scanf("%d %d", &linha, &coluna) == 2)
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

void jogadaComputador(char tabuleiro[3][3], char marcador, Nodo **listaJogadas)
{
    int i, j;
    printf("\nComputador (%c) jogando", marcador);
    for (i = 0; i < 3; i++)
    {
        Sleep(500);
        printf(".");
    }
    Sleep(500);
    printf("\n");

    for (i = 0; i < 3; i++)
    {
        for (j = 0; j < 3; j++)
        {
            if (tabuleiro[i][j] == ' ')
            {
                tabuleiro[i][j] = marcador;
                inserirJogada(listaJogadas, i, j);
                return;
            }
        }
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

void iniciarPartidas()
{
    char tabuleiro[3][3];
    int numeroPartida = 1;
    char continuar = 's';
    int quemComecaPartida;

    Partida *historicoInicio = NULL;
    Partida *historicoAtual = NULL;

    srand((unsigned int)time(NULL));

    printf("\nIniciando nova sessao de partidas!\n");

    while (continuar == 's' || continuar == 'S')
    {
        printf("\n=============================");
        printf("\n       PARTIDA %d", numeroPartida);
        printf("\n=============================\n");

        inicializarTabuleiro(tabuleiro);

        Partida *partidaCorrente = criarPartida(numeroPartida, "Usuario", "Computador");

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

        Sleep(1000);
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

        if (historicoInicio == NULL)
        {
            historicoInicio = partidaCorrente;
        }
        else
        {
            historicoAtual->prox = partidaCorrente;
        }
        historicoAtual = partidaCorrente;

        printf("\nDeseja jogar mais uma partida? (s/n): ");
        scanf(" %c", &continuar);
        numeroPartida++;
    }

    printf("\n=== HISTORICO DA SESSAO ===\n");
    Partida *p = historicoInicio;
    while (p != NULL)
    {
        printf("\nPartida %d | Vencedor: %s\n", p->id, p->resultado);
        printf("Jogadas Usuario: ");
        imprimirJogadas(p->jogadasUsuario);
        printf("Jogadas Computador: ");
        imprimirJogadas(p->jogadasComputador);
        p = p->prox;
    }
    printf("===========================\n");
    system("pause");
}