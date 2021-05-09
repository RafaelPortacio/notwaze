#include <iostream>
#include <bits/stdc++.h>
#include <string>
#include <algorithm>    // std::min_element, std::max_element
#include <optional>

#include "graph.hpp"

using namespace std;


vector<id_t> shortest_path_dijkstra (const Graph& G, const id_t& s, const id_t& t) {
    unordered_map<id_t, double> d;
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
        
        for 
    } 
}

int main() {
    cout << "hello" << endl;
}
