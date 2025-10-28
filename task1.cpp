#include <iostream>
#include <string>
#include <exception>

class MathException : public std::exception {
private:
    std::string message_;
    
public:
    MathException() : message_("Sho-to ne to") {}
    

    MathException(const std::string& message) : message_(message) {}
    

    virtual const char* what() const noexcept override {
        return message_.c_str();
    }
};

class OverflowException : public MathException {
public:
    OverflowException() : MathException("Mdaa") {}
    OverflowException(const std::string& message) : MathException(message) {}
};

class UnderflowException : public MathException {
public:
    UnderflowException() : MathException("Chel, ty") {}
    UnderflowException(const std::string& message) : MathException(message) {}
};

class DivisionByZeroException : public MathException {
public:
    DivisionByZeroException() : MathException("NA 0 NE DELI!!!") {}
    DivisionByZeroException(const std::string& message) : MathException(message) {}
};

int divide(int x, int y) {
    if (y == 0) throw DivisionByZeroException();
    return x / y;
}
