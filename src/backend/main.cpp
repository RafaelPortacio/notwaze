#include <iostream>
#include <bits/stdc++.h>
#include <string>
#include <algorithm>    // std::min_element, std::max_element
#include <optional>

#include "graph.hpp"

using namespace std;


pair<vector<id_t>, eta_t> shortest_path_dijkstra (const Graph& G, const id_t& s, const id_t& t) {
    unordered_map<id_t, eta_t> d;
    unordered_map<id_t, id_t> p;
    unordered_set<id_t> F;

    for (auto iter = G.cbegin_nodes(); iter != G.cend_nodes(); ++iter) {
        d.insert({iter->first, INFINITY});
        p.insert({iter->first, 0});
        F.insert(iter->first);
    }

    d[s] = 0;

    while (!F.empty()) {
        id_t mv = *min_element(F.cbegin(), F.cend(), [&d](auto l ,auto r){ return d[l] < d[r]; });
        
        for (auto iter = G.cbegin_outedges(mv); iter != G.cend_outedges(mv); ++iter) {
            if (d[iter->first] < d[mv] + iter->second.eta) {
                d[iter->first] = d[mv] + iter->second.eta;
                p[iter->first] = mv;
            }
        }
        F.erase(mv);
    }

    vector<id_t> path {t};
    id_t current = t;
    while (current != s) {
        current = p[current];
        path.push_back(current);
    }
    return {path, d[t]};
}

int main() {
    cout << "hello" << endl;
}
