#pragma once
#include <cstdlib>
#include <ctime>
#include <limits>

class CRandom {
public:
    CRandom();
    CRandom(int seed); 
    ~CRandom() {}
    void init(int seed);
    void test(int a, int b, int N);
    void test_wrong(int a, int b, int N);
    int nextInt(int a = 0, int b = std::numeric_limits<int>::max());
};
