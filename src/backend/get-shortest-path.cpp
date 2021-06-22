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
    std::optional<std::vector<id_t>> maybe_path;
    if (method == "dijkstra") {
        maybe_path = shortest_path(graph, starting_point, ending_point, dijkstra_heuristic);
    } else if (method == "astar-euclidean") {
        maybe_path = shortest_path(graph, starting_point, ending_point, euclidean_heuristic);
    } else if (method == "astar-manhattan") {
        maybe_path = shortest_path(graph, starting_point, ending_point, manhattan_heuristic);
    } else {
        std::cerr << "Bad shortest path method: " << method << std::endl;
        return 1;
    }

    if (!maybe_path) {
        std::cout << std::endl;
        return 0;
    }
    const std::vector<id_t>& path = *maybe_path;

    assert(path.front() == starting_point);
    assert(path.back()  == ending_point);

    for (const auto& node_id : path) {
        std::cout << node_id << " ";
    }
    std::cout << std::endl;
}
