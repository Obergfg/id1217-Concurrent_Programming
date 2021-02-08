#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <time.h>
#include <sys/time.h>

#define LENGTH 10000
#define MAXTHREADS 8

typedef struct arguments
{
    int *array;
    int left;
    int right;
} arguments;

pthread_mutex_t mutex;
double start_time, end_time;
int workers = 1;

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

void swap(int *array, int left, int right)
{
    int temp = array[left];
    array[left] = array[right];
    array[right] = temp;
}

int partition(int *array, int left, int right, int pivot)
{

    while (left <= right)
    {

        while (array[left] < pivot)
        {
            left++;
        }

        while (array[right] > pivot)
        {
            right--;
        }

        if (left <= right)
        {
            swap(array, left, right);
            left++;
            right--;
        }
    }
    return left;
}

void *quickSort(void *args)
{
    arguments *arg = (arguments *)args;

    if (arg->left < arg->right)
    {
        arguments new_arg;
        int pivot = arg->array[(arg->left + arg->right) / 2];

        int index = partition(arg->array, arg->left, arg->right, pivot);

        new_arg.array = arg->array;
        new_arg.left = index;
        new_arg.right = arg->right;
        arg->right = index - 1;

        pthread_mutex_lock(&mutex);

        if (workers < MAXTHREADS)
        {
            workers++;
            pthread_mutex_unlock(&mutex);

            pthread_t thread;

            pthread_create(&thread, NULL, quickSort, (void *)&new_arg);
            quickSort((void *)arg);
            pthread_join(thread, NULL);

            pthread_mutex_lock(&mutex);
            workers--;
            pthread_mutex_unlock(&mutex);
        }
        else
        {
            pthread_mutex_unlock(&mutex);
            quickSort((void *)arg);
            quickSort((void *)&new_arg);
        }
    }
}

void print_array(int *array)
{
    for (int i = 0; i < LENGTH; i++)
        printf("%d ", array[i]);

    printf("\n");
}

int main()
{
    pthread_attr_t attr;
    pthread_t threads[MAXTHREADS];
    arguments *arg = malloc(sizeof(arguments));

    pthread_attr_init(&attr);
    pthread_attr_setscope(&attr, PTHREAD_SCOPE_SYSTEM);

    pthread_mutex_init(&mutex, NULL);

    int *array = malloc(LENGTH * sizeof(int));
    double time;

    printf("multiquick\n");

    for (int i = 0; i < MAXTHREADS; i++)
    {
        time = 0;

        for (int k = 0; k < 10; k++)
        {

            for (int i = 0; i < LENGTH; i++)
                array[i] = rand() % 99;

            arg->array = array;
            arg->left = 0;
            arg->right = LENGTH - 1;

            start_time = read_timer();

            for (int j = 0; j < i; j++)
                pthread_create(&threads[j], &attr, quickSort, (void *)arg);

            for (int j = 0; j < i; j++)
                pthread_join(threads[j], NULL);

            end_time = read_timer();

            time += (end_time - start_time);
        }
        //print_array(array);
        printf("% d %g\n",(i+1), time/10);
    }

    free(array);
    free(arg);
}