# Compute Sum, Min and Max of Matrix Elements

The purpose of this problem is to introduce you to the basic OpenMP usage:
Download, compile and run a program matrixSum-openmp.c. The program computes a sum of matrix elements in parallel using OpenMP. Develop the following three modified versions of the program.

(a) Extend the program so that in addition to sum it finds and prints a value and a position (indexes) of the maximum element of the matrix, and a value and a position of the minimum element of the matrix.  Initialize elements of the matrix to random values to check your solution. Use OpenMP constructs.

(b) Change the program developed in (a) so that the master thread prints the final results. Do not use explicit barriers and do not use arrays for partial results, such as  sums in the pthreads example.

(c) Run the program on different number of processors and report the speedup (sequential execution time divided by parallel execution time) for different number of processors (up to at least 4) and different sizes of matrices (at least 3 different sizes). Run each program several (at least 5) times and use the median value for execution time. Try to provide reasonable explanations to your  results. Measure only the parallel part of your program. Specify the number of processors used by specifying different number of threads (set the OMP_NUM_THREADS environment variable or use a call to omp_set_num_threads(), see the OpenMP specification).

To measure the execution time, use the omp_get_wtime function (see omp_get_wtime).
