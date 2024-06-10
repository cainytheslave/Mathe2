#include "../lib/CMyMatrix.h"
#include "../lib/Helper.h"
#include <cmath>
#include <stdexcept>
#include <iostream>

const int CMyMatrix::NEWTON_MAX_STEPS = 50;
const double CMyMatrix::NEWTON_MAX_ERROR = 1e-5;
const bool CMyMatrix::DEBUG = false;

CMyMatrix::CMyMatrix(int rows, int columns) {
    m_data = std::vector<std::vector<double>>(rows, std::vector<double>(columns, 0.0));
}

CMyMatrix::CMyMatrix(std::initializer_list<CMyVector> values) {
    m_data = std::vector<std::vector<double>>(values.size());
    int i = 0;
    for (auto it = values.begin(); it != values.end(); it++) {
        for(int j = 0; j < it->dimension(); j++) {
            m_data[i].push_back(it->get(j));
        }
        i++;
    }
}

CMyMatrix::~CMyMatrix() {
    m_data.clear();
}

std::tuple<int, int> CMyMatrix::dimensions() const {
    return std::make_tuple(m_data.size(), m_data[0].size());
}

double CMyMatrix::get(int row, int column) const {
    auto [rows, columns] = dimensions();

    if(row < 0 || row >= rows || column < 0 || column >= columns) {
        throw std::invalid_argument("Index out of bounds.");
    }

    return m_data[row][column];
}

CMyVector CMyMatrix::row(int index) const {
    return CMyVector(m_data[index]);
}

CMyVector CMyMatrix::column(int index) const {
    std::vector<double> column_data;

    for (int i = 0; i < m_data.size(); i++) {
        column_data.push_back(m_data[i][index]);
    }

    return CMyVector(column_data);
}

void CMyMatrix::set(int row, int column, double value) {
    m_data[row][column] = value;
}

CMyMatrix CMyMatrix::operator+(const CMyMatrix& other) const {
    auto [rows, columns] = dimensions();
    auto [other_rows, other_columns] = other.dimensions();

    if(rows != other_rows || columns != other_columns) {
        throw std::invalid_argument("Matrices must have the same dimensions.");
    }

    CMyMatrix result(rows, columns);

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < columns; j++) {
            result.set(i, j, m_data[i][j] + other.m_data[i][j]);
        }
    }

    return result;
}

CMyMatrix CMyMatrix::operator-(const CMyMatrix& other) const {
    return *this + (-other);
}

CMyMatrix CMyMatrix::operator-() const {
    auto [rows, columns] = dimensions();
    CMyMatrix result(rows, columns);

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < columns; j++) {
            result.set(i, j, -m_data[i][j]);
        }
    }

    return result;
}

CMyMatrix CMyMatrix::operator*(const double scalar) const {
    auto [rows, columns] = dimensions();
    CMyMatrix result(rows, columns);

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < columns; j++) {
            result.set(i, j, m_data[i][j] * scalar);
        }
    }

    return result;
}

CMyVector CMyMatrix::operator*(const CMyVector& other) const {
    auto [rows, columns] = dimensions();
    if(columns != other.dimension()) {
        throw std::invalid_argument("Matrix columns must match vector dimension.");
    }

    std::vector<double> result_data(rows, 0.0);

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < columns; j++) {
            result_data[i] += m_data[i][j] * other.get(j);
        }
    }

    return CMyVector(result_data);
}

CMyMatrix CMyMatrix::operator*(const CMyMatrix& other) const {
    auto [rows, columns] = dimensions();
    auto [other_rows, other_columns] = other.dimensions();

    if(columns != other_rows) {
        throw std::invalid_argument("Matrix columns must match other matrix rows.");
    }

    CMyMatrix result(rows, other_columns);

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < other_columns; j++) {
            for (int k = 0; k < columns; k++) {
                result.set(i, j, result.get(i, j) + m_data[i][k] * other.get(k, j));
            }
        }
    }

    return result;
}

CMyMatrix CMyMatrix::transpose() const {
    auto [rows, columns] = dimensions();
    CMyMatrix result(columns, rows);

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < columns; j++) {
            result.set(j, i, m_data[i][j]);
        }
    }

    return result;
}

double CMyMatrix::determinant() const {
    auto [rows, columns] = dimensions();
    if(rows != columns) {
        throw std::invalid_argument("Matrix must be square.");
    }

    if(rows == 1) {
        return m_data[0][0];
    }

    if(rows == 2) {
        return m_data[0][0] * m_data[1][1] - m_data[0][1] * m_data[1][0];
    }

    double result = 0.0;
    for (int i = 0; i < rows; i++) {
        CMyMatrix submatrix(rows - 1, columns - 1);
        for (int j = 1; j < rows; j++) {
            for (int k = 0; k < columns; k++) {
                if(k < i) {
                    submatrix.set(j - 1, k, m_data[j][k]);
                } else if(k > i) {
                    submatrix.set(j - 1, k - 1, m_data[j][k]);
                }
            }
        }
        result += m_data[0][i] * submatrix.determinant() * (i % 2 == 0 ? 1 : -1);
    }

    return result;
}

