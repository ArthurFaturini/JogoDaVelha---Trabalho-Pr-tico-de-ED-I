#ifndef JOGO_H
#define JOGO_H

// Função principal do menu para gerenciar partidas
void iniciarPartidas();

// Estrutura do tabuleiro e par ou ímpar
void inicializarTabuleiro(char tabuleiro[3][3]);
void mostrarTabuleiro(char tabuleiro[3][3]);
int decidirQuemComeca();

// Jogadas
void jogadaUsuario(char tabuleiro[3][3], char marcador);
void jogadaComputador(char tabuleiro[3][3], char marcador);

// Checagens de término da partida
int verificarVencedor(char tabuleiro[3][3], char marcador);
int verificarEmpate(char tabuleiro[3][3]);

#endif