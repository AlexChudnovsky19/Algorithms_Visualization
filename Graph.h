#pragma once
#ifndef GRAPH_H
#define GRAPH_H

#include <vector>
#include "Node.h"
#include "Edge.h"

class Graph {
public:
    void addNode(Node* node);
    void addEdge(Edge* edge);
    Node* getNode(int id);
    void createGridGraph(int width, int height, int gridSize);

    int getHeight() const;
    int getWidth() const;
    int getGridSize() const;

private:
    std::vector<Node*> nodes;
    std::vector<Edge*> edges;
    int width;
    int height;
    int gridSize;
};

#endif // GRAPH_H
