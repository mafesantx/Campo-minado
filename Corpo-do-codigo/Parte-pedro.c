#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

//Definição de constantes e struct

#define MAX 8
#define MINAS_CURTO 2
#define MINAS_MEDIO 6
#define MINAS_LONGO 10

//struct celula

typedef struct
{
    int temMina;
    int revelado;
    char dono;
} Celula;

//declaração do tabuleiro

Celula tabuleiro[MAX][MAX];
int tamanhoTabuleiro;
int totalMinas;

//Funções de Manipulação
//1 Conversão de Letra para Índice

int letraParaIndice(char letra)
{
    return letra - 'A';
}

//2 Inicialização do Tabuleiro

void inicializarTabuleiro(int tamanho, int minas)
{
    //a Define o tamanho e quantidade de minas
    
    tamanhoTabuleiro = tamanho;
    totalMinas = minas;
    
//b Preenche todo o tabuleiro zerado
    
    for (int i = 0; i < tamanho; i++)
    {
        for (int j = 0; j < tamanho; j++)
        {
            tabuleiro[i][j].temMina = 0;
            tabuleiro[i][j].revelado = 0;
            tabuleiro[i][j].dono = ' ';
        }
    }
//c Distribui as minas de forma aleatória
    
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

//3 Mostrar o Tabuleiro (Modo Debug)

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

//4 Revelar Célula

int revelarCelula(char jogador, char colunaLetra, int linha)
{
    
    //a Converte a entrada do jogador para índices da matriz
    
    int i = linha - 1;
    int j = letraParaIndice(colunaLetra);

//b Valida se está dentro do tabuleiro
    
    if (i < 0 || i >= tamanhoTabuleiro || j < 0 || j >= tamanhoTabuleiro)
        return -1;

//c verifica se a célula já foi revelada
    
    if (tabuleiro[i][j].revelado)
        return -2;

//d Revela a célula e marca o dono
    
    tabuleiro[i][j].revelado = 1;
    tabuleiro[i][j].dono = jogador;

//e Retorna: 1 se o jogador revelou uma mina (perde ponto). 0 se a célula era segura. -1 se jogada inválida (fora do tabuleiro). -2 se célula já estava revelada
    
    if (tabuleiro[i][j].temMina)
        return 1;
    else
        return 0;
}
//Resumo do que eu fiz
//Criei a estrutura do tabuleiro e das células.
//Implementei o sistema de inicialização e distribuição aleatória das minas.
//Garanti a visualização do tabuleiro para debug.
//Fiz a lógica básica que permite o jogador jogar revelando células e identificando se pisou em mina ou não.
