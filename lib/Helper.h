#pragma once

#include <iostream>
#include <fstream>
#include <functional>

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
};
