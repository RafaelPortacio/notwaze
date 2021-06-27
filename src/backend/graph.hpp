#pragma once

#include <cassert>
#include <cmath>
#include <vector>
#include <unordered_map>
#include <algorithm>

using node_id = unsigned long int;

using weight_t = double; // in minutes or meters

struct Node {
    double latitude;
    double longitude;

    friend double squared_euclidean_distance(const Node& x, const Node& y) {
        double latdiff  = x.latitude  - y.latitude;
        double longdiff = x.longitude - y.longitude;
        return latdiff*latdiff + longdiff*longdiff;
    }

    friend double euclidean_distance(const Node& x, const Node& y) {
        return std::sqrt(squared_euclidean_distance(x, y));
    }

    friend double manhattan_distance(const Node& x, const Node& y) {
        return std::abs(x.latitude - y.latitude) + std::abs(x.longitude - y.longitude);
    }

    friend double distance_to_segment(const Node& s1,
                                      const Node& s2,
                                      const Node& p) {
        auto pair_dot_prod = [](std::pair<double, double> vec1, std::pair<double, double> vec2) {
            return vec1.first*vec2.first + vec1.second*vec2.second;
        };
        auto pair_cross_prod = [](std::pair<double, double> vec1, std::pair<double, double> vec2) {
            return vec1.first*vec2.second - vec1.second*vec2.first;
        };

        std::pair<double, double> vec_s1_to_p = {p.latitude - s1.latitude, p.longitude - s1.longitude};
        std::pair<double, double> vec_s2_to_p = {p.latitude - s2.latitude, p.longitude - s2.longitude};
        std::pair<double, double> vec_s1_to_s2 = {s2.latitude - s1.latitude, s2.longitude - s1.longitude};

        if (pair_dot_prod(vec_s1_to_p, vec_s1_to_s2) < 0)
            return euclidean_distance(s1, p);
        if (-pair_dot_prod(vec_s2_to_p, vec_s1_to_s2) < 0)
            return euclidean_distance(s2, p);

        return std::abs(pair_cross_prod(vec_s1_to_p, vec_s2_to_p)) / euclidean_distance(s1, s2);
    }

    friend std::pair<int, Node> projection(const Node& s1,
                                           const Node& s2,
                                           const Node& p) {
        auto pair_dot_prod = [](std::pair<double, double> vec1, std::pair<double, double> vec2) {
            return vec1.first*vec2.first + vec1.second*vec2.second;
        };

        std::pair<double, double> vec_s1_to_p = {p.latitude - s1.latitude, p.longitude - s1.longitude};
        std::pair<double, double> vec_s2_to_p = {p.latitude - s2.latitude, p.longitude - s2.longitude};
        std::pair<double, double> vec_s1_to_s2 = {s2.latitude - s1.latitude, s2.longitude - s1.longitude};

        if (pair_dot_prod(vec_s1_to_p, vec_s1_to_s2) < 0)
            return {0, Node {.latitude = s1.latitude, .longitude = s1.longitude}};
        if (-pair_dot_prod(vec_s2_to_p, vec_s1_to_s2) < 0)
            return {1, Node {.latitude = s2.latitude, .longitude = s2.longitude}};

        double proj_frac = std::abs(pair_dot_prod(vec_s1_to_p, vec_s1_to_s2)) / squared_euclidean_distance(s1, s2);
        return { 2, Node { .latitude = s1.latitude + proj_frac*vec_s1_to_s2.first,
                           .longitude = s1.longitude + proj_frac*vec_s1_to_s2.second }};
    }
};

struct Edge {
    weight_t eta;
    weight_t length;
};

class Graph {
    private:
        std::unordered_map<node_id, Node> _nodes;
        std::unordered_map<node_id, std::vector<std::pair<node_id, Edge>>> _edges;

    public:
        Graph() = default;

        const Node& operator[](const node_id& node_id) const {
            assert(_nodes.count(node_id));
            return _nodes.at(node_id);
        }

        void add_node(const node_id& id, const Node& node) {
            _nodes.insert({id, node});
        }
        void add_edge(const node_id& start_point, const node_id& end_point, const Edge& edge) {
            if (!_edges.count(start_point))
                _edges.insert({start_point, {}});
            _edges[start_point].push_back({end_point, edge});
        }
        void done() {
            for (const auto& [id, _] : _nodes)
                _edges.insert({id, {}});
        }

