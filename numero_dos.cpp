#include <cassert>
#include <utility>


template <typename T>
class Grid final {
public:
    using value_type = T;
    using size_type = unsigned;
    
private:
    T* data = nullptr;
    size_type y_size = 0, x_size = 0;

    // Вложенный класс-прокси для строки (реализация двойных скобок)
    class RowProxy {
    private:
        T* row_data;
        size_type row_length;
        
    public:
        RowProxy(T* data, size_type length) 
            : row_data(data), row_length(length) {}
        
        T& operator[](size_type x_idx) {
            return row_data[x_idx];
        }
        
        const T& operator[](size_type x_idx) const {
            return row_data[x_idx];
        }
    };

public:
    // Конструктор 1: неявное преобразование из T в Grid<T>
    Grid(T const &t) : y_size(1), x_size(1) {
        data = new T[1];
        data[0] = t;
    }

    // Конструктор 2: создание сетки с default initialization
    Grid(size_type y_size, size_type x_size) : y_size(y_size), x_size(x_size) {
        data = new T[y_size * x_size];
        for (size_type i = 0; i < y_size * x_size; ++i) {
            new (data + i) T();
        }
    }

    // Конструктор 3: создание сетки с заполнением копиями объекта t
    Grid(size_type y_size, size_type x_size, T const &t) : y_size(y_size), x_size(x_size) {
        data = new T[y_size * x_size];
        for (size_type i = 0; i < y_size * x_size; ++i) {
            data[i] = t;
        }
    }

    // Деструктор (правило пяти - 1)
    ~Grid() {
        if (data) {
            delete[] data;
        }
    }

    // Конструктор копирования (правило пяти - 2)
    Grid(Grid<T> const &other) : y_size(other.y_size), x_size(other.x_size) {
        data = new T[y_size * x_size];
        for (size_type i = 0; i < y_size * x_size; ++i) {
            data[i] = other.data[i];
        }
    }

    // Конструктор перемещения (правило пяти - 3)
    Grid(Grid<T>&& other) noexcept 
        : data(other.data), y_size(other.y_size), x_size(other.x_size) {
        other.data = nullptr;
        other.y_size = 0;
        other.x_size = 0;
    }

    // Оператор присваивания копированием (правило пяти - 4)
    Grid<T>& operator=(Grid<T> const &other) {
        if (this != &other) {
            Grid<T> temp(other);
            swap(*this, temp);
        }
        return *this;
    }

    // Оператор присваивания перемещением (правило пяти - 5)
    Grid<T>& operator=(Grid<T>&& other) noexcept {
        if (this != &other) {
            if (data) {
                delete[] data;
            }
            data = other.data;
            y_size = other.y_size;
            x_size = other.x_size;
            other.data = nullptr;
            other.y_size = 0;
            other.x_size = 0;
        }
        return *this;
    }

    // Вспомогательная функция для обмена
    friend void swap(Grid<T>& first, Grid<T>& second) noexcept {
        using std::swap;
        swap(first.data, second.data);
        swap(first.y_size, second.y_size);
        swap(first.x_size, second.x_size);
    }

    // ОПЕРАТОРЫ ИНДЕКСИРОВАНИЯ ДЛЯ ДВОЙНЫХ СКОБОК []
    const RowProxy operator[](size_type y_idx) const {
        return RowProxy(data + y_idx * x_size, x_size);
    }
    
    RowProxy operator[](size_type y_idx) {
        return RowProxy(data + y_idx * x_size, x_size);
    }

    // Оригинальные операторы доступа через круглые скобки
    T operator()(size_type y_idx, size_type x_idx) const {
        return data[y_idx * x_size + x_idx];
    }
    
    T& operator()(size_type y_idx, size_type x_idx) {
        return data[y_idx * x_size + x_idx];
    }

    // Оператор присваивания значения всем элементам сетки
    Grid<T>& operator=(T const &t) {
        for (auto it = data, end = data + x_size * y_size; it != end; ++it) {
            *it = t;
        }
        return *this;
    }

    size_type get_y_size() const { return y_size; }
    size_type get_x_size() const { return x_size; }
};





int main() {
    Grid<float> g(3,2,0.0f);
    assert(3==g.get_y_size());
    assert(2==g.get_x_size());

    using gsize_t=Grid<float>::size_type;

    for (gsize_t y_idx=0; y_idx!=g.get_y_size(); ++y_idx)
        for (gsize_t x_idx=0; x_idx!=g.get_x_size(); ++x_idx)
            assert(0.0f == g[y_idx][x_idx]);
            
    for (gsize_t y_idx=0; y_idx!=g.get_y_size(); ++y_idx)
        for (gsize_t x_idx=0; x_idx!=g.get_x_size(); ++x_idx)
            g[y_idx][x_idx] = 1.0f;

    for (gsize_t y_idx=0; y_idx!=g.get_y_size(); ++y_idx)
        for (gsize_t x_idx=0; x_idx!=g.get_x_size(); ++x_idx)
            assert(1.0f == g(y_idx, x_idx));

    return 0;
}