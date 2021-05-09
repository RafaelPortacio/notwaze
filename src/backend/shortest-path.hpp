#pragma once

#include <tuple>
#include <vector>

#include "graph.hpp"


std::pair<std::vector<id_t>, eta_t> shortest_path_dijkstra(const Graph& G, const id_t& s, const id_t& t);
