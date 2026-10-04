#ifndef JOGO_H
#define JOGO_H

#include "lista.h" // Inclui as estruturas de dados

// Função principal do menu para gerenciar partidas
void iniciarPartidas(Partida **historico, int *proximoId); // alterei para que a função receba o ponteiro do histórico e o próximo ID
void salvarPartidas(Partida **historico);

// Estrutura do tabuleiro e par ou ímpar
void inicializarTabuleiro(char tabuleiro[3][3]);
void mostrarTabuleiro(char tabuleiro[3][3]);
int decidirQuemComeca();

// Jogadas atualizadas para receberem o ponteiro da lista onde devem salvar o movimento
void jogadaUsuario(char tabuleiro[3][3], char marcador, Nodo **listaJogadas);
void jogadaComputador(char tabuleiro[3][3], char marcador, Nodo **listaJogadas);

// Checagens de término da partida
int verificarVencedor(char tabuleiro[3][3], char marcador);
int verificarEmpate(char tabuleiro[3][3]);

// Função que pausa a tela
void pause();

#endif