/**
 * @file main.cpp
 * @brief Entry point for the Rubik's Cube visualization application
 * 
 * This application demonstrates a complete 3x3x3 Rubik's Cube implementation
 * with 3D visualization capabilities.
 * 
 * ## Features
 * - Complete cube state management with piece-based representation
 * - Standard Singmaster notation move support (U, R, F, etc.)
 * - Random scramble generation
 * - Interactive 3D visualization with raylib
 * - Mouse-controlled camera system
 * 
 * ## Usage
 * Run the executable to open the 3D visualizer. Use mouse to orbit around
 * the cube and mouse wheel to zoom. The cube starts in the solved state.
 * 
 * ## Future Enhancements
 * - Keyboard controls for applying moves in real-time
 * - Display of current cube state and move history
 * - Solving algorithms integration
 * - Move animation system
 */

#include "../include/RubiksCube.hpp"
#include "../include/Visualizer.hpp"

int main() {
    // Create a new Rubik's Cube in the solved state
    RubiksCube cube;
    
    // Initialize the 3D visualizer
    Visualizer vis(cube);
    
    // Start the interactive visualization
    vis.run();
    
    return 0;
}