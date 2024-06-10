#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>
#include <cmath>
#include <iomanip>
#include <iostream>
#include "../lib/CComplex.h"
#include "../lib/Helper.h"

using namespace Catch::Matchers;

TEST_CASE("Helper function can read valid file", "[Helper]") {
    std::vector<CComplex> values1 = Helper::readValues("data/example1.txt");

    REQUIRE(values1.size() == 1000);
    REQUIRE(values1[0] == CComplex(11.9, 0));
    REQUIRE(values1[499] == CComplex(0.7442903, 0));
    REQUIRE(values1[999] == CComplex(11.9492903, 0));


    std::vector<CComplex> values2 = Helper::readValues("data/example2.txt");

    REQUIRE(values2.size() == 1000);
    REQUIRE(values2[0] == CComplex(15, 3));
    REQUIRE(values2[499] == CComplex(26.1557097, -2.998519681));
    REQUIRE(values2[999] == CComplex(14.9507097, 2.998519681));
}

TEST_CASE("Simple DFT-IDFT Test", "[CComplex]") {
    std::vector<CComplex> values = {
        CComplex(1, 0),
        CComplex(2, -1),
        CComplex(0, -1),
        CComplex(-1, 2),
    };

    std::vector<CComplex> dftValues = CComplex::dft(values);

    REQUIRE_THAT(dftValues[0].re(), WithinAbs(1, 1e-10));
    REQUIRE_THAT(dftValues[0].im(), WithinAbs(0, 1e-10));
    REQUIRE_THAT(dftValues[1].re(), WithinAbs(-1, 1e-10));
    REQUIRE_THAT(dftValues[1].im(), WithinAbs(-1, 1e-10));
    REQUIRE_THAT(dftValues[2].re(), WithinAbs(0, 1e-10));
    REQUIRE_THAT(dftValues[2].im(), WithinAbs(-1, 1e-10));
    REQUIRE_THAT(dftValues[3].re(), WithinAbs(2, 1e-10));
    REQUIRE_THAT(dftValues[3].im(), WithinAbs(2, 1e-10));
    
    std::vector<CComplex> idftValues = CComplex::idft(dftValues);
    
    REQUIRE_THAT(idftValues[0].re(), WithinAbs(1, 1e-10));
    REQUIRE_THAT(idftValues[0].im(), WithinAbs(0, 1e-10));
    REQUIRE_THAT(idftValues[1].re(), WithinAbs(2, 1e-10));
    REQUIRE_THAT(idftValues[1].im(), WithinAbs(-1, 1e-10));
    REQUIRE_THAT(idftValues[2].re(), WithinAbs(0, 1e-10));
    REQUIRE_THAT(idftValues[2].im(), WithinAbs(-1, 1e-10));
    REQUIRE_THAT(idftValues[3].re(), WithinAbs(-1, 1e-10));
    REQUIRE_THAT(idftValues[3].im(), WithinAbs(2, 1e-10));
}

TEST_CASE("DFT deviations for example1 are correct", "[CComplex]") {
    std::vector<CComplex> values = Helper::readValues("data/example1.txt");
    std::vector<CComplex> dftValues = CComplex::dft(values);
    Helper::writeValues("data/example1_dft_default.txt", dftValues);
    Helper::writeValues("data/example1_dft_0001.txt", dftValues, 0.001);
    Helper::writeValues("data/example1_dft_001.txt", dftValues, 0.01);
    Helper::writeValues("data/example1_dft_01.txt", dftValues, 0.1);
    Helper::writeValues("data/example1_dft_1.txt", dftValues, 1);
    
    std::vector<CComplex> dft_default = Helper::readValues("data/example1_dft_default.txt");
    std::vector<CComplex> dft_0001 = Helper::readValues("data/example1_dft_0001.txt");
    std::vector<CComplex> dft_001 = Helper::readValues("data/example1_dft_001.txt");
    std::vector<CComplex> dft_01 = Helper::readValues("data/example1_dft_01.txt");
    std::vector<CComplex> dft_1 = Helper::readValues("data/example1_dft_1.txt");

    std::vector<CComplex> idft_default = CComplex::idft(dft_default);
    std::vector<CComplex> idft_0001 = CComplex::idft(dft_0001);
    std::vector<CComplex> idft_001 = CComplex::idft(dft_001);
    std::vector<CComplex> idft_01 = CComplex::idft(dft_01);
    std::vector<CComplex> idft_1 = CComplex::idft(dft_1);
    
    double deviation_default = Helper::maxDeviation(values, idft_default);
    double deviation_0001 = Helper::maxDeviation(values, idft_0001);
    double deviation_001 = Helper::maxDeviation(values, idft_001);
    double deviation_01 = Helper::maxDeviation(values, idft_01);
    double deviation_1 = Helper::maxDeviation(values, idft_1);

    std::cout << std::endl << "Example 1:" << std::endl;
    std::cout << "Deviation default: " << deviation_default << std::endl;
    std::cout << "Deviation 0.001: " << deviation_0001 << std::endl;
    std::cout << "Deviation 0.01: " << deviation_001 << std::endl;
    std::cout << "Deviation 0.1: " << deviation_01 << std::endl;
    std::cout << "Deviation 1: " << deviation_1 << std::endl;
}

