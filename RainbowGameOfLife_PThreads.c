#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <sys/time.h>

//Biblioteca Concorrente
#include <pthread.h>

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

//Calcula a média entre as 8 células vizinhas  da célula atual
float CalculateAverage(float sum){
    float avg;
    avg = sum / 8;
    return avg;
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

//Imprime a Matriz 50x50 usando ■ para células vivas e □ para células mortas
void PrintBoard_50x50(float** grid, int generation){ 
    int i, j;

    printf("Geração número: %d\n", generation);
    getAliveCells(grid);

    for(i=0 ; i<50 ; i++){
        for(j=0 ; j<50 ; j++){
            if(grid[i][j] == 1)
                printf("■ ");
            else
                printf("□ ");
        }
        printf("\n");
    }
    printf("----------------------------------------------------------------------------------------------------\n");
}


//Retorna no número de células vizinhas vivas da célula (i, j)
int getNeighbors(float **grid, int i, int j) {
    float count = 0;
    float avg;
    int newi, newj;
    int x, y;

    for(x=-1 ; x<=1 ; x++){
        for(y=-1 ; y<=1 ; y++){

            if (x == 0 && y == 0) { //Ignora a célula atual
                continue;
            }

            newi = (i + x + N) % N;
            newj = (j + y + N) % N; // Tratamento mundo infinito
            if (grid[newi][newj] > 0) { // Se a célula vizinha estiver viva
                count += grid[newi][newj]; //Soma o valor de todas as células vizinhas 
            }
            
        }
    }

    avg = CalculateAverage(count); //Chama a função para calcular a média dos valores das células vizinhas

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
void *play_one_round(void *arg) {
    struct ThreadData *data = (struct ThreadData *)arg;
    int id = data->id;
    float **grid = data->grid;
    float **newgrid = data->newgrid;
    int n = data->n;
    int lines = id;

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

    pthread_exit(NULL);
}

//Inicia o Game Of Life 
void play_GameOfLife(float **grid, float **newgrid) {
    int i, board_count = 0;
    pthread_t threads[NUM_THREADS];
    struct ThreadData thread_data[NUM_THREADS];
    float time_taken;

    struct timespec start;
    struct timespec end;

    clock_gettime(CLOCK_REALTIME, &start); //Obtém o tempo em que se inicia (start)

    for (i = 0; i < EXECUTIONS; i++) {
        int n = (N / NUM_THREADS) + 1;
        //Manipulação e disparo das threads
        for (int t = 0; t < NUM_THREADS; t++) {
            thread_data[t].id = t;
            thread_data[t].grid = grid;
            thread_data[t].newgrid = newgrid;
            thread_data[t].n = n;
            pthread_create(&threads[t], NULL, play_one_round, &thread_data[t]);
        }

        for (int t = 0; t < NUM_THREADS; t++) {
            pthread_join(threads[t], NULL);
        }

        // Atualiza os grids após cada iteração
        float **temp = grid;
        grid = newgrid;
        newgrid = temp;

        //Imprime os cinco primeiros tabuleiros
        if(board_count < 5){
            PrintBoard_50x50(grid, board_count+1);
            board_count++;
        }
    }

    clock_gettime(CLOCK_REALTIME, &end); //Obtém o tempo em que se termina (end)
 
    time_taken = get_time_taken(&start, &end);

    printf("O tempo que as gerações tomaram foi: %0.3fs\n", time_taken);
}

//Inseri a configuração inicial do R-Pentomino
void init_RPentomino(float **grid) {
    int lin = 10, col = 30;
    grid[lin][col + 1] = 1.0;
    grid[lin][col + 2] = 1.0;
    grid[lin + 1][col] = 1.0;
    grid[lin + 1][col + 1] = 1.0;
    grid[lin + 2][col + 1] = 1.0;
}

//Inseri a configuração inicial do Glider
void init_glider(float **grid) {
    int lin = 1, col = 1;
    grid[lin][col + 1] = 1.0;
    grid[lin + 1][col + 2] = 1.0;
    grid[lin + 2][col] = 1.0;
    grid[lin + 2][col + 1] = 1.0;
    grid[lin + 2][col + 2] = 1.0;
}

//Função main
int main(int argc, char *argv[]) {
    float **grid, **newgrid;

    // Alocação grid
    grid = alocarMatriz();

    // Alocação newgrid
    newgrid = alocarMatriz();

    //Congiurações inicias
    init_glider(grid);
    init_RPentomino(grid);

    //Inicia o jogo
    play_GameOfLife(grid, newgrid);

    //Imprimi o tabuleiro final
    PrintBoard_50x50(grid, EXECUTIONS);

    return 0;
}
