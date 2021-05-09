#pragma once

#include <vector>
#include <unordered_map>


using id_t = unsigned int; // we could use size_t, but unsigned int seems to be
                           // enough for us, and using it will save us plenty of
                           // memory we might need.

using eta_t = double; // in minutes


class Graph {
    public:
        struct Node {
            double latitude;
            double longitude;
        };

        struct Edge {
            id_t origin_node;
            id_t destination_node;
            eta_t eta;
        };

    private:
        std::unordered_map<id_t, Node> _nodes;
        std::vector<Edge> _edges;

    public:
        Graph() = default;

        void add_node(const id_t& id, const Node& node) { _nodes.insert({id, node}); }
        void add_edge(const Edge& edge)                 { _edges.push_back(edge); }

        size_t n_nodes() const { return _nodes.size(); }
        size_t n_edges() const { return _edges.size(); }

        decltype(_nodes)::iterator        begin_nodes()       { return _nodes. begin(); }
        decltype(_nodes)::iterator          end_nodes()       { return _nodes.   end(); }
        decltype(_nodes)::const_iterator cbegin_nodes() const { return _nodes.cbegin(); }
        decltype(_nodes)::const_iterator   cend_nodes() const { return _nodes.  cend(); }
        decltype(_edges)::iterator        begin_edges()       { return _edges. begin(); }
        decltype(_edges)::iterator          end_edges()       { return _edges.   end(); }
        decltype(_edges)::const_iterator cbegin_edges() const { return _edges.cbegin(); }
        decltype(_edges)::const_iterator   cend_edges() const { return _edges.  cend(); }
};
