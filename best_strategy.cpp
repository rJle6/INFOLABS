#include <iostream>
#include <chrono>
#include <fstream>

using namespace std;
using namespace std::chrono;

size_t strategy_add_one(size_t current) { 
    return current + 1; 
}
size_t strategy_fixed_chunk(size_t current) { 
    return current + 100; 
}
size_t strategy_multiplicative(size_t current) { 
    if (current==0) {
        return 1;
    }
    return current*2;
}

void test_strategy(size_t (*strategy)(size_t), size_t max_ops) {
    int* data = nullptr;
    size_t capacity = 0;
    size_t size = 0;

    for (size_t i = 0; i < max_ops; i++) {
        if (size == capacity) {
            auto start = high_resolution_clock::now();
            
            size_t new_cap = strategy(capacity);
            int* new_data = new int[new_cap];
            
            for (size_t j = 0; j < size; j++) {
                new_data[j] = data[j];
            }
            
            delete[] data;
            data = new_data;
            capacity = new_cap;
            
            auto end = high_resolution_clock::now();
            cout << i << "," << duration_cast<nanoseconds>(end - start).count() << endl;
        }
        
        data[size++] = i;
    }

    delete[] data;
    capacity=0;
    size=0;
}

int main() {
    const size_t max_ops = 100000;
    
    //test_strategy( strategy_add_one, max_ops);
    //test_strategy( strategy_fixed_chunk, max_ops);
    //test_strategy( strategy_multiplicative, max_ops);
}