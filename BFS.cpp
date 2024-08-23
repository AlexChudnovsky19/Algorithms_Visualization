#include "BFS.h"
#include <queue>
#include <unordered_map>
#include <iostream>
#include <thread>
#include <chrono>

void BFS::findPath(Graph* graph, Node* start, Node* end, const std::vector<std::vector<bool>>& impassable, UpdateCallback callback) {
    path.clear();
    visitedNodes.clear();
    if (!start || !end) {
        std::cerr << "Start or end node is null!" << std::endl;
        return;
    }

    int rows = graph->getHeight() / graph->getGridSize();
    int cols = graph->getWidth() / graph->getGridSize();

    std::queue<Node*> q;
    std::unordered_map<Node*, Node*> cameFrom;
    std::unordered_map<Node*, bool> visited;

    q.push(start);
    visited[start] = true;
    visitedNodes.push_back(start);

    while (!q.empty()) {
        Node* current = q.front();
        q.pop();

        if (current == end) {
            break;
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
                q.push(neighbor);
                visited[neighbor] = true;
                cameFrom[neighbor] = current;
                visitedNodes.push_back(neighbor);

                // Update the visualizer
                callback(neighbor);
                std::this_thread::sleep_for(std::chrono::milliseconds(50)); // Adjust the delay as needed
            }
        }
    }

    if (cameFrom.find(end) == cameFrom.end()) {
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

std::vector<Node*> BFS::getPath() const {
    return path;
}

std::vector<Node*> BFS::getVisitedNodes() const {
    return visitedNodes;
}

