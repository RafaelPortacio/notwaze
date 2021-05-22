#include "shortest-path.hpp"

#include <bits/stdc++.h>
#include <string>
#include <algorithm>    // std::min_element, std::max_element
#include <queue>

#include "graph.hpp"

using namespace std;


vector<id_t> shortest_path_dijkstra (const Graph& graph,
                                     const id_t& start_point,
                                     const id_t& end_point) {
    priority_queue<pair <id_t, eta_t>> frontier;
    unordered_map<id_t, id_t> came_from;
    unordered_map<id_t, eta_t> cost_so_far;

    frontier.push({start_point, 0});
    cost_so_far.insert({start_point, 0});

    while (!frontier.empty()) {
        pair<id_t, eta_t> current = frontier.top();
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

    vector<id_t> path {end_point};
    id_t current = end_point;
    while (current != start_point) {
        current = came_from.at(current);
        path.push_back(current);
    }
    
    return path;
}
