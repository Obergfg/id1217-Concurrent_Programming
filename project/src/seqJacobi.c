#include <stdlib.h>
#include <stdio.h>
#include <omp.h>

#define GRID 10
#define ITERATIONS 100
#define TESTS 5

double **new;
double **grid;
double times[TESTS];

double start_time, end_time, maxDiff, temp;
int gridSize, iterations, boundary;
FILE *file;


int compareFunction(const void *a, const void *b)
{
    if (*(double*)a > *(double*)b) return 1;
    else if (*(double*)a < *(double*)b) return -1;
    else return 0;
}

void allocateGrids()
{
    boundary = gridSize + 2;

    grid = malloc(boundary * sizeof(double *));
    new = malloc(boundary * sizeof(double *));
    for (size_t i = 0; i < boundary; i++)
    {
        grid[i] = (double *)malloc(boundary * sizeof(double));
        new[i] = (double *)malloc(boundary * sizeof(double));
    }
}

void initializeGrids()
{
    for (int i = 0; i < boundary; i++)
        for (int j = 0; j < boundary; j++)
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

void findMaxDiff()
{
    int i, j;
    maxDiff = 0;
    for (i = 1; i <= gridSize; i++)
        for (j = 1; j <= gridSize; j++)
        {
            temp = grid[i][j] - new[i][j];
            if (temp < 0)
                temp = -temp;
            if (temp > maxDiff)
                maxDiff = temp;
        }
}

void jacobi()
{
    int i, j, k;
    for (i = 0; i < iterations; i++)
    {
        for (j = 1; j <= gridSize; j++)
            for (k = 1; k <= gridSize; k++)
                new[j][k] = (grid[j - 1][k] + grid[j + 1][k] + grid[j][k - 1] + grid[j][k + 1]) * 0.25;

        for (j = 1; j <= gridSize; j++)
            for (k = 1; k <= gridSize; k++)
                grid[j][k] = (new[j - 1][k] + new[j + 1][k] + new[j][k - 1] + new[j][k + 1]) * 0.25;
    }
}
void output()
{
    qsort(times, TESTS, sizeof(double), compareFunction);

    printf("Grid size: %d\tIterations: %d\tTime: %g  MaxDiff: %g\n", gridSize, iterations*2, times[TESTS/2], maxDiff);

    file = fopen("output/seqJacobi.txt", "w");

    for (int i = 0; i < boundary; i++)
        for (size_t j = 0; j < boundary; j++)
        {
            fprintf(file, "%.4f ", grid[i][j]);
            if (j == boundary - 1)
                fprintf(file, "\n");
        }

    fclose(file);
}

void initiate()
{

    allocateGrids();

    for (int i = 0; i < TESTS; i++)
    {
        initializeGrids();

        start_time = omp_get_wtime();
        jacobi();
        findMaxDiff();
        end_time = omp_get_wtime();

        times[i] = end_time - start_time;
    }
        output();
}

int main(int argc, char *argv[])
{
    gridSize = (argc > 1) ? atoi(argv[1]) : GRID;
    iterations = (argc > 2) ? atoi(argv[2])*0.5 : ITERATIONS*0.5;

    if (gridSize > GRID)
        gridSize = GRID;
    if (iterations > ITERATIONS)
        iterations = ITERATIONS;

    initiate();

    free(new);
    free(grid);
}