TEST_CASE("DFT deviations for example2 are correct", "[CComplex]") {
    std::vector<CComplex> values = Helper::readValues("data/example2.txt");
    std::vector<CComplex> dftValues = CComplex::dft(values);
    Helper::writeValues("data/example2_dft_default.txt", dftValues);
    Helper::writeValues("data/example2_dft_0001.txt", dftValues, 0.001);
    Helper::writeValues("data/example2_dft_001.txt", dftValues, 0.01);
    Helper::writeValues("data/example2_dft_01.txt", dftValues, 0.1);
    Helper::writeValues("data/example2_dft_1.txt", dftValues, 1);
    
    std::vector<CComplex> dft_default = Helper::readValues("data/example2_dft_default.txt");
    std::vector<CComplex> dft_0001 = Helper::readValues("data/example2_dft_0001.txt");
    std::vector<CComplex> dft_001 = Helper::readValues("data/example2_dft_001.txt");
    std::vector<CComplex> dft_01 = Helper::readValues("data/example2_dft_01.txt");
    std::vector<CComplex> dft_1 = Helper::readValues("data/example2_dft_1.txt");

    std::vector<CComplex> idft_default = CComplex::idft(dft_default);
    std::vector<CComplex> idft_0001 = CComplex::idft(dft_0001);
    std::vector<CComplex> idft_001 = CComplex::idft(dft_001);
    std::vector<CComplex> idft_01 = CComplex::idft(dft_01);
    std::vector<CComplex> idft_1 = CComplex::idft(dft_1);
    
    double deviation_default = Helper::maxDeviation(values, idft_default);
    double deviation_0001 = Helper::maxDeviation(values, idft_0001);
    double deviation_001 = Helper::maxDeviation(values, idft_001);
    double deviation_01 = Helper::maxDeviation(values, idft_01);
    double deviation_1 = Helper::maxDeviation(values, idft_1);

    std::cout << std::endl << "Example 2:" << std::endl;
    std::cout << "Deviation default: " << deviation_default << std::endl;
    std::cout << "Deviation 0.001: " << deviation_0001 << std::endl;
    std::cout << "Deviation 0.01: " << deviation_001 << std::endl;
    std::cout << "Deviation 0.1: " << deviation_01 << std::endl;
    std::cout << "Deviation 1: " << deviation_1 << std::endl;
}

TEST_CASE("DFT images", "[CComplex]") {
    std::vector<CComplex> values = Helper::readValues("data/image_original.txt");
    std::vector<CComplex> dftValues = CComplex::dft(values);

    Helper::writeValues("data/image_dft.txt", dftValues);
    Helper::writeValues("data/image_dft_10.txt", dftValues, 10);
    Helper::writeValues("data/image_dft_30.txt", dftValues, 30);
    Helper::writeValues("data/image_dft_100.txt", dftValues, 100);
    Helper::writeValues("data/image_dft_300.txt", dftValues, 300);
    Helper::writeValues("data/image_dft_1000.txt", dftValues, 1000);

    std::vector<CComplex> dft_default = Helper::readValues("data/image_dft.txt");
    std::vector<CComplex> dft_10 = Helper::readValues("data/image_dft_10.txt");
    std::vector<CComplex> dft_30 = Helper::readValues("data/image_dft_30.txt");
    std::vector<CComplex> dft_100 = Helper::readValues("data/image_dft_100.txt");
    std::vector<CComplex> dft_300 = Helper::readValues("data/image_dft_300.txt");
    std::vector<CComplex> dft_1000 = Helper::readValues("data/image_dft_1000.txt");

    std::vector<CComplex> idft_default = CComplex::idft(dft_default);
    std::vector<CComplex> idft_10 = CComplex::idft(dft_10);
    std::vector<CComplex> idft_30 = CComplex::idft(dft_30);
    std::vector<CComplex> idft_100 = CComplex::idft(dft_100);
    std::vector<CComplex> idft_300 = CComplex::idft(dft_300);
    std::vector<CComplex> idft_1000 = CComplex::idft(dft_1000);

    Helper::writeValues("data/image_idft.txt", idft_default);
    Helper::writeValues("data/image_idft_10.txt", idft_10);
    Helper::writeValues("data/image_idft_30.txt", idft_30);
    Helper::writeValues("data/image_idft_100.txt", idft_100);
    Helper::writeValues("data/image_idft_300.txt", idft_300);
    Helper::writeValues("data/image_idft_1000.txt", idft_1000);
}
