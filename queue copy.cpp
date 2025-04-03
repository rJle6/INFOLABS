#include <iostream>
#include <chrono>
#include <fstream>

using namespace std;
using namespace std::chrono;

struct Queue {
    int* data;
    size_t capacity;
    size_t read_idx;
    size_t write_idx;
    size_t size;
};

void queue_new(Queue& q) {
    q.data = nullptr;
    q.capacity = 0;
    q.read_idx = 0;
    q.write_idx = 0;
    q.size = 0;
}

bool queue_resize(Queue& q) {
    size_t new_capacity;
    if (q.capacity==0) {
        new_capacity=1;
    }
    else {
        new_capacity=q.capacity*2;
    }
    int* new_data = new (nothrow) int[new_capacity];
    if (!new_data) return false;

    if (q.size > 0) {
        if (q.write_idx > q.read_idx) {
            copy(q.data + q.read_idx, q.data + q.write_idx, new_data);
        } 
        else {
            copy(q.data + q.read_idx, q.data + q.capacity, new_data);
            copy(q.data, q.data + q.write_idx, new_data + (q.capacity - q.read_idx));
        }
    }

    delete[] q.data;
    q.data = new_data;
    q.read_idx = 0;
    q.write_idx = q.size;
    q.capacity = new_capacity;
    return true;
}

bool queue_push(Queue& q, int element) {
    if (q.size == q.capacity) {
        if (!queue_resize(q)) return false;
    }

    q.data[q.write_idx] = element;
    q.write_idx = (q.write_idx + 1) % q.capacity;
    q.size+=1;
    return true;
}

bool queue_poll(Queue& q, int& element) {
    if (q.size == 0) return false;

    element = q.data[q.read_idx];
    q.read_idx = (q.read_idx + 1) % q.capacity;
    q.size-=1;
    return true;
}

void queue_del(Queue& q) {
    delete[] q.data;
    q.data = nullptr;
}

void test_performance() {

    Queue q;
    queue_new(q);
    const size_t max_ops = 1000000;

    for (size_t i = 0; i < max_ops; i++) {
        auto start = high_resolution_clock::now();
        if (!queue_push(q, i)) break;
        auto end = high_resolution_clock::now();
        cout << i << "," << duration_cast<nanoseconds>(end - start).count();
    }
    for (size_t i = 0; i < max_ops; i++) {
        int val;
        auto start = high_resolution_clock::now();
        if (!queue_poll(q, val)) break;
        auto end = high_resolution_clock::now();
        cout << i << "," << duration_cast<nanoseconds>(end - start).count();
    }
    queue_del(q);
}

int main() {
    test_performance();
}