#include <iostream>
#include <functional>
#include <math.h>

#include "graph.hpp"
#include "io.hpp"
#include "graph.hpp"
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

    std::cout << ", pronto!" << std::endl;
    std::cout << "Which algoritm to use?" << std::endl;
    while (algoritm != 0 && algoritm != 1 && algoritm != 2) {
        std::cout << "[0] Dijkstra" << std::endl 
                  << "[1] A* with euclidean heuristic" << std::endl
                  << "[2] A* with manhattan heuristic" << std::endl;
        std::cin >> algoritm;
    }

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

    long double starting_point_x = std::stold(starting_point_str_x);
    long double starting_point_y = std::stold(starting_point_str_y);
    long double ending_point_x = std::stold(ending_point_str_x);
    long double ending_point_y = std::stold(ending_point_str_y);


    std::cout << "distancia: " << pow(pow(ending_point_x - starting_point_x, 2.0) + pow(ending_point_y - starting_point_y, 2.0), 0.5) << std::endl;



    // node_id starting_point = std::stoi(starting_point_str);
    // node_id ending_point = std::stoi(ending_point_str);

    return 0;

    // if (argc != 4) {
    //     std::cerr << "Usage: " << argv[0] << " <METHOD> <START_NODE> <END_NODE>" << std::endl;
    //     return 2;
    // }

    // std::string method(argv[1]);
    // std::string graph_path(argv[2]);
    // node_id starting_point = std::stoi(argv[3]);
    // node_id ending_point = std::stoi(argv[4]);


    // // Eval shortest path
    // std::optional<std::vector<node_id>> maybe_path;
    // if (method == "dijkstra") {
    //     maybe_path = shortest_path_dijkstra(graph, starting_point, ending_point, get_weight_length);
    // } else if (method == "astar-euclidean") {
    //     maybe_path = shortest_path_astar(graph, starting_point, ending_point, euclidean_heuristic, get_weight_length);
    // } else if (method == "astar-manhattan") {
    //     maybe_path = shortest_path_astar(graph, starting_point, ending_point, manhattan_heuristic, get_weight_length);
    // } else {
    //     std::cerr << "Bad shortest path method: " << method << std::endl;
    //     return 1;
    // }

    // if (!maybe_path) {
    //     std::cout << std::endl;
    //     return 0;
    // }
    // const std::vector<node_id>& path = *maybe_path;

    // assert(path.front() == starting_point);
    // assert(path.back()  == ending_point);

    // for (const auto& node_id : path) {
    //     std::cout << node_id << " ";
    // }
    // std::cout << std::endl;
}
