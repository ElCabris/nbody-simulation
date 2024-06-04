#include "../../include/vectors_operations.hpp"

#include <cmath>

void operator+=(std::array<double, 2>& v1, const std::array<double , 2>& v2) {
    v1[0] += v2[0];
    v1[1] += v2[1];
}

std::array<double , 2> operator+(const std::array<double , 2>& v1, const std::array<double , 2>& v2) {
    return {v1[0] + v2[0], v1[1] + v2[1]};
}

std::array<double , 2> operator-(const std::array<double , 2>& v1, const std::array<double , 2>& v2) {
    return {v1[0] - v2[0], v1[1] - v2[1]};
}

std::array<double , 2> operator*(const double& n, const std::array<double , 2>& v) {
    return {n * v[0], n * v[1]};
}

double  norm_of_a_vector(const std::array<double , 2>& v) {
    return std::sqrt(v[0] * v[0] + v[1] * v[1]);
}