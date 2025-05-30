#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

// Tamanhos máximos
#define MAX 8
#define MINAS_CURTO 2
#define MINAS_MEDIO 6
#define MINAS_LONGO 10

// Estrutura da célula
typedef struct {
    int temMina;
    int revelado;
    char dono; // '1' ou '2' (jogador que abriu a célula)
} Celula;

Celula tabuleiro[MAX][MAX];
int tamanhoTabuleiro;
int totalMinas;

// Função para converter letra (A-H) para índice
int letraParaIndice(char letra) {
    return letra - 'A';
}

// Inicializa o tabuleiro vazio
void inicializarTabuleiro(int tamanho, int minas) {
    tamanhoTabuleiro = tamanho;
    totalMinas = minas;

    // Zera todas as células
    for (int i = 0; i < tamanho; i++) {
        for (int j = 0; j < tamanho; j++) {
            tabuleiro[i][j].temMina = 0;
            tabuleiro[i][j].revelado = 0;
            tabuleiro[i][j].dono = ' ';
        }
    }

    // Sorteia minas aleatórias
    int colocadas = 0;
    while (colocadas < minas) {
        int i = rand() % tamanho;
        int j = rand() % tamanho;
        if (tabuleiro[i][j].temMina == 0) {
            tabuleiro[i][j].temMina = 1;
            colocadas++;
        }
    }
}

// Exibe o tabuleiro (modo visível para testes)
void mostrarTabuleiroDebug() {
    printf("   ");
    for (int j = 0; j < tamanhoTabuleiro; j++) {
        printf(" %c ", 'A' + j);
    }
    printf("\n");

    for (int i = 0; i < tamanhoTabuleiro; i++) {
        printf(" %d ", i + 1);
        for (int j = 0; j < tamanhoTabuleiro; j++) {
            if (tabuleiro[i][j].revelado) {
                if (tabuleiro[i][j].temMina)
                    printf(" * ");
                else
                    printf(" %c ", tabuleiro[i][j].dono);
            } else {
                printf(" # ");
            }
        }
        printf("\n");
    }
}

// Revela a célula jogada (retorna 1 se foi mina, 0 se livre)
int revelarCelula(char jogador, char colunaLetra, int linha) {
    int i = linha - 1;
    int j = letraParaIndice(colunaLetra);

    if (i < 0 || i >= tamanhoTabuleiro || j < 0 || j >= tamanhoTabuleiro)
        return -1; // inválido

    if (tabuleiro[i][j].revelado)
        return -2; // já revelada

    tabuleiro[i][j].revelado = 1;
    tabuleiro[i][j].dono = jogador;

    if (tabuleiro[i][j].temMina)
        return 1; // acertou mina
    else
        return 0; // célula livre
}
