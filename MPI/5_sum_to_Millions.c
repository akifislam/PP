#include <stdio.h>
#include <mpi.h>

int main(int argc, char *argv[]) {
    MPI_Init(&argc, &argv);
    
    int range_of_sum = 1000000000LL;
    double startTime = MPI_Wtime();

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    // Define the range for each process
    long long start = rank * (range_of_sum / size) + 1;
    long long end = (rank + 1) * (range_of_sum / size);

    // Handle the case where 10 million is not divisible evenly by the number of processes
    if (rank == size - 1) {
        end = range_of_sum;
    }

    long long localSum = 0;
    for (long long i = start; i <= end; ++i) {
        localSum += i;
    }

    printf("Process %d local sum for range %lld to %lld: %lld\n", rank, start, end, localSum);

    long long globalSum;
    MPI_Reduce(&localSum, &globalSum, 1, MPI_LONG_LONG, MPI_SUM, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        printf("Total sum: %lld\n", globalSum);
    }

    double endTime = MPI_Wtime();
    double elapsedTime = endTime - startTime;

    if (rank == 0) {
        printf("\n***Total execution time: %f seconds***\n", elapsedTime);
    }

    MPI_Finalize();
    return 0;
}
