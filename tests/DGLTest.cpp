#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>
#include <cmath>
#include <iomanip>
#include <iostream>
#include "../lib/CDGLSolver.h"

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

TEST_CASE("DGLSolver can be used to minimize", "[CDGLSolver]") {
    std::vector<double> target = {0.015, 0.027, 0.059, 0.112, 0.209, 0.350, 0.523};

    auto equation = [](const CMyVector y, double x, double lambda) {
        CMyVector dy(1);
        dy[0] = lambda * y.get(0) * (1 - y.get(0));
        return dy;
    };

    auto approximate = [target, equation](double lambda, double u) {
        double m0 = target.at(0);
        CMyVector y_start({u * m0});

        CDGLSolver solver([target, equation, lambda](const CMyVector y, double x) {
            return equation(y, x, lambda);
        });
        
        CMyVector result(target.size());

        for(int i = 0; i < target.size(); ++i) {
            result[i] = solver.heun(0.0, i, 10000, y_start).get(0);
        }

        return result;
    };

    auto error = [target, approximate](const CMyVector x){
        double lambda = x.get(0);
        double u = x.get(1);

        CMyVector approx = approximate(lambda, u);
        
        double sum = 0.0;
        for(int i = 0; i < target.size(); ++i) {
            sum += pow(approx.get(i) - target.at(i), 2);
        }

        return sum;
    };

    CMyVector x_start({0.712, 0.933});

    CMyVector result = CMyVector::minimize(x_start, error, 0.001);

    std::cout << "Ergebnis: " << result.to_string() << std::endl;
}

