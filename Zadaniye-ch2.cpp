#include <iostream>
#include <sstream>
#include <string>
#include <cctype>

class numfilterbuf : public std::streambuf {
private:
    std::istream* in;
    std::ostream* out;
    
    int cur; // последнее считанное значение, используется в underflow()
    
protected:
    /* функции записи в поток: */
    
    virtual int overflow(int c) override {
        // Проверяем, является ли символ цифрой или пробелом
        if (std::isdigit(c) or c == ' ') {
            // Выводим символ в выходной поток
            if (out && *out) {
                out->put(static_cast<char>(c));
            }
            return c; // Успешно записали
        }
        return traits_type::eof(); // Не цифра и не пробел
    }
    
    /* функции чтения из потока: */
    
    // реализация по-умолчанию инкрементирует позицию указателя в буфере и вызывает segmentation fault
    virtual int uflow() override {
        // Читаем символ из входного потока с фильтрацией
        int result = underflow();
        
        // Если символ успешно прочитан, сдвигаем позицию
        if (result != traits_type::eof()) {
            // Нужно считать следующий символ при следующем вызове
            cur = traits_type::eof();
        }
        
        return result;
    }
    
    virtual int underflow() override {
        // Если у нас уже есть текущий символ, возвращаем его
        if (cur != traits_type::eof()) {
            return cur;
        }
        
        // Читаем символы из входного потока, пока не найдем подходящий
        while (in && *in) {
            int c = in->get();
            
            // Проверяем, является ли символ цифрой или пробелом
            if (std::isdigit(c) or c == ' ') {
                cur = c;
                return c;
            }
            
            // Если достигнут конец файла
            if (c == traits_type::eof()) {
                return traits_type::eof();
            }
        }
        
        return traits_type::eof();
    }
    
public:
    numfilterbuf(std::istream& _in, std::ostream& _out)
        : in(&_in), out(&_out), cur(traits_type::eof())
    {}
    
    // Деструктор для очистки
    virtual ~numfilterbuf() {}
};

int main() {
    const char str1[] = "In 4 bytes contains 32 bits";
    const char str2[] = "Unix time starts from Jan 1, 1970";
    std::istringstream str(str1);
    
    numfilterbuf buf(str, std::cout); // читать из stringstream, выводить в консоль
    std::iostream numfilter(&buf); // таким образом обходимся без реализации своего наследника iostream
    
    std::string val;
    std::getline(numfilter, val);
    numfilter.clear(); // сбросить невалидное состояние после EOF в процессе чтения из stringstream
    
    std::cout << "Original: '" << str1 << "'" << std::endl;
    std::cout << "Read from numfilter: '" << val << "'" << std::endl;
    
    std::cout << "Original: '" << str2 << "'" << std::endl;
    std::cout << "Written to numfilter: '";
    numfilter << str2;
    std::cout << "'" << std::endl;
    
    return 0;
}