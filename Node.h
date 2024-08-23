#pragma once
#ifndef NODE_H
#define NPDE_H

#include <vector>
using namespace std;
#include "Edge.h"

class Node {
public:
	Node(int id);
	int getId() const;
	void addEdge(Edge* edge);
	const vector<Edge*>& getEdges() const;

private:
	int id;
	vector<Edge*> edges;

};
#endif // !NODE_H

