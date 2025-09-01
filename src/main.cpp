#include "../include/RubiksCube.hpp"
#include "../include/Visualizer.hpp"


int main() {
    RubiksCube cube;
    Visualizer vis(cube);
    vis.run();
    return 0;
}