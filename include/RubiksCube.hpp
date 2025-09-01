#ifndef Rubiks_CUBE_HPP
#define Rubiks_CUBE_HPP

#include <vector>
#include "Side.hpp"


class RubiksCube {
    // 6 sides: typically W, Y, R, O, G, B
    std::vector<Side> cube;

    public:
        RubiksCube();
        ~RubiksCube();
};

#endif
