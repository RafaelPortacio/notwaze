#pragma once

#include <cassert>
#include <cmath>
#include <vector>
#include <unordered_map>
#include <algorithm>


using node_id = unsigned long int; // we could use size_t, but unsigned int seems to be
                           		// enough for us, and using it will save us plenty of
                           		// memory we might need.

using eta_t = double; // in minutes
using leng_t = double; // in meters

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
        

        auto pair_dot_prod = [](std::pair<double, double> vec1,
                             std::pair<double, double> vec2) {
            return vec1.first*vec2.first + vec1.second*vec2.second;
        };

        auto pair_cross_prod = [](std::pair<double, double> vec1,
                               std::pair<double, double> vec2) {
            return vec1.first*vec2.second - vec1.second*vec2.first;
        };


        std::pair<double, double> vec_s1_to_p;
        vec_s1_to_p.first = p.latitude - s1.latitude;
        vec_s1_to_p.second = p.longitude - s1.longitude;
        
        std::pair<double, double> vec_s2_to_p;
        vec_s2_to_p.first = p.latitude - s2.latitude;
        vec_s2_to_p.second = p.longitude - s2.longitude;
        
        std::pair<double, double> vec_s1_to_s2;
        vec_s1_to_s2.first = s2.latitude - s1.latitude;
        vec_s1_to_s2.second = s2.longitude - s1.longitude;
        
        if (pair_dot_prod(vec_s1_to_p, vec_s1_to_s2) < 0){
            return euclidean_distance(s1, p);
        }else if(-pair_dot_prod(vec_s2_to_p, vec_s1_to_s2) < 0){
            return euclidean_distance(s2, p);
        }else{
            return std::abs(pair_cross_prod(vec_s1_to_p, vec_s2_to_p))/
                   euclidean_distance(s1, s2);
        }
    }
};

struct Edge {
    eta_t eta;
    leng_t leng;
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

        std::pair<node_id, const Node&> lookup_node(const Node& query_node) const {
            // TODO: we'll probably want to use a k-D tree here
            return *std::min_element(cbegin_nodes(), cend_nodes(),
                                     [&query_node](const auto& l, const auto& r) {
                                         return squared_euclidean_distance(query_node, l.second)
                                             <  squared_euclidean_distance(query_node, r.second);
                                     });
        }
};
