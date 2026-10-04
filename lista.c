#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "lista.h"

// Função para inicializar uma nova partida na memória
Partida *criarPartida(int id, const char *usuario, const char *computador)
{
    Partida *novaPartida = (Partida *)malloc(sizeof(Partida));

    if (novaPartida == NULL)
    {
        printf("Erro de alocacao de memoria para a partida!\n");
        exit(1);
    }

    novaPartida->id = id;
    strcpy(novaPartida->nomeUsuario, usuario);
    strcpy(novaPartida->nomeComputador, computador);

    // As listas de jogadas começam vazias (NULL)
    novaPartida->jogadasUsuario = NULL;
    novaPartida->jogadasComputador = NULL;

    strcpy(novaPartida->resultado, "Em andamento");
    novaPartida->prox = NULL;

    return novaPartida;
}

// Função para inserir uma jogada no final da lista encadeada correspondente
void inserirJogada(Nodo **inicio, int linha, int coluna)
{
    // Aloca memória para o novo nó
    Nodo *novoNodo = (Nodo *)malloc(sizeof(Nodo));

    if (novoNodo == NULL)
    {
        printf("Erro de alocacao de memoria para a jogada!\n");
        exit(1);
    }

    // Formata a jogada no padrão exigido: "linha-coluna" (ex: "1-2")
    // Usamos linha+1 e coluna+1 porque no código a matriz vai de 0 a 2, mas para o usuário é 1 a 3.
    snprintf(novoNodo->jogada, sizeof(novoNodo->jogada), "%d-%d", linha + 1, coluna + 1);
    novoNodo->prox = NULL; // Como entra no fim, o próximo é sempre NULL

    // Se a lista estiver vazia, o novo nó passa a ser o primeiro
    if (*inicio == NULL)
    {
        *inicio = novoNodo;
    }
    else
    {
        // Se não estiver vazia, percorre até achar o último nó
        Nodo *atual = *inicio;
        while (atual->prox != NULL)
        {
            atual = atual->prox;
        }
        // Conecta o último nó ao novo nó
        atual->prox = novoNodo;
    }
}

// Função para testar/exibir as jogadas registradas na lista
void imprimirJogadas(Nodo *inicio)
{
    Nodo *atual = inicio;
    if (atual == NULL)
    {
        printf("Nenhuma jogada.");
    }
    while (atual != NULL)
    {
        printf("[%s] ", atual->jogada);
        atual = atual->prox;
    }
    printf("\n");
}

// Função importantíssima para liberar a memória alocada ao final (evitar memory leak)
void liberarJogadas(Nodo *inicio)
{
    Nodo *atual = inicio;
    Nodo *proximo;

    while (atual != NULL)
    {
        proximo = atual->prox; // Salva o endereço do próximo
        free(atual);           // Libera o atual
        atual = proximo;       // Avança para o próximo
    }
}

void salvarPartidas(Partida **historico)
{
    if (historico == NULL || *historico == NULL)
    {
        printf("\nNenhuma partida na memoria para salvar.\n");
        return;
    }

    FILE *arquivo = fopen("partidas_velha.txt", "a");

    if (arquivo == NULL)
    {
        printf("Erro ao abrir o arquivo para salvar as partidas.\n");
        pause();
        return;
    }

    printf("\n=== SALVANDO PARTIDAS ===\n");

    Partida *p = *historico;
    Partida *proximaPartida = NULL;

    while (p != NULL)
    {
        printf("Salvando partida %d no arquivo", p->id);
        int i;
        for (i = 0; i < 3; i++)
        {
            printf(".");
            // sleep(1);
        }
        printf("\n");

        // 1. ID e Nome do Usuário
        fprintf(arquivo, "%d;%s;", p->id, p->nomeUsuario);

        // 2. Lista de jogadas do Usuário (separadas por ;)
        Nodo *jUser = p->jogadasUsuario;
        while (jUser != NULL)
        {
            fprintf(arquivo, "%s;", jUser->jogada);
            jUser = jUser->prox;
        }

        // 3. Nome do Computador
        fprintf(arquivo, "%s;", p->nomeComputador);

        // 4. Lista de jogadas do Computador (separadas por ;)
        Nodo *jComp = p->jogadasComputador;
        while (jComp != NULL)
        {
            fprintf(arquivo, "%s;", jComp->jogada);
            jComp = jComp->prox;
        }

        // 5. Resultado da partida e quebra de linha
        fprintf(arquivo, "%s\n", p->resultado);

        // Guarda a referência da próxima partida antes de liberar a atual
        proximaPartida = p->prox;

        // Libera a memória das sublistas de jogadas e do nó da partida
        liberarJogadas(p->jogadasUsuario);
        liberarJogadas(p->jogadasComputador);
        free(p);

        p = proximaPartida;
    }

    fclose(arquivo);

    // Reseta o ponteiro da main para NULL indicando que a memória foi limpa, evita duplicidade de partidas no "partidas_vela.txt"
    *historico = NULL;

    printf("\nPartidas salvas com sucesso!\n");
}

