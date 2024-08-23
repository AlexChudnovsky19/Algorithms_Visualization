#include "Graph.h"
#include <iostream> // For debugging output

void Graph::addNode(Node* node) {
    if (!node) {
        std::cerr << "Attempted to add a null node!" << std::endl;
        return;
    }
    nodes.push_back(node);
}

void Graph::addEdge(Edge* edge) {
    if (!edge || !edge->getFrom() || !edge->getTo()) {
        std::cerr << "Attempted to add an invalid edge!" << std::endl;
        return;
    }
    edges.push_back(edge);
    edge->getFrom()->addEdge(edge);
}

Node* Graph::getNode(int id) {
    if (id < 0 || id >= nodes.size()) {
        std::cerr << "Invalid node id: " << id << std::endl;
        return nullptr;
    }
    return nodes[id];
}

void Graph::createGridGraph(int width, int height, int gridSize) {
    this->width = width;
    this->height = height;
    this->gridSize = gridSize;

    int rows = height / gridSize;
    int cols = width / gridSize;

    nodes.resize(rows * cols, nullptr);

    // Create all nodes first
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            int id = i * cols + j;
            nodes[id] = new Node(id);
        }
    }

    // Connect nodes with edges
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            int id = i * cols + j;

            if (i > 0) { // Connect to the node above
                addEdge(new Edge(nodes[id], nodes[id - cols]));
            }

            if (i < rows - 1) { // Connect to the node below
                addEdge(new Edge(nodes[id], nodes[id + cols]));
            }

            if (j > 0) { // Connect to the node to the left
                addEdge(new Edge(nodes[id], nodes[id - 1]));
            }

            if (j < cols - 1) { // Connect to the node to the right
                addEdge(new Edge(nodes[id], nodes[id + 1]));
            }
        }
    }

    // Debug output for node creation
    std::cout << "Nodes created: ";
    for (Node* node : nodes) {
        if (node != nullptr) {
            std::cout << node->getId() << " ";
        }
    }
    std::cout << std::endl;
}

int Graph::getHeight() const {
    return height;
}

int Graph::getWidth() const {
    return width;
}

int Graph::getGridSize() const {
    return gridSize;
}
