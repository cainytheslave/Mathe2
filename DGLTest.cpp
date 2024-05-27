#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>
#include <cmath>
#include <iomanip>
#include <iostream>
#include "lib/CDGLSolver.h"

using namespace Catch::Matchers;

TEST_CASE("CDGLSolver can solve systems", "[CDGLSolver]") {
    CDGLSolver solver([](const CMyVector y, double x) {
        return CMyVector({
                2 * y.get(1) - x * y.get(0),
                y.get(0) * y.get(1) - 2 * pow(x, 3)
        });
    });

    //CMyVector yStart({0.0, 1.0});
    //CMyVector yEnd = solver.euler(0.0, 2.0, 100, yStart);
    
    //CMyVector yEndHeun = solver.heun(0.0, 2.0, 100, yStart);

    //REQUIRE_THAT(yEnd.get(0), WithinAbs(0.5440211108893698, 1e-6));
    //REQUIRE_THAT(yEnd.get(1), WithinAbs(-0.8390715290764524, 1e-6));
}

double dgl_nth_order(const CMyVector y, double x) {
    return 2 * x * y.get(1) * y.get(2) + 2 * y.get(0) * y.get(0) * y.get(1);
}

TEST_CASE("CDGLSolver can solve nth order equations", "[CDGLSolver]") {
    CDGLSolver solver(static_cast<std::function<double(const CMyVector y, double x)>>(dgl_nth_order));

    CMyVector yStart({1.0, -1.0, 2.0});

    double xStart = 1.0;
    double xEnd = 2.0;
    double exakt = 0.5;
     
    std::vector<int> schritte = {10, 100, 1000};
    
    std::cout << std::fixed << std::setprecision(8);

    for (int n : schritte) {
        SECTION("Euler-Verfahren mit " + std::to_string(n) + " Schritten") {
            CMyVector resultEuler = solver.euler(xStart, xEnd, n, yStart);
            double abweichungEuler = std::abs(resultEuler.get(0) - exakt);
            
            std::cout << "\n\nEuler mit n = " << n << std::endl;
            std::cout << "Berechneter Wert (Euler): " << resultEuler.get(0) << std::endl;
            std::cout << "Abweichung (Euler):       " << abweichungEuler << std::endl;
        }

        SECTION("Heun-Verfahren mit " + std::to_string(n) + " Schritten") {
            CMyVector resultHeun = solver.heun(xStart, xEnd, n, yStart);
            double abweichungHeun = std::abs(resultHeun.get(0) - exakt);
            
            std::cout << "\nHeun mit n = " << n << std::endl;
            std::cout << "Berechneter Wert (Heun):  " << resultHeun.get(0) << std::endl;
            std::cout << "Abweichung (Heun):        " << abweichungHeun << std::endl;
        }
    }
}
