# Compute Sum, Min and Max of Matrix Elements

The purpose of this problem is to introduce you to the Pthreads library. Download, compile and run a program matrixSum.cFörhandsgranska dokumentet. The program computes a sum of matrix elements in parallel using Pthreads. Develop the following three modified versions of the program.

(a) Extend the program so that in addition to sum it finds and prints a value and a position (indexes) of the maximum element of the matrix, and a value and a position of the minimum element of the matrix.  Initialize elements of the matrix to random values (rather than to all ones like in the above example) to check your solution.

(b) Change the program developed in (a) so that the main thread prints the final results. Do not call the Barrier function, and do not use arrays for partial results, such as  sums in the above example.

(c) Change the program developed in (b) so that it uses a "bag of tasks" that is represented as a row counter, which is initialized to 0. A worker gets a task (i.e. the number of the row to process) out of the bag by reading a value of the counter and incrementing the counter as described for the matrix multiplication problem in Slides 27-29 about "bag of tasks" in Lecture 5.
