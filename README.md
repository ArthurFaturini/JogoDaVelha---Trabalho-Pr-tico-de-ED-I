# Jogo da Velha - Trabalho Prático I (Fase Atual)
Aqui estão as instruções rápidas para rodar a nossa versão atual do projeto. O jogo já conta com a lógica completa da partida, regra do par/ímpar e armazenamento dos movimentos nas listas lineares encadeadas.

# 📂 Arquivos Necessários
Certifica-te de que todos estes 5 arquivos estão juntos na mesma pasta:

- `main.c` (Controla o menu principal)

- `jogo.c` e `jogo.h` (Lógica do tabuleiro, turnos e verificações de vitória/empate)

- `lista.c e lista.h` (Estrutura dos Nodos e gerenciamento do histórico na memória)

# ⚙️ Como Compilar
Para essa forma é preciso usar um compilador, eu utilizei o MINGW64, bem tranquilo de instalar e configurar, qualquer coisa pede ajuda para IA. Acredito que funcione no Code Blocks ou Dev C++, mas aí tem que ver direitinho.

Abra o *Prompt de Comando (cmd)* nativo do Windows na pasta do projeto.
(Dica: Evite usar o terminal bash do MINGW64 se ele der erro de "Acesso Negado" ou erro 5).

Execute o seguinte comando para compilar o código de forma modular:

```DOS
gcc -Wall -Wextra main.c jogo.c lista.c -o jogovelha
```

# ▶️ Como Jogar
Se o comando acima não retornar nenhum erro, o arquivo jogovelha.exe foi criado com sucesso. Para iniciar o jogo, basta digitar:

```DOS
jogovelha
```
O que já está funcionando para testares:

1. Escolher a opção 1 no menu.

2. Fazer o sorteio de quem começa.

3. Jogar contra o computador (ele ainda joga no primeiro espaço vazio).

4. Ao decidir parar de jogar (apertar 'n'), o programa vai usar as listas encadeadas para exibir o histórico completo da sessão e pausar a tela para leitura antes de voltar ao menu.