        size_t n_nodes() const { return _nodes.size(); }
        size_t n_edges() const {
            size_t out = 0;
            for (const auto& [_, es] : _edges)
                out += es.size();
            return out;
        }

        decltype(_nodes)::iterator        begin_nodes()       { return _nodes. begin(); }
        decltype(_nodes)::iterator          end_nodes()       { return _nodes.   end(); }
        decltype(_nodes)::const_iterator cbegin_nodes() const { return _nodes.cbegin(); }
        decltype(_nodes)::const_iterator   cend_nodes() const { return _nodes.  cend(); }
        decltype(_edges)::iterator        begin_edges()       { return _edges. begin(); }
        decltype(_edges)::iterator          end_edges()       { return _edges.   end(); }
        decltype(_edges)::const_iterator cbegin_edges() const { return _edges.cbegin(); }
        decltype(_edges)::const_iterator   cend_edges() const { return _edges.  cend(); }

        std::vector<std::pair<node_id, Edge>>::const_iterator cbegin_outedges(node_id origin_node) const {
            assert(_edges.count(origin_node));
            return _edges.at(origin_node).cbegin();
        }
        std::vector<std::pair<node_id, Edge>>::const_iterator cend_outedges(node_id origin_node) const {
            assert(_edges.count(origin_node));
            return _edges.at(origin_node).cend();
        }

        const Edge& get_edge(node_id origin_node, node_id destination_node) const {
            return std::find_if(cbegin_outedges(origin_node), cend_outedges(origin_node),
                                [destination_node](const auto& x) {
                                    return x.first == destination_node;
                                })->second;
        }

        const Node& get_node(node_id the_node_id) const {
            return _nodes.at(the_node_id);
        }

        std::pair<node_id, node_id> lookup_nodes(const Node& query_node) const {
            double min_dist_to_segment = INFINITY;
            node_id min_origin = -1;
            node_id min_receiver = -1;
            for (auto it = cbegin_edges(); it != cend_edges(); ++it) {
                node_id origin = it->first;
                const auto& edges_from = it->second;
                for (auto it2 = edges_from.cbegin(); it2 != edges_from.cend(); ++it2) {
                    node_id receiver = it2->first;
                    double dist_to_segment = distance_to_segment(get_node(origin), get_node(receiver), query_node);
                    if (dist_to_segment < min_dist_to_segment) {
                        min_dist_to_segment = dist_to_segment;
                        min_origin = origin;
                        min_receiver = receiver;
                    }
                }
            }

            return {min_origin, min_receiver};
        }

        std::tuple<std::pair<node_id,node_id>, std::pair<node_id,node_id>,
                   std::pair<int, Node>, std::pair<int, Node>, double, double>
                       coords_to_ids(const std::pair<double, double>& start_coords,
                                     const std::pair<double, double>& end_coords) const {
            Node starting_point = Node {.latitude=start_coords.first, .longitude=start_coords.second};
            Node ending_point = Node {.latitude=end_coords.first, .longitude=end_coords.second};
            std::pair<node_id,node_id> start_edge = lookup_nodes(starting_point);
            std::pair<node_id,node_id> end_edge = lookup_nodes(ending_point);

            Node start_node_1 = get_node(start_edge.first);
            Node start_node_2 = get_node(start_edge.second);
            Node end_node_1 = get_node(end_edge.first);
            Node end_node_2 = get_node(end_edge.second);

            std::pair<int, Node> start_proj = projection(start_node_1, start_node_2, starting_point);
            std::pair<int, Node> end_proj = projection(end_node_1, end_node_2, ending_point);


            double start_proj_fraction
                = euclidean_distance(start_node_2, start_proj.second) / euclidean_distance(start_node_1, start_node_2);

            double end_proj_fraction
                = euclidean_distance(end_node_1, end_proj.second) / euclidean_distance(end_node_1, end_node_2);

            return {
                start_edge,
                end_edge,
                start_proj,
                end_proj,
                start_proj_fraction,
                end_proj_fraction,
            };
        }
};
