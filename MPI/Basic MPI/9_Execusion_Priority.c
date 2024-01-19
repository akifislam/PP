// Response from Different Processors and their Execusion

#include<stdio.h>
#include<mpi.h>

int main(int argc, char **argv) {
    
    MPI_Init(&argc,&argv);

    int rank,size;

    MPI_Comm_rank(MPI_COMM_WORLD,&rank);
    MPI_Comm_rank(MPI_COMM_WORLD,&size);

    int i = 0;
    for(int i = 0; i<10; i++){
        
        if (rank==i){
            printf("Hello from processor %d\n",rank);
        }
    }



    MPI_Finalize();
}