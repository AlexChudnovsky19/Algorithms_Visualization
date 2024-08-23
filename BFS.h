#pragma once
#ifndef BFS_H
#define BFS_H

#include "Graph.h"
#include "Node.h"
#include "Edge.h"
#include "Algorithm.h"
#include <vector>
#include <unordered_map>
#include <functional>

class BFS : public Algorithm {
public:
    using UpdateCallback = std::function<void(Node*)>;

    void findPath(Graph* graph, Node* start, Node* end, const std::vector<std::vector<bool>>& impassable, UpdateCallback callback) override;
    std::vector<Node*> getPath() const;
    std::vector<Node*> getVisitedNodes() const;

private:
    std::vector<Node*> path;
    std::vector<Node*> visitedNodes;
};

#endif // BFS_H
