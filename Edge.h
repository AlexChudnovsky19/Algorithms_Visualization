#pragma once
#ifndef  EDGE_H
#define EDGE_H

class Node;

class Edge {
public:
	Edge(Node* from, Node* to);

	Node* getFrom() const;
	Node* getTo() const;

private:
	Node* from;
	Node* to;
};



#endif // ! EDGE_H

