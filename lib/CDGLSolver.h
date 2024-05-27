#pragma once

#include "CMyVector.h"
#include <functional>

class CDGLSolver {
private:
    static const bool DEBUG; 
    std::function<CMyVector(CMyVector y, double x)> dgl;
    std::function<double(CMyVector y, double x)> dgl_nth_order;
    CMyVector derivatives(const CMyVector y, double x) const;
    bool is_system;

public:
    CDGLSolver(std::function<CMyVector(const CMyVector y, double x)> dgl);
    CDGLSolver(std::function<double(const CMyVector y, double x)> dgl);
    CMyVector euler(double xStart, double xEnd, int steps, const CMyVector yStart) const;
    CMyVector heun(double xStart, double xEnd, int steps, const CMyVector yStart) const; 
};

