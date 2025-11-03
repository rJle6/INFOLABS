template <typename T>
class Grid final {
public:
    using value_type = T;
    using size_type = unsigned;
    
private:
    T* data = nullptr;
    size_type y_size = 0, x_size = 0;

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
            new (data + i) T(); // placement new для default initialization
        }
    }

    // Конструктор 3: создание сетки с заполнением копиями объекта t
    Grid(size_type y_size, size_type x_size, T const &t) : y_size(y_size), x_size(x_size) {
        data = new T[y_size * x_size];
        for (size_type i = 0; i < y_size * x_size; ++i) {
            data[i] = t;
        }
    }

    // DESTROY (#1)
    ~Grid() {
        if (data) {
            delete[] data;
        }
    }

    // Constructor copirovaniye (#2)
    Grid(Grid<T> const &other) : y_size(other.y_size), x_size(other.x_size) {
        data = new T[y_size * x_size];
        for (size_type i = 0; i < y_size * x_size; ++i) {
            data[i] = other.data[i];
        }
    }

    // Constructor peremescheniye (#3)
    Grid(Grid<T>&& other) noexcept 
        : data(other.data), y_size(other.y_size), x_size(other.x_size) {
        other.data = nullptr;
        other.y_size = 0;
        other.x_size = 0;
    }

    // copirovaniye #4
    Grid<T>& operator=(Grid<T> const &other) {
        if (this != &other) {
            // copy
            Grid<T> temp(other);
            swap(*this, temp);
        }
        return *this;
    }

    // peremescheniye #5
    Grid<T>& operator=(Grid<T>&& other) noexcept {
        if (this != &other) {
            if (data) {
                delete[] data;
            }
            // from other
            data = other.data;
            y_size = other.y_size;
            x_size = other.x_size;
            // other = 0
            other.data = nullptr;
            other.y_size = 0;
            other.x_size = 0;
        }
        return *this;
    }

    // swap
    friend void swap(Grid<T>& first, Grid<T>& second) noexcept {
        using std::swap;
        swap(first.data, second.data);
        swap(first.y_size, second.y_size);
        swap(first.x_size, second.x_size);
    }

    // access to elements
    T operator()(size_type y_idx, size_type x_idx) const {
        return data[y_idx * x_size + x_idx];
    }
    
    T& operator()(size_type y_idx, size_type x_idx) {
        return data[y_idx * x_size + x_idx];
    }

    // for all elements 
    Grid<T>& operator=(T const &t) {
        for (auto it = data, end = data + x_size * y_size; it != end; ++it) {
            *it = t;
        }
        return *this;
    }

    size_type get_y_size() const { return y_size; }
    size_type get_x_size() const { return x_size; }
};