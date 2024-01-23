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
    MPI_Init(&argc, &argv);

    // Start Clock
    double start_time = MPI_Wtime();
    // Arg 0 is the Number of Process
    int K = atoi(argv[1]);
    int M = atoi(argv[2]);
    int N = atoi(argv[3]);
    int P = atoi(argv[4]);

    int rank, size;

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    freopen("matrix_input.txt","r",stdin);


    int A[M][N];
    int B[N][P];
    
    // printf("Argument Received K,M,N,P = %d %d %d %d\n",K,M,N,P);

    if (M%size !=0 ) {
        if (rank == 0) {
            printf("Wrong number of process selected for these matrices.\n");
            printf("The no of row in first matrix, must have to be divisible by the total number of process\n");

        }
        MPI_Finalize();
        return 1;
    }

    for (int i = 0; i < M; i++)
    {
        for (int j = 0; j < N; j++)
        {
            scanf("%d",&A[i][j]);
        }
        
    }
    
    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < P; j++)
        {
            scanf("%d",&B[i][j]);
        }
    }


    

        
        int local_A[M / size][N]; // Each process holds N/size rows of matrix A
        int local_C[M / size][P]; // Each process computes N/size rows of the result matrix C

        MPI_Scatter(A, (M / size)*N, MPI_INT, local_A, (M / size)*N, MPI_INT, 0, MPI_COMM_WORLD);
        MPI_Bcast(B, N * P, MPI_INT, 0, MPI_COMM_WORLD);

        

        // ---Check each process receives          nbues? -------
        // printf("\nRank %d received this matrix\n",rank);
        
        // for (int i = 0; i < M/size; i++) {
        //     for (int j = 0; j < N; j++) {
        //         printf("%d\t", local_A[i][j]);
        //     }
        //     printf("\n");
        // }
        // printf("\n");

        //-------------------------------------



      

    while(K--){  
        for (int i = 0; i < M / size; i++) {
            for (int j = 0; j < P; j++) {
                local_C[i][j] = 0;
                for (int k = 0; k < N; k++) {
                    local_C[i][j] += local_A[i][k] * B[k][j];
                }
            }
        }
    }

    int C[M][P];
    MPI_Gather(local_C, (M / size) * P, MPI_INT, C, (M/ size)*P, MPI_INT, 0, MPI_COMM_WORLD);

    
    // // ------ -Check result per process -------
    // printf("\nRank %d output matrix\n",rank);

    // for (int i = 0; i < M/size; i++) {
    //     for (int j = 0; j < P; j++) {
    //         printf("%d\t", local_C[i][j]);
    //     }
    //     printf("\n");
    // }
    // printf("\n");

    //-------------------------------------

    if (rank == 0 ) {
        printf("Matrix A:\n");
        printMatrix(M,N,A);

        printf("Matrix B:\n");
        printMatrix(N,P,B);

        printf("Matrix C (A x B):\n");
        printMatrix(M,P,C);

        double end_time = MPI_Wtime();
        printf("Total time required : %.9f secs\n",end_time-start_time);
        }   
    

    MPI_Finalize();
 
    return 0;
}