void exibirRanking()
{
    FILE *arquivo = fopen("partidas_velha.txt", "r");

    if (arquivo == NULL)
    {
        printf("\nNenhum registro encontrado em 'partidas_velha.txt'. Jogue e salve partidas primeiro!\n");
        pause();
        return;
    }

    JogadorRanking ranking[100]; // Suporta até 100 jogadores diferentes
    int totalJogadores = 0;
    char linha[1024];

    // Ler linha por linha do arquivo .txt
    while (fgets(linha, sizeof(linha), arquivo) != NULL)
    {
        // Remove a quebra de linha do final (\n) se existir
        linha[strcspn(linha, "\n")] = '\0';
        linha[strcspn(linha, "\r")] = '\0';

        if (strlen(linha) == 0)
            continue;

        // O resultado da partida é sempre o último campo da linha (separado por ;)
        char *ultimoPontoVirgula = strrchr(linha, ';');
        if (ultimoPontoVirgula != NULL)
        {
            char vencedor[50];
            strcpy(vencedor, ultimoPontoVirgula + 1);

            // Ignora empates na contagem de vitórias do ranking
            if (strcmp(vencedor, "Empate") != 0 && strlen(vencedor) > 0)
            {
                int encontrado = 0;

                // Verifica se o jogador já está no nosso vetor de ranking
                for (int i = 0; i < totalJogadores; i++)
                {
                    if (strcmp(ranking[i].nome, vencedor) == 0)
                    {
                        ranking[i].vitorias++;
                        encontrado = 1;
                        break;
                    }
                }

                // Se for um novo jogador, adiciona no vetor
                if (!encontrado && totalJogadores < 100)
                {
                    strcpy(ranking[totalJogadores].nome, vencedor);
                    ranking[totalJogadores].vitorias = 1;
                    totalJogadores++;
                }
            }
        }
    }

    fclose(arquivo);

    if (totalJogadores == 0)
    {
        printf("\nNenhuma vitoria registrada no arquivo 'partidas_velha.txt' ate o momento.\n");
        pause();
        return;
    }

    // Ordenação do Ranking em ordem decrescente (Bubble Sort simples)
    for (int i = 0; i < totalJogadores - 1; i++)
    {
        for (int j = 0; j < totalJogadores - i - 1; j++)
        {
            if (ranking[j].vitorias < ranking[j + 1].vitorias)
            {
                JogadorRanking temp = ranking[j];
                ranking[j] = ranking[j + 1];
                ranking[j + 1] = temp;
            }
        }
    }

    // Exibição formatada do Ranking
    printf("\n=========================================\n");
    printf("     RANKING DE JOGADORES (VITORIAS)     \n");
    printf("=========================================\n");
    for (int i = 0; i < totalJogadores; i++)
    {
        printf("%dº Lugar | %-20s : %d vitoria(s)\n", i + 1, ranking[i].nome, ranking[i].vitorias);
    }
    printf("=========================================\n");
}