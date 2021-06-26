#pragma once

#include <tuple>
#include <optional>
#include <vector>
#include <algorithm>
#include <queue>
#include <chrono>

#include "graph.hpp"
#include "priority-queue.hpp"


// Comparator
namespace {
    struct Compare {
        bool operator() (const std::pair<node_id, weight_t>& l, const std::pair<node_id, weight_t>& r) {
            return l.second < r.second;
        }
    };
}

// Methods enum
enum class ShortestPathMethod {
    Dijkstra,
    AStarEuclidean,
    AStarManhattan,
};

// Dijkstra
template <typename GetWeight>
std::optional<std::vector<node_id>> shortest_path_dijkstra(const Graph& graph,
                                                           const node_id& start_point, const node_id& end_point,
                                                           GetWeight get_weight) {
    // Base case
    if (start_point == end_point)
        return {{start_point}};

    // Storage structures
    PriorityQueue<std::pair<node_id, weight_t>, Compare> frontier;
    std::unordered_map<node_id, node_id> came_from;
    std::unordered_map<node_id, weight_t> cost_so_far;

    // Initiazation
    frontier.push({start_point, 0});
    cost_so_far.insert({start_point, 0});

    // Main loop
    while (!frontier.empty()) {
        std::pair<node_id, weight_t> current = frontier.top();
        frontier.pop();

        if (current.first == end_point)
            break;

        for (auto iter = graph.cbegin_outedges(current.first); iter != graph.cend_outedges(current.first); ++iter) {
            // Fill in came_from and cost_so_far
            weight_t new_cost = cost_so_far.at(current.first) + iter->second.eta;

            if (!cost_so_far.count(iter->first) || new_cost < cost_so_far.at(iter->first)) {
                cost_so_far[iter->first] = new_cost;
                weight_t priority = new_cost;  // heuristic equals zero
                frontier.push({iter->first, priority});
                came_from[iter->first] = current.first;
            }
        }
    }

    if (came_from.find(end_point) == came_from.cend())
        // If `end_point` isn't in `came_from`, then we haven't really found a path
        return std::nullopt;

    // Constructor path
    std::vector<node_id> path {end_point};
    node_id current = end_point;
    while (current != start_point) {
        current = came_from.at(current);
        path.push_back(current);
    }

    return path;
}


// A*
template <typename Heuristic, typename GetWeight>
std::optional<std::vector<node_id>> shortest_path_astar(const Graph& graph,
                                                        const node_id& start_point, const node_id& end_point,
                                                        Heuristic heuristic, GetWeight get_weight) {
    // Base case
    if (start_point == end_point)
        return {{start_point}};

    // Storage structures
    PriorityQueue<std::pair<node_id, weight_t>, Compare> frontier;
    std::unordered_map<node_id, node_id> came_from;
    std::unordered_map<node_id, weight_t> cost_so_far;

    // Initiazation
    frontier.push({start_point, 0});
    cost_so_far.insert({start_point, 0});

    // Main loop
    while (!frontier.empty()) {
        std::pair<node_id, weight_t> current = frontier.top();
        frontier.pop();

        if (current.first == end_point)
            break;

        for (auto iter = graph.cbegin_outedges(current.first); iter != graph.cend_outedges(current.first); ++iter) {
            // Fill in came_from and cost_so_far
            weight_t new_cost = cost_so_far.at(current.first) + get_weight(iter->second);

            if (!cost_so_far.count(iter->first) || new_cost < cost_so_far.at(iter->first)) {
                cost_so_far[iter->first] = new_cost;
                weight_t priority = new_cost + heuristic(graph[iter->first], graph[end_point]);
                frontier.push({iter->first, priority});
                came_from[iter->first] = current.first;
            }
        }
    }

    if (came_from.find(end_point) == came_from.cend())
        // If `end_point` isn't in `came_from`, then we haven't really found a path
        return std::nullopt;

    // Constructor path
    std::vector<node_id> path {end_point};
    node_id current = end_point;
    while (current != start_point) {
        current = came_from.at(current);
        path.push_back(current);
    }

    return path;
}


inline weight_t euclidean_heuristic(const Node& current, const Node& destination) {
    return euclidean_distance(current, destination);
}

