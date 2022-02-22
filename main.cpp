#include <iostream>
#include <vector>
#include <map>
#include <InputOutputHandler.h>
#include "hash_table/api/HashTable.h"


static const char *const inputFileName = "./resources/input.txt";

int main() {
    std::map<int, std::vector<int>> inputFileGraphBuffer;
    auto inputOutputHandler = InputOutputHandler<int>();
    auto fileMetadata = inputOutputHandler.readInputGraph(inputFileName, inputFileGraphBuffer);

    if (!fileMetadata.getOperationStatus())
        return -1;

    HashTable hashTable = HashTable<int>(inputFileGraphBuffer, fileMetadata.getNumberOfNodes() * 2);
    hashTable.insert(0); // Insert the source

    std::cout << hashTable << std::endl;

    hashTable.computeNotReachableNodes(0);
//    std::cout<<"\n";
//    tableIterator.computeNotReachableNodes(0);

    return 0;
}
