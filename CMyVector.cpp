#include "CMyVector.h"
#include <cmath>
#include <stdexcept>
#include <iostream>
#include <string>
#include <functional>

int main() {
    std::function<double(CMyVector)> f = [](CMyVector x) {
        return sin(x[0]*x[1]) + sin(x[0]) + cos(x[1]);
    };

    CMyVector f_start = {0.2, -2.1};

    std::function<double(CMyVector)> g = [](CMyVector x) {
        return -(2*x[0]*x[0] - 2*x[0]*x[1] + x[1]*x[1] + x[2]*x[2] - 2*x[0] - 4*x[2]);
    };

    CMyVector g_start = {0.0, 0.0, 0.0}; 

    //std::cout << f_start.maximize(f).to_string() << std::endl;
    //std::cout << g_start.maximize(g, 0.1).to_string() << std::endl;
    
    std::cout << CMyVector::curveFit({{-1.5, -1.9}, {-1, -0.6}, {-0.5, -0.4}, {0, 0.3}, {0.5, 0}, {1, 0.6}, {1.5, -0.1}}, 2).to_string() << std::endl;

    return 0;
}

const int CMyVector::MAX_STEPS = 25;
const double CMyVector::MAX_ERROR = 1e-5;

CMyVector::CMyVector(int dimension) : m_data(dimension) {}

CMyVector::CMyVector(std::initializer_list<double> values) : m_data(values) {}

CMyVector::~CMyVector() {
    m_data.clear();
}

int CMyVector::dimension() const {
    return m_data.size();
}

CMyVector CMyVector::operator+(const CMyVector& other) const {
    if(dimension() != other.dimension()) {
        throw std::invalid_argument("Vectors must have the same dimension.");
    }

    CMyVector result(m_data.size());
    for (int i = 0; i < m_data.size(); i++) {
        result[i] = m_data[i] + other.m_data[i];
    }

    return result;
}

CMyVector CMyVector::operator-(const CMyVector& other) const {
    if(dimension() != other.dimension()) {
        throw std::invalid_argument("Vectors must have the same dimension.");
    }

    CMyVector result(m_data.size());
    for (int i = 0; i < m_data.size(); i++) {
        result[i] = m_data[i] - other.m_data[i];
    }

    return result;
}

CMyVector CMyVector::operator-() const {
    return this->operator*(-1);
}

CMyVector CMyVector::operator*(double scalar) const {
    CMyVector result(m_data.size());
    for (int i = 0; i < m_data.size(); i++) {
        result[i] = m_data[i] * scalar;
    }

    return result;
}

CMyVector CMyVector::operator*(CMyVector& other) const {
    if(dimension() != other.dimension()) {
        throw std::invalid_argument("Vectors must have the same dimension.");
    }

    CMyVector result(m_data.size());
    for (int i = 0; i < m_data.size(); i++) {
        result[i] = m_data[i] * other.m_data[i];
    }

    return result;
}

double CMyVector::magnitude() const {
    double sum = 0;

    for(double value : m_data) {
        sum += std::pow(value, 2);
    }

    return std::sqrt(sum);
}

CMyVector CMyVector::normalize() const {
    double mag = magnitude();

    if(std::abs(mag) < 1e-12) {
        return CMyVector(dimension());
    }

    return *this * (1 / mag);
}

double& CMyVector::operator[](int index) {
    if(index < 0 || index >= m_data.size()) {
        throw std::out_of_range("Index out of range.");
    }

    return m_data[index];
}

double CMyVector::get(int index) const {
    if(index < 0 || index >= dimension()) {
        throw std::out_of_range("Index out of range.");
    }

    return m_data[index];
}

CMyVector CMyVector::gradient(std::function<double(CMyVector)> f, double h) const {
    CMyVector result(dimension());

    for (int i = 0; i < dimension(); i++) {
        CMyVector hVector(dimension());
        hVector[i] = h;

        result[i] = (f(*this + hVector) - f(*this)) / h;
    }

    return result;
}

CMyVector CMyVector::minimize(std::function<double(CMyVector)> f, double lambda, double h) const {
    return maximize([f](CMyVector x) { return -f(x); }, lambda, h);
}

