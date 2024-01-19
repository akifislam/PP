#include<iostream>
#include<mpi.h>

using namespace std;

int main(int argc, char** argv){

    MPI_Init(&argc,&argv);

    int size,rank;
    MPI_Comm_size(MPI_COMM_WORLD,&size);
    MPI_Comm_rank(MPI_COMM_WORLD,&rank);

    if (rank == 0){
        freopen("input.txt","r",stdin);

        string word;
        int totalwords;

        while(cin>>word){
            cout<<word<<"  ";
            totalwords+=1;
        }

        cout<<"\nTotal Words "<<totalwords<<endl; 

    }
    MPI_Finalize();
    
    
}