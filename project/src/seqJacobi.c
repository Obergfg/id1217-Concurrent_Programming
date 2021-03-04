#include <stdlib.h>
#include <stdio.h>

#define MAXSIZE 10
#define MAXITERATIONS 20

double ** new;
double ** grid;



void allocateGrids(int boundary)
{
    grid = malloc(boundary * sizeof(double*));
    new = malloc(boundary * sizeof(double*));

    for (size_t i = 0; i < boundary; i++)
    {
        grid[i] = (double*)malloc(boundary * sizeof(double));
        new[i] = (double*)malloc(boundary * sizeof(double));
    }
}

void initializeGrids(int boundary)
{
    for (int i = 0; i < boundary; i++)
    {
        for (int j = 0; j < boundary; j++)
        {
            if (i == 0 || j == 0 || i == boundary - 1 || j == boundary - 1)
            {
                grid[i][j] = 1;
                new[i][j] = 1;
            }
            else
            {
                grid[i][j] = 0;
                new[i][j] = 0;
            }
        }
    }
}


void solveGrid(int gridSize, int iterations){

    int i,j,k;
  
    for(i = 0; i < iterations; i++){
        for(j = 1; j < gridSize; j++){
           for(k = 1; k < gridSize; k++){
                 new[j][k] = (grid[j-1][k] + grid[j+1][k] + grid[j][k-1] + grid[j][k+1]) * 0.25;
            }
        }
        for(j = 1; j < gridSize; j++){
           for(k = 1; k < gridSize; k++){
                grid[j][k] = (new[j-1][k] + new[j+1][k] + new[j][k-1] + new[j][k+1]) * 0.25;          
           }
        }
    }
}

void print_grid(int boundary)
{
    for (int i = 0; i < boundary; i++)
        for (size_t j = 0; j < boundary; j++)
        {
            printf("%.2f ", grid[i][j]);
            if (j == boundary - 1)
                printf("\n");
        }

    printf("\n");
}

int main(int argc, char *argv[])
{
    int gridSize, iterations, boundary;

    gridSize = (argc > 1) ? atoi(argv[1]) : MAXSIZE;
    iterations = (argc > 2) ? atoi(argv[2]) : MAXITERATIONS;

    if (gridSize > MAXSIZE)
        gridSize = MAXSIZE;
    if (iterations > MAXITERATIONS)
        iterations = MAXITERATIONS;

    boundary = gridSize + 1;
    iterations = iterations * 0.5;

    allocateGrids(boundary);
    initializeGrids(boundary);
   // print_grid(boundary);
    solveGrid(gridSize, iterations);
    print_grid(boundary);

    free(new);
    free(grid);
}