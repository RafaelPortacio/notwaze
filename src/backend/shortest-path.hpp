#pragma once

#include <tuple>
#include <vector>

#include "graph.hpp"


std::vector<id_t> shortest_path_dijkstra(const Graph& graph, 
                                         const id_t& start_point, 
                                         const id_t& end_point);
