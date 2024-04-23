#include <initializer_list>
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
public:
    CMyVector(int dimension);
    CMyVector(std::initializer_list<double> values);
    ~CMyVector();
    int dimension() const;
    double& operator[](int index);
    double get(int index) const;
    CMyVector operator+(const CMyVector& other) const;
    CMyVector operator-(const CMyVector& other) const;
    CMyVector operator-() const;
    CMyVector operator*(double scalar) const;
    CMyVector operator*(CMyVector& other) const;
    double magnitude() const;
    CMyVector normalize() const;
    CMyVector gradient(std::function<double(CMyVector)> f, double h = 1e-10) const;
    CMyVector minimize(std::function<double(CMyVector)> f, double lambda = 1.0, double h = 1e-10) const;
    CMyVector maximize(std::function<double(CMyVector)> f, double lambda = 1.0, double h = 1e-10) const;
    static std::function<double(double)> polynomial(CMyVector coefficients);
    static CMyVector curveFit(std::vector<CMyVector> points, int degree);
    std::string to_string() const;
};
