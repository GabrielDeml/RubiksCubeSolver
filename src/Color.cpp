#include "../include/Color.hpp"

// ColorObject
ColorObject::ColorObject(Color color) : value(color) {}
ColorObject::~ColorObject() = default;
Color ColorObject::getColor() const { return value; }


