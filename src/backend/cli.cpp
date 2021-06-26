#include <iostream>
#include <functional>
#include <math.h>
#include <time.h>
#include <chrono>

#include "graph.hpp"
#include "io.hpp"
#include "shortest-path.hpp"


int main() {
    // Initialization variables
    int algoritm = -1;
    std::string starting_point_str_x;
    std::string starting_point_str_y;
    std::string ending_point_str_x;
    std::string ending_point_str_y;

    std::cout << "... Loading index";

    // Load graph
    const Graph graph = load_graph_from_json_file("rj_graph_database.json");

    // Algoritm
    std::cout << ", pronto!" << std::endl;
    std::cout << "Which algoritm to use?" << std::endl;
    while (algoritm != 0 && algoritm != 1 && algoritm != 2) {
        std::cout << "[0] Dijkstra" << std::endl
                  << "[1] A* with euclidean heuristic" << std::endl
                  << "[2] A* with manhattan heuristic" << std::endl;
        std::cin >> algoritm;
    }

    // Starting point and ending point
    std::cout << "Enter the origin: ";
    std::cin >> starting_point_str_x;
    std::cin >> starting_point_str_y;
    std::cout << std::endl;
    std::cout << "Enter the destination: ";
    std::cin >> ending_point_str_x;
    std::cin >> ending_point_str_y;
    std::cout << std::endl;

    starting_point_str_x = starting_point_str_x.substr(0, starting_point_str_x.size()-1);
    ending_point_str_x = ending_point_str_x.substr(0, ending_point_str_x.size()-1);

    double starting_point_x = std::stod(starting_point_str_x);
    double starting_point_y = std::stod(starting_point_str_y);
    double ending_point_x = std::stod(ending_point_str_x);
    double ending_point_y = std::stod(ending_point_str_y);

    // Get the desired method
    ShortestPathMethod method;
    if (algoritm == 0)
        method = ShortestPathMethod::Dijkstra;
    else if (algoritm == 1)
        method = ShortestPathMethod::AStarEuclidean;
    else if (algoritm == 2)
        method = ShortestPathMethod::AStarManhattan;
    else
        throw std::runtime_error("bad shortest path method");

    // Get shortest path
    auto [compute_time, maybe_data] = get_path_data(graph,
                                                    {starting_point_x, starting_point_y},
                                                    {ending_point_x, ending_point_y},
                                                    method,
                                                    get_weight_length);

    // Time and run code
    if (!maybe_data)
        return 1;
    auto [eta, length, path] = *maybe_data;

    // Print shortest path
    if (algoritm == 0) {
        std::cout << "The shortest path using Dijkstra is ";
    } else if (algoritm == 1) {
        std::cout << "The shortest path using A star with euclidean heuristic is ";
    } else {
        std::cout << "The shortest path using A star with manhattan heuristic is ";
    };
    std::cout << length;
    std::cout << " meters (";
    std::cout << (compute_time / 1000.0);
    std::cout << " seconds)" << std::endl;

    // Print path
    std::cout << "Coordinates of the shortest path:" << std::endl;
    for (const auto& pt : path)
        std::cout << "[" << pt.first << ", " << pt.second << "]" << std::endl;

    return 0;
}
