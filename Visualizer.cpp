#include "Visualizer.h"
#include <iostream>
#include <algorithm>
#include <thread>
#include <chrono>

Visualizer::Visualizer(int width, int height, int gridSize)
    : window(sf::VideoMode(width, height), "Shortest Path Visualizer"),
    width(width), height(height), gridSize(gridSize),
    startNode(nullptr), endNode(nullptr) {
    graph.createGridGraph(width, height, gridSize);
    impassable.resize(height / gridSize, std::vector<bool>(width / gridSize, false));
}

void Visualizer::run() {
    while (window.isOpen()) {
        processEvents();
        render();
    }
}

void Visualizer::processEvents() {
    sf::Event event;
    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed)
            window.close();
        if (event.type == sf::Event::MouseButtonPressed) {
            isDragging = true;
            handleMouseClick(event.mouseButton.x, event.mouseButton.y);
        }
        if (event.type == sf::Event::MouseButtonReleased) {
            isDragging = false;
        }
        if (event.type == sf::Event::MouseMoved && isDragging) {
            handleMouseDrag(event.mouseMove.x, event.mouseMove.y);
        }
        if (event.type == sf::Event::KeyPressed) {
            toggleAction(event.key.code);
        }
    }
}

void Visualizer::toggleAction(sf::Keyboard::Key key) {
    switch (key) {
    case sf::Keyboard::S:
        currentAction = Action::SetStart;
        std::cout << "Action: Set Start Point" << std::endl;
        break;
    case sf::Keyboard::E:
        currentAction = Action::SetEnd;
        std::cout << "Action: Set End Point" << std::endl;
        break;
    case sf::Keyboard::I:
        currentAction = Action::SetImpassable;
        std::cout << "Action: Set Impassable Point" << std::endl;
        break;
    case sf::Keyboard::P:
        currentAction = Action::FindPath;
        std::cout << "Action: Find Path" << std::endl;
        findPath();
        break;
    case sf::Keyboard::B:
        currentAlgorithm = AlgorithmType::BFS;
        std::cout << "Algorithm: BFS" << std::endl;
        break;
    case sf::Keyboard::D:
        currentAlgorithm = AlgorithmType::DFS;
        std::cout << "Algorithm: DFS" << std::endl;
        break;
    default:
        currentAction = Action::None;
        break;
    }
}

void Visualizer::handleMouseClick(int x, int y) {
    int row = y / gridSize;
    int col = x / gridSize;
    int id = row * (width / gridSize) + col;

    std::cout << "Mouse clicked at (" << x << ", " << y << "), Grid position: (" << col << ", " << row << ")" << std::endl;

    switch (currentAction) {
    case Action::SetStart:
        startNode = graph.getNode(id);
        std::cout << "Set start node at (" << col << ", " << row << ")" << std::endl;
        break;
    case Action::SetEnd:
        endNode = graph.getNode(id);
        std::cout << "Set end node at (" << col << ", " << row << ")" << std::endl;
        break;
    case Action::SetImpassable:
        impassable[row][col] = true;
        std::cout << "Set impassable node at (" << col << ", " << row << ")" << std::endl;
        break;
    case Action::FindPath:
        findPath();
        break;
    default:
        break;
    }
    mouseCellStart = std::make_pair(col, row);
}

int Visualizer::sign(int value) {
    return (value > 0) - (value < 0);
}

void Visualizer::handleMouseDrag(int x, int y) {
    if (currentAction == Action::SetImpassable) {
        int row = y / gridSize;
        int col = x / gridSize;
        std::pair<int, int> mouseCell = std::make_pair(col, row);

        int lineSize = 1 + std::max(abs(mouseCell.first - mouseCellStart.first), abs(mouseCell.second - mouseCellStart.second));
        int stepX = sign(mouseCell.first - mouseCellStart.first);
        int stepY = sign(mouseCell.second - mouseCellStart.second);

        for (int a = 0; a < lineSize; ++a) {
            int cellX = mouseCellStart.first + stepX * floor(a * (1 + abs(mouseCell.first - mouseCellStart.first)) / static_cast<float>(lineSize));
            int cellY = mouseCellStart.second + stepY * floor(a * (1 + abs(mouseCell.second - mouseCellStart.second)) / static_cast<float>(lineSize));

            if (cellX >= 0 && cellY >= 0 && cellX < width / gridSize && cellY < height / gridSize) {
                impassable[cellY][cellX] = true;
            }
        }

        mouseCellStart = mouseCell;
    }
}

