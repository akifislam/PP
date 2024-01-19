#include<iostream>
#include<vector>
#include <fstream>
#include<mpi.h>

using namespace std;

int main(int argc, char** argv){

    MPI_Init(&argc,&argv);

    int size,rank;

    MPI_Comm_size(MPI_COMM_WORLD,&size);
    MPI_Comm_rank(MPI_COMM_WORLD,&rank);

    if (rank == 0){

        ifstream inputFile("input.txt"); 
        vector<string> wordlist;

        string word;
        int totalwords=0;

        while(inputFile>>word){
            cout<<word<<"  "; 
            totalwords+=1;
            wordlist.push_back(word);
        }

        cout<<"\nTotal Words "<<totalwords<<endl; 
        cout<<"Vector Size : "<<wordlist.size()<<endl;

        int wordsPerProcess = totalwords / (size - 1); 

        //Broadcast
        MPI_Bcast(&totalwords,1,MPI_INT,0,MPI_COMM_WORLD);
        MPI_Bcast(&wordsPerProcess,1,MPI_INT,0,MPI_COMM_WORLD);
        FILE *file;
        file = fopen("input.txt","r");

        for (int dest_process = 1; dest_process < size; dest_process++) {
            MPI_Send(&wordsPerProcess, 1, MPI_INT, dest_process, 0, MPI_COMM_WORLD);
            for (int i = 0; i < wordsPerProcess; i++) {
                char word[50];
                if (fscanf(file, "%s", word) == 1) {
                    MPI_Send(&word,50, MPI_CHAR, dest_process, 0, MPI_COMM_WORLD);
                }
            }
        }
        inputFile.close();
    }
    else{
        
        int totalwords, wordsPerProcess;
                
        MPI_Bcast(&totalwords,1,MPI_INT,0,MPI_COMM_WORLD);
        MPI_Bcast(&wordsPerProcess,1,MPI_INT,0,MPI_COMM_WORLD);

        int wordsToReceive = (rank == size - 1) ? (totalwords - (rank - 1) * wordsPerProcess) : wordsPerProcess;
        
        cout<<"Received Words by "<<rank<<": "<<wordsToReceive<<endl;


        for (int i = 0; i < wordsToReceive; i++) {
            
            char word[50];

            MPI_Recv(word, 50, MPI_CHAR, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            // Check if the word is already in the array
            cout<<"Received Words by "<<rank<<": "<<word<<endl;
        }
    }

    MPI_Finalize();

}
    
