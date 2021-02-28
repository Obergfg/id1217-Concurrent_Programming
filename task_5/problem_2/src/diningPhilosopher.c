/**
* To run the program type the following in the command line:
*
* mpicc -o philo diningPhilosopher.c
* mpiexec --hostfile hostfile -np 6 philo
*
*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include "mpi.h"

#define PHILOSOPHERS 5

int forks[PHILOSOPHERS] = {1,1,1,1,1};
int isDining[PHILOSOPHERS] = {0,0,0,0,0};

void waiter()
{
    int eat, philosopher, left, right;
    printf("The waiter is ready to serve the philosophers\n");

    while (1)
    {

        MPI_Recv(&philosopher, 1, MPI_INT, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

        left = philosopher - 1;
        right = (philosopher) % PHILOSOPHERS;

        if (isDining[left])
        {
            forks[left] = 1;
            forks[right] = 1;
            isDining[left] = 0;
        }
        else
        {
            eat = forks[left] * forks[right];
         
            if (eat)
            {
                isDining[left] = 1;
                forks[left] = 0;
                forks[right] = 0;
            }

            MPI_Send(&eat, 1, MPI_INT, philosopher, 0, MPI_COMM_WORLD);
        }
    }
}

void philosopher(int rank)
{
    int eat = 0;
    sleep(rand() % 5);
    printf("Philosopher %d has taken a place at the table\n", rank);

    while (1)
    {
        MPI_Send(&rank, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
        MPI_Recv(&eat, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

        if (eat)
        {
            printf("Philosopher %d is eating\n", rank);
            sleep(2 + rand() % 5);
            printf("Philosopher %d returns his forks\n", rank);
            MPI_Send(&rank, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
            sleep(1);
        }
        else
        {
            sleep(1);
            printf("Philosopher %d is thinking\n", rank);
        }
        sleep(rand() % 5);
    }
}

int main(int argc, char *argv[])
{
    int rank, size;

    MPI_Init(&argc, &argv);

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (rank)
    {
       philosopher(rank);
    }
    else
    {
        waiter();
    }

    MPI_Finalize();

    return 0;
}