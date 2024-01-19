#include <stdio.h>
#include <mpi.h>

int main(int argc, char **argv) {
    
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int send_data[size];
    int recv_data;

    if (rank == 0) {
        // Initialize send_data at the root process
        for (int i = 0; i < size; i++) {
            send_data[i] = i * 10;
        }
    }

    // Scatter the data from the root to all processes
    MPI_Scatter(send_data, 1, MPI_INT, &recv_data, 1, MPI_INT, 0, MPI_COMM_WORLD);

    printf("Processor %d received %d\n", rank, recv_data);

    MPI_Finalize();

    return 0;
}
