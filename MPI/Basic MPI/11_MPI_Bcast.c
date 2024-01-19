#include <stdio.h>
#include <mpi.h>

int main(int argc, char **argv) {
    
    MPI_Init(&argc, &argv);

    int rank, size;

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int cnt = 999;

    // Broadcast from the root (rank 0) to all other processes
    MPI_Bcast(&cnt, 1, MPI_INT, 0, MPI_COMM_WORLD);

    // All processes, including the root, receive the broadcast
    printf("Processor %d received %d from Processor 0\n", rank, cnt);

    MPI_Finalize();

    return 0;
}
