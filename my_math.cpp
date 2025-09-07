#include "/home/rJle6/CCCCC/project/include/my_math.hpp"
#include <cmath>

AnswerHandler my_sin(double x) {
    AnswerHandler result;
    result.x = x;
    
    x = fmod(x, 2 * M_PI);
    
    double term = x;
    double sum = term;
    
    for (int n = 1; n < 10; ++n) {
        term *= -x * x / ((2 * n) * (2 * n + 1));
        sum += term;
    }
    
    result.result = sum;
    return result;
}

AnswerHandler my_cos(double x) {
    AnswerHandler result;
    result.x = x;
    
    x = fmod(x, 2 * M_PI);
    
    double term = 1.0;
    double sum = term;
    
    for (int n = 1; n < 10; ++n) {
        term *= -x * x / ((2 * n - 1) * (2 * n));
        sum += term;
    }
    
    result.result = sum;
    return result;
}

AnswerHandler my_tan(double x) {
    AnswerHandler result;
    result.x = x;
    
    AnswerHandler sin_val = my_sin(x);
    AnswerHandler cos_val = my_cos(x);
    
    if (fabs(cos_val.result) < 1e-10) {
        result.result = NAN;
    } else {
        result.result = sin_val.result / cos_val.result;
    }
    
    return result;
}