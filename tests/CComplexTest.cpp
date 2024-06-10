#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>
#include "../lib/CComplex.h"
#include <cmath>

using namespace Catch::Matchers;

TEST_CASE("CComplex default constructor", "[CComplex]") {
    CComplex c;
    REQUIRE(c.re() == 0);
    REQUIRE(c.im() == 0);
};

TEST_CASE("CComplex constructor with real and imaginary part", "[CComplex]") {
    CComplex c(1, 2);
    REQUIRE(c.re() == 1);
    REQUIRE(c.im() == 2);
}

TEST_CASE("CComplex constructor with angle", "[CComplex]") {
    CComplex c(M_PI / 2);
    REQUIRE_THAT(c.re(), WithinAbs(0, 1e-10));
    REQUIRE_THAT(c.im(), WithinAbs(1, 1e-10));
}

TEST_CASE("CComplex copy constructor", "[CComplex]") {
    CComplex c(1, 2);
    CComplex c2(c);
    REQUIRE(c2.re() == 1);
    REQUIRE(c2.im() == 2);
}

TEST_CASE("CComplex abs", "[CComplex]") {
    CComplex c(3, 4);
    REQUIRE(c.abs() == 5);
}

TEST_CASE("CComplex absSq", "[CComplex]") {
    CComplex c(3, 4);
    REQUIRE(c.absSq() == 25);
}

TEST_CASE("CComplex assignment operator", "[CComplex]") {
    CComplex c(1, 2);
    CComplex c2;
    c2 = c;
    REQUIRE(c2.re() == 1);
    REQUIRE(c2.im() == 2);
}

TEST_CASE("CComplex addition operator", "[CComplex]") {
    CComplex c1(1, 2);
    CComplex c2(3, 4);
    CComplex c3 = c1 + c2;
    REQUIRE(c3.re() == 4);
    REQUIRE(c3.im() == 6);
}

TEST_CASE("CComplex subtraction operator", "[CComplex]") {
    CComplex c1(1, 2);
    CComplex c2(3, 4);
    CComplex c3 = c1 - c2;
    REQUIRE(c3.re() == -2);
    REQUIRE(c3.im() == -2);
}

TEST_CASE("CComplex multiplication operator", "[CComplex]") {
    CComplex c1(1, 2);
    CComplex c2(3, 4);
    CComplex c3 = c1 * c2;
    REQUIRE(c3.re() == -5);
    REQUIRE(c3.im() == 10);
}

TEST_CASE("CComplex division operator", "[CComplex]") {
    CComplex c1(1, 2);
    CComplex c2(3, 4);
    CComplex c3 = c1 / c2;
    REQUIRE(c3.re() == 11.0 / 25);
    REQUIRE(c3.im() == 2.0 / 25);
}

TEST_CASE("CComplex multiplication with scalar", "[CComplex]") {
    CComplex c(1, 2);
    CComplex c2 = c * 2;
    REQUIRE(c2.re() == 2);
    REQUIRE(c2.im() == 4);
}

TEST_CASE("CComplex equality operator", "[CComplex]") {
    CComplex c1(1, 2);
    CComplex c2(1, 2);
    REQUIRE(c1 == c2);
}

TEST_CASE("CComplex inequality operator", "[CComplex]") {
    CComplex c1(1, 2);
    CComplex c2(3, 4);
    REQUIRE(c1 != c2);
}

