#include <iostream>
#include <vector>
#include <map>
#include <InputOutputHandler.h>
#include "graph/api/Graph.h"

static const char *const inputFileName = "input.txt";

int main() {
    std::map<int, std::vector<int>> inputFileGraphBuffer;
    auto inputOutputHandler = InputOutputHandler<int>();
    auto fileMetadata = inputOutputHandler.readInputGraph(inputFileName, inputFileGraphBuffer);

    if (!fileMetadata.getOperationStatus())
        return -1;

//    Graph graph = Graph<int>(inputFileGraphBuffer, fileMetadata.getNumberOfNodes());

    Graph graph = Graph<int>();
    graph.insert(std::make_shared<GraphNode<int>>(2));

    std::cout << "Hello, World!" << std::endl;
    return 0;
}
