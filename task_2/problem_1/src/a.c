#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>

typedef struct index
{
    int value;
    int row;
    int column;
} index;

double start_time, end_time;

#define SIZE 20 /* maximum matrix size */
#define WORKERS 10
#define MAXVALUE 99

int matrix[SIZE][SIZE];

int main(int argc, char *argv[])
{

    int i, j, maxV, minV, maxR, maxC, minR, minC, total = 0;

    omp_set_num_threads(WORKERS);

    for (i = 0; i < SIZE; i++)
    {
        printf("[ ");
        for (j = 0; j < SIZE; j++)
        {
            matrix[i][j] = rand() % MAXVALUE;
            printf(" %d", matrix[i][j]);
        }
         printf(" ]\n");
    }

    maxV = matrix[0][0];
    minV = matrix[0][0];

    start_time = omp_get_wtime();

#pragma omp parallel for reduction(+ \
                                   : total) private(j)
    for (i = 0; i < SIZE; i++)
    {
        int localMax = -1, localMaxR, localMaxC, localMin = MAXVALUE + 1,localMinR,localMinC;
        for (j = 0; j < SIZE; j++)
        {
            total += matrix[i][j];

            if (matrix[i][j] > localMax)
            {
                localMax = matrix[i][j];
                localMaxR = i;
                localMaxC = j;
            }

            if (matrix[i][j] < localMin)
            {
                localMin = matrix[i][j];
                localMinR = i;
                localMinC = j;
            }
        }

#pragma omp critical
        {
            if (maxV < localMax)
            {
                maxV = localMax;
                maxR = localMaxR;
                maxC = localMaxC;
            }

            if (minV > localMin)
            {
                minV = localMin;
                minR = localMinR;
                minC = localMinC;
            }
        }
    }

    end_time = omp_get_wtime();

    printf("the total is %d\n", total);
    printf("the max value is %d at row %d and column %d\n", maxV, maxR + 1, maxC + 1);
    printf("the min value is %d at row %d and column %d\n", minV, minR + 1, minC + 1);
    printf("it took %g seconds\n", end_time - start_time);
}

// #pragma omp critical
//     {
//             if (matrix[i][j] > maxV.value)
//             {
//                 maxV.value = matrix[i][j];
//                 maxV.row = i;
//                 maxV.column = j;
//             }

//             if (matrix[i][j] < minV.value)
//             {
//                 minV.value = matrix[i][j];
//                 minV.row = i;
//                 minV.column = j;
//             }
//     }
