#include "Edge.h"

Edge::Edge(Node* from, Node* to) : from(from), to(to) {}

Node* Edge::getFrom() const{
	return from;
}

Node* Edge::getTo() const{
	return to;
}
