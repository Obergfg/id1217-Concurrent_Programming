/**
* To run the program type the following in the command line:
*
* mpicc -o pair distributedPairing.c
* mpiexec --hostfile hostfile -np 25 pair
*
*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include "mpi.h"


void teacher(int students)
{
    int student, partner, group = 0;

    while (0 < students)
    {
        MPI_Recv(&student, 1, MPI_INT, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        
        students--;

        if (!group && 0 < students){
            partner = student;
            group++;
        }else if(1 == group && 0 <= students){
            MPI_Send(&partner, 1, MPI_INT, student, 0, MPI_COMM_WORLD);
            MPI_Send(&student, 1, MPI_INT, partner, 0, MPI_COMM_WORLD);
            group = 0;
        }else
            MPI_Send(&student, 1, MPI_INT, student, 0, MPI_COMM_WORLD);
        
    }

    printf("I am the teacher and all students are grouped!\n");
}

void student(int rank)
{
    int partner;

    MPI_Send(&rank, 1,	MPI_INT, 0, 0, MPI_COMM_WORLD); 
    MPI_Recv(&partner, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    
    printf("I am student %d and my group partner is %d\n", rank, partner);

}

int main(int argc, char *argv[])
{
    int rank, size;

    MPI_Init(&argc, &argv);

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if(rank)
        student(rank);
    else
        teacher(size - 1);

    MPI_Finalize();

    return 0;
}