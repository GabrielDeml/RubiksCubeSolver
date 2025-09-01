#ifndef Rubiks_CUBE_HPP
#define Rubiks_CUBE_HPP

#include <vector>

class Color {
    char value;
    public:
        explicit Color(char color);
        ~Color();
        char getColor() const;
};

class Side {
    std::vector<Color> colors; // 9 stickers per side
    public:
        explicit Side(Color color);
        ~Side();
        std::vector<Color> getColors() const;
        Color getColor(int index) const;
};


class RubiksCube {
    // 6 sides: typically W, Y, R, O, G, B
    std::vector<Side> cube;

    public:
        RubiksCube();
        ~RubiksCube();
};

#endif
