//
// Created by saso on 12/02/2022.
//

#include "../api/Graph.h"

template
class Graph<int>;

template<typename T>
Graph<T>::Graph(const std::map<T, std::vector<T>> &graphData, int numberOfNodes) {
    graph.reserve(numberOfNodes);
    for (auto const&[keyOfTheNode, edgesOfTheNode]: graphData) {
        graph.insert(std::make_shared<GraphNode<T>>(keyOfTheNode));
    }

    for (auto const&[keyOfTheNode, edgesOfTheNode]: graphData) {
        for (auto const &edgeKey: edgesOfTheNode) {
//            graph.find(keyOfTheNode)->get()->addEdge(graph.find(edgeKey).operator*());
        }
    }
}

template<typename T>
void Graph<T>::insert(std::shared_ptr<GraphNode<T>> node) {
    graph.insert(node);
}
