//
// Created by saso on 24/01/2022.
//

#ifndef HASHGRAPH_HASHTABLEITERATOR_H
#define HASHGRAPH_HASHTABLEITERATOR_H

#include <set>
#include "HashTable.h"

template<typename T>
class HashTableIterator {
public:
    explicit HashTableIterator(HashTable<T> &hashTable);

    void dfs(T keyOfStartingNode);

    std::set<std::shared_ptr<GraphNodeReachable<T>>> computeNotReachableNodes(T keyOfStartingNode);

protected:
    std::vector<std::shared_ptr<GraphNodeReachable<T>>>
    getNotReachableNeighbours(const std::shared_ptr<GraphNodeReachable<T>> &source);

private:
    HashTable<T> &hashTable;
};


#endif //HASHGRAPH_HASHTABLEITERATOR_H
