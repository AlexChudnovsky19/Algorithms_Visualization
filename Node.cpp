#include "Node.h"

using namespace std;

Node::Node(int id) : id(id) {}

int Node::getId() const {
    return id;
}

void Node::addEdge(Edge* edge) {
    edges.push_back(edge);
}

const vector<Edge*>& Node::getEdges() const {
    return edges;
}