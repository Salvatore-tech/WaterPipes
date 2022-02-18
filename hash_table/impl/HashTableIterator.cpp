//
// Created by saso on 24/01/2022.
//

#include <stack>
#include <iostream>
#include "../api/HashTableIterator.h"


template
class HashTableIterator<int>;

template<typename T>
HashTableIterator<T>::HashTableIterator(HashTable<T> &hashTable):hashTable(hashTable) {}

template<typename T>
void HashTableIterator<T>::dfs(T keyOfStartingNode) const {
    std::map<std::shared_ptr<GraphNode<T>>, bool> isReachableFromSource;
    std::stack<std::shared_ptr<GraphNode<T>>> stack;
    auto startingNode = hashTable.getByKey(keyOfStartingNode);

    for (int i = 0; i < hashTable.getSize(); i++) {
        isReachableFromSource.insert(std::make_pair(hashTable[i], false));
    }


    if (!startingNode) {
        std::cout << "The key inserted is invalid, could not perform the DFS\n";
        return;
    }

    stack.push(startingNode);
    while (!stack.empty()) {
        auto currentGraphNode = stack.top();
        stack.pop();
        if (isReachableFromSource.at(currentGraphNode) == false) {
            std::cout << currentGraphNode->getKey() << " ";
            isReachableFromSource.at(currentGraphNode) = true;
        }
        for (const auto &edge: currentGraphNode->getEdges())
            if (const auto observe = edge.lock()) {
                if (isReachableFromSource.find(observe) == isReachableFromSource.end())
                    stack.push(observe);
            }
    }

//    std::vector<int> nums{1,2,3,4};
//    auto binary_op = [](int num1, int num2) { return num1 + num2; };
//    int result = std::reduce(std::execution::par, nums.begin(), nums.end(), 0, binary_op);

//    auto notReachableFromSource = std::reduce(std::execution::par, std::cbegin(hashTable), std::cend(hashTable), 0, binary_op);

//    for(auto const& it :isReachableFromSource){
//        if(it.second == false){
//
//        }
//    }

    std::cout << "";

}
