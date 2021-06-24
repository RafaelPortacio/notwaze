#include <iostream>
#include <vector>

using namespace std;

struct Compare {
    bool operator() (const pair<id_t, eta_t>& l, const pair<id_t, eta_t>& r) {
        return l.second < r.second;
    }

class priorityQueue {
    
    vector<pair<id_t, eta_t>> harr;

public:
    // Construtor
    priorityQueue();

    int parent(int i) { return (i - 1)/2; }

    int l_child(int i) { return (2*i + 1); }

    int r_child(int i) { return (2*i + 2); }

    void push(pair<id_t, eta_t> k);

    pair<pair<id_t, eta_t>, bool> top();

    void pop();

    void priorityQueuefy(int i);

    bool empty(int i) { harr.empty() };

    pair<id_t, eta_t> min() {return harr[0]; }
};

void priorityQueue::push(pair<id_t, eta_t> k) {

    int i = harr.size;
    harr[i] = k;

    while (i > 0 && harr[parent(i)] > harr[i]) {
        std::swap(&harr[i], &harr[parent(i)]);
        i = parent(i);
    }
}

pair<pair<id_t, eta_t>, bool> priorityQueue::top() {
    return {harr[0], false};
}

void priorityQueue::pop() {
    if (!(this->priorityQueue::top()).second) {

        harr[0] = harr[harr.size - 1];
        priorityQueuefy(0);

    }
    
}

void priorityQueue::priorityQueuefy(int i) {
    
    int l = priorityQueue::l_child(i);
    int r = priorityQueue::r_child(i);
    int smallest = i;
    if (l < harr.size && Compare(harr[l], harr[i])) {
        smallest = l;
    }
    if (r < harr.size && Compare(harr[r], harr[smallest])) {
        smallest = r;
    }

    if (smallest != i) {
        replacement(&harr[i], &harr[smallest]);
        priorityQueuefy(smallest);
    }
}

priorityQueue::priorityQueue() {
    vector<::pair<id_t, eta_t>> harr;
}