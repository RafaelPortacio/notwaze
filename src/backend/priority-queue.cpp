#include <iostream>

using namespace std;

void replacement(int* x, int* y) {
    int temp = *x;
    *x = *y;
    *y = temp;
}

class priorityQueue {
    
    int* harr;
    int capacity;
    int priority_size;

public:
    // Construtor
    priorityQueue(int capacity);

    int parent(int i) { return (i - 1)/2; }

    int l_child(int i) { return (2*i + 1); }

    int r_child(int i) { return (2*i + 2); }

    void push(int k);

    pair<int, bool> top();

    void pop();

    void priorityQueuefy(int i);

    int min() {return harr[0]; }
};

void priorityQueue::push(int k) {

    if (priority_size >= capacity) {
        cout << "\nDeu ruim: espaço de memoria excedido\n";
        return;
    }

    int i = priority_size;
    priority_size++;
    harr[i] = k;

    while (i > 0 && harr[parent(i)] > harr[i]) {
        replacement(&harr[i], &harr[parent(i)]);
        i = parent(i);
    }
}

pair<int, bool> priorityQueue::top() {
    if (priority_size <= 0) {
        return {0, true};
    }

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
    if (l < priority_size && harr[l] < harr[i]) {
        smallest = l;
    }
    if (r < priority_size && harr[r] < harr[smallest]) {
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
    harr = new int[cap];
}