CMyMatrix CMyMatrix::inverse() const {
    auto [rows, columns] = dimensions();
    if(rows != 2 || columns != 2) {
        throw std::invalid_argument("Matrix must be 2x2.");
    }

    double det = determinant();
    if(std::abs(det) < 1e-10) {
        throw std::invalid_argument("Matrix is singular.");
    }

    CMyMatrix result = {{m_data[1][1], -m_data[0][1]},
                        {-m_data[1][0], m_data[0][0]}};

    return result * (1 / det);
}

std::string CMyMatrix::to_string(std::string title) const {
    std::string output;
    size_t pos = 0;
    size_t last_pos = 0;

    while ((pos = title.find('\t', last_pos)) != std::string::npos) {
        output += title.substr(last_pos, pos - last_pos);
        output += std::string(8, ' ');
        last_pos = pos + 1;
    }

    output += title.substr(last_pos);

    std::string result = title;
    std::string filler = std::string(output.size(), ' ');

    for (int i = 0; i < m_data.size(); i++) {
        for (int j = 0; j < m_data[i].size(); j++) {
            result += std::to_string(m_data[i][j]) + " ";
        }
        if(i < m_data.size() - 1)
            result += "\n" + filler;
        else 
            result += "\n";
    }
    return result;
}

CMyMatrix CMyMatrix::jacobi(const CMyVector& x, std::function<CMyVector(CMyVector)> f, double h) {
    int f_dims = f(x).dimension();

    std::vector<std::function <double(CMyVector)>> partials;
    
    for (int i = 0; i < f_dims; i++) {
        partials.push_back([i, &f](CMyVector x) {
                return f(x).get(i);
        });
    }

    CMyMatrix result(f_dims, x.dimension());

    for (int i = 0; i < f_dims; i++) {
        CMyVector gradient = CMyVector::gradient(x, partials[i], h);
        for (int j = 0; j < x.dimension(); j++) {
            result.set(i, j, gradient.get(j));
        }
    }

    return result;
}

CMyVector CMyMatrix::newton(const CMyVector& x, std::function<CMyVector(CMyVector)> f, double h) {
    auto unmute = Helper::muteOutput(!DEBUG);

    CMyVector current_pos = CMyVector(x);

    for (int i = 0; i < NEWTON_MAX_STEPS; i++) {
        CMyMatrix jacobiMatrix = jacobi(current_pos, f, h);
        CMyVector f_x = f(current_pos);
        CMyMatrix inverse = jacobiMatrix.inverse();
        CMyVector step = inverse * f_x;

        
        if(f_x.magnitude() < NEWTON_MAX_ERROR) {
            std::cout << "\nEnde wegen ||f(x)|| < " << NEWTON_MAX_ERROR << " bei" << std::endl;
            std::cout << "\tx = " << current_pos.to_string() << std::endl;
            std::cout << "\tf(x) = " << f_x.to_string() << std::endl;
            std::cout << "\t||f(x)|| = " << f_x.magnitude() << std::endl << std::endl;
            unmute();
            return current_pos;
        }else{
            std::cout << "\nSchritt " << i << ":" << std::endl;
            std::cout << "\tx = " << current_pos.to_string() << std::endl;
            std::cout << "\tf(x) = " << f_x.to_string() << std::endl;
            std::cout << jacobiMatrix.to_string("\tf'(x) = ") << std::endl;
            std::cout << inverse.to_string("\tf'(x)^(-1) = ") << std::endl;
            std::cout << "\tdx = " << step.to_string() << std::endl;
            std::cout << "\t||f(x)|| = " << f_x.magnitude() << std::endl;
            
            current_pos = current_pos - step;
        }
    }
    
    CMyMatrix jacobiMatrix = jacobi(current_pos, f, h);
    CMyVector f_x = f(current_pos);
    CMyVector step = jacobiMatrix.inverse() * f_x;

    std::cout << "\nEnde wegen Schritt = " << NEWTON_MAX_STEPS << " bei" << std::endl;
    std::cout << "\tx = " << current_pos.to_string() << std::endl;
    std::cout << "\tf(x) = " << f_x.to_string() << std::endl;
    std::cout << "\t||f(x)|| = " << f_x.magnitude() << std::endl << std::endl;
    unmute();
    return current_pos;
}
