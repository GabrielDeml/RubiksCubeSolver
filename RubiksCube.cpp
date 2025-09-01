#include "RubiksCube.hpp"
#include <iostream>
#include <stdexcept>

// Color
Color::Color(char color) : value(color) {}
Color::~Color() = default;
char Color::getColor() const { return value; }

// Side
Side::Side(Color color) : colors(9, color) {}
Side::~Side() = default;
std::vector<Color> Side::getColors() const { return colors; }
Color Side::getColor(int index) const {
    if (index < 0 || index >= static_cast<int>(colors.size())) {
        throw std::out_of_range("Side::getColor index out of range");
    }
    return colors[static_cast<size_t>(index)];
}

// RubiksCube
RubiksCube::RubiksCube() :
    cube{ Side(Color('W')), Side(Color('Y')), Side(Color('R')),
          Side(Color('O')), Side(Color('G')), Side(Color('B')) } {
    std::cout << "RubiksCube constructor" << std::endl;
}

RubiksCube::~RubiksCube() {
    std::cout << "RubiksCube destructor" << std::endl;
}
