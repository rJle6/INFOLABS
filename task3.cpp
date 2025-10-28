#include <string>
#include <sstream>
#include <exception>
#include <iostream>


class bad_from_string : public std::exception {
private:
    std::string message_;
    
public:
    bad_from_string() : message_("Bad conversion from string") {}
    
    bad_from_string(const char* message) : message_(message) {}
    
    bad_from_string(const std::string& message) : message_(message) {}
    
    virtual const char* what() const noexcept override {
        return message_.c_str();
    }
};

template<class T>
T from_string(std::string const& s) {
    std::istringstream iss(s);
    T result;
    
    iss >> std::noskipws >> result;
    
    if (iss.fail() || !iss.eof()) {
        throw bad_from_string("Failed to convert string to requested type");
    }
    
    return result;
}

