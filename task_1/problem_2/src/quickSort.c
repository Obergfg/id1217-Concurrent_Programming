#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <time.h>
#include <sys/time.h>

#define LENGTH 10000000

double start_time, end_time;

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

void quickSort(int *array, int left, int right)
{

    if (left >= right)
        return;

    int pivot = array[(left + right) / 2];

    int index = partition(array, left, right, pivot);

    quickSort(array, left, index - 1);
    quickSort(array, index, right);
}

void print_array(int *array)
{
    for (int i = 0; i < LENGTH; i++)
        printf("%d ", array[i]);

    printf("\n");
}

int main()
{

    int *array = malloc(LENGTH * sizeof(int));

    for (int i = 0; i < LENGTH; i++)
        array[i] = rand() % 99;

    start_time = read_timer();
    quickSort(array, 0, LENGTH - 1);
    end_time = read_timer();

    // print_array(array);
    printf("The execution time is %g sec\n", end_time - start_time);

    free(array);
}