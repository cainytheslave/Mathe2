#pragma once

#include "CComplex.h"

#include <cmath>
#include <iostream>
#include <fstream>
#include <functional>
#include <vector>

namespace Helper {
    inline std::function<void()> muteOutput(bool mute) {
        std::streambuf* oldCoutStreamBuf = std::cout.rdbuf();

        if(mute) {
            std::ofstream nullStream("/dev/null");
            std::cout.rdbuf(nullStream.rdbuf());
        }

        return [oldCoutStreamBuf, mute]() {
            if(mute) std::cout.rdbuf(oldCoutStreamBuf);
        };
    }

    inline std::vector<CComplex> readValues(const std::string filename) {
        int i, N, idx;
        double re, im;
        std::vector<CComplex> values;
         
        std::ifstream fp;
        fp.open(filename);
        
        fp >> N;
        
        values.resize(N);
        CComplex null(0,0);
        for (i = 0; i < N; i++)
            values[i] = null;
       
        while (!fp.eof())
        {
            fp >> idx >> re >> im;
            CComplex a(re,im);
            values[idx] = a;
        }
         
        fp.close();

        return values;
    }

    inline void writeValues(const std::string dateiname, std::vector<CComplex> values, double epsilon = -1)
    {
        int i;
        int N = values.size();

        std::ofstream fp;
        fp.open(dateiname);

        fp << N << std::endl;

        fp.precision(10);
        for (i = 0; i < N; i++)
            if (values[i].abs() > epsilon)
                fp << i << "\t" << values[i].re() << "\t" << values[i].im() << std::endl;
        
        fp.close();
    }

    inline double maxDeviation(const std::vector<CComplex>& values1, const std::vector<CComplex>& values2) {
        if(values1.size() != values2.size()) {
            throw std::invalid_argument("Both vectors must have the same size");
        }

        double maxDeviation = 0;
        for (int i = 0; i < values1.size(); i++) {
            double deviation = (values1[i] - values2[i]).abs();
            if (deviation > maxDeviation) {
                maxDeviation = deviation;
            }
        }

        return maxDeviation;
    }
};
