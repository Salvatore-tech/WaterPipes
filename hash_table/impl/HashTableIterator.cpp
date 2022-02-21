//
// Created by saso on 24/01/2022.
//

#include <stack>
#include <iostream>
#include <set>
#include "../api/HashTableIterator.h"

template
class HashTableIterator<int>;

template<typename T>
HashTableIterator<T>::HashTableIterator(HashTable<T> &hashTable):hashTable(hashTable) {}

template<typename T>
void HashTableIterator<T>::dfs(T keyOfStartingNode) {
    std::vector<bool> visited(hashTable.getSize(), false);
    std::stack<std::shared_ptr<GraphNodeReachable<T>>> stack;
    auto startingNode = hashTable.getByKey(keyOfStartingNode);

    if (!startingNode) {
        std::cout << "The key inserted is invalid, could not perform the DFS\n";
        return;
    }

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
std::set<std::shared_ptr<GraphNodeReachable<T>>> HashTableIterator<T>::computeNotReachableNodes(T keyOfStartingNode) {

    dfs(keyOfStartingNode);

    std::set<std::shared_ptr<GraphNodeReachable<T>>> notReachablesFromSource;
    for (auto &element: hashTable) {
        auto observe = element.get();
        if (observe && !observe->isReachable1())
            notReachablesFromSource.insert(element);
    }

    for (const auto &notReachable: notReachablesFromSource) {
        auto neighbours = getNotReachableNeighbours(notReachable);
        if (!neighbours.empty())
            for (const auto &element: neighbours)
                notReachablesFromSource.erase(element);
    }
    std::cout << "\n You have to build " << notReachablesFromSource.size() << " edges to these nodes: ";
    for (auto &it: notReachablesFromSource)
        std::cout << " " << it.get()->getKey();

    return notReachablesFromSource;
}

template<typename T>
std::vector<std::shared_ptr<GraphNodeReachable<T>>>
HashTableIterator<T>::getNotReachableNeighbours(const std::shared_ptr<GraphNodeReachable<T>> &source) {
    std::vector<std::shared_ptr<GraphNodeReachable<T>>> notReachablesFromSourceNeighbours;
    std::stack<std::shared_ptr<GraphNodeReachable<T>>> stack;
    std::vector<bool> visited(hashTable.getSize(), false);

    if (!source.get() || !source->getKey()) {
        std::cout << "The key inserted is invalid, could not perform the DFS\n";
        return {};
    }

    stack.push(source);
    while (!stack.empty()) {
        auto currentGraphNode = stack.top();
        stack.pop();
        if (!visited[currentGraphNode->getKey()]) {
            if (!currentGraphNode->isReachable1()) {
                std::cout << currentGraphNode->getKey() << " ";
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




//    std::map<std::shared_ptr<GraphNode<T>>, bool> isReachableFromSource;
//    std::stack<std::shared_ptr<GraphNode<T>>> stack;
//    auto startingNode = hashTable.getByKey(keyOfStartingNode);
//
//    for (int i = 0; i < hashTable.getSize(); i++) {
//        isReachableFromSource.insert(std::make_pair(hashTable[i], false));
//    }
//
//
//    if (!startingNode) {
//        std::cout << "The key inserted is invalid, could not perform the DFS\n";
//        return;
//    }
//
//    stack.push(startingNode);
//    while (!stack.empty()) {
//        auto currentGraphNode = stack.top();
//        stack.pop();
//        if (isReachableFromSource.at(currentGraphNode) == false) {
//            std::cout << currentGraphNode->getKey() << " ";
//            isReachableFromSource.at(currentGraphNode) = true;
//        }
//        for (const auto &edge: currentGraphNode->getEdges())
//            if (const auto observe = edge.lock()) {
//                if (isReachableFromSource.find(observe) == isReachableFromSource.end())
//                    stack.push(observe);
//            }
//    }
//    std::cout << "";

