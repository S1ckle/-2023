#include <iostream>
#include <functional>
#include <initializer_list>

int main() {
    std::function<double(const std::initializer_list<double>&)> lambda_sum = [&](const std::initializer_list<double>& list) {
        int size = list.size();
        double* arr = new double[size];

        int i = 0;
        for (auto element : list) {
            arr[i++] = element;
        }

        std::function<double(int, double*)> recursive_sum = [&](int index, double* arr) -> double {
            if (index >= size) {
                return 0;
            } else {
                return arr[index] + recursive_sum(index + 1, arr);
            }
        };

        double sum = recursive_sum(0, arr);
        delete[] arr;
        return sum;
    };
    std::cout << "lambda_sum: " << lambda_sum({2, 3, 4, 5, 5.5, 7.5, 9.0, -12.5}) << std::endl;

    return 0;
}