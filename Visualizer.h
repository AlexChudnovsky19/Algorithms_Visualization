#ifndef VISUALIZER_H
#define VISUALIZER_H

#include <SFML/Graphics.hpp>
#include "Graph.h"
#include "BFS.h"
#include "DFS.h"
#include <vector>

class Visualizer {
public:
    Visualizer(int width, int height, int gridSize);
    void run();

private:
    void processEvents();
    void toggleAction(sf::Keyboard::Key key);
    void handleMouseClick(int x, int y);
    void handleMouseDrag(int x, int y);
    void findPath();
    void render();
    void drawGrid();
    void drawPath();
    void drawVisitedNodes();
    int sign(int value);

    void updateVisualization(Node* node);

    sf::RenderWindow window;
    int width;
    int height;
    int gridSize;
    Graph graph;
    Node* startNode;
    Node* endNode;
    std::vector<std::vector<bool>> impassable;
    std::vector<Node*> path;
    std::vector<Node*> visitedNodes;
    bool isDragging = false;

    enum class Action { None, SetStart, SetEnd, SetImpassable, FindPath } currentAction = Action::None;
    enum class AlgorithmType { BFS, DFS } currentAlgorithm = AlgorithmType::BFS;
    std::pair<int, int> mouseCellStart;
};

#endif // VISUALIZER_H
