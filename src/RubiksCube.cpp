#include "../include/RubiksCube.hpp"
#include "../include/Color.hpp"
#include "../include/Side.hpp"
#include <iostream>

// RubiksCube
RubiksCube::RubiksCube() :
    cube{ Side(Color('W')), Side(Color('Y')), Side(Color('R')),
          Side(Color('O')), Side(Color('G')), Side(Color('B')) } {
    std::cout << "RubiksCube constructor" << std::endl;
}

RubiksCube::~RubiksCube() {
    std::cout << "RubiksCube destructor" << std::endl;
}
