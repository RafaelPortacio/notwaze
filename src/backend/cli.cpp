#include <iostream>
#include <functional>
#include <math.h>
#include <time.h>

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

    long double starting_point_x = std::stold(starting_point_str_x);
    long double starting_point_y = std::stold(starting_point_str_y);
    long double ending_point_x = std::stold(ending_point_str_x);
    long double ending_point_y = std::stold(ending_point_str_y);

    // Shortest path

    double start_time;
    double end_time;
    double time;

    start_time = clock();

    double distance = pow(pow(ending_point_x - starting_point_x, 2.0) + pow(ending_point_y - starting_point_y, 2.0), 0.5);

    end_time = clock();

    time = (end_time - start_time) * 1000.0 / CLOCKS_PER_SEC;

    // Print shortest path
    if (algoritm == 0) {
        std::cout << "The shortest path using Dijkstra is ";
    } else if (algoritm == 1) {
        std::cout << "The shortest path using A star with euclidean heuristic is ";
    } else {
        std::cout << "The shortest path using A star with manhattan heuristic is ";
    };

    std::cout << distance;
    std::cout << " meters (";
    std::cout << time;
    std::cout << " seconds)" << std::endl;

    return 0;
}
