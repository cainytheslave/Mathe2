#include "../lib/CComplex.h"
#include <cmath>
#include <iostream>

const bool CComplex::DEBUG = false;

CComplex::CComplex() : m_re(0), m_im(0) {}

CComplex::CComplex(double re, double im) : m_re(re), m_im(im) {}

CComplex::CComplex(double phi) : m_re(cos(phi)), m_im(sin(phi)) {}

CComplex::CComplex(const CComplex& c) : m_re(c.m_re), m_im(c.m_im) {}

CComplex::~CComplex() {}

double CComplex::re() const {
    return m_re;
}

double CComplex::im() const {
    return m_im;
}

double CComplex::abs() const {
    return sqrt(absSq());
}

double CComplex::absSq() const {
    return m_re * m_re + m_im * m_im;
}

CComplex& CComplex::operator=(const CComplex& c) {
    m_re = c.m_re;
    m_im = c.m_im;
    return *this;
}

CComplex& CComplex::operator+=(const CComplex& c) {
    m_re += c.m_re;
    m_im += c.m_im;
    return *this;
}

CComplex& CComplex::operator-=(const CComplex& c) {
    m_re -= c.m_re;
    m_im -= c.m_im;
    return *this;
}

CComplex& CComplex::operator*=(const CComplex& c) {
    double re = m_re * c.m_re - m_im * c.m_im;
    double im = m_re * c.m_im + m_im * c.m_re;
    m_re = re;
    m_im = im;
    return *this;
}

CComplex& CComplex::operator/=(const CComplex& c) {
    double re = (m_re * c.m_re + m_im * c.m_im) / c.absSq();
    double im = (m_im * c.m_re - m_re * c.m_im) / c.absSq();
    m_re = re;
    m_im = im;
    return *this;
}

CComplex& CComplex::operator*=(double d) {
    m_re *= d;
    m_im *= d;
    return *this;
}

CComplex& CComplex::operator/=(double d) {
    m_re /= d;
    m_im /= d;
    return *this;
}

CComplex CComplex::operator+(const CComplex& c) const {
    return CComplex(*this) += c;
}

CComplex CComplex::operator-(const CComplex& c) const {
    return CComplex(*this) -= c;
}

CComplex CComplex::operator*(const CComplex& c) const {
    return CComplex(*this) *= c;
}

CComplex CComplex::operator/(const CComplex& c) const {
    return CComplex(*this) /= c;
}

CComplex CComplex::operator*(double d) const {
    return CComplex(*this) *= d;
}

CComplex CComplex::operator/(double d) const {
    return CComplex(*this) /= d;
}

bool CComplex::operator==(const CComplex& c) const {
    return m_re == c.m_re && m_im == c.m_im;
}

bool CComplex::operator!=(const CComplex& c) const {
    return !(*this == c);
}

std::string CComplex::to_string() const {
    return std::to_string(m_re) + " + " + std::to_string(m_im) + "j";
}

std::vector<CComplex> CComplex::dft(const std::vector<CComplex>& values) {
    int N = values.size();
    std::vector<CComplex> result(N);
    for (int k = 0; k < N; k++) {
        CComplex sum;
        for (int n = 0; n < N; n++) {
            sum += values[n] * CComplex(-2 * M_PI * n * k / N);
        }
        result[k] = sum / sqrt(N);
    }
    return result;
}

std::vector<CComplex> CComplex::idft(const std::vector<CComplex>& values) {
    int N = values.size();
    std::vector<CComplex> result(N);
    for (int n = 0; n < N; n++) {
        CComplex sum;
        for (int k = 0; k < N; k++) {
            sum += values[k] * CComplex(2 * M_PI * n * k / N);
        }
        result[n] = sum / sqrt(N);
    }
    return result;
}

std::vector<CComplex> CComplex::fft(const std::vector<CComplex>& values, bool inverse) {
    auto result = fftRecursive(values, inverse);

    double scale = 1.0 / std::sqrt(result.size());

    for(auto &c : result) {
        c *= scale;
    }

    return result;
}

std::vector<CComplex> CComplex::fftRecursive(const std::vector<CComplex>& values, bool inverse) {
    int N = values.size();
    if (N == 1) return values;

    std::vector<CComplex> even(N / 2), odd(N / 2);
    for (int i = 0; i < N / 2; i++) {
        even[i] = values[2 * i];
        odd[i] = values[2 * i + 1];
    }

    even = fftRecursive(even, inverse);
    odd = fftRecursive(odd, inverse);

    double factor = (inverse ? 2.0 : -2.0) * M_PI / N;

    std::vector<CComplex> result(N);
    for (int k = 0; k < N / 2; k++) {
        CComplex t = CComplex(factor * k) * odd[k];

        result[k] = even[k] + t;
        result[k + N / 2] = even[k] - t;
    }

    return result;
}