CMyVector CMyVector::maximize(std::function<double(CMyVector)> f, double lambda, double h) const {
    CMyVector current_pos = CMyVector(*this);
    double step_size = lambda;
    int step = 0;

    while(true){
        CMyVector gradient = current_pos.gradient(f, h);
        CMyVector new_pos = current_pos + (gradient * step_size);
        
        std::cout << std::endl;
        
        if(gradient.magnitude() < MAX_ERROR || step >= MAX_STEPS) {
            if(gradient.magnitude() < MAX_ERROR)
                std::cout << "Ende wegen ||grad f(x)|| < " << MAX_ERROR << " bei" << std::endl;
            else
                std::cout << "Ende wegen Schrittanzahl = " << MAX_STEPS << " bei" << std::endl;
            std::cout << "\tx = " << current_pos.to_string() << std::endl;
            std::cout << "\tlambda = " << step_size << std::endl;
            std::cout << "\tf(x) = " << f(current_pos) << std::endl;
            std::cout << "\tgrad f(x) = " << gradient.to_string() << std::endl;
            std::cout << "\t||grad f(x)|| = " << gradient.magnitude() << std::endl;
            return current_pos;
        }
        
        std::cout << "Schritt " << step << ":" << std::endl;
        std::cout << "\tx = " << current_pos.to_string() << std::endl;
        std::cout << "\tlambda = " << step_size << std::endl;

        std::cout << "\tf(x) = " << f(current_pos) << std::endl;
        std::cout << "\tgrad f(x) = " << gradient.to_string() << std::endl;
        std::cout << "\t||grad f(x)|| = " << gradient.magnitude() << std::endl;
        
        std::cout << std::endl;
        
        std::cout << "\tx_neu = " << new_pos.to_string() << std::endl;
        std::cout << "\tf(x_neu) = " << f(new_pos) << std::endl;
        

        if(f(new_pos) > f(current_pos)) {
            double test_step_size = step_size * 2.0;
            
            std::cout << std::endl;
            std::cout << "\tTest mit doppelter Schrittweite (lambda = " << test_step_size << ")" << std::endl;

            CMyVector test_new_pos = current_pos + (gradient * test_step_size);

            std::cout << "\tx_test = " << test_new_pos.to_string() << std::endl;
            std::cout << "\tf(x_test) = " << f(test_new_pos) << std::endl;

            if(f(test_new_pos) > f(new_pos)) {
                std::cout << "\tverdopple Schrittweite" << std::endl;
                current_pos = test_new_pos;
                step_size = test_step_size;
            }else{
                std::cout << "\tbehalte alte Schrittweite!" << std::endl;
                current_pos = new_pos;
            }
        } else {
            while(f(new_pos) < f(current_pos)) {
                step_size /= 2.0;
                
                new_pos = current_pos + (gradient * step_size);

                std::cout << std::endl;
                std::cout << "\thalbiere Schrittweite (lambda = " << step_size << "):" << std::endl;
                std::cout << "\tx_neu = " << new_pos.to_string() << std::endl;
                std::cout << "\tf(x_neu) = " << f(new_pos) << std::endl;
            }

            current_pos = new_pos;
        }

        step++;
    }

    return current_pos;
}

std::function<double(double)> CMyVector::polynomial(CMyVector coefficients) {
    return [coefficients](double x) {
        double result = 0;

        for (int i = 0; i < coefficients.dimension(); i++) {
            result += coefficients.get(i) * std::pow(x, coefficients.dimension() - i - 1);
        }
        
        return result;
    };
}

CMyVector CMyVector::curveFit(std::vector<CMyVector> points, int degree) {
    CMyVector result(degree + 1);

    std::function<double(CMyVector)> error = [degree, points](CMyVector coefficients) {
        std::function<double(double)> f = polynomial(coefficients);

        double deviation = 0;

        for (const CMyVector& point : points) {
            deviation += std::pow(f(point.get(0)) - point.get(1), 2);
        } 

        return deviation;
    };

    return result.minimize(error, 0.1); 
}

std::string CMyVector::to_string() const {
    std::string result = "(";
    for (int i = 0; i < m_data.size(); i++) {
        result += std::to_string(m_data[i]);
        if(i < m_data.size() - 1) {
            result += ", ";
        }
    }
    result += ")";

    return result;
}
