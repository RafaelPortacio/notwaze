#include <iostream>
#include <numeric>
#include <chrono>
#include <fstream>
#include <indicators/block_progress_bar.hpp>
#include <indicators/cursor_control.hpp>

#include "graph.hpp"
#include "io.hpp"
#include "shortest-path.hpp"


namespace std {
    template <typename U, typename V>
    struct hash<std::pair<U, V>> {
        size_t operator()(const std::pair<U, V>& pair) const {
            return std::hash<size_t>{}(pair.first) ^ std::hash<size_t>{}(pair.second);
        }
    };
}


enum class ShortestPathMethod {
    Dijkstra,
    AStarEuclidean,
    AStarManhattan,
};

template <ShortestPathMethod method, typename... Args>
std::optional<std::vector<node_id>> run_shortest_path(Args... args) {
    if constexpr (method == ShortestPathMethod::Dijkstra)
        return shortest_path_dijkstra(args...);
    else if constexpr (method == ShortestPathMethod::AStarEuclidean)
        return shortest_path_astar(args..., euclidean_heuristic);
    else if constexpr (method == ShortestPathMethod::AStarManhattan)
        return shortest_path_astar(args..., manhattan_heuristic);
}

template <ShortestPathMethod method>
std::unordered_map<std::pair<node_id, node_id>, double> benchmark_shortest_path(const Graph& graph) {
    constexpr size_t n_nodes_to_test = 10;
    constexpr size_t n_samples = 10;

    std::vector<node_id> nodes_to_test(n_nodes_to_test);
    std::transform(graph.cbegin_nodes(), std::next(graph.cbegin_nodes(), n_nodes_to_test), std::begin(nodes_to_test),
                   [](const auto& p) { return p.first; });

    std::unordered_map<std::pair<node_id, node_id>, double> measurements;

    indicators::BlockProgressBar progress_bar {
        indicators::option::BarWidth(80),
        indicators::option::Start("["),
        indicators::option::End("]"),
        indicators::option::ForegroundColor{indicators::Color::white},
        indicators::option::ShowElapsedTime{true},
        indicators::option::ShowRemainingTime{true},
        indicators::option::MaxProgress{n_nodes_to_test*n_nodes_to_test*n_samples},
    };

    for (const node_id& start_node : nodes_to_test) {
        for (const node_id& end_node : nodes_to_test) {
            std::vector<long> samples(n_samples);
            for (size_t k = 0; k < n_samples; ++k) {
                auto t0 = std::chrono::high_resolution_clock::now();
                run_shortest_path<method>(graph, start_node, end_node);
                auto t1 = std::chrono::high_resolution_clock::now();
                samples[k] = std::chrono::duration_cast<std::chrono::milliseconds>(t1 - t0).count();
                progress_bar.tick();
            }
            measurements.insert({
                {start_node, end_node},
                std::accumulate(std::begin(samples), std::end(samples), 0.0) / (double)samples.size()
            });
        }
    }

    progress_bar.mark_as_completed();

    return measurements;
}

int main(int argc, char* argv[]) {
    if (argc != 4) {
        std::cerr << "Usage: " << argv[0] << " <METHOD> <GRAPH_FILE> <OUT_FILE>" << std::endl;
        return 2;
    }

    std::string method(argv[1]);
    std::string graph_path(argv[2]);
    std::string out_path(argv[3]);

    // Load graph
    std::cout << "Reading the graph..." << std::flush;
    const Graph graph = load_graph_from_json_file(graph_path);
    std::cout << " Ok!" << std::endl;

    // Run the benchmarks
    std::unordered_map<std::pair<node_id, node_id>, double> measurements;
    if (method == "dijkstra")
        measurements = benchmark_shortest_path<ShortestPathMethod::Dijkstra>(graph);
    else if (method == "astar-euclidean")
        measurements = benchmark_shortest_path<ShortestPathMethod::AStarEuclidean>(graph);
    else if (method == "astar-manhattan")
        measurements = benchmark_shortest_path<ShortestPathMethod::AStarManhattan>(graph);
    else {
        std::cerr << "bad shortest path method: " << method << std::endl;
        return 2;
    }

    // Write the output CSV
    std::ofstream os(out_path);
    for (const auto& [n, t] : measurements)
        os << n.first << "," << n.second << "," << t << std::endl;
}
