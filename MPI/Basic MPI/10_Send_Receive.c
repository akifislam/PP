#include <stdio.h>
#include <mpi.h>

int main(int argc, char **argv) {
    
    MPI_Init(&argc, &argv);

    int rank, size;

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int sender_rank = rank;
    int receiver_rank = (rank + 1) % size;

    int cnt = 100;

    // Send from the current rank to the next rank
    MPI_Send(&cnt, 1, MPI_INT, receiver_rank, 0, MPI_COMM_WORLD);
    printf("Processor %d sent %d to Processor %d\n", rank, cnt, receiver_rank);

    // Receive from the previous rank
    MPI_Recv(&cnt, 1, MPI_INT, (rank - 1 + size) % size, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    printf("Processor %d received %d from Processor %d\n", rank, cnt, (rank - 1 + size) % size);

    MPI_Finalize();

    return 0;
}
