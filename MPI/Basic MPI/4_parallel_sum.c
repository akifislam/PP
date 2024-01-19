#include <stdio.h>
#include <mpi.h>

int main(int argc, char *argv[]) {
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    // Define the range for each process
    int start = rank * (10 / size) + 1;
    int end = (rank + 1) * (10 / size);

    // Handle the case where 10 is not divisible evenly by the number of processes
    if (rank == size - 1) {
        end = 10;
    }

    int localSum = 0;
    for (int i = start; i <= end; ++i) {
        localSum += i;
    }

    printf("Process %d local sum: %d\n", rank, localSum);

    int globalSum;
    MPI_Reduce(&localSum, &globalSum, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        printf("Total sum: %d\n", globalSum);
    }

    MPI_Finalize();
    return 0;
}
