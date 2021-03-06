#include <stdlib.h>
#include <stdio.h>
#include <omp.h>

#define GRID 10
#define ITERATIONS 10
#define MINITERATIONS 4
#define TESTS 5
#define LEVELS 4
#define HIGHEST (LEVELS-1)
#define WORKERS 4

double start_time, end_time, maxDiff, temp, times[TESTS], ***grid, ***new;
int iterations,workers, boundary[LEVELS], gridSize[LEVELS];
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
        gridSize[i] = 2 * gridSize[i - 1] - 1;

    for (size_t i = 0; i < LEVELS; i++)
        boundary[i] = gridSize[i] + 2;

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

void interpolation(int coarse)
{

    int i, j, a, b;
    int fine = coarse + 1;

    for (i = 1, a = i; i <= gridSize[coarse]; i++)
    {
        for (j = 1, b = 1; j <= gridSize[coarse]; j++)
        {
            grid[fine][a][b] = grid[coarse][i][j];
            b += 2;
        }
        a += 2;
    }

    for (i = 2; i <= gridSize[fine]; i += 2)
        for (j = 1; j <= gridSize[fine]; j += 2)
            grid[fine][i][j] = (grid[fine][i - 1][j] + grid[fine][i + 1][j]) * 0.5;

    for (i = 1; i <= gridSize[fine]; i++)
        for (j = 2; j <= gridSize[fine]; j += 2)
            grid[fine][i][j] = (grid[fine][i][j - 1] + grid[fine][i][j + 1]) * 0.5;
}

void restriction(int fine)
{
    int i, j, a, b;
    int coarse = fine - 1;

    for (i = 1, a = 1; i <= gridSize[coarse]; i++)
    {
        for (j = 1, b = 1; j <= gridSize[coarse]; j++)
        {
            grid[coarse][i][j] = grid[fine][a][b]*0.5 + (grid[fine][a-1][b] + grid[fine][a][b-1] + grid[fine][a][b + 1] + grid[fine][a + 1][b]) * 0.125;
            b += 2;
        }
        a += 2;
    }
}

void findMaxDiff()
{
    int i, j;
    maxDiff = 0;
    for (i = 1; i < gridSize[HIGHEST]; i++)
        for (j = 1; j < gridSize[HIGHEST]; j++)
        {
            temp = grid[HIGHEST][i][j] - new[HIGHEST][i][j];
            if (temp < 0)
                temp = -temp;
            if (temp > maxDiff)
                maxDiff = temp;
        }
}

void jacobi(int level, int maxLevel)
{
    int i, j, k, iter;

    if(0 == level)
        iter = iterations;
    else
        iter = MINITERATIONS;

    for (i = 0; i < iter; i++)
    {
        for (j = 1; j <= gridSize[level]; j++)
            for (k = 1; k <= gridSize[level]; k++)
                new[level][j][k] = (grid[level][j - 1][k] + grid[level][j + 1][k] + grid[level][j][k - 1] + grid[level][j][k + 1]) * 0.25;

        for (j = 1; j <= gridSize[level]; j++)
            for (k = 1; k <= gridSize[level]; k++)
                grid[level][j][k] = (new[level][j - 1][k] + new[level][j + 1][k] + new[level][j][k - 1] + new[level][j][k + 1]) * 0.25;
    }

    if (maxLevel)
    {
        if (level == maxLevel)
        {
            restriction(level);
            jacobi(level-1, maxLevel-1);
        }
        else
        {
            interpolation(level);
            jacobi(level + 1, maxLevel);
        }
    }
    else if (!level)
        interpolation(level);
    else if (HIGHEST > level)
    {
        interpolation(level);
        jacobi(level + 1, 0);
    }
}

void output()
{
    qsort(times, TESTS, sizeof(double), compareFunction);

    printf("Grid size: %d\tIterations: %d\tTime: %g  MaxDiff: %g\n", gridSize[0], ITERATIONS, times[0], maxDiff);

    file = fopen("output/seqMultigrid.txt", "w");

        for (size_t i = 0; i < boundary[HIGHEST]; i++)
            for (size_t j = 0; j < boundary[3]; j++)
            {
                fprintf(file, "%.4f ", grid[HIGHEST][i][j]);
                if (j == boundary[HIGHEST] - 1)
                    fprintf(file, "\n");
            }
        fprintf(file, "\n");
 

    fclose(file);
}

void initiate()
{
    allocateGrids();

    for (int i = 0; i < TESTS; i++)
    {
        initializeGrids();

        start_time = omp_get_wtime();
        jacobi(0, 1);
        jacobi(1, 2);
        jacobi(1, 3);
        jacobi(1, 0);
        findMaxDiff();
        end_time = omp_get_wtime();

        times[i] = end_time - start_time;
    }
    output();
}

int main(int argc, char *argv[])
{
    gridSize[0] = (argc > 1) ? atoi(argv[1]) : GRID;
    iterations = (argc > 2) ? atoi(argv[2]) * 0.5 : ITERATIONS * 0.5;
    workers = (argc > 3) ? atoi(argv[3]) : WORKERS;

    if (gridSize[0] > GRID)
        gridSize[0] = GRID;
    if (iterations > ITERATIONS)
        iterations = ITERATIONS;
    if (workers > WORKERS)
        workers = WORKERS;

    omp_set_num_threads(workers);

    initiate();

    free(new);
    free(grid);
}