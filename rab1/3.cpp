#include <iostream>
#include <cassert>

template <typename T, unsigned int N>
class Grid final {
public:
    using value_type = T;
    using size_type = unsigned;

private:
    T* data;
    size_type sizes[N];

public:
    Grid(const T& t) : data(new T[1]) {
        data[0] = t;
        for (unsigned int i = 0; i < N; ++i) {
            sizes[i] = 1;
        }
    }

    Grid(size_type size1, size_type size2, const T& t) : data(new T[size1 * size2]) {
        sizes[0] = size1;
        sizes[1] = size2;

        for (size_type i = 0; i < size1 * size2; ++i) {
            data[i] = t;
        }
    }

    template<typename... Args>
    Grid(size_type size1, Args... args, const T& t) : data(new T[size1 * get_sizes(args...)]) {
        sizes[0] = size1;
        set_sizes(1, args...);

        size_type total_size = size1 * get_sizes(args...);

        for (size_type i = 0; i < total_size; ++i) {
            data[i] = t;
        }
    }

    template<typename... Args>
    size_type get_sizes(size_type size1, Args... args) const {
        return size1 * get_sizes(args...);
    }

    size_type get_sizes() const {
        return 1;
    }

    void set_sizes(size_type) {}

    template<typename... Args>
    void set_sizes(size_type size1, Args... args) {
        sizes[N - sizeof...(Args) - 1] = size1;
        set_sizes(args...);
    }

    Grid(const Grid& other) : data(new T[get_total_size(other)]), sizes{ other.sizes } {
        std::copy(other.data, other.data + get_total_size(other), data);
    }

    Grid(Grid&& other) noexcept : data(other.data), sizes{ other.sizes } {
        other.data = nullptr;
        std::fill(std::begin(other.sizes), std::end(other.sizes), 0);
    }

    Grid& operator=(const Grid& other) {
        if (this != &other) {
            delete[] data;
            data = new T[get_total_size(other)];
            std::copy(other.data, other.data + get_total_size(other), data);
            sizes = other.sizes;
        }
        return *this;
    }

    Grid& operator=(Grid&& other) noexcept {
        if (this != &other) {
            delete[] data;
            data = other.data;
            sizes = other.sizes;
            other.data = nullptr;
            std::fill(std::begin(other.sizes), std::end(other.sizes), 0);
        }
        return *this;
    }

    ~Grid() {
        delete[] data;
    }

    template<typename... Args>
    T& operator()(size_type idx, Args... args) {
        return data[get_index(idx, args...)];
    }

    template<typename... Args>
    T operator()(size_type idx, Args... args) const {
        return data[get_index(idx, args...)];
    }

    template<typename... Args>
    size_type get_index(size_type idx, Args... args) const {
        assert(N == sizeof...(Args) + 1);

        size_type indices[] = { idx, static_cast<size_type>(args)... };
        size_type index = indices[0];

        for (size_type i = 1; i < N; ++i) {
            assert(indices[i] < sizes[i]);

            size_type dim_size = get_dimension_size(i);

            index = index * dim_size + indices[i];
        }

        return index;
    }

    size_type get_total_size(const Grid& other) const {
        size_type total_size = 1;
        for (size_type i = 0; i < N; ++i) {
            total_size *= other.sizes[i];
        }
        return total_size;
    }

    size_type get_dimension_size(size_type dim) const {
        assert(dim < N);
        return sizes[dim];
    }
};

int main() {
    Grid<float, 3> const g3(2, 3, 4, 1.0f);
    assert(1.0f == g3(1, 1, 1));

    Grid<float, 2> g2(2, 5, 2.0f);
    assert(2.0f == g2(1, 1));

    g2 = g3[1];
    assert(1.0f == g2(1, 1));

    return 0;
}