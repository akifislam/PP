#include <iostream>
#include <string>
#include <mpi.h>
#include <fstream>
#include <map>
#include <vector>

using namespace std;

// Function to search for a person by name in a phonebook
void searchInPhonebook(const string& personName, const string& phonebookFileName, vector<string>& foundContacts) {
    ifstream phonebookFile(phonebookFileName);

    if (!phonebookFile.is_open()) {
        cerr << "Error opening phonebook file" << endl;
        return;
    }

    string line;

    while (getline(phonebookFile, line)) {
        size_t commaPos = line.find(',');
        if (commaPos != string::npos) {
            string name = line.substr(0, commaPos);

            if (name == personName) {
                // Only add to foundContacts if not already present
                if (find(foundContacts.begin(), foundContacts.end(), line) == foundContacts.end()) {
                    foundContacts.push_back(line);
                }
            }
        }
    }

    phonebookFile.close();
    return;
}

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);

    int world_size, rank;

    MPI_Comm_size(MPI_COMM_WORLD, &world_size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    if (argc < 2) {
        if (rank == 0) {
            cout << "Usage: " << argv[0] << " <phonebook_count> <phonebook1> <phonebook2> ... " << endl;
        }
        MPI_Finalize();
        return 1;
    }

    int phonebook_count = atoi(argv[1]);

    // Check if the number of processes is less than the number of phonebooks
    if (world_size - 1 < phonebook_count) {
        if (rank == 0) {
            cout << "Error: Insufficient number of processes for distributing phonebooks." << endl;
        }
        MPI_Finalize();
        return 1;
    }

    vector<string> foundContacts;  // Vector to store unique found contacts

    if (rank == 0) {
        // Master process
        for (int i = 0; i < phonebook_count; i++) {
            // Distribute phonebooks to worker processes
            MPI_Send(argv[i + 2], strlen(argv[i + 2]) + 1, MPI_CHAR, i + 1, 0, MPI_COMM_WORLD);
        }
    } else {
        // Worker processes
        char received_phonebook[256]; // Adjust the array size based on your needs

        // Receive the phonebook file name
        MPI_Recv(received_phonebook, sizeof(received_phonebook), MPI_CHAR, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

        cout << "Process " << rank << " received phonebook: " << received_phonebook << endl;

        searchInPhonebook("Akif Islam", received_phonebook, foundContacts);
    }

    // Gather all found contacts from worker processes to the master
    MPI_Gather(foundContacts.data(), foundContacts.size() * sizeof(string), MPI_CHAR,
               nullptr, 0, MPI_CHAR, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        // Master process
        cout << "Unique Found Contacts:" << endl;
        for (const string& contact : foundContacts) {
            cout << contact << endl;
        }
    }

    MPI_Finalize();
    
    return 0;
}
