#include "/home/rJle6/CCCCC/project/include/my_math.hpp"
#include <iostream>
#include <cmath>

int main() {
    double angle = M_PI / 4;
    
    AnswerHandler sin_result = my_sin(angle);
    AnswerHandler cos_result = my_cos(angle);
    AnswerHandler tan_result = my_tan(angle);
    
    std::cout << "sin(" << sin_result.x << ") = " << sin_result.result;
    std::cout << " (actual: " << sin(angle) << ")" << std::endl;
    
    std::cout << "cos(" << cos_result.x << ") = " << cos_result.result;
    std::cout << " (actual: " << cos(angle) << ")" << std::endl;
    
    std::cout << "tan(" << tan_result.x << ") = " << tan_result.result;
    std::cout << " (actual: " << tan(angle) << ")" << std::endl;
    
    return 0;
}