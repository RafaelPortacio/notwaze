#pragma once

#include <iostream>
#include <vector>

#include "graph.hpp"

using namespace std;

class priorityQueue {
    
    vector<pair<id_t, eta_t>> harr;

public:

    int parent(int i) { return (i - 1)/2; }

    int l_child(int i) { return (2*i + 1); }

    int r_child(int i) { return (2*i + 2); }

    void push(pair<id_t, eta_t> k);

    pair<id_t, eta_t> top();

    void pop();

    void priorityQueuefy(int i);

    bool empty();

    pair<id_t, eta_t> min() {return harr[0]; }
};

void priorityQueue::push(pair<id_t, eta_t> k) {

    int i = harr.size();
    harr[i] = k;

    while (i > 0 && harr[parent(i)].second > harr[i].second) {
        swap(harr[i], harr[parent(i)]);
        i = parent(i);
    }
}

bool priorityQueue::empty() {
    return harr.empty();
}

pair<id_t, eta_t> priorityQueue::top() {
    return harr[0];
}

void priorityQueue::pop() {
    if (!(this->priorityQueue::top()).second) {

        harr[0] = harr[harr.size() - 1];
        priorityQueuefy(0);

    }
    
}

void priorityQueue::priorityQueuefy(int i) {
    
    int l = priorityQueue::l_child(i);
    int r = priorityQueue::r_child(i);
    int smallest = i;
    if (l < harr.size() && harr[l].second < harr[i].second) {
        smallest = l;
    }
    if (r < harr.size() && harr[r].second < harr[smallest].second) {
        smallest = r;
    }

    if (smallest != i) {
        swap(harr[i], harr[smallest]);
        priorityQueuefy(smallest);
    }
}
