#include <initializer_list>
#pragma once

#include <vector>
#include <string>
#include <functional>

/**
 * Stores n-dimensional vectors.
**/
class CMyVector {
private:
    std::vector<double> m_data;
    static const int MAX_STEPS;
    static const double MAX_ERROR;
    static const bool DEBUG;
public:
    CMyVector(int dimension);
    CMyVector(std::initializer_list<double> values);
    CMyVector(std::vector<double> values);
    ~CMyVector();
    int dimension() const;
    double& operator[](int index);
    double get(int index) const;
    CMyVector operator+(const CMyVector& other) const;
    CMyVector operator-(const CMyVector& other) const;
    CMyVector operator-() const;
    CMyVector operator*(double scalar) const;
    CMyVector operator*(const CMyVector& other) const;
    bool operator==(const CMyVector& other) const;
    bool operator!=(const CMyVector& other) const;
    double magnitude() const;
    CMyVector normalize() const;
    static CMyVector gradient(const CMyVector& x, std::function<double(CMyVector)> f, double h = 1e-10);
    static CMyVector minimize(const CMyVector& x, std::function<double(CMyVector)> f, double lambda = 1.0, double h = 1e-10);
    static CMyVector maximize(const CMyVector& x, std::function<double(CMyVector)> f, double lambda = 1.0, double h = 1e-10);
    static std::function<double(double)> polynomial(CMyVector coefficients);
    static CMyVector curveFit(std::vector<CMyVector> points, int degree);
    std::string to_string() const;
};
