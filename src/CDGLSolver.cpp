#include "../lib/CDGLSolver.h"
#include "../lib/Helper.h"
#include <iostream>

const bool CDGLSolver::DEBUG = false;

CDGLSolver::CDGLSolver(std::function<CMyVector(const CMyVector y, double x)> dgl)
    : dgl(dgl), is_system(true) {}

CDGLSolver::CDGLSolver(std::function<double(const CMyVector y, double x)> dgl)
    : dgl_nth_order(dgl), is_system(false) {}

CMyVector CDGLSolver::derivatives(const CMyVector y, double x) const {
    if (is_system) return dgl(y, x);

    int n = y.dimension();
    CMyVector result(n);

    for (int i = 0; i < n - 1; ++i) {
        result[i] = y.get(i + 1); // y_i' = y_(i+1)
    }
    
    result[n - 1] = dgl_nth_order(y, x);

    return result;
}

CMyVector CDGLSolver::euler(double xStart, double xEnd, int steps, const CMyVector yStart) const {
    auto unmute = Helper::muteOutput(!DEBUG);

    double h = (xEnd - xStart) / steps;
    CMyVector y = yStart;

    std::cout << "h = " << h << std::endl;

    for (int i = 0; i < steps; ++i) {
        std::cout << "\nSchritt " << i << ":" << std::endl;
        std::cout << "\tx = " << xStart + i * h << std::endl;
        std::cout << "\ty = " << y.to_string() << std::endl;
        std::cout << "\ty' = " << derivatives(y, xStart + i * h).to_string() << std::endl;

        y = y + derivatives(y, xStart + i * h) * h;
    }
    
    std::cout << "\nEnde bei" << std::endl;
    std::cout << "\tx = " << xEnd << std::endl;
    std::cout << "\ty = " << y.to_string() << std::endl;
    
    unmute(); 
    return y;
}

CMyVector CDGLSolver::heun(double xStart, double xEnd, int steps, const CMyVector yStart) const {
    auto unmute = Helper::muteOutput(!DEBUG);

    double h = (xEnd - xStart) / steps;
    CMyVector y = yStart;

    std::cout << "h = " << h << std::endl;
    
    for (int i = 0; i < steps; ++i) {
        double x = xStart + i * h;

        std::cout << "\nSchritt " << i << ":" << std::endl;
        std::cout << "\tx = " << x << std::endl;
        std::cout << "\ty = " << y.to_string() << std::endl;

        CMyVector d_start = derivatives(y, x);
        CMyVector y_test = y + d_start * h;
        CMyVector d_end = derivatives(y_test, x + h);

        std::cout << "\ty'_orig = " << d_start.to_string() << std::endl;

        std::cout << "\n\ty_test = " << y_test.to_string() << std::endl;
        std::cout << "\ty'_test = " << d_end.to_string() << std::endl;

        CMyVector y_mittel = (d_start + d_end) * 0.5;

        std::cout << "\n\ty'_mittel = " << y_mittel.to_string() << std::endl;

        y = y + y_mittel * h;
    }
    
    std::cout << "\nEnde bei" << std::endl;
    std::cout << "\tx = " << xEnd << std::endl;
    std::cout << "\ty = " << y.to_string() << std::endl;

    unmute();
    return y;
}
