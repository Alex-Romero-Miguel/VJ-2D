//#ifndef _ASTAR_INCLUDE
//#define _ASTAR_INCLUDE
//
//#include <vector>
//#include <cmath>
////#include "Node.h"
//#include <queue>
//#include <unordered_map>
//#include <unordered_set>
//#include <algorithm>
//
//class AStar {
//public:
//    AStar(const std::vector<std::vector<int>>& grid);
//    std::vector<Node> findPath(Node start, Node goal);
//
//private:
//    bool isWalkable(int x, int y);
//    int heuristic(const Node& a, const Node& b);
//    std::vector<Node> getNeighbors(const Node& node);
//
//    std::vector<std::vector<int>> map;
//};
//
//#endif

#pragma once
#include <glm/glm.hpp>
#include <vector>
#include <queue>
#include <unordered_map>
#include "TileMap.h"   //importante: para usar isWalkable(), getMapSize(), etc.

struct Node {
    glm::ivec2 pos;
    float f, g, h;

    bool operator>(const Node& other) const { return f > other.f; }
    bool operator==(const Node& other) const { return pos == other.pos; }
};

class AStar {
public:
    AStar(TileMap* tileMap);
    std::vector<Node> findPath(glm::ivec2 start, glm::ivec2 goal);

private:
    TileMap* map;  // puntero directo al mapa real
    glm::ivec2 mapSize;
    float heuristic(glm::ivec2 a, glm::ivec2 b) const;
    bool isWalkable(int x, int y) const;
};
