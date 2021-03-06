# PDE Solvers for the Laplace's Equation

Objectives

The main objectives of this project are summarized as follows:

    To understand how to develop a parallel program and optimize it for performance;
    To understand how use shared memory or message programming models in developing a parallel program:
        how to use threads in a parallel program and how to use shared memory for thread interaction;
        how to create a parallel program for a distributed memory platform and how to use message passing in a parallel program
    To learn how to use parallel programming environments such as MPI, Pthreads, and OpenMP;
    To implement a parallel application for a shared-memory or/and a distributed memory multiprocessor.

Task

In this project you are to develop and to evaluate efficient parallel programs for a grid computation to solve partial differential equations (a.k.a. PDE solvers), conduct timing experiments to analyze the performance of your programs, and write a report describing your results and what you have learned. Laplace's equation is an example of a two-dimensional partial differential equation. This kind of equation can be approximated by a finite different method as described in Section 11.1 of the G.Andrews textbook "Foundations of Multithreaded, Parallel, and Distributed Programming". There are several iterative techniques used in PDE solvers: Jacobi, Gauss-Seidel, successive over-relaxation (SOR), and multigrid. The table below shows how the new value for each point of the grid is computed on each iteration by different methods. Note that here we consider PDE with Dirichlet boundary condition where values on boundaries on the grid are constant.
 
Method 	Computation of a new value for a i,j point of the grid of points
Jacobi 	T[i,j] = (G[i,j-1] + G[i-1,j] +G[i+1,j] + G[i,j+1]) / 4
After each value is computed, G is replaced by T (or better yet, their roles are swapped)
Gauss-Seidel 	G[i,j] = (G[i,j-1] + G[i-1,j] + G[i+1,j] + G[i,j+1]) / 4
New values are computed in place.
SOR
(generalization of Gauss-Seidel) 	G[i,j] = omega * (G[i,j-1] + G[i-1,j] + G[i+1,j] + G[i,j+1]) / 4 + (1 - omega) * G[i,i]
Here omega is called the over-relaxation parameter (0 < omega < 2); if it is 1, SOR is Gauss-Seidel.
Programs to Develop and to Evaluate

You are to develop and to evaluate the following four programs

    A sequential Jacobi iteration program (option: sequential red/black Gauss-Seidel program)
    A parallel Jacobi iteration program (option: parallel red/black Gauss-Seidel program)
    A sequential multigrid program
    A parallel multigrid program

You may write the programs in one of the following three programming environments:

    C/C++ with the MPI library using message passing programming model
    C/C++ with the Pthreads library using shared-memory programming model
    C/C++ using the OpenMP API for a shared-memory programming model with/without tasks.

Develop your programs on any machines at KTH or at home, but run performance experiments on several PCs or a multicore server at KTH. NOTE: It's OK if you perform "performance evaluation" (as an illustration) just on a SINGLE (multi-core) computer, i.e. run your application (all processes) on the SAME machine and measure the execution time as a function of the number of processes/threads. You might not observe any speedup but rather slowdown. As an alternative evaluation task you may estimate an amount of communication in your distributed version using MPI (the number of messages) instead of performance (the execution time). In this case, you can run a parallel program on one computer.

Jacobi iteration is described in Sections 11.1.2 (sequential version), 11.1.3 (parallel version using shared variables) and 11.3.4 (parallel version using message passing) of the G.Andrews textbook "Foundations of Multithreaded, Parallel, and Distributed Programming". An outline of a sequential program is given in Figure 11.2; outlines of parallel programs are given in figures 11.3, 11.4 and 11.5. Gauss-Seidel and red/black programs are described in Section 11.1.5 of the text and a parallel program using shared variables is shown in Figure 11.6. You will need to convert these to either C plus Pthreads or C plus MPI or C plus OpenMP and to fill in all the details. Multigrid methods are described in Section 11.1.6, but an actual code is not given. It is up to you to figure out the details for the sequential program and then to parallelize that program.

