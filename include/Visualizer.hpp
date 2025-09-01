#ifndef VISUALIZER_HPP
#define VISUALIZER_HPP

#include <cstdint>

class RubiksCube;

class Visualizer {
    RubiksCube& cube;

public:
    explicit Visualizer(RubiksCube& cubeInstance);
    ~Visualizer();

    void run();
};

#endif


