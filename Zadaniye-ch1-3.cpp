#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <iomanip>
#include <sstream>

int main() {
    std::ifstream file("input.txt");
    
    if (!file.is_open()) {
        std::cerr << "Не удалось открыть файл input.txt" << std::endl;
        return 1;
    }
    
    int N, M;
    file >> N >> M;
    
    std::string dummy;
    std::getline(file, dummy);

    std::vector<std::vector<int>> table(N, std::vector<int>(M));
    
    for (int i = 0; i < N; ++i) {
        std::string line;
        std::getline(file, line);
        
        std::stringstream ss(line);
        std::string cell;
        
        for (int j = 0; j < M; ++j) {
            if (j < M - 1) {
                std::getline(ss, cell, ',');
            } else {
                std::getline(ss, cell);
            }
            table[i][j] = std::stoi(cell);
        }
    }
    
    file.close();

    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < M; ++j) {
            std::cout << std::setw(10) << table[i][j];
            if (j < M - 1) {
                std::cout << " ";
            }
        }
        std::cout << std::endl;
    }
    
    return 0;
}