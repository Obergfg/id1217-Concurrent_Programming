#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <time.h>
#include <omp.h>

#define LENGTH 100
#define WORKERS 4

double start_time, end_time;


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

    #pragma omp task firstprivate(array, left, index)
        quickSort(array, left, index - 1);

    #pragma omp task firstprivate(array, right, index)
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
    omp_set_num_threads(WORKERS);
    int *array = malloc(LENGTH * sizeof(int));
    int elements = LENGTH;

    for (int i = 0; i < LENGTH; i++)
        array[i] = rand() % 99;



        start_time = omp_get_wtime();

        #pragma omp parallel shared (array, elements)
            #pragma omp single nowait
                quickSort(array, 0, elements - 1);

        end_time = omp_get_wtime();
           print_array(array);
        printf("%g sec\n", end_time - start_time);
    
    free(array);
}