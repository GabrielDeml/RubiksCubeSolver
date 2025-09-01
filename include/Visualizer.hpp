#ifndef VISUALIZER_HPP
#define VISUALIZER_HPP

/**
 * @file Visualizer.hpp
 * @brief 3D visualization of the Rubik's Cube using raylib
 */

class RubiksCube;

/**
 * @class Visualizer
 * @brief Provides 3D visualization and interaction for a RubiksCube
 * 
 * This class creates a 3D rendering window using raylib to display the cube state.
 * It provides mouse-based camera controls for viewing the cube from different angles.
 * 
 * ## Features
 * - Interactive 3D cube visualization
 * - Orbit camera with mouse controls (click and drag to rotate)
 * - Mouse wheel zoom in/out
 * - Real-time display of cube state
 * 
 * ## Controls
 * - **Left mouse drag**: Orbit around the cube
 * - **Mouse wheel**: Zoom in/out
 * - **ESC or close button**: Exit visualization
 * 
 * ## Integration
 * The visualizer takes a reference to a RubiksCube instance and can display
 * its current state. Future enhancements could include keyboard controls for
 * applying moves and real-time state updates.
 */
class Visualizer {
    RubiksCube& cube;  ///< Reference to the cube being visualized

public:
    /**
     * @brief Constructs a visualizer for the given cube
     * @param cubeInstance Reference to the RubiksCube to visualize
     */
    explicit Visualizer(RubiksCube& cubeInstance);
    
    /**
     * @brief Destructor
     */
    ~Visualizer();

    /**
     * @brief Starts the visualization main loop
     * 
     * Creates a raylib window and enters the main rendering loop.
     * The loop continues until the user closes the window or presses ESC.
     * Handles user input for camera controls and renders the 3D cube.
     */
    void run();
};

#endif


