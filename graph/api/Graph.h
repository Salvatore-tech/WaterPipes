//
// Created by saso on 12/02/2022.
//

#ifndef WATERPIPES_GRAPH_H
#define WATERPIPES_GRAPH_H


#include <map>
#include <memory>
#include <bits/stdc++.h>
#include <GraphNode.h>

template<typename T>
class Graph {
public:
    Graph() {}

    Graph(const std::map<T, std::vector<T>> &graphData, int numberOfNodes);
    void insert(std::shared_ptr<GraphNode<T>> node);
private:
    std::unordered_set<std::shared_ptr<GraphNode<T>>> graph;
};


#endif //WATERPIPES_GRAPH_H
