#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <sys/time.h>

//Biblioteca Concorrente
#include <omp.h>

#define N 2048 //Tamanho do tabuleiro NxN
#define EXECUTIONS 2000 //Número de gerações 

#define NUM_THREADS 8 //Número de Threads

//Struct para a definição das Threads
struct ThreadData {
    int id;
    float **grid;
    float **newgrid;
    int n;
};

//Aloca uma matriz dinamicamente
float **alocarMatriz() {
    float **matriz;
    matriz = (float **)malloc(N * sizeof(float *));

    for (int i = 0; i < N; i++) {
        matriz[i] = (float *)malloc(N * sizeof(float));
    }

    return matriz;
}

//Retorna a diferença de tempo entre "start" e "end"
float get_time_taken(struct timespec *start, struct timespec *end) {
    float time_taken;
    time_taken = (end->tv_sec - start->tv_sec) + 1e-9 * (end->tv_nsec - start->tv_nsec);
    return time_taken;
}

//Imprime a Matriz 50x50 usando ■ para células vivas e □ para células mortas
void PrintBoard_50x50(float **grid) {
    int i, j;

    for (i = 0; i < 50; i++) {
        for (j = 0; j < 50; j++) {
            if (grid[i][j] == 1)
                printf("■ ");
            else
                printf("□ ");
        }
        printf("\n");
    }
}

//Retorna no número de células vivas no tabuleiro
void getAliveCells(float **grid) {
    int alive = 0;
    int i, j;

    for (i = 0; i < N; i++) {
        for (j = 0; j < N; j++) {
            if (grid[i][j] == 1)
                alive++;
        }
    }

    printf("Células vivas: %d\n", alive);
}

//Retorna no número de células vizinhas vivas da célula (i, j)
int getNeighbors(float **grid, int i, int j) {
    int count = 0;
    int newi, newj;
    int x, y;

    for (x = -1; x <= 1; x++) {
        for (y = -1; y <= 1; y++) {

            if (x == 0 && y == 0) { // Ignora a célula atual
                continue;
            }

            newi = (i + x + N) % N;
            newj = (j + y + N) % N; // Tratamento mundo infinito
            if (grid[newi][newj] == 1) {
                count++;
            }
        }
    }

    return count;
}

//Atualiza o tabuleiro matando e revivendo as células
void UpdateGrid(float **grid, float **newgrid, int lines, int j){
    int neighbors;
    neighbors = getNeighbors(grid, lines, j);
    if (grid[lines][j] == 1) {
        if (neighbors == 2 || neighbors == 3) {
            newgrid[lines][j] = 1;
        } else {
            newgrid[lines][j] = 0;
        }
    } else {
        if (neighbors == 3) {
            newgrid[lines][j] = 1;
        } else {
            newgrid[lines][j] = 0;
        }
    }
}

//Executa uma rodada do Game Of Life
void play_one_round(float **grid, float **newgrid, int id, int n, int lines) {
    int i, j;

    //Cada thread responsável apenas por N/NUM_THREADS + 1 linhas
    for (i = 0; i < n; i++) {
        if (lines >= N)
            break;
        for (j = 0; j < N; j++) {
            UpdateGrid(grid, newgrid, lines, j);
        }
        lines += NUM_THREADS;
    }
}

//Inicia o Game Of Life 
void play_GameOfLife(float **grid, float **newgrid) {
    int i;
    int id;
    float time_taken;

    struct timespec start;
    struct timespec end;

    //Manipulação e disparo das threads
    omp_set_num_threads(NUM_THREADS); //Obtém o tempo em que se inicia (start)

    clock_gettime(CLOCK_REALTIME, &start);

    for (i = 0; i < EXECUTIONS; i++) {
        int n = (N / NUM_THREADS) + 1;
        #pragma omp parallel private(id)
        {
            id = omp_get_thread_num();
            int lines = id;
            play_one_round(grid, newgrid, id, n, lines);
        }

        // Troque os grids após cada iteração
        float **temp = grid;
        grid = newgrid;
        newgrid = temp;
    }

    clock_gettime(CLOCK_REALTIME, &end); //Obtém o tempo em que se termina (end)

    time_taken = get_time_taken(&start, &end);

    printf("O tempo que as gerações tomaram foi: %0.3fs\n", time_taken);
}

//Inseri a configuração inicial do R-Pentomino
void init_RPentomino(float** grid){
    int lin =10, col = 30;
    grid[lin  ][col+1] = 1.0;
    grid[lin  ][col+2] = 1.0;
    grid[lin+1][col  ] = 1.0;
    grid[lin+1][col+1] = 1.0;
    grid[lin+2][col+1] = 1.0;
}

//Inseri a configuração inicial do Glider
void init_glider(float** grid){
    int lin = 1, col = 1;
    grid[lin  ][col+1] = 1.0;
    grid[lin+1][col+2] = 1.0;
    grid[lin+2][col  ] = 1.0;
    grid[lin+2][col+1] = 1.0;
    grid[lin+2][col+2] = 1.0;

}

//Função main
int main(int argc, char *argv[]){
    float **grid, **newgrid;
    
    //Alocação grid
    grid = alocarMatriz();

    //Alocação newgrid
    newgrid = alocarMatriz();

    //Congiurações inicias
    init_glider(grid);
    init_RPentomino(grid);

    //Inicia o jogo
    play_GameOfLife(grid, newgrid);

    //Imprimi o número de células vivas
    getAliveCells(grid);

    //Imprimi o tabuleiro

    PrintBoard_50x50(grid);

    return 0;
}


