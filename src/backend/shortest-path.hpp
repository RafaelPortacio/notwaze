#pragma once

#include <tuple>
#include <optional>
#include <vector>
#include <algorithm>
#include <queue>

#include "graph.hpp"
#include "priority-queue.hpp"


// dijkstra
std::optional<std::vector<node_id>> shortest_path_dijkstra(const Graph& graph, const node_id& start_point, const node_id& end_point) {
    if (start_point == end_point) {
        return {{start_point}};
    }

    priorityQueue frontier;
    std::unordered_map<id_t, id_t> came_from;
    std::unordered_map<id_t, eta_t> cost_so_far;

    frontier.push({start_point, 0});
    cost_so_far.insert({start_point, 0});

    while (!frontier.empty()) {
        std::pair<node_id, eta_t> current = frontier.top();
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
                eta_t priority = new_cost;
                frontier.push({iter->first, priority});
                came_from[iter->first] = current.first;
            }
        }
    }

    if (came_from.find(end_point) == came_from.cend()) {
        // If `end_point` isn't in `came_from`, then we haven't really found a path
        return std::nullopt;
    }

    std::vector<node_id> path {end_point};
    node_id current = end_point;
    while (current != start_point) {
        current = came_from.at(current);
        path.push_back(current);
    }
    std::reverse(std::begin(path), std::end(path));

    return path;
}


// A star
template <typename Heuristic>
std::optional<std::vector<node_id>> shortest_path_astar(const Graph& graph, const node_id& start_point, const node_id& end_point, Heuristic heuristic) {
    if (start_point == end_point) {
        return {{start_point}};
    }

    struct Compare {
        bool operator() (const std::pair<node_id, eta_t>& l, const std::pair<node_id, eta_t>& r) {
            return l.second > r.second;
        }
    };

    std::priority_queue<std::pair<node_id, eta_t>, std::vector<std::pair<node_id, eta_t>>, Compare> frontier;
    std::unordered_map<node_id, node_id> came_from;
    std::unordered_map<node_id, eta_t> cost_so_far;

    frontier.push({start_point, 0});
    cost_so_far.insert({start_point, 0});

    while (!frontier.empty()) {
        std::pair<node_id, eta_t> current = frontier.top();
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

    if (came_from.find(end_point) == came_from.cend()) {
        // If `end_point` isn't in `came_from`, then we haven't really found a path
        return std::nullopt;
    }

    std::vector<node_id> path {end_point};
    node_id current = end_point;
    while (current != start_point) {
        current = came_from.at(current);
        path.push_back(current);
    }
    std::reverse(std::begin(path), std::end(path));

    return path;
}


inline eta_t euclidean_heuristic(const Node& current, const Node& destination) {
    return euclidean_distance(current, destination);
}


inline eta_t manhattan_heuristic(const Node& current, const Node& destination) {
    return manhattan_distance(current, destination);
}
