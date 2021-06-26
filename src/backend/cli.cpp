#include <iostream>
#include <functional>
#include <math.h>
#include <time.h>
#include <chrono>

#include "graph.hpp"
#include "io.hpp"
#include "shortest-path.hpp"


int main() {
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

    // Shortest path
    double start_time;
    double end_time;

    auto [starting_point, ending_point, _, __] = graph.coords_to_ids({starting_point_x, starting_point_y}, 
                                                                     {ending_point_x, ending_point_y});

    // Time and run code 
    auto t0 = std::chrono::high_resolution_clock::now();

    std::optional<std::vector<node_id>> maybe_path;
    if (algoritm == 0) {
        maybe_path = shortest_path_dijkstra(graph, starting_point, ending_point, get_weight_length);
    } else if (algoritm == 1) {
        maybe_path = shortest_path_astar(graph, starting_point, ending_point, euclidean_heuristic, get_weight_length);
    } else {
        maybe_path = shortest_path_astar(graph, starting_point, ending_point, manhattan_heuristic, get_weight_length);
    }

    auto t1 = std::chrono::high_resolution_clock::now();

    if (!maybe_path) {
        return 2;
    }

    std::vector<node_id> path = *maybe_path;

    auto time = std::chrono::duration_cast<std::chrono::milliseconds>(t1 - t0).count();

    // Print shortest path
    if (algoritm == 0) {
        std::cout << "The shortest path using Dijkstra is ";
    } else if (algoritm == 1) {
        std::cout << "The shortest path using A star with euclidean heuristic is ";
    } else {
        std::cout << "The shortest path using A star with manhattan heuristic is ";
    };

    weight_t distance = 0;
    for (size_t i = 0; i < path.size()-1; ++i) {
        distance += graph.get_edge(path[i], path[i+1]).length;
    };

    std::cout << distance;
    std::cout << " meters (";
    std::cout << time / 1000.0;
    std::cout << " seconds)" << std::endl;

    // Print path

    std::cout << "Coordinates of the shortest path:" << std::endl;
    for (auto i = 0; i < path.size(); ++i) {
        node_id id = path[i];
        std::cout << "[";
        std::cout << graph[id].latitude;
        std::cout << ", ";
        std::cout << graph[id].longitude;
        std::cout << "]" << std::endl; 
    }


    return 0;
}
