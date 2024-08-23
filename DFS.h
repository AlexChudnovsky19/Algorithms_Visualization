#pragma once
#ifndef DFS_H
#define DFS_H

#include "Graph.h"
#include "Node.h"
#include "Edge.h"
#include "Algorithm.h"
#include <vector>
#include <unordered_map>

class DFS : public Algorithm {
public:
    void findPath(Graph* graph, Node* start, Node* end, const std::vector<std::vector<bool>>& impassable, UpdateCallback callback) override;
    std::vector<Node*> getPath() const;
    std::vector<Node*> getVisitedNodes() const;

private:
    bool dfs(Node* current, Node* end, std::unordered_map<Node*, Node*>& cameFrom, std::unordered_map<Node*, bool>& visited, const std::vector<std::vector<bool>>& impassable, UpdateCallback callback, int cols);
    std::vector<Node*> path;
    std::vector<Node*> visitedNodes;
};

#endif // DFS_H