#include <iostream>
#include <vector>
#include <chrono>
#include <cmath>
#include <fstream>

using namespace std;
using namespace std::chrono;

size_t strategy_add_one(size_t current_capacity) {
    return current_capacity + 1;
}

size_t strategy_fixed_chunk(size_t current_capacity) {
    const size_t chunk_size = 100;
    return current_capacity + chunk_size;
}

size_t strategy_multiplicative(size_t current_capacity) {
    if (current_capacity == 0) {
        return 1;
    }
    return current_capacity * 2;
}

void test_strategy(size_t (*strategy_func)(size_t), size_t max_elements) {
    vector<int> array;
    size_t allocated_size = 0;
    size_t size = 0;
       
    for (size_t i = 0; i < max_elements; i++) {
        if (size == allocated_size) {
            auto start = high_resolution_clock::now();
            
            size_t new_capacity = strategy_func(allocated_size);
            vector<int> new_array(new_capacity);
            
            for (size_t j = 0; j < size; j++) {
                new_array[j] = array[j];
            }
            
            array = move(new_array);
            allocated_size = new_capacity;
            
            auto end = high_resolution_clock::now();
            auto itog = duration_cast<nanoseconds>(end - start);
            
            cout << i << "," << itog.count() << endl;
        }
        
        if (size < array.size()) {
            array[size] = i;
        }
        size+=1;
    }
}

int main() {
    const size_t max_elements = 100000;

    test_strategy(strategy_add_one, max_elements);
}