void Visualizer::findPath() {
    if (startNode && endNode) {
        auto updateCallback = [this](Node* node) { updateVisualization(node); };

        switch (currentAlgorithm) {
        case AlgorithmType::BFS:
        {
            BFS bfs;
            bfs.findPath(&graph, startNode, endNode, impassable, updateCallback);
            visitedNodes = bfs.getVisitedNodes();
            path = bfs.getPath();
            break;
        }
        case AlgorithmType::DFS:
        {
            DFS dfs;
            dfs.findPath(&graph, startNode, endNode, impassable, updateCallback);
            visitedNodes = dfs.getVisitedNodes();
            path = dfs.getPath();
            break;
        }
        }

        std::cout << "Path found." << std::endl;
        std::cout << "Visited Nodes: ";
        for (Node* node : visitedNodes) {
            std::cout << node->getId() << " ";
        }
        std::cout << std::endl;
        std::cout << "Path: ";
        for (Node* node : path) {
            std::cout << node->getId() << " ";
        }
        std::cout << std::endl;
    }
    else {
        std::cerr << "Start or end node not set!" << std::endl;
    }
}

void Visualizer::updateVisualization(Node* node) {
    int id = node->getId();
    int row = id / (width / gridSize);
    int col = id % (width / gridSize);

    sf::RectangleShape cell(sf::Vector2f(gridSize, gridSize));
    cell.setPosition(col * gridSize, row * gridSize);
    cell.setFillColor(sf::Color(0, 255, 255, 100)); // Semi-transparent cyan for visited nodes
    cell.setOutlineColor(sf::Color(192, 192, 192));
    cell.setOutlineThickness(1);

    window.draw(cell);
    window.display();
    std::this_thread::sleep_for(std::chrono::milliseconds(10)); // Adjust delay for smooth transition
}

void Visualizer::render() {
    window.clear(sf::Color::White);
    drawGrid();
    drawVisitedNodes();
    drawPath();
    window.display();
}

void Visualizer::drawGrid() {
    for (int i = 0; i < width / gridSize; ++i) {
        for (int j = 0; j < height / gridSize; ++j) {
            sf::RectangleShape cell(sf::Vector2f(gridSize, gridSize));
            cell.setPosition(i * gridSize, j * gridSize);
            cell.setOutlineColor(sf::Color(192, 192, 192));
            cell.setOutlineThickness(1);

            if (impassable[j][i]) {
                cell.setFillColor(sf::Color::Red);
            }
            else if (graph.getNode(j * (width / gridSize) + i) == startNode) {
                cell.setFillColor(sf::Color::Green);
            }
            else if (graph.getNode(j * (width / gridSize) + i) == endNode) {
                cell.setFillColor(sf::Color::Blue);
            }
            else {
                cell.setFillColor(sf::Color::White);
            }

            window.draw(cell);
        }
    }
}

void Visualizer::drawPath() {
    for (Node* node : path) {
        int id = node->getId();
        int row = id / (width / gridSize);
        int col = id % (width / gridSize);

        sf::RectangleShape cell(sf::Vector2f(gridSize, gridSize));
        cell.setPosition(col * gridSize, row * gridSize);
        cell.setFillColor(sf::Color::Yellow);
        cell.setOutlineColor(sf::Color(192, 192, 192));
        cell.setOutlineThickness(1);

        window.draw(cell);
    }
}

void Visualizer::drawVisitedNodes() {
    for (Node* node : visitedNodes) {
        if (std::find(path.begin(), path.end(), node) != path.end()) {
            continue; // Skip nodes that are part of the final path
        }

        int id = node->getId();
        int row = id / (width / gridSize);
        int col = id % (width / gridSize);

        if (graph.getNode(row * (width / gridSize) + col) == startNode || graph.getNode(row * (width / gridSize) + col) == endNode) {
            continue; // Skip the start and end nodes
        }

        sf::RectangleShape cell(sf::Vector2f(gridSize, gridSize));
        cell.setPosition(col * gridSize, row * gridSize);
        cell.setFillColor(sf::Color(0, 255, 255, 100)); // Semi-transparent cyan for visited nodes
        cell.setOutlineColor(sf::Color(192, 192, 192));
        cell.setOutlineThickness(1);

        window.draw(cell);
    }
}
