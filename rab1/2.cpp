#include <iostream>
#include <cassert>

template <typename T>
class Grid final {
public:
    using value_type = T;
    using size_type = unsigned;
private:
    T* data;
    size_type y_size, x_size;

public:
    // конструкторы задачи
    Grid(T const& t) : data(new T[1]), y_size(1), x_size(1) {
        data[0] = t;}
    Grid(size_type y_size, size_type x_size) : data(new T[y_size * x_size]), y_size(y_size), x_size(x_size) {
        for (size_type i = 0; i < y_size * x_size; ++i) {
            data[i] = T();}}
    Grid(size_type y_size, size_type x_size, T const& t) : data(new T[y_size * x_size]), y_size(y_size), x_size(x_size) {
        for (size_type i = 0; i < y_size * x_size; ++i) {
            data[i] = t;}}
    // правило пяти
    Grid(Grid const& other) : data(new T[other.y_size * other.x_size]), y_size(other.y_size), x_size(other.x_size) {
        for (size_type i = 0; i < y_size * x_size; ++i) {
            data[i] = other.data[i];}}
    Grid(Grid&& other) noexcept : data(other.data), y_size(other.y_size), x_size(other.x_size) {
        other.data = nullptr;
        other.y_size = 0;
        other.x_size = 0;}
    Grid& operator=(Grid const& other) {
        if (this != &other) {
            delete[] data;
            data = new T[other.y_size * other.x_size];
            y_size = other.y_size;
            x_size = other.x_size;
            for (size_type i = 0; i < y_size * x_size; ++i) {
                data[i] = other.data[i];
            }}return *this;}
    Grid& operator=(Grid&& other) noexcept {
        if (this != &other) {
            delete[] data;
            data = other.data;
            y_size = other.y_size;
            x_size = other.x_size;
            other.data = nullptr;
            other.y_size = 0;
            other.x_size = 0;
        }return *this;}
    ~Grid() {delete[] data;}
    T operator()(size_type y_idx, size_type x_idx) const {
        return data[y_idx * x_size + x_idx];}
    T& operator()(size_type y_idx, size_type x_idx) {
        return data[y_idx * x_size + x_idx];}


    size_type get_y_size() const {
        return y_size;}
    size_type get_x_size() const {
        return x_size;}


    struct Dp {
        T* data;
        size_type x_size;
        
        T& operator[](size_type x_idx) { return data[x_idx]; }
        T operator[](size_type x_idx) const { return data[x_idx]; }
    };

    Dp operator[](size_type y_idx) { return {data + y_idx * x_size, x_size}; }
    Dp operator[](size_type y_idx) const { return {data + y_idx * x_size, x_size}; }
};

int main() {
    Grid<float> g(3, 2, 0.0f);
    assert(3 == g.get_y_size()) ;
    assert(2 == g.get_x_size()) ;

    using gsize_t = typename Grid<int>::size_type;

    for (gsize_t y_idx = 0 ; y_idx != g.get_y_size(); ++y_idx)
        for (gsize_t x_idx = 0 ; x_idx != g.get_x_size(); ++x_idx)
            assert(0.0f == g[y_idx][x_idx]);

    for (gsize_t y_idx = 0 ; y_idx != g.get_y_size(); ++y_idx)
        for (gsize_t x_idx = 0 ; x_idx != g.get_x_size(); ++x_idx)
            g[y_idx][x_idx] = 1.0f;

    for (gsize_t y_idx = 0 ; y_idx != g.get_y_size(); ++y_idx)
        for (gsize_t x_idx = 0 ; x_idx != g.get_x_size(); ++x_idx)
            assert ( 1.0f == g(y_idx, x_idx));

    return 0;}




