#pragma once

#include <string>
#include <vector>
class CComplex {
private:
    static const bool DEBUG;
    static std::vector<CComplex> fftRecursive(const std::vector<CComplex>& values, bool inverse);
    double m_re;
    double m_im;

public:
    CComplex();
    CComplex(double re, double im);
    CComplex(double phi);
    CComplex(const CComplex& c);
    ~CComplex();

    double re() const;
    double im() const;
    double abs() const;
    double absSq() const;

    CComplex& operator=(const CComplex& c);
    CComplex& operator+=(const CComplex& c);
    CComplex& operator-=(const CComplex& c);
    CComplex& operator*=(const CComplex& c);
    CComplex& operator/=(const CComplex& c);
    CComplex& operator*=(double d);
    CComplex& operator/=(double d);

    CComplex operator+(const CComplex& c) const;
    CComplex operator-(const CComplex& c) const;
    CComplex operator*(const CComplex& c) const;
    CComplex operator/(const CComplex& c) const;
    CComplex operator*(double d) const;
    CComplex operator/(double d) const;

    bool operator==(const CComplex& c) const;
    bool operator!=(const CComplex& c) const;

    std::string to_string() const;

    static std::vector<CComplex> dft(const std::vector<CComplex>& values);
    static std::vector<CComplex> idft(const std::vector<CComplex>& values);
    static std::vector<CComplex> fft(const std::vector<CComplex>& values, bool inverse = false);
};
