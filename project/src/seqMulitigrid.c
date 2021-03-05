#include <stdlib.h>
#include <stdio.h>
#include <omp.h>

#define GRID 5
#define ITERATIONS 10
#define TESTS 5
#define LEVELS 4

double start_time, end_time, maxDiff, temp, times[TESTS], ***grid, ***new;
int iterations, boundary[LEVELS], gridSize[LEVELS];
FILE *file;

int compareFunction(const void *a, const void *b)
{
    if (*(double *)a > *(double *)b)
        return 1;
    else if (*(double *)a < *(double *)b)
        return -1;
    else
        return 0;
}

void allocateGrids()
{
    for (size_t i = 1; i < LEVELS; i++)
        gridSize[i] = 2*gridSize[i-1]-1;
    
    for (size_t i = 0; i < LEVELS; i++)
        boundary[i] = gridSize[i]+2;
    
    grid = (double ***)malloc(LEVELS * sizeof(double **));
    new = (double ***)malloc(LEVELS * sizeof(double **));

    for (size_t i = 0; i < LEVELS; i++)
    {
        grid[i] = (double **)malloc(boundary[i] * sizeof(double *));
        new[i] = (double **)malloc(boundary[i] * sizeof(double *));

        for (size_t j = 0; j < boundary[i]; j++)
        {
            grid[i][j] = (double *)malloc(boundary[i] * sizeof(double));
            new[i][j] = (double *)malloc(boundary[i] * sizeof(double));
        }
    }
}

void initializeGrids()
{
    for (size_t i = 0; i < LEVELS; i++)
        for (int j = 0; j < boundary[i]; j++)
            for (int k = 0; k < boundary[i]; k++)
                if (j == 0 || k == 0 || j == boundary[i] - 1 || k == boundary[i] - 1)
                {
                    grid[i][j][k] = 1;
                    new[i][j][k] = 1;
                }
                else
                {
                    grid[i][j][k] = 0;
                    new[i][j][k] = 0;
                }
}

void interpolation(int coarse){

    int i,j,a,b;
    int fine = coarse+1;

    for (i = 1, a = i; i <= gridSize[coarse]; i++)
    {
        for (j = 1, b = 1; j <= gridSize[coarse]; j++)
        {
            grid[fine][a][b] = grid[coarse][i][j];
            b += 2;
        }
        a += 2;
    }
    
    for(i = 2; i <= gridSize[fine]; i += 2)
        for(j = 1; j <= gridSize[fine]; j += 2)
            grid[fine][i][j] = (grid[fine][i-1][j] + grid[fine][i+1][j])*0.5; 
        
    for(i = 1; i <= gridSize[fine]; i++)
        for(j = 2; j <= gridSize[fine]; j += 2)
            grid[fine][i][j] = (grid[fine][i][j-1] + grid[fine][i][j+1])*0.5;
}




// void findMaxDiff()
// {
//     int i, j;
//     maxDiff = 0;
//     for (i = 1; i < gridSize; i++)
//         for (j = 1; j < gridSize; j++)
//         {
//             temp = grid[i][j] - new[i][j];
//             if (temp < 0)
//                 temp = -temp;
//             if (temp > maxDiff)
//                 maxDiff = temp;
//         }
// }

void solveGrid(int level)
{
    int i, j, k;
    for (i = 0; i < iterations; i++)
    {
        for (j = 1; j <= gridSize[level]; j++)
            for (k = 1; k <= gridSize[level]; k++)
                new[level][j][k] = (grid[level][j - 1][k] + grid[level][j + 1][k] + grid[level][j][k - 1] + grid[level][j][k + 1]) * 0.25;

        for (j = 1; j <= gridSize[level]; j++)
            for (k = 1; k <= gridSize[level]; k++)
                grid[level][j][k] = (new[level][j - 1][k] + new[level][j + 1][k] + new[level][j][k - 1] + new[level][j][k + 1]) * 0.25;
    }
}

void output()
{
    // qsort(times, TESTS, sizeof(double), compareFunction);

    // printf("Grid size: %d\tIterations: %d\tTime: %g  MaxDiff: %g\n", gridSize, iterations, times[2], maxDiff);

    file = fopen("output/seqMultigrid.txt", "w");

    for (size_t l = 0; l < LEVELS; l++)
    {
        for (size_t i = 0; i < boundary[l]; i++)
            for (size_t j = 0; j < boundary[l]; j++)
            {
                fprintf(file, "%.4f ", grid[l][i][j]);
                if (j == boundary[l] - 1)
                    fprintf(file, "\n");
            }
        fprintf(file, "\n"); 
    }
        
    fclose(file);
}

void initiate()
{
    allocateGrids();
    initializeGrids();
    solveGrid(0);
    interpolation(0);
    // for (int i = 0; i < 5; i++)
    // {
    //     initializeGrids();

    //     start_time = omp_get_wtime();
    //     solveGrid();
    //     findMaxDiff();
    //     end_time = omp_get_wtime();

    //     times[i] = end_time - start_time;
    // }
         output();
}

int main(int argc, char *argv[])
{
    gridSize[0] = (argc > 1) ? atoi(argv[1]) : GRID;
    iterations = (argc > 2) ? atoi(argv[2])*0.5 : ITERATIONS*0.5;

    if (gridSize[0] > GRID)
        gridSize[0] = GRID;
    if (iterations > ITERATIONS)
        iterations = ITERATIONS;

    initiate();

    free(new);
    free(grid);
}