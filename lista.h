// lista.h
#ifndef LISTA_H
#define LISTA_H

// Nó para armazenar uma jogada específica
typedef struct Nodo
{
    char jogada[25]; // Guarda o formato "1-2" com o terminador nulo e tamanho 25 apenas para silenciar o compilador
    struct Nodo *prox;
} Nodo;

// Estrutura que guarda todo o contexto de uma partida
typedef struct Partida
{
    int id;
    char nomeUsuario[50];
    char nomeComputador[50];
    Nodo *jogadasUsuario;
    Nodo *jogadasComputador;
    char resultado[50];
    struct Partida *prox; // Ponteiro para encadear múltiplas partidas no histórico
} Partida;

// Funções para gerenciar as jogadas
Partida *criarPartida(int id, const char *usuario, const char *computador);
void inserirJogada(Nodo **inicio, int linha, int coluna);
void imprimirJogadas(Nodo *inicio);
void liberarJogadas(Nodo *inicio);

#endif