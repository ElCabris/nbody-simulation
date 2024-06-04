#ifndef VECTORS_OPERATIONS_HPP
#define VECTORS_OPERATIONS_HPP
#include <array>
#include <iostream>

void operator+=(std::array<double , 2>&, const std::array<double , 2>&);
std::array<double , 2> operator+(const std::array<double , 2>&, const std::array<double , 2>&);
std::array<double , 2> operator-(const std::array<double , 2>&, const std::array<double , 2>&);
std::array<double , 2> operator*(const double &, const std::array<double , 2>&);
double  norm_of_a_vector(const std::array<double , 2>&);

#endif