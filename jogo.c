#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <windows.h>
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

    numeroComputador = rand() % 6; // Sorteio aleatório de 0 a 5 do computador

    soma = numeroUsuario + numeroComputador;
    printf("\nO computador escolheu o numero %d.\n", numeroComputador);
    printf("Soma = %d (%s)\n", soma, (soma % 2 == 0) ? "PAR" : "IMPAR");

    if (soma % 2 == escolhaUsuario)
    {
        printf("Voce venceu no par ou impar! Voce comeca com o 'X'.\n");
        return 1; // 1 = Usuario
    }
    else
    {
        printf("O computador venceu no par ou impar! Ele comeca com o 'X'.\n");
        return 0; // 0 = Computador
    }
}

void jogadaUsuario(char tabuleiro[3][3], char marcador)
{
    int linha, coluna;
    int valida = 0;

    while (!valida)
    {
        printf("Sua vez (%c). Digite a linha e coluna (1-3 1-3): ", marcador);
        if (scanf("%d %d", &linha, &coluna) == 2)
        {
            linha--; // Converte de 1..3 para índice 0..2
            coluna--;

            if (linha >= 0 && linha < 3 && coluna >= 0 && coluna < 3)
            {
                if (tabuleiro[linha][coluna] == ' ')
                {
                    tabuleiro[linha][coluna] = marcador;
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
            // Limpa o buffer caso o usuário digite letras
            while (getchar() != '\n')
                ;
            printf("Entrada invalida! Digite numeros.\n");
        }
    }
}

void jogadaComputador(char tabuleiro[3][3], char marcador)
{
    int i;
    printf("\nComputador (%c) jogando", marcador);
    // For para colocar dinâmica na jogada do computador
    for (i = 0; i < 3; i++)
    {
        Sleep(500);
        printf(".");
    }
    Sleep(500);
    printf("\n");
    // Jogada simples no primeiro espaço livre (temporário)
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            if (tabuleiro[i][j] == ' ')
            {
                tabuleiro[i][j] = marcador;
                return;
            }
        }
    }
}

int verificarVencedor(char tabuleiro[3][3], char marcador)
{
    // Linhas e Colunas
    for (int i = 0; i < 3; i++)
    {
        if ((tabuleiro[i][0] == marcador && tabuleiro[i][1] == marcador && tabuleiro[i][2] == marcador) ||
            (tabuleiro[0][i] == marcador && tabuleiro[1][i] == marcador && tabuleiro[2][i] == marcador))
        {
            return 1;
        }
    }
    // Diagonais
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
                return 0; // Ainda há casas livres
        }
    }
    return 1; // Tabuleiro cheio sem vencedor
}

void iniciarPartidas()
{
    char tabuleiro[3][3];
    int numeroPartida = 1;
    char continuar = 's';
    int quemComecaPartida;

    srand((unsigned int)time(NULL));

    printf("\nIniciando nova sessao de partidas!\n");

    while (continuar == 's' || continuar == 'S')
    {
        printf("\n=============================");
        printf("\n       PARTIDA %d", numeroPartida);
        printf("\n=============================\n");

        inicializarTabuleiro(tabuleiro);

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

        Sleep(2000);
        mostrarTabuleiro(tabuleiro);
        // Sleep(2000);

        int turnoAtual = quemComecaPartida; // 1 = Usuario, 0 = Computador
        int fimDeJogo = 0;

        // Loop da Partida Completa
        while (!fimDeJogo)
        {
            char marcadorAtual = (turnoAtual == quemComecaPartida) ? 'X' : 'O';

            if (turnoAtual == 1)
            {
                jogadaUsuario(tabuleiro, marcadorAtual);
            }
            else
            {
                jogadaComputador(tabuleiro, marcadorAtual);
            }

            mostrarTabuleiro(tabuleiro);

            if (verificarVencedor(tabuleiro, marcadorAtual))
            {
                if (turnoAtual == 1)
                    printf("Parabens! Voce venceu a partida %d!\n", numeroPartida);
                else
                    printf("O computador venceu a partida %d!\n", numeroPartida);
                fimDeJogo = 1;
            }
            else if (verificarEmpate(tabuleiro))
            {
                printf("Empate! Ninguem venceu esta partida.\n");
                fimDeJogo = 1;
            }

            // Alterna o turno interno da partida (entre 1 e 0)
            turnoAtual = (turnoAtual == 1) ? 0 : 1;
        }

        printf("\nDeseja jogar mais uma partida? (s/n): ");
        scanf(" %c", &continuar);
        numeroPartida++;
    }
}