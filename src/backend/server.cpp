#include <iostream>

#include "graph.hpp"
#include "io.hpp"
#include "shortest-path.hpp"
#include <vector>


int main() {
    const Graph graph = load_graph_from_json_file("rand_graph2.json");

    std::vector<id_t> t = shortest_path_dijkstra(graph, 0, 41);

    for (const auto x:t) {
        std::cout << x << std::endl;
    }

    std::cout << "n_nodes = " << graph.n_nodes() << std::endl;
    std::cout << "n_edges = " << graph.n_edges() << std::endl;
}
