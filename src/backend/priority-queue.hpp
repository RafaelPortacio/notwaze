#pragma once

#include <iostream>
#include <vector>

#include "graph.hpp"


template <typename T, typename Compare>
class PriorityQueue {
    public:
        void priority_queuefy(size_t i) {
            size_t l = l_child(i);
            size_t r = r_child(i);
            size_t smallest = i;
            if (l < data.size() && compare(data[l], data[smallest]))
                smallest = l;
            if (r < data.size() && compare(data[r], data[smallest]))
                smallest = r;

            if (smallest != i) {
                std::swap(data[i], data[smallest]);
                priority_queuefy(smallest);
            }
        }

        [[nodiscard]] size_t size() const {
            return data.size();
        }
        [[nodiscard]] T top() const {
            assert(!empty());
            return data.front();
        }
        [[nodiscard]] bool empty() const {
            return data.empty();
        }

        void push(const T& k) {
            data.push_back(k);

            for (size_t i = data.size() - 1; i > 0 && compare(data[i], data[parent(i)]); i = parent(i))
                swap(data[i], data[parent(i)]);
        }
        void pop() {
            assert(!empty());
            data.front() = data.back();
            data.pop_back();
            priority_queuefy(0);
        }

    private:
        static size_t parent(size_t i) { return ((i+1) >> 1) - 1; }
        static size_t l_child(size_t i) { return ((i+1) << 1) - 1; }
        static size_t r_child(size_t i) { return ((i+1) << 1); }

        std::vector<T> data;
        Compare compare;
};
