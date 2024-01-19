#include<stdio.h>
#include<mpi.h>

int main(int argc,char** argv){

    MPI_Init(&argc,&argv);
    int i;

    for (i = 1; i <= 10; i++)
    {
        printf("%d,",i);
    }
    
    MPI_Finalize();
}