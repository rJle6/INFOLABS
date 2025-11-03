#include <vector>
#include <memory>
#include <cassert>
#include <initializer_list>
#include <stdexcept>
#include <iostream>

template <typename T, size_t N>
class Grid {
public:
    using value_type = T;
    using size_type = size_t;
    
private:
    std::vector<size_type> dimensions;
    std::vector<T> data;
    
    // Вспомогательная функция для вычисления индекса
    size_type compute_index(const std::vector<size_type>& indices) const {
        if (indices.size() != N) {
            throw std::invalid_argument("Wrong number of indices");
        }
        
        size_type index = 0;
        size_type multiplier = 1;
        
        for (size_type i = 0; i < N; ++i) {
            if (indices[i] >= dimensions[i]) {
                throw std::out_of_range("Index out of range");
            }
            index += indices[i] * multiplier;
            multiplier *= dimensions[i];
        }
        
        return index;
    }

public:
    // Конструктор по умолчанию (нужен для operator[])
    Grid() = default;
    
    // Конструктор для создания сетки с заданными размерами и значением по умолчанию
    template<typename... Args>
    Grid(Args... args) {
        static_assert(sizeof...(args) >= N, "Not enough dimensions provided");
        static_assert(sizeof...(args) <= N + 1, "Too many arguments provided");
        
        std::vector<size_type> dims = {static_cast<size_type>(args)...};
        
        // Если передано N+1 параметров, последний - значение для заполнения
        if (dims.size() == N + 1) {
            dimensions = std::vector<size_type>(dims.begin(), dims.begin() + N);
            T fill_value = static_cast<T>(dims.back());
            
            size_type total_size = 1;
            for (auto dim : dimensions) {
                total_size *= dim;
            }
            
            data = std::vector<T>(total_size, fill_value);
        }
        // Если передано ровно N параметров - default initialization
        else if (dims.size() == N) {
            dimensions = dims;
            
            size_type total_size = 1;
            for (auto dim : dimensions) {
                total_size *= dim;
            }
            
            data = std::vector<T>(total_size);
        }
    }
    
    // Конструктор для создания из размеров и данных (для operator[])
    Grid(const std::vector<size_type>& dims, const std::vector<T>& data_vec)
        : dimensions(dims), data(data_vec) {}
    
    // Конструктор копирования
    Grid(const Grid<T, N>& other) 
        : dimensions(other.dimensions), data(other.data) {}
    
    // Конструктор перемещения
    Grid(Grid<T, N>&& other) noexcept
        : dimensions(std::move(other.dimensions)), data(std::move(other.data)) {}
    
    // Оператор присваивания копированием
    Grid<T, N>& operator=(const Grid<T, N>& other) {
        if (this != &other) {
            dimensions = other.dimensions;
            data = other.data;
        }
        return *this;
    }
    
    // Оператор присваивания перемещением
    Grid<T, N>& operator=(Grid<T, N>&& other) noexcept {
        if (this != &other) {
            dimensions = std::move(other.dimensions);
            data = std::move(other.data);
        }
        return *this;
    }
    
    // Оператор доступа через круглые скобки (для N индексов)
    template<typename... Indices>
    T& operator()(Indices... indices) {
        static_assert(sizeof...(indices) == N, "Wrong number of indices");
        
        std::vector<size_type> idxs = {static_cast<size_type>(indices)...};
        return data[compute_index(idxs)];
    }
    
    template<typename... Indices>
    const T& operator()(Indices... indices) const {
        static_assert(sizeof...(indices) == N, "Wrong number of indices");
        
        std::vector<size_type> idxs = {static_cast<size_type>(indices)...};
        return data[compute_index(idxs)];
    }
    
    // Оператор [] для получения срезов (только для N >= 2)
    template<size_t M = N>
    typename std::enable_if<M >= 2, Grid<T, N-1>>::type
    operator[](size_type index) const {
    if (index >= dimensions[0]) {
            throw std::out_of_range("Index out of range");
        }
        
        // Создаем новую сетку размерности N-1
        std::vector<size_type> new_dims(dimensions.begin() + 1, dimensions.end());
        
        // Вычисляем смещение для среза
        size_type slice_size = 1;
        for (size_type i = 1; i < N; ++i) {
            slice_size *= dimensions[i];
        }
        
        size_type start_index = index * slice_size;
        std::vector<T> slice_data(data.begin() + start_index, 
                                 data.begin() + start_index + slice_size);
        
        // Создаем сетку меньшей размерности используя публичный конструктор
        return Grid<T, N-1>(new_dims, slice_data);
    }
    
    // Оператор [] для одномерной сетки (возвращает элемент)
    template<size_t M = N>
    typename std::enable_if<M == 1, T&>::type
    operator[](size_type index) {
        return data[index];
    }
    
    template<size_t M = N>
    typename std::enable_if<M == 1, const T&>::type
    operator[](size_type index) const {
        return data[index];
    }
    
    // Получение размеров
    size_type get_size(size_type dim) const {
        if (dim >= N) {
            throw std::out_of_range("Dimension out of range");
        }
        return dimensions[dim];
    }
    
    // Получение общего числа измерений
    constexpr size_type dimensionality() const {
        return N;
    }
};

// Специализация для 1D сетки
template <typename T>
class Grid<T, 1> {
public:
    using value_type = T;
    using size_type = size_t;
    
private:
    std::vector<size_type> dimensions;
    std::vector<T> data;

public:
    // Конструктор по умолчанию
    Grid() = default;
    
    // Конструктор для создания из размеров и данных
    Grid(const std::vector<size_type>& dims, const std::vector<T>& data_vec)
        : dimensions(dims), data(data_vec) {}
    
    // Конструкторы для 1D
    Grid(size_type size, T fill_value = T()) 
        : dimensions({size}), data(size, fill_value) {}
    
    // Конструктор копирования
    Grid(const Grid<T, 1>& other) 
        : dimensions(other.dimensions), data(other.data) {}
    
    // Конструктор перемещения
    Grid(Grid<T, 1>&& other) noexcept
        : dimensions(std::move(other.dimensions)), data(std::move(other.data)) {}
    
    // Оператор присваивания
    Grid<T, 1>& operator=(const Grid<T, 1>& other) {
        if (this != &other) {
            dimensions = other.dimensions;
            data = other.data;
        }
        return *this;
    }
    
    Grid<T, 1>& operator=(Grid<T, 1>&& other) noexcept {
        if (this != &other) {
            dimensions = std::move(other.dimensions);
            data = std::move(other.data);
        }
        return *this;
    }
    
    // Оператор доступа
    T& operator()(size_type index) {
        if (index >= dimensions[0]) {
            throw std::out_of_range("Index out of range");
        }
        return data[index];
    }
    
    const T& operator()(size_type index) const {
        if (index >= dimensions[0]) {
            throw std::out_of_range("Index out of range");
        }
        return data[index];
    }
    
    // Оператор [] для элементов
    T& operator[](size_type index) {
        return data[index];
    }
    
    const T& operator[](size_type index) const {
        return data[index];
    }
    
    // Получение размеров
    size_type get_size(size_type dim = 0) const {
        if (dim >= 1) {
            throw std::out_of_range("Dimension out of range");
        }
        return dimensions[0];
    }
    
    constexpr size_type dimensionality() const {
        return 1;
    }
};


int main() {
    Grid<float,3> g3(2,3,4, 1.0f);
    assert(1.0f == g3(1,1,1));

    Grid<float,2> g2(2,5,2.0f);
    assert(2.0f == g2(1,1));

    g2=g3[1];
    assert(1.0 == g2(1,1));
}
