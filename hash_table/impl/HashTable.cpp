//
// Created by saso on 01/01/2022.
//

#include <iostream>
#include <map>
#include <stack>
#include <assert.h>
#include <cstring>
#include "../api/HashTable.h"

template
class HashTable<int>; // Types of values stored into the hash table

template<typename T>
HashTable<T>::HashTable(int bucketNo) : capacity{bucketNo} {
    table.resize(bucketNo);
    hashingStrategy = new LinearProbingStrategy<T>(capacity);
    size = 0;
    loadFactor = 0;
}

template<typename T>
HashTable<T>::HashTable(const std::map<T, std::vector<T>> &graphData, int numberOfNodes) : HashTable(numberOfNodes) {
    std::vector<std::shared_ptr<GraphNode<T>>> nodeCache;
    nodeCache.reserve(graphData.size());

    for (auto const&[keyOfTheNode, edgesOfTheNode]: graphData) {
        int index = insert(std::make_shared<GraphNodeReachable<T>>(keyOfTheNode));
        if (index >= 0)
            nodeCache.emplace_back(table[index]);
        else
            std::cerr << "Error when reading from graph buffer\n";
    }

    assert(nodeCache.size() == graphData.size());

    int i = 0;
    for (auto const&[keyOfTheNode, edgesOfTheNode]: graphData) {
        for (auto const &edgeKey: edgesOfTheNode) {
            auto ptr = getByKey(edgeKey);
            nodeCache.at(i).get()->addEdge(ptr);
        }
        i++;
    }
}

template<typename T>
void HashTable<T>::insert(T nodeKey) {
    if (insert(std::make_shared<GraphNodeReachable<T>>(nodeKey)) >= 0)
        std::cout << "Added node to the table with key = " << nodeKey << std::endl;
    else
        std::cout << "Could not add the node to the table with key = " << nodeKey << std::endl;
}

template<typename T>
int HashTable<T>::insert(std::shared_ptr<GraphNodeReachable<T>> graphNode) {
    int hashIndex = 0;
    if (getByKey(graphNode->key, hashIndex) && loadFactor < max_load_factor) // Avoid duplicate keys in the table
        return -1;
    size++;
    table[hashIndex] = graphNode;
    loadFactor = (float) size / (float) capacity;
    return hashIndex;
}


template<typename T>
std::shared_ptr<GraphNodeReachable<T>> HashTable<T>::getByKey(T key) {
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
std::shared_ptr<GraphNodeReachable<T>> HashTable<T>::operator[](int index) const {
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
    std::vector<bool> visited(size, false);
    std::stack<std::shared_ptr<GraphNodeReachable<T>>> stack;
    auto startingNode = getByKey(keyOfStartingNode);

    if (!startingNode) {
        std::cout << "The key inserted is invalid, could not perform the DFS\n";
        return;
    }

    std::cout << "Performing DFS from " << keyOfStartingNode << std::endl;

    stack.push(startingNode);
    while (!stack.empty()) {
        auto currentGraphNode = stack.top();
        stack.pop();
        if (!visited[currentGraphNode->getKey()]) {
            std::cout << currentGraphNode->getKey() << " ";
            visited[currentGraphNode->getKey()] = true;
            currentGraphNode->setIsReachable(true);
        }
        for (const auto &edge: currentGraphNode->getEdges())
            if (const auto observe = edge.lock()) {
                if (!visited[observe->getKey()])
                    stack.push(std::static_pointer_cast<GraphNodeReachable<T>>(observe));
            }
    }
}

template<typename T>
std::set<std::shared_ptr<GraphNodeReachable<T>>> HashTable<T>::computeNotReachableNodes(T keyOfStartingNode) {
    dfs(keyOfStartingNode); // O(V + E)

    std::set<std::shared_ptr<GraphNodeReachable<T>>> notReachablesFromSource;
    for (auto &element: table) { // O(V)
        auto observe = element.get();
        if (observe && !observe->isReachable())
            notReachablesFromSource.insert(element);
    } // linear in V
    for (const auto &notReachable: notReachablesFromSource) { // N loops
        auto neighbours = getNotReachableNeighbours(notReachable); // V1 + E1
        if (!neighbours.empty())
            for (const auto &element: neighbours)  // N1 loops
                notReachablesFromSource.erase(element); // log(N1)
    } // N * (V1 + E1) * N1 * log(N1)
    std::cout << "\n You have to build " << notReachablesFromSource.size() << " edges to these nodes: ";
    for (auto &it: notReachablesFromSource)
        std::cout << " " << it.get()->getKey();
    return notReachablesFromSource;
}

template<typename T>
std::vector<std::shared_ptr<GraphNodeReachable<T>>>
HashTable<T>::getNotReachableNeighbours(const std::shared_ptr<GraphNodeReachable<T>> &source) {
    std::vector<std::shared_ptr<GraphNodeReachable<T>>> notReachablesFromSourceNeighbours;
    std::stack<std::shared_ptr<GraphNodeReachable<T>>> stack;
    std::vector<bool> visited(size, false);

    if (!source.get() || !source->getKey()) {
        std::cout << "The key inserted is invalid, could not perform the DFS\n";
        return {};
    }

    stack.push(source);
    while (!stack.empty()) {
        auto currentGraphNode = stack.top();
        stack.pop();
        if (!visited[currentGraphNode->getKey()]) {
            if (!currentGraphNode->isReachable()) {
                visited[currentGraphNode->getKey()] = true;
                notReachablesFromSourceNeighbours.emplace_back(currentGraphNode);
            }
        }
        for (const auto &edge: currentGraphNode->getEdges())
            if (const auto observe = edge.lock()) {
                if (!visited[observe->getKey()])
                    stack.push(std::static_pointer_cast<GraphNodeReachable<T>>(observe));
            }
    }

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
        hashingStrategy = new LinearProbingStrategy<T>(size);
        std::cout << "Using a linear probing hashing strategy" << std::endl;
    } else {
        hashingStrategy = new DoubleHashingStrategy<T>(size);
        std::cout << "Using a double hashing strategy" << std::endl;
    }
}







