#ifndef _REENTRANT
#define _REENTRANT
#endif

#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <time.h>
#include <sys/time.h>
#define MAXSIZE 6    /* maximum matrix size */
#define MAXWORKERS 3 /* maximum number of workers */
#define MAXVALUE 99
#define DEBUG

typedef struct element
{
  int value;
  int row;
  int column;
}element;

pthread_mutex_t barrier; /* mutex lock for the barrier */
pthread_cond_t go;       /* condition variable for leaving */
element *max;
element *min;

int numWorkers;               /* number of workers */
int numArrived = 0;           /* number who have arrived */
double start_time, end_time;  /* start and end times */
int size, stripSize;          /* assume size is multiple of numWorkers */
int sums[MAXWORKERS];         /* partial sums */
int matrix[MAXSIZE][MAXSIZE]; /* matrix */

void Barrier()
{
  pthread_mutex_lock(&barrier);
  numArrived++;
  if (numArrived == numWorkers)
  {
    numArrived = 0;
    pthread_cond_broadcast(&go);
  }
  else
    pthread_cond_wait(&go, &barrier);
  pthread_mutex_unlock(&barrier);
}

double read_timer()
{
  static bool initialized = false;
  static struct timeval start;
  struct timeval end;
  if (!initialized)
  {
    gettimeofday(&start, NULL);
    initialized = true;
  }
  gettimeofday(&end, NULL);
  return (end.tv_sec - start.tv_sec) + 1.0e-6 * (end.tv_usec - start.tv_usec);
}

/* Each worker sums the values in one strip of the matrix.
   After a barrier, worker(0) computes and prints the total */
void *Worker(void *arg)
{
  long myid = (long)arg;
  int total, i, j, first, last;

#ifdef DEBUG
  printf("worker %ld (pthread id %ld) has started\n", myid, pthread_self());
#endif

  /* determine first and last rows of my strip */
  first = myid * stripSize;
  last = (myid == numWorkers - 1) ? (size - 1) : (first + stripSize - 1);

  max[myid].value = -1;
  min[myid].value = MAXVALUE + 1;

  /* sum values in my strip */
  total = 0;

  for (i = first; i <= last; i++)
    for (j = 0; j < size; j++)
    {

      if (matrix[i][j] > max[myid].value)
      {
        max[myid].value = matrix[i][j];
        max[myid].row = i;
        max[myid].column = j;
      }

      if (matrix[i][j] < min[myid].value)
      {
        min[myid].value = matrix[i][j];
        min[myid].row = i;
        min[myid].column = j;
      }

      total += matrix[i][j];
    }
  sums[myid] = total;

  Barrier();

  if (myid == 0)
  {

    int maxVal = -1;
    int minVal = MAXVALUE + 1;
    int minRow, minColumn, maxRow, maxColumn;

    total = 0;

    for (i = 0; i < numWorkers; i++)
    {
      if(max[i].value > maxVal){
        maxVal = max[i].value;
        maxRow = max[i].row;
        maxColumn = max[i].column;

      }

      if (min[i].value < minVal){
        minVal = min[i].value;
        minRow = min[i].row;
        minColumn = min[i].column;
      }

      total += sums[i];
    }

    /* get end time */
    end_time = read_timer();
    /* print results */
    printf("The total is %d\n", total);
    printf("The maximum value is %d at row %d and column %d\n", maxVal, maxRow + 1, maxColumn + 1);
    printf("The minimum value is %d at row %d and column %d\n", minVal, minRow + 1, minColumn + 1);
    printf("The execution time is %g sec\n", end_time - start_time);
  }
}

int main(int argc, char *argv[])
{
  int i, j;
  long l; /* use long in case of a 64-bit system */

  pthread_attr_t attr;
  pthread_t workerid[MAXWORKERS];

  max = malloc(MAXWORKERS*sizeof(element));
  min = malloc(MAXWORKERS*sizeof(element));

  /* set global thread attributes */
  pthread_attr_init(&attr);
  pthread_attr_setscope(&attr, PTHREAD_SCOPE_SYSTEM);

  /* initialize mutex and condition variable */
  pthread_mutex_init(&barrier, NULL);
  pthread_cond_init(&go, NULL);

  /* read command line args if any */
  size = (argc > 1) ? atoi(argv[1]) : MAXSIZE;
  numWorkers = (argc > 2) ? atoi(argv[2]) : MAXWORKERS;

  if (size > MAXSIZE)
    size = MAXSIZE;

  if (numWorkers > MAXWORKERS)
    numWorkers = MAXWORKERS;

  stripSize = size / numWorkers;

  /* initialize the matrix */
  for (i = 0; i < size; i++)
    for (j = 0; j < size; j++)
      matrix[i][j] = rand() % MAXVALUE;

      /* print the matrix */
#ifdef DEBUG
  for (i = 0; i < size; i++)
  {
    printf("[ ");
    for (j = 0; j < size; j++)
    {
      printf(" %d", matrix[i][j]);
    }
    printf(" ]\n");
  }
#endif

  /* do the parallel work: create the workers */
  start_time = read_timer();
  for (l = 0; l < numWorkers; l++)
    pthread_create(&workerid[l], &attr, Worker, (void *)l);

  pthread_exit(NULL);
}
