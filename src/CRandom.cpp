#include "../lib/CRandom.h"

#include <iostream>
#include <vector>

CRandom::CRandom() {
    srand(time(NULL));
}

CRandom::CRandom(int seed) {
    srand(seed);
}

void CRandom::init(int seed) {
    srand(seed);
}

int CRandom::nextInt(int a, int b) {
    return a + rand() % (b - a + 1);
}

void CRandom::test(int a, int b, int N) {
    std::vector<int> results(b-a+1, 0);
    for (int i = 0; i < N; i++) {
        results[nextInt(a, b) - a]++;
    }
    
    std::cout << "test(" << a << ", " << b << ", " << N << "):" << std::endl;
    for (int i = 0; i < b-a+1; i++) {
        std::cout << i+a << ": " << results[i] << "x" << std::endl;
    }
    std::cout << std::endl;
}

void CRandom::test_wrong(int a, int b, int N) {
    std::vector<int> results(b-a+1, 0);
    for (int i = 0; i < N; i++) {
        srand(time(NULL));
        results[nextInt(a, b) - a]++;
    }
    
    std::cout << "test_wrong(" << a << ", " << b << ", " << N << "):" << std::endl;
    for (int i = 0; i < b-a+1; i++) {
        std::cout << i+a << ": " << results[i] << "x" << std::endl;
    }
    std::cout << std::endl;
}


