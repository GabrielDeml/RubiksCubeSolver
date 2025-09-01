#include "../include/Side.hpp"
#include <stdexcept>

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


