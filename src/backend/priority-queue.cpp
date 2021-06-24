#include <iostream>
#include <vector>

using namespace std;

void replacement(pair<id_t, eta_t>* x, pair<id_t, eta_t>* y) {
    pair<id_t, eta_t> temp = *x;
    *x = *y;
    *y = temp;
}

struct Compare {
    bool operator() (const pair<id_t, eta_t>& l, const pair<id_t, eta_t>& r) {
        return l.second < r.second;
    }

class priorityQueue {
    
    vector<pair<id_t, eta_t>>* harr;
    int capacity;
    int priority_size;

public:
    // Construtor
    priorityQueue(int capacity);

    int parent(int i) { return (i - 1)/2; }

    int l_child(int i) { return (2*i + 1); }

    int r_child(int i) { return (2*i + 2); }

    void push(pair<id_t, eta_t> k);

    pair<pair<id_t, eta_t>, bool> top();

    void pop();

    void priorityQueuefy(int i);

    pair<id_t, eta_t> min() {return harr[0]; }
};

void priorityQueue::push(pair<id_t, eta_t> k) {

    if (priority_size >= capacity) {
        cout << "\nDeu ruim: espaço de memoria excedido\n";
        return;
    }

    int i = priority_size;
    priority_size++;
    harr[i] = std::pair<id_t, eta_t>;

    while (i > 0 && harr[parent(i)] > harr[i]) {
        replacement(&harr[i], &harr[parent(i)]);
        i = parent(i);
    }
}

pair<pair<id_t, eta_t>, bool> priorityQueue::top() {
    // if (priority_size <= 0) {
    //     return {pair<0, 0>, true};
    // }

    return {harr[0], false};
}

void priorityQueue::pop() {

    if (!(this->priorityQueue::top()).second) {

        harr[0] = harr[priority_size - 1];
        priority_size--;
        priorityQueuefy(0);

    }
    
}

void priorityQueue::priorityQueuefy(int i) {
    
    int l = priorityQueue::l_child(i);
    int r = priorityQueue::r_child(i);
    int smallest = i;
    if (l < priority_size && Compare(harr[l], harr[i])) {
        smallest = l;
    }
    if (r < priority_size && Compare(harr[r], harr[smallest])) {
        smallest = r;
    }

    if (smallest != i) {
        replacement(&harr[i], &harr[smallest]);
        priorityQueuefy(smallest);
    }
}

priorityQueue::priorityQueue(int cap) {
    priority_size = 0;
    capacity = cap;
    harr = new pair<id_t, eta_t>[cap];
}