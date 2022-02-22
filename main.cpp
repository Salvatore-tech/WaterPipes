#include <iostream>
#include <vector>
#include <map>
#include <InputOutputHandler.h>
#include "hash_table/api/HashTable.h"

static const char *const inputFileName = "./resources/input.txt";

void displayMenu();

int main() {
    int choice = 0;
    int sourceNodeKey = -1;
    std::map<int, std::vector<int>> inputFileGraphBuffer;
    auto inputOutputHandler = InputOutputHandler<int>();
    auto fileMetadata = inputOutputHandler.readInputGraph(inputFileName, inputFileGraphBuffer);

    if (!fileMetadata.getOperationStatus())
        return -1;

    HashTable hashTable = HashTable<int>(inputFileGraphBuffer, fileMetadata.getNumberOfNodes() * 2);
    hashTable.insert(0); // Insert the source

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
    std::cout << "3) Find cities without water missing \n";
    std::cout << "4) Build missing links to cities that are not receiving water \n";
    std::cout << "0) Exit \n";
}