Your task is to write efficient programs. See the discussion on pages 539-40 of the G.Andrews textbook for the kinds of programming "tricks" that help make programs fast. Initialize the boundary points of the grids to 1.0 and the interior points to 0.0. This will make it easy for you to check the correctness of your programs and the quality of the results for different algorithms. Remember that according to the Dirichlet boundary condition, values in the boundary points of the grid are constant.

For the parallel programs, divide the grid into horizontal strips of rows of points. This includes all the grids in the multigrid programs. Use one worker process per strip. Each strip should contain about the same number of rows so as to balance the computational load. (The largest strip should contain at most one more row than the smallest strip.) If you use a shared memory programming model, in the parallel programs, implement an efficient dissemination barrier and use it when you need barrier synchronization. Use counter variables and busy waiting, as described at the end of Section 3.4 of the G.Andrews textbook. Make sure your barrier is correct!

For the multigrid programs, use a four-level V cycle as illustrated in Figure 11.8. Use the restriction and interpolation operators described on pages 550-51 of the the G.Andrews textbook. Use Jacobi iteration (option: red/black Gauss-Seidel) for the iterations on each level. Use exactly four iterations on each of the finer grids, and use the command-line argument numIters (see below) for the number of iterations on the coarsest (smallest) grid.

Input Parameters

Your programs should have three command-line arguments in the following order:
gridSize 	the grid size, not including boundaries
numIters 	the number of iterations to use
numWorkers 	the number of worker processes (for the parallel programs)
Note: if you use MPI, numWorkers can be specified with -np option for mpirun.

Assume that all grids are square. For the multigrid programs, the value of gridSize is the size of the coarsest (smallest) grid. The size of the next larger grid should then be 2*gridSize + 1, the next larger 2*(2*gridSize + 1) + 1, and so on. As illustrated in Figure 11.7 in the G.Andrews textbook, the physical boundaries of all grids should be the same, but the mesh size and distance between points varies.

Output

The output from your programs should be:

    the command-line arguments
    the execution time for the computational part
    the maximum error in final values
    the final grid values

Write the first three items to standard out. Write the data values to a filedata.out.

To calculate the execution time, read the clock after you have initialized all variables and just before you create the processes (in the parallel programs). Read the clock again as soon as the computation is complete and the worker processes have terminated (in the parallel programs). The maximum error in final values should be the maximum difference between the final values of points and 1.0. (One does not normally know what the final values should be, of course, but the maximum error is more interesting than the value of epsilon described in Section 11.1.).

Performance Evaluation

Your second task is to evaluate your programs with a series of timing experiments. In particular, you are to execute your programs for the following combinations of command-line arguments:

    program 1 for grid sizes of 100 and 200
    program 2 for grid sizes of 100 and 200 and for 1-4 worker processes
    program 3 for grid sizes of 12 and 24 (smallest grid)
    program 4 for grid sizes of 12 and 24 and for 1-4 worker processes

There are a total of 20 different timing tests. In order to get more precise timing estimates, you should run a program at least 5 times and take the median (rather than the mean) of the timing values.

For each sequential program and grid size, first figure out what the value of numIters should be so that the execution time of the program is about 30 seconds for that grid size. Then use the same value of numIters for the parallel versions of that sequential program. You will be using four different values for the numIters argument. To measure time, you can use different functions depending on which environment you use to develop the program

    If you develop your programs using Pthreads, use the times function (read a man page: man -s 2 times).
    If you develop your programs using OpenMP, use the omp_get_wtime function (see omp_get_wtime)
    If you develop your programs using MPI, use the MPI_Wtime function (see MPI_Wtime)

Reports

In a report you should explain what you have done and what you have learned. Your report should be a few pages of text plus tables and figures. It should have four or five sections, as follows:

    Introduction. Briefly describe the problem and what your report will show.
    Programs. Describe your programs, stating what each does and how. Explain the program-level optimizations you have implemented.
    Performance Evaluation. Present the results from the timing experiments. Use tables to present the raw data and graphs to show speedups and comparisons. Also explain your results. Do not just present the output data! What do the results show? Why?
    Conclusion. Briefly summarize what your report has shown, and describe what you have learned, and any problems that you have faced when implementing the project.


