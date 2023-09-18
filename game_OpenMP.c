#include <stdio.h>

#include <stdlib.h>

#include <math.h>

//#include <omp.h>

 

#define N 10

#define EXECUTIONS 10

 

float **alocarMatriz() {

    float **matriz;

    matriz = (float **)malloc(N * sizeof(float *));

 

    for (int i = 0; i < N; i++) {

        matriz[i] = (float *)malloc(N * sizeof(float));

    }

 

    return matriz;

}

 

int getAliveCells(float** grid){

    int alive, i, j;

 

    for(i=0 ; i<N ; i++){

        for(j=0 ; j<N ; j++){

            if(grid[i][j] == 1)

                alive++;

        }

    }

 

    return alive;

}

 

int getNeighbors(float** grid, int i, int j){

    int count = 0;

    int newi, newj;

    int x, y;

 

    for(x=-1 ; x<=1 ; x++){

        for(y=-1 ; y<=1 ; y++){

 

            if (x == 0 && y == 0) { //Ignora a célula atual

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

 

void play_one_round(float** grid, float** newgrid){

    int i, j, neighbors;

    for(i=0 ; i<N ; i++){

        for(j=0 ; j<N ; j++){

 

            neighbors = getNeighbors(grid, i, j);

            if(grid[i][j] == 1){

                if(neighbors == 2 || neighbors == 3){

                    newgrid[i][j] = 1;

                }

                else{

                    newgrid[i][j] = 0;

                }

            }

            else{

                if(neighbors == 3){

                    newgrid[i][j] = 1;

                }

                else{

                    newgrid[i][j] = 0;

                }

            }

        }

    }

 

    for(i=0 ; i<N ; i++){

        for(j=0 ; j<N ; j++){

            grid[i][j] = newgrid[i][j];

        }

    }

}

 

void init_glider(float** grid){

    int lin = 1, col = 1;

    grid[lin  ][col+1] = 1.0;

    grid[lin+1][col+2] = 1.0;

    grid[lin+2][col  ] = 1.0;

    grid[lin+2][col+1] = 1.0;

    grid[lin+2][col+2] = 1.0;

 

}

 

 

int main(int argc, char *argv[]){

    int i,j;

    float **grid, **newgrid;

 

    //Alocação grid

    grid = alocarMatriz();

 

    //Alocação newgrid

    newgrid = alocarMatriz();

 

    init_glider(grid);

 

    for(i=0 ; i<EXECUTIONS ; i++)

        play_one_round(grid, newgrid);

 

    printf("Células vivas: %d\n", getAliveCells(newgrid));

 

    for(i=0 ; i<N ; i++){

        for(j=0 ; j<N ; j++)

            printf("%.0f ", newgrid[i][j]);

        printf("\n");

    }

    return 0;

}