#include "../include/RubiksCube.hpp"
#include <iostream>

// RubiksCube
RubiksCube::RubiksCube() {
    reset();
}

RubiksCube::~RubiksCube() {
    std::cout << "RubiksCube destructor" << std::endl;
}

void RubiksCube::reset() {
    for (uint8_t i = 0; i < 8; ++i) {
        cp[i] = i;
        co[i] = 0;
    }
    for (uint8_t i = 0; i < 12; ++i) {
        ep[i] = i;
        eo[i] = 0;
    }
}

bool RubiksCube::isSolved() const {
    for (uint8_t i = 0; i < 8; ++i) {
        if (cp[i] != i || co[i] != 0) return false;
    }
    for (uint8_t i = 0; i < 12; ++i) {
        if (ep[i] != i || eo[i] != 0) return false;
    }
    return true;
}
