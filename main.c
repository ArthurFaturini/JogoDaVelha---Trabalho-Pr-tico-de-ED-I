#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "jogo.h"

int main()
{
    int opcao;
    char confirmarSaida;
    Partida *historico = NULL;
    int proximoId = 1;

    do
    {
#if defined(_WIN32)
        system("cls");
#else
        if (isatty(fileno(stdout)))
            system("clear");
#endif
        printf("\n=== JOGO DA VELHA ===\n");
        printf("1) Jogar partidas de Jogo da Velha\n");
        printf("2) Salvar as partidas do Jogo da Velha\n");
        printf("3) Ranquear os usuarios do Jogo da Velha\n");
        printf("4) Sair do Jogo da Velha\n");
        printf("Escolha uma opcao: ");

        if (scanf("%d", &opcao) != 1)
        {
            while (getchar() != '\n')
                ;
            opcao = 0;
        }

        switch (opcao)
        {
        case 1:
            iniciarPartidas(&historico, &proximoId);
            break;
        case 2:
            salvarPartidas(&historico);
            pause();
            break;
        case 3:
            // exibirRanking();
            pause();
            break;
        case 4:
            printf("\nDeseja salvar as partidas da sessao no arquivo antes de sair? (S/N): ");
            scanf(" %c", &confirmarSaida);
            if (confirmarSaida == 'S' || confirmarSaida == 's')
            {
                salvarPartidas(&historico);
            }
            printf("\nEncerrando o programa. Ate logo!\n");
            break;
        default:
            printf("\nOpcao invalida! Tente novamente.\n");
            pause();
        }
    } while (opcao != 4);

    return 0;
}