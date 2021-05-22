#pragma once

#include <vector>
#include <unordered_map>


using id_t = unsigned int; // we could use size_t, but unsigned int seems to be
                           // enough for us, and using it will save us plenty of
                           // memory we might need.

using eta_t = double; // in minutes


struct Node {
    double latitude;
    double longitude;
};

struct Edge {
    eta_t eta;
};

class Graph {
    private:
        std::unordered_map<id_t, Node> _nodes;
        std::unordered_map<id_t, std::vector<std::pair<id_t, Edge>>> _edges;

    public:
        Graph() = default;

        void add_node(const id_t& id, const Node& node) {
            _nodes.insert({id, node});
        }
        void add_edge(const id_t& start_point, const id_t& end_point, const Edge& edge) {
            if (!_edges.count(start_point))
                _edges.insert({start_point, {}});
            _edges[start_point].push_back({end_point, edge});
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

        std::vector<std::pair<id_t, Edge>>::const_iterator cbegin_outedges(id_t origin_node) const {
            return _edges.at(origin_node).cbegin();
        }
        std::vector<std::pair<id_t, Edge>>::const_iterator cend_outedges(id_t origin_node) const {
            return _edges.at(origin_node).cend();
        }
};
