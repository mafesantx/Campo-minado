#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <locale.h>

#define MAX 8
#define MINAS_CURTO 2
#define MINAS_MEDIO 6
#define MINAS_LONGO 10
#define TEMPO_PARTIDA 600

static sem_t semaforo;

typedef struct {
    int temMina;
    int revelado;
    char dono;
} Celula;

Celula tabuleiro[MAX][MAX];
int tamanhoTabuleiro;
int totalMinas;
int minasRestantes;
int turno = 0;  // 0 = jogador A, 1 = jogador B
int fimDeJogo = 0;
int minasJogadorA = 0;
int minasJogadorB = 0;
int tempoEsgotado = 0;

int letraParaIndice(char letra) {
    return letra - 'A';
}

void inicializarTabuleiro(int tamanho, int minas) {
    tamanhoTabuleiro = tamanho;
    totalMinas = minas;
    minasRestantes = minas;

    for (int i = 0; i < tamanho; i++) {
        for (int j = 0; j < tamanho; j++) {
            tabuleiro[i][j].temMina = 0;
            tabuleiro[i][j].revelado = 0;
            tabuleiro[i][j].dono = ' ';
        }
    }

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

int revelarCelula(char jogador, char colunaLetra, int linha) {
    int i = linha - 1;
    int j = letraParaIndice(colunaLetra);

    if (i < 0 || i >= tamanhoTabuleiro || j < 0 || j >= tamanhoTabuleiro)
        return -1;

    if (tabuleiro[i][j].revelado)
        return -2;

    tabuleiro[i][j].revelado = 1;
    tabuleiro[i][j].dono = jogador;

    if (tabuleiro[i][j].temMina){
        if (jogador == 'A') {
            minasJogadorA++;
        } else {
            minasJogadorB++;
        }
        return 1;
    } else {
        return 0;
    }
        
}
void* threadTempo(void* arg) {
    sleep(TEMPO_PARTIDA);

    sem_wait(&semaforo);

    if (!fimDeJogo) {
        tempoEsgotado = 1;
        fimDeJogo = 1;
        printf("\nTempo esgotado! Fim de jogo. Ambos os jogadores perderam.\n");
    }
    sem_post(&semaforo);

    return NULL;
}

void* threadJogador(void* arg) {
    char jogador = *(char*)arg;
    int jogadorID = (jogador == 'A') ? 0 : 1;


   while (1) {
        while(1) {
            sem_wait(&semaforo);

            if (fimDeJogo) {
                sem_post(&semaforo);
                free(arg);
                pthread_exit(NULL);
            }
            if (turno == jogadorID){
                break;
            }
        
            sem_post(&semaforo);

            usleep(100000);
        }
        

        mostrarTabuleiroDebug();
        printf("Jogador %c, escolha uma célula (ex: A3): ", jogador);

        char coluna;
        int linha;
        scanf(" %c%d", &coluna, &linha);

        int resultado = revelarCelula(jogador, coluna, linha);

        if (resultado == -1 || resultado == -2) {
            printf("Jogada inválida. Tente novamente.\n");
        } else {
            if (resultado == 1) {
                printf("Jogador %c encontrou uma mina!\n", jogador);
                minasRestantes--;
                if (jogador == 'A') {
                    minasJogadorA++;
                } else {
                    minasJogadorB++;
                }
            } else {
                printf("Nada aqui. Boa jogada!\n");
            }

            if (minasRestantes <= 0) {
                fimDeJogo = 1;
                printf("\nFim de jogo! Todas as minas foram encontradas.\n");

                mostrarTabuleiroDebug();

                if(minasJogadorA > minasJogadorB) {
                    printf("Jogador B venceu!\n");
                } else if(minasJogadorA < minasJogadorB) {
                    printf("Jogador A venceu!\n");
                } else {
                    printf("Empate!\n");
                }
            }
            else {
                turno = 1 - turno;
            }
            
        }
        sem_post(&semaforo);
    }
    return NULL;
}

int main() {
    setlocale(LC_ALL, "Portuguese");

    srand(time(NULL));


    int escolha;
    printf("Escolha a dificuldade (1 - Fácil, 2 - Intermediário, 3 - Difícil): ");	
    scanf("%d", &escolha);

    switch (escolha) {
        case 1: inicializarTabuleiro(4, MINAS_CURTO); break;
        case 2: inicializarTabuleiro(6, MINAS_MEDIO); break;
        case 3: inicializarTabuleiro(8, MINAS_LONGO); break;
        default:
            printf("Opção inválida. Usando dificuldade média.\n");
            inicializarTabuleiro(6, MINAS_MEDIO);
            break;
    }

    pthread_t t1, t2, tempoThread;

    char jogadorA = 'A';
    char jogadorB = 'B';

    sem_init(&semaforo, 0, 1);

    pthread_create(&tempoThread, NULL, threadTempo, NULL);
    pthread_create(&t1, NULL, threadJogador, &jogadorA);
    pthread_create(&t2, NULL, threadJogador, &jogadorB);

    pthread_join(t1, NULL);
    pthread_join(t2, NULL);
    pthread_join(tempoThread, NULL);

    sem_destroy(&semaforo);

    printf("\nJogo encerrado. Obrigado por jogar!\n");
    return 0;
}