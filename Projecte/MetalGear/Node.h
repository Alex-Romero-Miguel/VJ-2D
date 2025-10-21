//#ifndef _NODE_INCLUDE
//#define _NODE_INCLUDE
//
//#include <vector>
//
//struct Node {
//    int x, y;           // coordenadas de tile
//    int f, g, h;        // heuristicos: 
//    // f = g + h coste total estimado
//    // h = distancia estimado que falta hasta final
//    // g = coste del camino recorrido
//
//    Node(int x_ = 0, int y_ = 0) : x(x_), y(y_), g(0), h(0), f(0) {}
//
//    // Para poder usar Node como clave en un unordered_map
//    bool operator==(const Node& other) const { return x == other.x && y == other.y; }
//};
//
//// Necesario para el unordered_map/set
//struct NodeHash {
//    std::size_t operator()(const Node& n) const {
//        // Un hash simple combinando x e y
//        return std::hash<int>()(n.x) ^ (std::hash<int>()(n.y) << 1);
//    }
//};
//
//// Para que la cola de prioridad sepa cómo ordenar los nodos (menor 'f' primero)
//struct CompareNode {
//    bool operator()(const Node& a, const Node& b) const {
//        return a.f > b.f;
//    }
//};
//
//#endif