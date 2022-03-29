//
// Created by saso on 01/01/2022.
//

#include <iostream>
#include <map>
#include <stack>
#include <assert.h>
#include <cstring>
#include "../api/HashTable.h"
#include "../../ExecutionTimer.h"

template
class HashTable<int>; // Types of values stored into the hash table

template<typename T>
HashTable<T>::HashTable(int capacity) : capacity{capacity} {
    table.resize(capacity);
    size = 0;
    loadFactor = 0;
}

template<typename T>
void HashTable<T>::fillTable(const std::map<T, std::vector<T>> &graphData) {
    std::vector<int> nodesIndexCache;
    nodesIndexCache.reserve(graphData.size());

    for (auto const&[keyOfTheNode, edgesOfTheNode]: graphData) {
        int index = insert(std::make_shared<GraphNode<T>>(keyOfTheNode));
        if (index >= 0)
            nodesIndexCache.emplace_back(index);
        else
            std::cerr << "Error when reading from graph buffer\n";
    }

    assert(nodesIndexCache.size() == graphData.size());

    int i = 0;
    for (auto const&[keyOfTheNode, edgesOfTheNode]: graphData) {
        for (auto const &edgeKey: edgesOfTheNode) {
            table[nodesIndexCache.at(i)]->addEdge(getByKey(edgeKey));
        }
        i++;
    }
}

template<typename T>
void HashTable<T>::insert(T nodeKey) {
    if (insert(std::make_shared<GraphNode<T>>(nodeKey)) >= 0)
        std::cout << "Added node to the table with key = " << nodeKey << std::endl;
    else
        std::cout << "Could not add the node to the table with key = " << nodeKey << std::endl;
}

template<typename T>
int HashTable<T>::insert(std::shared_ptr<GraphNode<T>> graphNode) {
    int hashIndex = 0;
    if (loadFactor > max_load_factor) // Too many nodes
    {
        std::cerr << "Graph size must be increased to insert more nodes\n";
        return -1;
    }
    if (getByKey(graphNode->key, hashIndex).get() != nullptr) // Avoid duplicate keys in the table
        return -1;
    size++;
    table[hashIndex] = graphNode;
    loadFactor = (float) size / (float) capacity;
    return hashIndex;
}


template<typename T>
std::shared_ptr<GraphNode<T>> HashTable<T>::getByKey(T key) {
    int hashIndex = hashingStrategy->hashCode(key);
    int iterationNo = 0;

    while (table.at(hashIndex).get() && table.at(hashIndex)->key != key)
        hashIndex = hashingStrategy->rehash(key, ++iterationNo);

    return table[hashIndex];
}

template<typename T>
std::shared_ptr<GraphNode<T>> HashTable<T>::getByKey(T key, int &hashIndex) {
    hashIndex = hashingStrategy->hashCode(key);
    int iterationNo = 0;

    while (table.at(hashIndex).get() && table.at(hashIndex)->key != key)
        hashIndex = hashingStrategy->rehash(key, ++iterationNo);
    return table.at(hashIndex);
}

template<typename T>
bool HashTable<T>::findEdge(T sourceNodeKey, T targetNodeKey) {
    auto sourceNode = getByKey(sourceNodeKey);
    if (!sourceNode.get())
        return false;
    return sourceNode->hasEdge(targetNodeKey);
}

template<typename T>
bool HashTable<T>::findEdge(std::shared_ptr<GraphNode<T>> sourceNode, std::shared_ptr<GraphNode<T>> targetNode) {
    return findEdge(sourceNode->key, targetNode->key);
}

template<typename T>
void HashTable<T>::addEdge(T sourceNodeKey, T targetNodeKey) {
    auto sourceNode = getByKey(sourceNodeKey);
    auto targetNode = getByKey(targetNodeKey);
    if (sourceNode.get() && targetNode.get() && !findEdge(sourceNode, targetNode)) {// Avoid duplicate edges
        if (sourceNode->addEdge(targetNode))
            std::cout << "Added edge between " << sourceNodeKey << " --> " << targetNodeKey << std::endl;
    } else
        std::cout << "Could not add the edge between " << sourceNodeKey << " --> " << targetNodeKey << std::endl;
}

template<typename T>
void HashTable<T>::removeEdge(T sourceNodeKey, T targetNodeKey) {
    auto sourceNode = getByKey(sourceNodeKey);
    if (!sourceNode.get()) {
        std::cout << "Could not find a node with the given key\n ";
        return;
    }

    auto edgesOfTheSourceNode = sourceNode->getEdgesPtr();
    int initialEdges = edgesOfTheSourceNode->size();

    std::erase_if(*edgesOfTheSourceNode, [&targetNodeKey](auto edge) {
        return edge.lock()->getKey() == targetNodeKey;
    });

    int edgesAfterEraseOperation = edgesOfTheSourceNode->size();
    if (edgesAfterEraseOperation < initialEdges)
        std::cout << "Erased edge from " << sourceNodeKey << " --> " << targetNodeKey << std::endl;
    else
        std::cout << "Could not erase edge from " << sourceNodeKey << " --> " << targetNodeKey << std::endl;
}

template<typename T>
std::shared_ptr<GraphNode<T>> HashTable<T>::operator[](int index) const {
    if (index >= capacity) {
        std::cout << "Index out of bound" << std::endl;
    }
    return table[index];
}

