#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

 // Size of the square matrices


void printMatrix(int row, int column, int mat[row][column]) {
    for (int i = 0; i <row; i++) {
        for (int j = 0; j < column; j++) {
            printf("%d\t", mat[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}

int main(int argc, char **argv) {
    int rank, size;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);



    int K = 3;
    int M = 2;
    int N = 4;
    int P = 3;

    // if (M !=size ) {
    //     if (rank == 0) {
    //         printf("The matrix size M must be evenly divisible by the number of processes.\n");
    //     }
    //     MPI_Finalize();
    //     return 1;
    // }


 
    int A [2][4]=
    {
        {87 ,95, 19, 67},
        {52 ,27, 2, 9}
    };

    int B [4][3]=
    {
        {33 ,65, 72},
        {66 ,81, 89},
        {31, 37, 62},
        {75 ,76, 93}
    };

    // int A[3][4] = {
    //     {3, 2, 1, 4},
    //     {4, 0, 0, 0},
    //     {1, 0, 1, 0}
    // };


    // int B[4][2] = {
    //     {7, 3},
    //     {2, 1},
    //     {1, 1},
    //     {3, 1}
    // };
  
    
    int local_A[M / size][N]; // Each process holds N/size rows of matrix A
    int local_C[M / size][P]; // Each process computes N/size rows of the result matrix C

    MPI_Scatter(A, (M / size)*N, MPI_INT, local_A, (M / size)*N, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(B, N * P, MPI_INT, 0, MPI_COMM_WORLD);

    

     // ---Check each process receives          nbues? -------
    printf("\nRank %d received this matrix\n",rank);
    
    for (int i = 0; i < M/size; i++) {
        for (int j = 0; j < N; j++) {
            printf("%d\t", local_A[i][j]);
        }
        printf("\n");
    }
    printf("\n");

    //-------------------------------------



    
    for (int i = 0; i < M / size; i++) {
        for (int j = 0; j < P; j++) {
            local_C[i][j] = 0;
            for (int k = 0; k < N; k++) {
                local_C[i][j] += local_A[i][k] * B[k][j];
            }
        }
    }

    int C[M][P];
    MPI_Gather(local_C, (M / size) * P, MPI_INT, C, (M/ size)*P, MPI_INT, 0, MPI_COMM_WORLD);

    
    // ------ -Check result per process -------
    printf("\nRank %d output matrix\n",rank);

    for (int i = 0; i < M/size; i++) {
        for (int j = 0; j < P; j++) {
            printf("%d\t", local_C[i][j]);
        }
        printf("\n");
    }
    printf("\n");

    //-------------------------------------

    if (rank == 0) {
        printf("Matrix A:\n");
        printMatrix(M,N,A);

        printf("Matrix B:\n");
        printMatrix(N,P,B);

        printf("Matrix C (A x B):\n");
        printMatrix(M,P,C);
    }

    MPI_Finalize();
    return 0;
}