#include <iostream>
#include <fstream>
#include <string>

int main() {
    std::ifstream input_file("input.txt");
    
    if (!input_file.is_open()) {
        std::cerr << "Не удалось открыть файл input.txt" << std::endl;
        return 1;
    }
    
    std::string line;
    while (std::getline(input_file, line)) {
        std::cout << line << std::endl;
    }
    
    input_file.close();
    return 0;
}