#include <cstddef>  
#include <utility> 
#include <iostream>
#include <tuple>    

// numero 1
template<typename T, typename U>
struct is_same {
    static constexpr bool value = false;
};

template<typename T>
struct is_same<T, T> {
    static constexpr bool value = true;
};

// numero 2
template<typename... Types>
struct are_same;

template<typename T>
struct are_same<T> {
    static constexpr bool value = true;
};


template<typename T, typename U>
struct are_same<T, U> {
    static constexpr bool value = is_same<T, U>::value;
};


template<typename T, typename U, typename V, typename... Rest>
struct are_same<T, U, V, Rest...> {
    static constexpr bool value = is_same<T, U>::value && is_same<T, V>::value && are_same<T, Rest...>::value;
};

// numero 3
template<typename T, std::size_t N>
class Array {
private:
    T data[N];
public:
    T* begin() { return data; }
    const T* begin() const { return data; }
    T* end() { return data + N; }
    const T* end() const { return data + N; }
    
    T& operator[](std::size_t index) { return data[index]; }
    const T& operator[](std::size_t index) const { return data[index]; }
};

template<typename T, std::size_t N>
constexpr std::size_t size(const Array<T, N>&) {
    return N;
}

// numero 4
template<typename Func, typename... Args>
auto invoke(Func&& func, Args&&... args) 
    -> decltype(std::forward<Func>(func)(std::forward<Args>(args)...)) {
    return std::forward<Func>(func)(std::forward<Args>(args)...);
}

// numero 5
#include <iostream>
#include <type_traits>

template<typename T>
struct is_array : std::false_type {};

template<typename T, std::size_t N>
struct is_array<Array<T, N>> : std::true_type {};

template<typename T>
constexpr bool is_array_v = is_array<T>::value;

template<typename T>
void flatten(const T& container, std::ostream& out);

template<typename T, std::size_t N>
void flatten(const Array<T, N>& container, std::ostream& out) {
    for (const auto& elem : container) {
        flatten(elem, out); 
    }
}

template<typename T>
std::enable_if_t<!is_array_v<T>> flatten(const T& elem, std::ostream& out) {
    out << elem << " ";
}