inline weight_t manhattan_heuristic(const Node& current, const Node& destination) {
    return manhattan_distance(current, destination);
}

inline weight_t get_weight_eta(const Edge& edge) {
    return edge.eta;
}

inline weight_t get_weight_length(const Edge& edge) {
    return edge.length;
}


template <typename GetWeight>
std::pair<unsigned long, std::optional<std::tuple<weight_t, weight_t, std::vector<std::pair<double, double>>>>>
    get_path_data(const Graph& graph,
                  const std::pair<double, double>& starting_point,
                  const std::pair<double, double>& ending_point,
                  ShortestPathMethod method,
                  GetWeight get_weight) {
    auto [start_edge, end_edge, start_proj, end_proj,
          start_proj_fraction, end_proj_fraction]
        = graph.coords_to_ids(starting_point, ending_point);

    node_id starting_point_id = start_edge.second;
    node_id ending_point_id = end_edge.first;

    if(start_proj.first == 0)
        starting_point_id = start_edge.first;

    if(end_proj.first == 1)
        ending_point_id = end_edge.second;

    /*
    if(start_edge == end_edge && start_proj_fraction <= end_proj_fraction||
      start_edge == {end_edge.second, end_edge.first}) {
        caminho = conectar os nos start_proj.second e end_proj.second
        const Edge& edge = graph.get_edge(start_edge.first, start_edge.second);
        eta = edge.eta*std::abs(start_proj_fraction, end_proj_fraction);
        length = edge.length*std::abs(start_proj_fraction, end_proj_fraction);
        
    }

    if(graph.get_edge(start_edge.second, start_edge.first).eta>0.00001) {
        if(start_proj.first == 0){
            node_id alt_starting_point_id = start_edge.second;
        }else{
            node_id alt_starting_point_id =start_edge.first;
        }
    }

    if(graph.get_edge(end_edge.second, end_edge.first).eta>0.00001) {
        if(end_proj.first == 1){
            node_id alt_ending_point_id = end_edge.first;
        }else{
            node_id alt_starting_point_id =start_edge.second;
        }
    }
    */

    std::optional<std::vector<node_id>> maybe_path;
    auto start_time = std::chrono::high_resolution_clock::now();
    switch (method) {
        case ShortestPathMethod::Dijkstra:
            maybe_path = shortest_path_dijkstra(graph, starting_point_id, ending_point_id, get_weight);
            break;
        case ShortestPathMethod::AStarEuclidean:
            maybe_path = shortest_path_astar(graph, starting_point_id, ending_point_id, euclidean_heuristic, get_weight);
            break;
        case ShortestPathMethod::AStarManhattan:
            maybe_path = shortest_path_astar(graph, starting_point_id, ending_point_id, manhattan_heuristic, get_weight);
            break;
    }
    auto end_time = std::chrono::high_resolution_clock::now();

    unsigned long int compute_time
        = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time).count();

    if (!maybe_path)
        return {compute_time, std::nullopt};
        // return json::object({{"compute-time", compute_time}});
    std::vector<node_id> path = *maybe_path;

    std::vector<std::pair<double, double>> coord_path = {};

    for (auto it = path.begin(); it != path.end(); it++)
        coord_path.push_back({graph.get_node(*it).latitude, graph.get_node(*it).longitude});
    if (end_proj.first == 2)
        coord_path.insert(coord_path.begin(),
                          {(end_proj.second).latitude, (end_proj.second).longitude});
    if (start_proj.first == 2)
        coord_path.push_back({(start_proj.second).latitude, (start_proj.second).longitude});

    weight_t eta = 0;
    weight_t length = 0;

    for (size_t i = 0; i < path.size()-1; ++i) {
        const Edge& edge = graph.get_edge(path[i+1], path[i]);
        eta += edge.eta;
        length += edge.length;
    }

    {
        const Edge& edge = graph.get_edge(start_edge.first, start_edge.second);
        eta += edge.eta*start_proj_fraction;
        length += edge.length*start_proj_fraction;
        const Edge& edge2 = graph.get_edge(end_edge.first, end_edge.second);
        eta += edge2.eta*end_proj_fraction;
        length += edge2.length*end_proj_fraction;
    }

    return {compute_time, {{eta, length, coord_path}}};
}
