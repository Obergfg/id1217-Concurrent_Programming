#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>

double start_time, end_time;

#define SIZE 10000 /* maximum matrix size */
#define MAXVALUE 99

int matrix[SIZE][SIZE];

int main(int argc, char *argv[])
{
    int i, j, maxV = -1, minV = MAXVALUE + 1, maxR, maxC, minR, minC, total = 0;
    for (i = 0; i < SIZE; i++)
    {
        //printf("[ ");
        for (j = 0; j < SIZE; j++)
        {
            matrix[i][j] = rand() % MAXVALUE;
          //  printf(" %d", matrix[i][j]);
        }
        //printf(" ]\n");
    }


    start_time = omp_get_wtime();

    for (i = 0; i < SIZE; i++)
    {
        for (j = 0; j < SIZE; j++)
        {
            total += matrix[i][j];

            if (matrix[i][j] > maxV)
            {
                maxV = matrix[i][j];
                maxR = i;
                maxC = j;
            }

            if (matrix[i][j] < minV)
            {
                minV = matrix[i][j];
                minR = i;
                minC = j;
            }
        }
    }

    end_time = omp_get_wtime();

    printf("the total is %d\n", total);
    printf("the max value is %d at row %d and column %d\n", maxV, maxR + 1, maxC + 1);
    printf("the min value is %d at row %d and column %d\n", minV, minR + 1, minC + 1);
    printf("it took %g seconds\n", end_time - start_time);
}