template<typename T>
int HashTable<T>::getSize() const {
    return size;
}

template<typename T>
HashTable<T>::~HashTable() {
    delete hashingStrategy;
    for (int i = 0; i < capacity; i++) {
        table[i].reset();
    }
    table.clear();
}

template<typename T>
void HashTable<T>::dfs(T keyOfStartingNode) {
    std::set<T> visited;
    std::stack<std::shared_ptr<GraphNode<T>>> stack;
    auto startingNode = getByKey(keyOfStartingNode);

    if (!startingNode) {
        std::cout << "The key " << keyOfStartingNode << " is invalid, could not perform the DFS\n";
        return;
    }
    std::cout << "Performing DFS from " << keyOfStartingNode << std::endl;
    stack.push(startingNode);
    while (!stack.empty()) {
        auto currentGraphNode = stack.top();
        stack.pop();
        std::cout << " " << currentGraphNode->getKey() << std::endl; // TODO: comment if load testing
        if (!visited.contains(currentGraphNode->getKey())) {
            visited.insert(currentGraphNode->getKey());
            currentGraphNode->setNodeStatus(reachable);
        }
        for (const auto &edge: currentGraphNode->getEdges())
            if (!edge.expired()) {
                auto observe = edge.lock();
                if (!visited.contains(observe->getKey())) {
                    stack.push(observe);
                }
            }
    }
    std::cout << "Visited " << visited.size() << " nodes" << std::endl;

}

template<typename T>
std::set<std::shared_ptr<GraphNode<T>>> HashTable<T>::computeNotReachableNodes(T keyOfStartingNode) {
    dfs(keyOfStartingNode);

    std::set<std::shared_ptr<GraphNode<T>>> notReachablesFromSource;
    for (auto &element: table) {
        if (element && element->getNodeStatus() == unreachable)
            notReachablesFromSource.insert(element);
    }
    std::cout << "\n ** Not reachable nodes: " << notReachablesFromSource.size() << "/" << getSize() << std::endl;

    for (auto &notReachable: notReachablesFromSource) {
        auto neighbours = getNotReachableNeighbours(notReachable);
        if (!neighbours.empty())
            for (auto &element: neighbours)
                notReachablesFromSource.erase(element);
    }

    std::cout << "\n You have to build " << notReachablesFromSource.size() << " edges to these nodes: ";
    for (auto &it: notReachablesFromSource) {
        it->setNodeStatus(reachable); // Hydratating the status
        std::cout << it.get()->getKey() << " "; //TODO: comment it if load testing
    }
    return notReachablesFromSource;
}

template<typename T>
std::vector<std::shared_ptr<GraphNode<T>>>
HashTable<T>::getNotReachableNeighbours(const std::shared_ptr<GraphNode<T>> &source) {
    std::vector<std::shared_ptr<GraphNode<T>>> notReachablesFromSourceNeighbours;
    std::stack<std::shared_ptr<GraphNode<T>>> stack;
    std::set<T> visited;

    if (source == nullptr) {
        std::cout << "The key inserted is invalid, could not perform the DFS\n";
        return {};
    }

    int i = 0;
    source->setNodeStatus(marked); // Marking the source node

    stack.push(source);
    while (!stack.empty()) {
        i++;
        auto currentGraphNode = stack.top();
        stack.pop();
        visited.insert(currentGraphNode->getKey());
        notReachablesFromSourceNeighbours.emplace_back(currentGraphNode);
        if (i != 1)
            currentGraphNode->setNodeStatus(reachable); // All the children of the marked parent node will be reachable

        for (const auto &edge: currentGraphNode->getEdges()) {
            if (auto observe = edge.lock()) {
                if (observe->getNodeStatus() == unreachable) {
                    stack.push(observe);
                } else if (observe->getNodeStatus() == marked &&
                           !visited.contains(observe->getKey())) { // Refreshing a previous marked node
                    observe->setNodeStatus(reachable);
                    notReachablesFromSourceNeighbours.emplace_back(observe);
                }
            }
        }
    }

    if (!notReachablesFromSourceNeighbours.empty())
        notReachablesFromSourceNeighbours.erase(std::begin(notReachablesFromSourceNeighbours));

    return notReachablesFromSourceNeighbours;
}

template<typename T>
void HashTable<T>::addEdge(std::shared_ptr<GraphNode<T>> sourceNode, std::shared_ptr<GraphNode<T>> targetNode) {
    if (sourceNode.get() && targetNode.get()) {

    }
    sourceNode->addEdge(targetNode);
}

template<typename T>
void HashTable<T>::setHashingStrategy(char *strategy) {
    if (strategy == nullptr || strcmp("linear", strategy) == 0) {
        hashingStrategy = new LinearProbingStrategy<T>(capacity);
        std::cout << "Using a linear probing hashing strategy" << std::endl;
    } else {
        hashingStrategy = new DoubleHashingStrategy<T>(capacity);
        std::cout << "Using a double hashing strategy with table size equal to " << capacity << std::endl;
    }
}

template<typename T>
void HashTable<T>::resetReachbility() {
    for (auto &element: table)
        if (element)
            element->setNodeStatus(unreachable);
}







