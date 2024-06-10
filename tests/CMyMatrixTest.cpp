#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>
#include <cmath>
#include "../lib/CMyMatrix.h"

using namespace Catch::Matchers;

TEST_CASE("CMyMatrix constructors initialize correctly", "[CMyMatrix]") {
    SECTION("Default constructor initializes zero matrix") {
        CMyMatrix matrix(2, 3);
        REQUIRE(matrix.get(0, 0) == 0.0);
        REQUIRE(matrix.get(0, 1) == 0.0);
        REQUIRE(matrix.get(0, 2) == 0.0);
        REQUIRE(matrix.get(1, 0) == 0.0);
        REQUIRE(matrix.get(1, 1) == 0.0);
        REQUIRE(matrix.get(1, 2) == 0.0);
    }

    SECTION("Initializer list constructor initializes with given values") {
        CMyMatrix matrix({{1, 2}, {3, 4}});
        REQUIRE(matrix.get(0, 0) == 1.0);
        REQUIRE(matrix.get(0, 1) == 2.0);
        REQUIRE(matrix.get(1, 0) == 3.0);
        REQUIRE(matrix.get(1, 1) == 4.0);
    }
}

TEST_CASE("Matrix operations", "[CMyMatrix]") {
    CMyMatrix A({{1, 2}, {3, 4}});
    CMyMatrix B({{2, 0}, {1, 2}});

    SECTION("Matrix addition") {
        CMyMatrix result = A + B;
        REQUIRE(result.get(0, 0) == 3.0);
        REQUIRE(result.get(0, 1) == 2.0);
        REQUIRE(result.get(1, 0) == 4.0);
        REQUIRE(result.get(1, 1) == 6.0);
    }

    SECTION("Matrix subtraction") {
        CMyMatrix result = A - B;
        REQUIRE(result.get(0, 0) == -1.0);
        REQUIRE(result.get(0, 1) == 2.0);
        REQUIRE(result.get(1, 0) == 2.0);
        REQUIRE(result.get(1, 1) == 2.0);
    }

    SECTION("Matrix scalar multiplication") {
        CMyMatrix result = A * 2.0;
        REQUIRE(result.get(0, 0) == 2.0);
        REQUIRE(result.get(0, 1) == 4.0);
        REQUIRE(result.get(1, 0) == 6.0);
        REQUIRE(result.get(1, 1) == 8.0);
    }
}

TEST_CASE("Matrix utilities", "[CMyMatrix]") {
    CMyMatrix A({{4, 7}, {2, 6}});

    SECTION("Matrix determinant") {
        REQUIRE(A.determinant() == 10.0);
    }

    SECTION("Matrix inverse") {
        CMyMatrix inverse = A.inverse();
        REQUIRE_THAT(inverse.get(0, 0), WithinAbs(0.6, 0.001));
        REQUIRE_THAT(inverse.get(0, 1), WithinAbs(-0.7, 0.001));
        REQUIRE_THAT(inverse.get(1, 0), WithinAbs(-0.2, 0.001));
        REQUIRE_THAT(inverse.get(1, 1), WithinAbs(0.4, 0.001));
    }

    SECTION("Matrix transpose") {
        CMyMatrix transpose = A.transpose();
        REQUIRE(transpose.get(0, 0) == 4.0);
        REQUIRE(transpose.get(1, 0) == 7.0);
        REQUIRE(transpose.get(0, 1) == 2.0);
        REQUIRE(transpose.get(1, 1) == 6.0);
    }
}

TEST_CASE("Advanced matrix operations", "[CMyMatrix]") {
    CMyMatrix A({{1, 2}, {3, 4}});
    CMyVector v({5, 6});
    
    SECTION("Matrix-vector multiplication") {
        CMyVector result = A * v;
        REQUIRE(result.get(0) == 17.0);
        REQUIRE(result.get(1) == 39.0);
    }
    
    CMyMatrix B({{1, 2, 3}, {4, 5, 6}});
    CMyMatrix C({{1, 2}, {3, 4}, {5, 6}});
    
    SECTION("Matrix-matrix multiplication") {
        CMyMatrix result = B * C;
        REQUIRE(result.get(0, 0) == 22.0);
        REQUIRE(result.get(0, 1) == 28.0);
        REQUIRE(result.get(1, 0) == 49.0);
        REQUIRE(result.get(1, 1) == 64.0);
    }
}

