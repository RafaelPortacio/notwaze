#include "io.hpp"

#include <string>
#include <fstream>

#include <nlohmann/json.hpp>

using json = nlohmann::json;


Graph load_graph_from_json_file(const std::string& path) {
    std::string file_contents;
    {
        std::ifstream input_file(path);
        if (!input_file)
            throw std::runtime_error("could not open JSON graph input file: " + path);
        input_file >> file_contents;
    }

    auto document = json::parse(file_contents);

    Graph graph;

    for (const auto& node_data : document["nodes"].items())
        graph.add_node(std::stoul(node_data.key()), Graph::Node {
            .latitude = node_data.value()["latitude"].get<double>(),
            .longitude = node_data.value()["longitude"].get<double>(),
        });

    for (const auto& edge_data : document["edges"])
        graph.add_edge(std::stoul(edge_data["origin_node"].get<std::string>()),
                       std::stoul(edge_data["destination_node"].get<std::string>()),
                       Graph::Edge { .eta = edge_data["eta"].get<double>() });

    graph.done();

    return graph;
}
