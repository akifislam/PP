#include <mpi.h>
#include <iostream>
#include <cstdlib>
#include <ctime>

using namespace std;

int main(int argc, char** argv) {
    int size, rank;
    
    MPI_Init(&argc, &argv);

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    srand(time(nullptr));

    const int K = atoi(argv[1]);
    const int M = atoi(argv[2]);
    const int N = atoi(argv[3]);
    const int P = atoi(argv[4]);

    if (K % size != 0) {
        cout << "Total number of matrices should be divided by total number of processes" << endl;
        MPI_Finalize();
        return 1;
    }

    // Creating Matrices
    int A[K][M][N];
    int B[K][N][P];
    int C[K][M][P];

    // If process == 0
    if (rank == 0) {
        // Building Matrix A
        for (int k = 0; k < K; k++) {
            for (int i = 0; i < M; i++) {
                for (int j = 0; j < N; j++) {
                    A[k][i][j] = rand() % 10;
                }
            }
        }

        // Building Matrix B
        for (int k = 0; k < K; k++) {
            for (int i = 0; i < N; i++) {
                for (int j = 0; j < P; j++) {
                    B[k][i][j] = rand() % 10;
                }
            }
        }
    }

    MPI_Barrier(MPI_COMM_WORLD);

    int size_per_process = K / size;

    int local_A[size_per_process][M][N];
    int local_B[size_per_process][N][P];
    int local_C[size_per_process][M][P];

    MPI_Scatter(A, size_per_process * M * N, MPI_INT, local_A, size_per_process * M * N, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Scatter(B, size_per_process * N * P, MPI_INT, local_B, size_per_process * N * P, MPI_INT, 0, MPI_COMM_WORLD);

    // Perform Matrix Multiplication
    for (int k = 0; k < size_per_process; k++) {
        for (int i = 0; i < M; i++) {
            for (int j = 0; j < P; j++) {
                local_C[k][i][j] = 0;
                for (int c = 0; c < N; c++) {
                    local_C[k][i][j] += local_A[k][i][c] * local_B[k][c][j];
                }
            }
        }
    }
    MPI_Barrier(MPI_COMM_WORLD);

    MPI_Gather(local_C, size_per_process * M * P, MPI_INT, C, size_per_process * M * P, MPI_INT, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        // Output results only from rank 0
        for (int k = 0; k < K; k++) {
            cout << "Result of the matrix multiplication " << k + 1 << ":\n\n";
            for (int r = 0; r < M; r++) {
                for (int c = 0; c < P; c++) {
                    cout << C[k][r][c] << " ";
                }
                cout << "\n";
            }
            cout << "\n";
        }
    }

    MPI_Barrier(MPI_COMM_WORLD);

    double end_time = MPI_Wtime();

    // if (rank == 0) {
    //     cout << "Total execution time: " << end_time - start_time << " seconds." << endl;
    // }

    MPI_Finalize();

    return 0;
}
