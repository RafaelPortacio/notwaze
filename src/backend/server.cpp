#include <iostream>

#include "graph.hpp"
#include "io.hpp"


int main() {
    const Graph graph = load_graph_from_json_file("rj_graph_database.json");

    std::cout << "n_nodes = " << graph.n_nodes() << std::endl;
    std::cout << "n_edges = " << graph.n_edges() << std::endl;
}
