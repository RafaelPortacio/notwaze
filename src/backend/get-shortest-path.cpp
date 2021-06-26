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
    node_id starting_point = std::stoi(argv[3]);
    node_id ending_point = std::stoi(argv[4]);

    // Load graph
    const Graph graph = load_graph_from_json_file(graph_path);

    // Eval shortest path
    std::optional<std::vector<node_id>> maybe_path;
    if (method == "dijkstra") {
        maybe_path = shortest_path_dijkstra(graph, starting_point, ending_point, get_weight_length);
    } else if (method == "astar-euclidean") {
        maybe_path = shortest_path_astar(graph, starting_point, ending_point, euclidean_heuristic, get_weight_length);
    } else if (method == "astar-manhattan") {
        maybe_path = shortest_path_astar(graph, starting_point, ending_point, manhattan_heuristic, get_weight_length);
    } else {
        std::cerr << "Bad shortest path method: " << method << std::endl;
        return 1;
    }

    if (!maybe_path) {
        std::cout << std::endl;
        return 0;
    }
    std::vector<node_id>& path = *maybe_path;
    std::reverse(std::begin(path), std::end(path));

    assert(path.front() == starting_point);
    assert(path.back()  == ending_point);

    for (const auto& node_id : path) {
        std::cout << node_id << " ";
    }
    std::cout << std::endl;
}