TEST_CASE("Matrix utilities - Continued", "[CMyMatrix]") {
    CMyMatrix A({{1, 0}, {0, 1}});
    CMyMatrix B({{4, 7}, {2, 6}});

    SECTION("Matrix is square") {
        REQUIRE(A.dimensions() == std::make_tuple(2, 2));
        REQUIRE(B.dimensions() == std::make_tuple(2, 2));
    }

    SECTION("Non-square matrix throws on determinant calculation") {
        CMyMatrix nonSquare({{1, 2, 3}});
        REQUIRE_THROWS_AS(nonSquare.determinant(), std::invalid_argument);
    }

    SECTION("Matrix inversion error handling") {
        CMyMatrix singularMatrix({{1, 2}, {2, 4}});
        REQUIRE_THROWS_AS(singularMatrix.inverse(), std::invalid_argument);
    }
}

TEST_CASE("Jacobi matrix calculation", "[CMyMatrix]") {
    SECTION("Jacobi matrix for R^2 -> R^2") {
        CMyVector x({1.0, 2.0});
        std::function<CMyVector(CMyVector)> f = [](CMyVector v) {
            return CMyVector({2 * v.get(0), 3 * v.get(1)});
        };

        CMyMatrix jacobiMatrix = CMyMatrix::jacobi(x, f);
        REQUIRE(jacobiMatrix.dimensions() == std::make_tuple(2, 2));
        
        REQUIRE_THAT(jacobiMatrix.get(0, 0), WithinAbs(2.0, 0.001));
        REQUIRE_THAT(jacobiMatrix.get(1, 1), WithinAbs(3.0, 0.001));
        REQUIRE_THAT(jacobiMatrix.get(0, 1), WithinAbs(0.0, 0.001));
        REQUIRE_THAT(jacobiMatrix.get(1, 0), WithinAbs(0.0, 0.001));
    }

    SECTION("Jacobi matrix for R^4 -> R^3") {
        CMyVector x4({1, 2, 0, 3});
        std::function<CMyVector(CMyVector)> r4r3 = [](CMyVector v) {
            return CMyVector({
                v.get(0) * v.get(1) * std::exp(v.get(2)),
                v.get(1) * v.get(2) * v.get(3),
                v.get(3)
            });
        };


        CMyMatrix jacobiMatrix4 = CMyMatrix::jacobi(x4, r4r3);
        REQUIRE(jacobiMatrix4.dimensions() == std::make_tuple(3, 4));

        REQUIRE_THAT(jacobiMatrix4.get(0, 0), WithinAbs(2.0, 0.00001));
        REQUIRE_THAT(jacobiMatrix4.get(0, 1), WithinAbs(1.0, 0.00001));
        REQUIRE_THAT(jacobiMatrix4.get(0, 2), WithinAbs(2.0001, 0.00001));
        REQUIRE_THAT(jacobiMatrix4.get(0, 3), WithinAbs(0.0, 0.00001));
        REQUIRE_THAT(jacobiMatrix4.get(1, 0), WithinAbs(0.0, 0.00001));
        REQUIRE_THAT(jacobiMatrix4.get(1, 1), WithinAbs(0.0, 0.00001));
        REQUIRE_THAT(jacobiMatrix4.get(1, 2), WithinAbs(6.0, 0.00001));
        REQUIRE_THAT(jacobiMatrix4.get(1, 3), WithinAbs(0.0, 0.00001));
        REQUIRE_THAT(jacobiMatrix4.get(2, 0), WithinAbs(0.0, 0.00001));
        REQUIRE_THAT(jacobiMatrix4.get(2, 1), WithinAbs(0.0, 0.00001));
        REQUIRE_THAT(jacobiMatrix4.get(2, 2), WithinAbs(0.0, 0.00001));
        REQUIRE_THAT(jacobiMatrix4.get(2, 3), WithinAbs(1.0, 0.00001));
    }
}

TEST_CASE("Newton method", "[CMyMatrix]")  {
    SECTION("Newton method for R^2 -> R^2") {
        CMyVector x({1.0, 1.0});

        std::function<CMyVector(CMyVector)> f = [](CMyVector v) {
            return CMyVector({std::pow(v.get(0), 3) * std::pow(v.get(1), 3) - 2 * v.get(1),
                              v.get(0) - 2});
        };

        CMyVector result = CMyMatrix::newton(x, f);
        REQUIRE(result.dimension() == 2);

        REQUIRE_THAT(result.get(0), WithinAbs(2.0, 0.0001));
        REQUIRE_THAT(result.get(1), WithinAbs(-0.5, 0.0001));
    }

    SECTION("ETest R^2 -> R^2") {
        CMyVector x({1.0, 0.0});

        std::function<CMyVector(CMyVector)> f = [](CMyVector v) {
            return CMyVector({std::pow(v.get(0), 2) * v.get(1) - 1,
                              3*std::pow(v.get(1), 2)-2*v.get(0)-v.get(1) - 3});
        };

        CMyVector result = CMyMatrix::newton(x, f);
    }
}


