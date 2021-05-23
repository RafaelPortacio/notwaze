#include <iostream>
#include <functional>

#include "graph.hpp"
#include "io.hpp"
#include "graph.hpp"
#include "shortest-path.hpp"


int main(int argc, char* argv[]) {
    if (argc != 5) {
        std::cerr << "Usage: " << argv[0] << " <METHOD> <GRAPH_FILE> <START_NODE> <END_NODE>" << std::endl;
        return 2;
    }

    std::string method(argv[1]);
    std::string graph_path(argv[2]);
    id_t starting_point = std::stoi(argv[3]);
    id_t ending_point = std::stoi(argv[4]);

    // Load graph
    const Graph graph = load_graph_from_json_file(graph_path);

    // Eval shortest path
    std::vector<id_t> shortest_path;
    if (method == "dijkstra") {
        shortest_path = shortest_path_dijkstra(graph, starting_point, ending_point);
    } else if (method == "astar-euclidean") {
        shortest_path = shortest_path_A_star(graph, starting_point, ending_point);
    } else {
        std::cerr << "Bad shortest path method: " << method << std::endl;
        return 1;
    }

    assert(shortest_path.front() == starting_point);
    assert(shortest_path.back()  == ending_point);

    for (const auto& node_id : shortest_path) {
        std::cout << node_id << " ";
    }
    std::cout << std::endl;
}
