#pragma once

#include <iostream>
#include <vector>

#include "graph.hpp"


class priorityQueue {
   std::vector<std::pair<id_t, eta_t>> harr;

public:

    size_t parent(size_t i) { return (i - 1)/2; }

    size_t l_child(size_t i) { return (2*i + 1); }

    size_t r_child(size_t i) { return (2*i + 2); }

    void push(std::pair<id_t, eta_t> k);

    std::pair<id_t, eta_t> top();

    void pop();

    void priorityQueuefy(size_t i);

    bool empty();

    std::pair<id_t, eta_t> min() {return harr[0]; }
};

void priorityQueue::push(std::pair<id_t, eta_t> k) {

    size_t i = harr.size();
    harr[i] = k;

    while (i > 0 && harr[parent(i)].second > harr[i].second) {
        swap(harr[i], harr[parent(i)]);
        i = parent(i);
    }
}

bool priorityQueue::empty() {
    return harr.empty();
}

std::pair<id_t, eta_t> priorityQueue::top() {
    return harr[0];
}

void priorityQueue::pop() {
    if (!(this->priorityQueue::top()).second) {

        harr[0] = harr[harr.size() - 1];
        priorityQueuefy(0);

    }
    
}

void priorityQueue::priorityQueuefy(size_t i) {
    
    size_t l = priorityQueue::l_child(i);
    size_t r = priorityQueue::r_child(i);
    size_t smallest = i;
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
