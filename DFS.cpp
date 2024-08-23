#include "DFS.h"
#include <stack>
#include <iostream>
#include <thread>
#include <chrono>

bool DFS::dfs(Node* current, Node* end, std::unordered_map<Node*, Node*>& cameFrom, std::unordered_map<Node*, bool>& visited, const std::vector<std::vector<bool>>& impassable, UpdateCallback callback, int cols) {
    if (current == end) {
        return true;
    }

    for (Edge* edge : current->getEdges()) {
        if (!edge) {
            std::cerr << "Edge is null!" << std::endl;
            continue;
        }

        Node* neighbor = edge->getTo();
        if (!neighbor) {
            std::cerr << "Neighbor is null!" << std::endl;
            continue;
        }

        int id = neighbor->getId();
        int x = id % cols;
        int y = id / cols;

        // Check if the neighbor is impassable
        if (impassable[y][x]) {
            continue;
        }

        if (!visited[neighbor]) {
            visited[neighbor] = true;
            cameFrom[neighbor] = current;
            visitedNodes.push_back(neighbor);

            // Update the visualizer
            callback(neighbor);
            std::this_thread::sleep_for(std::chrono::milliseconds(50)); // Adjust the delay as needed

            if (dfs(neighbor, end, cameFrom, visited, impassable, callback, cols)) {
                return true;
            }
        }
    }

    return false;
}

void DFS::findPath(Graph* graph, Node* start, Node* end, const std::vector<std::vector<bool>>& impassable, UpdateCallback callback) {
    path.clear();
    visitedNodes.clear();
    if (!start || !end) {
        std::cerr << "Start or end node is null!" << std::endl;
        return;
    }

    int rows = graph->getHeight() / graph->getGridSize();
    int cols = graph->getWidth() / graph->getGridSize();

    std::unordered_map<Node*, Node*> cameFrom;
    std::unordered_map<Node*, bool> visited;

    visited[start] = true;
    visitedNodes.push_back(start);
    callback(start);

    if (!dfs(start, end, cameFrom, visited, impassable, callback, cols)) {
        std::cerr << "No path found!" << std::endl;
        return;
    }

    Node* current = end;
    while (current != start) {
        if (!current) {
            std::cerr << "Current node is null during path reconstruction!" << std::endl;
            return;
        }
        path.push_back(current);
        current = cameFrom[current];
    }
    path.push_back(start);
    std::reverse(path.begin(), path.end());
}

std::vector<Node*> DFS::getPath() const {
    return path;
}

std::vector<Node*> DFS::getVisitedNodes() const {
    return visitedNodes;
}
