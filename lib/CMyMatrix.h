#pragma once

#include <initializer_list>
#include <vector>
#include <string>
#include "CMyVector.h"

/*
 * Stores n*m-dimensional matrices.
*/
class CMyMatrix {
private:
    std::vector<std::vector<double>> m_data;
    static const int NEWTON_MAX_STEPS;
    static const double NEWTON_MAX_ERROR;
    static const bool DEBUG;
public:
    CMyMatrix(int rows, int columns);
    CMyMatrix(std::initializer_list<CMyVector> values);
    ~CMyMatrix();
    std::tuple<int, int> dimensions() const;
    double get(int row, int column) const;
    CMyVector row(int index) const;
    CMyVector column(int index) const;
    void set(int row, int column, double value);
    CMyMatrix operator+(const CMyMatrix& other) const;
    CMyMatrix operator-(const CMyMatrix& other) const;
    CMyMatrix operator-() const;
    CMyMatrix operator*(const double scalar) const;
    CMyVector operator*(const CMyVector& other) const;
    CMyMatrix operator*(const CMyMatrix& other) const;
    CMyMatrix transpose() const;
    double determinant() const;
    CMyMatrix inverse() const;
    std::string to_string(std::string title = "") const;
    static CMyMatrix jacobi(const CMyVector& x, std::function<CMyVector(CMyVector)> f, double h = 1e-4);
    static CMyVector newton(const CMyVector& x, std::function<CMyVector(CMyVector)> f, double h = 1e-4);
};
