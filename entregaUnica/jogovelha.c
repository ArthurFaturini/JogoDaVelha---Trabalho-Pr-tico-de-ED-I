//================================================================================
// PROJETO DA DISCIPLINA: JOGO DA VELHA EM C
// ALUNO(S): Arthur Faturini Rangel de Paula Araujo / Márcia Kamilla ...
//================================================================================

//================================================================================
// ARQUIVO: lista.h
//================================================================================
// lista.h
// #ifndef LISTA_H
// #define LISTA_H

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

// Estrutura auxiliar interna apenas para contagem e ordenação do Ranking
typedef struct
{
    char nome[50];
    int vitorias;
} JogadorRanking;

// Funções para gerenciar as jogadas
Partida *criarPartida(int id, const char *usuario, const char *computador);
void inserirJogada(Nodo **inicio, int linha, int coluna);
void imprimirJogadas(Nodo *inicio);
void liberarJogadas(Nodo *inicio);
void salvarPartidas(Partida **historico);
void exibirRanking();

// Função que pausa a tela
void pause();

// #endif

//================================================================================
// ARQUIVO: jogo.h
//================================================================================
// #ifndef JOGO_H
// #define JOGO_H

// #include "lista.h" // Inclui as estruturas de dados

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
int minimax(char tabuleiro[3][3], int profundidade, int eMaximizador, char marcadorComp, char marcadorUser, int limiteProfundidade);

// Checagens de término da partida
int verificarVencedor(char tabuleiro[3][3], char marcador);
int verificarEmpate(char tabuleiro[3][3]);

// Função que pausa a tela
void pause();

// #endif

//================================================================================
// ARQUIVO: lista.c
//================================================================================
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
// #include "lista.h"

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

//================================================================================
// ARQUIVO: jogo.c
//================================================================================
// #include <stdio.h>
// #include <stdlib.h>
#include <time.h>
// #include <string.h>
// #include <unistd.h>
// #include "jogo.h"

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

//================================================================================
// ARQUIVO: main.c
//================================================================================
// #include <stdio.h>
// #include <stdlib.h>
// #include <unistd.h>
// #include "jogo.h"

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
            exibirRanking();
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