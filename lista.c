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

void salvarPartidas(Partida *historico)
{
    FILE *arquivo = fopen("partidas_velha.txt", "a");

    if (arquivo == NULL)
    {
        printf("Erro ao abrir o arquivo para salvar as partidas.\n");
        return;
    }

    Partida *p = historico;

    printf("\n=== SALVANDO PARTIDAS ===\n");

    while (p != NULL)
    {
        printf("Salvando partida %d...\n", p->id);
        sleep(0.5);
        p = p->prox;
    }

    fclose(arquivo);
    pause();
}