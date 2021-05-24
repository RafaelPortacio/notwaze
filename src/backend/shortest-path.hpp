#pragma once

#include <tuple>
#include <vector>
#include <algorithm>
#include <queue>

#include "graph.hpp"


template <typename Heuristic>
std::vector<id_t> shortest_path(const Graph& graph, const id_t& start_point, const id_t& end_point,
                                Heuristic heuristic) {
    struct Compare {
        bool operator() (const std::pair<id_t, eta_t>& l, const std::pair<id_t, eta_t>& r) {
            return l.second > r.second;
        }
    };

    std::priority_queue<std::pair<id_t, eta_t>, std::vector<std::pair<id_t, eta_t>>, Compare> frontier;
    std::unordered_map<id_t, id_t> came_from;
    std::unordered_map<id_t, eta_t> cost_so_far;

    frontier.push({start_point, 0});
    cost_so_far.insert({start_point, 0});

    while (!frontier.empty()) {
        std::pair<id_t, eta_t> current = frontier.top();
        frontier.pop();

        if (current.first == end_point) {
            break;
        }

        for (auto iter = graph.cbegin_outedges(current.first);
             iter != graph.cend_outedges(current.first);
              ++iter) {
            eta_t new_cost = cost_so_far.at(current.first) + iter->second.eta;

            if (!cost_so_far.count(iter->first) || new_cost < cost_so_far.at(iter->first)) {
                cost_so_far[iter->first] = new_cost;
                eta_t priority = new_cost + heuristic(graph[iter->first], graph[end_point]);
                frontier.push({iter->first, priority});
                came_from[iter->first] = current.first;
            }
        }
    }

    std::vector<id_t> path {end_point};
    id_t current = end_point;
    while (current != start_point) {
        current = came_from.at(current);
        path.push_back(current);
    }
    std::reverse(std::begin(path), std::end(path));

    return path;
}


inline eta_t dijkstra_heuristic(const Node&, const Node&) {
    return 0;
}


inline eta_t euclidean_heuristic(const Node& current, const Node& destination) {
    return euclidean_distance(current, destination);
}


inline eta_t manhattan_heuristic(const Node& current, const Node& destination) {
    return manhattan_distance(current, destination);
}