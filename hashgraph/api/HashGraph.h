//
// Created by saso on 01/01/2022.
//

#ifndef HASHGRAPH_HASHTABLE_H
#define HASHGRAPH_HASHTABLE_H


#include <list>
#include <ostream>
#include <map>
#include <iomanip>
#include <memory>
#include <set>

#include "GraphNode.h"
#include "HashingStrategy.h"
#include "LinearProbingStrategy.h"
#include "DoubleHashingStrategy.h"

static const double max_load_factor = 0.70; // Maximum load factor for open addressing tecnique

template<typename T>
class HashGraph {
public:
    using TableType = std::vector<std::shared_ptr<GraphNode<T>>>;

    explicit HashGraph(int capacity);

    void fillTable(const std::map<T, std::vector<T>> &graphData);

    typename TableType::iterator begin() { return table.begin(); }

    typename TableType::iterator end() { return table.end(); }

    virtual ~HashGraph();

    int insert(std::shared_ptr<GraphNode<T>> graphNode);

    void insert(T nodeKey);

    std::shared_ptr<GraphNode<T>> getByKey(T key);

    std::shared_ptr<GraphNode<T>> getByKey(T key, int &hashIndex);

    bool findEdge(T sourceNodeKey, T targetNodeKey);

    bool findEdge(std::shared_ptr<GraphNode<T>> sourceNode, std::shared_ptr<GraphNode<T>> targetNode);

    void addEdge(T sourceNodeKey, T targetNodeKey);

    void addEdge(std::shared_ptr<GraphNode<T>> sourceNode, std::shared_ptr<GraphNode<T>> targetNode);

    void removeEdge(T sourceNodeKey, T targetNodeKey);

    int getSize() const;

    std::shared_ptr<GraphNode<T>> operator[](int) const;

    friend std::ostream &operator<<(std::ostream &os, const HashGraph<T> &table) {
        os << "HashTable data: " << "capacity " << table.capacity << "; elements inserted " << table.size
           << "; load factor: "
           << std::setprecision(2) << table.loadFactor << std::endl;
        for (int i = 0; i < table.capacity; i++)
            if (!table[i].get())
                os << "[" << i << "]: is empty" << std::endl;
            else {
                const auto keyOfTheNode = table[i]->getKey();
                auto edgesOfTheNode = table[i]->getEdges();

                if (edgesOfTheNode.empty()) { // The node no edges
                    os << "[" << i << "]: " << keyOfTheNode << std::endl;
                } else {
                    os << "[" << i << "]: " << keyOfTheNode << " has edges towards: ";
                    int j = 0;
                    for (const auto &edge: edgesOfTheNode) {
                        if (const auto observe = edge.lock()) {
                            os << observe->getKey() << " ";
                        } else
                            edgesOfTheNode.erase(edgesOfTheNode.begin() + j);
                        j++;
                    }
                    std::endl(os);
                }
            }
        return os;
    }

    void dfs(T keyOfStartingNode);

    std::set<std::shared_ptr<GraphNode<T>>> computeNotReachableNodes(T keyOfStartingNode);

    void setHashingStrategy(char *strategy);

    void resetReachbility();

protected:
    std::vector<std::shared_ptr<GraphNode<T>>>
    getNotReachableNeighbours(const std::shared_ptr<GraphNode<T>> &source);

private:
    TableType table;
    HashingStrategy<T> *hashingStrategy;
    int capacity;
    int size;
    float loadFactor;
};

#endif //HASHGRAPH_HASHTABLE_H
