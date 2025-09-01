#ifndef Rubiks_CUBE_HPP
#define Rubiks_CUBE_HPP

#include <array>
#include <cstdint>


class RubiksCube {
    // Cubie model: permutation and orientation of corners and edges
    // Corner indices: 0..7, Edge indices: 0..11
    std::array<uint8_t, 8> cp; // corner permutation
    std::array<uint8_t, 8> co; // corner orientation (0..2)
    std::array<uint8_t, 12> ep; // edge permutation
    std::array<uint8_t, 12> eo; // edge orientation (0..1)

    public:
        RubiksCube();
        ~RubiksCube();

        // Reset to solved state
        void reset();

        // Check if cube is in solved state
        bool isSolved() const;
};

#endif
