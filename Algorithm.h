#pragma once
#ifndef ALGORITHM_H
#define ALGORITHM_H

#include "Graph.h"
#include <vector>
#include <functional>

class Algorithm {
public:
    using UpdateCallback = std::function<void(Node*)>;

    virtual void findPath(Graph* graph, Node* start, Node* end, const std::vector<std::vector<bool>>& impassable, UpdateCallback callback) = 0;
    virtual ~Algorithm() = default; // Virtual destructor defined as default
};

#endif // ALGORITHM_H