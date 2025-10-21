#include "AStar.h"
#include <cmath>
#include <queue>
#include <unordered_map>
#include <algorithm>

AStar::AStar(TileMap* tileMap)
    : map(tileMap)
{
    mapSize = map->getMapSize();
}

float AStar::heuristic(glm::ivec2 a, glm::ivec2 b) const {
    // Manhattan distance (rápida y segura)
    return static_cast<float>(abs(a.x - b.x) + abs(a.y - b.y));
}

bool AStar::isWalkable(int x, int y) const {
    if (x < 0 || y < 0 || x >= mapSize.x || y >= mapSize.y)
        return false;
    return map->isWalkable(x, y);
}

std::vector<Node> AStar::findPath(glm::ivec2 start, glm::ivec2 goal)
{
    std::priority_queue<Node, std::vector<Node>, std::greater<Node>> open;
    std::unordered_map<int, glm::ivec2> cameFrom;
    std::unordered_map<int, float> gScore;

    auto hash = [&](glm::ivec2 p) { return p.y * mapSize.x + p.x; };

    gScore[hash(start)] = 0.f;
    open.push({ start, heuristic(start, goal), 0.f, heuristic(start, goal) });

    glm::ivec2 dirs[4] = { {1,0}, {-1,0}, {0,1}, {0,-1} };

    while (!open.empty()) {
        Node current = open.top(); open.pop();
        if (current.pos == goal) break;

        for (auto& d : dirs) {
            glm::ivec2 neighbor = current.pos + d;
            if (!isWalkable(neighbor.x, neighbor.y)) continue;

            float tentative = gScore[hash(current.pos)] + 1.f;
            if (!gScore.count(hash(neighbor)) || tentative < gScore[hash(neighbor)]) {
                gScore[hash(neighbor)] = tentative;
                float h = heuristic(neighbor, goal);
                open.push({ neighbor, tentative + h, tentative, h });
                cameFrom[hash(neighbor)] = current.pos;
            }
        }
    }

    // reconstruir camino
    std::vector<Node> path;
    glm::ivec2 current = goal;
    while (cameFrom.count(hash(current)) && current != start) {
        path.push_back({ current });
        current = cameFrom[hash(current)];
    }
    std::reverse(path.begin(), path.end());
    return path;
}