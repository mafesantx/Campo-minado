#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#define MAX 8
#define MINAS_CURTO 2
#define MINAS_MEDIO 6
#define MINAS_LONGO 10

typedef struct
{
    int temMina;
    int revelado;
    char dono;
} Celula;

Celula tabuleiro[MAX][MAX];
int tamanhoTabuleiro;
int totalMinas;

int letraParaIndice(char letra)
{
    return letra - 'A';
}

void inicializarTabuleiro(int tamanho, int minas)
{
    tamanhoTabuleiro = tamanho;
    totalMinas = minas;

    for (int i = 0; i < tamanho; i++)
    {
        for (int j = 0; j < tamanho; j++)
        {
            tabuleiro[i][j].temMina = 0;
            tabuleiro[i][j].revelado = 0;
            tabuleiro[i][j].dono = ' ';
        }
    }

    int colocadas = 0;
    while (colocadas < minas)
    {
        int i = rand() % tamanho;
        int j = rand() % tamanho;
        if (tabuleiro[i][j].temMina == 0)
        {
            tabuleiro[i][j].temMina = 1;
            colocadas++;
        }
    }
}

void mostrarTabuleiroDebug()
{
    printf("   ");
    for (int j = 0; j < tamanhoTabuleiro; j++)
    {
        printf(" %c ", 'A' + j);
    }
    printf("\n");

    for (int i = 0; i < tamanhoTabuleiro; i++)
    {
        printf(" %d ", i + 1);
        for (int j = 0; j < tamanhoTabuleiro; j++)
        {
            if (tabuleiro[i][j].revelado)
            {
                if (tabuleiro[i][j].temMina)
                    printf(" * ");
                else
                    printf(" %c ", tabuleiro[i][j].dono);
            }
            else
            {
                printf(" # ");
            }
        }
        printf("\n");
    }
}

int revelarCelula(char jogador, char colunaLetra, int linha)
{
    int i = linha - 1;
    int j = letraParaIndice(colunaLetra);

    if (i < 0 || i >= tamanhoTabuleiro || j < 0 || j >= tamanhoTabuleiro)
        return -1;

    if (tabuleiro[i][j].revelado)
        return -2;

    tabuleiro[i][j].revelado = 1;
    tabuleiro[i][j].dono = jogador;

    if (tabuleiro[i][j].temMina)
        return 1;
    else
        return 0;
}
