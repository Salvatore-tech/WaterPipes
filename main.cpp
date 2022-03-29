#include <iostream>
#include <vector>
#include <map>
#include <InputOutputHandler.h>
#include <chrono>
#include "hash_table/api/HashTable.h"
#include "ExecutionTimer.h"

/* GRAPH INPUT FILE DETAILS
 * NAME               : DESC
 * graphAssigned.txt  : input file provided, after running the algorithm 14 edges from the source need to be created: 2 4 7 8 9 10 16 17 18 19 20 21 22 23
 * graphSparse5.txt   : input file with 100 000 nodes and 100 000 edges, 5% of the nodes are directly connected to the source
 * graphSparse30.txt  : input file with 100 000 nodes and 100 000 edges, 30% of the nodes are directly connected to the source
* graphDense.txt      : input file with 1000 nodes and 500 000 edges
 * */

void displayMenu();

int main(int argc, char **argv) {
    int sourceNodeKey = -1;
    std::map<int, std::vector<int>> inputFileGraphBuffer; // Buffer that contains data from the input txt file
    auto inputOutputHandler = InputOutputHandler<int>();
    char *relativePathToInputFile = inputOutputHandler.getPathToInputFile(argv[1]);

    if (!relativePathToInputFile) {
        std::cerr << "Usage: ./WaterPipes inputFileName.txt hashingStrategy[OPTIONAL]" << std::endl;
        return -1;
    }

    auto fileMetadata = inputOutputHandler.readInputGraph(relativePathToInputFile,
                                                          inputFileGraphBuffer); // Filling the buffer

    if (!fileMetadata.getOperationStatus()) // File read failed
        return -1;

    HashTable hashTable = HashTable<int>(
            fileMetadata.getNumberOfNodes() * 2); // Create the hash table

    hashTable.setHashingStrategy(argv[2]); // Setting the hashing strategy, by default is linear probing

    hashTable.fillTable(inputFileGraphBuffer); // Fill the table with the data in the buffer
    hashTable.insert(0); // Insert the source node (if not inserted previously)

    int choice = 0;
    do {
        displayMenu();
        std::cin >> choice;
        switch (choice) {
            case 1:
                std::cout << hashTable << std::endl;
                break;
            case 2:
                std::cout << "Insert source node key: ";
                std::cin >> sourceNodeKey;
                hashTable.dfs(sourceNodeKey);
                hashTable.resetReachbility();
                break;
            case 3:
                std::cout << "Insert source node key: ";
                std::cin >> sourceNodeKey;
                hashTable.computeNotReachableNodes(sourceNodeKey);
                break;
            case 4:
                std::cout << "Insert source node key: ";
                std::cin >> sourceNodeKey;
                auto sourceNode = hashTable.getByKey(sourceNodeKey);
                auto citiesWithoutWater = hashTable.computeNotReachableNodes(sourceNodeKey);
                for (auto &city: citiesWithoutWater)
                    hashTable.addEdge(sourceNode, city);
                std::cout << "\nAdded edges to connect all the cities to the source node" << std::endl;
                break;
        }
    } while (choice != 0);
    return 0;
}

void displayMenu() {
    std::cout << "\n \t \t [WaterPipes menu] \n";
    std::cout << "Choose an operation \n";
    std::cout << "1) Display graph \n";
    std::cout << "2) Depth First Search from source \n";
    std::cout << "3) Find cities with missing water \n";
    std::cout << "4) Build missing links to cities that are not receiving water \n";
    std::cout << "0) Exit \n";
}
