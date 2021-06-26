#pragma once

#include <tuple>
#include <optional>
#include <vector>
#include <algorithm>
#include <queue>

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
