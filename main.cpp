#include <iostream>
#include <vector>
#include <map>
#include <InputOutputHandler.h>
#include "hash_table/api/HashTable.h"

static const char *const inputFileName = "./resources/input0_2_2.txt"; // Input file name

void displayMenu();

int main(int argc, char **argv) {
    int choice = 0;
    int sourceNodeKey = -1;
    std::map<int, std::vector<int>> inputFileGraphBuffer; // Buffer that contains data from the input txt file
    auto inputOutputHandler = InputOutputHandler<int>();
    auto fileMetadata = inputOutputHandler.readInputGraph(inputFileName, inputFileGraphBuffer); // Filling the buffer

    if (!fileMetadata.getOperationStatus()) // File read failed
        return -1;

    HashTable hashTable = HashTable<int>(inputFileGraphBuffer, fileMetadata.getNumberOfNodes() *
                                                               2); // Create the hash table and inserting data into it
    hashTable.setHashingStrategy(argv[1]); // Setting the hashing strategy, by default is linear probing
    hashTable.insert(0); // Insert the source node

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
