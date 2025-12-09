#include <iostream>
#include <fstream>
#include <string>

int main() {
    std::ifstream input_file("input.txt");
    std::ofstream output_file("output.txt");
    
    if (!input_file.is_open()) {
        std::cerr << "Не удалось открыть файл input.txt" << std::endl;
        return 1;
    }
    
    if (!output_file.is_open()) {
        std::cerr << "Не удалось открыть файл output.txt" << std::endl;
        input_file.close();
        return 1;
    }
    
    std::string line;
    while (std::getline(input_file, line)) {
        output_file << line << std::endl;
    }
    
    input_file.close();
    output_file.close();
    return 0;
}