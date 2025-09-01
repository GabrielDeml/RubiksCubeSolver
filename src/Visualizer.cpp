/**
 * @file Visualizer.cpp
 * @brief Implementation of the 3D Rubik's Cube visualizer using raylib
 * 
 * This file contains the complete 3D rendering system for visualizing a Rubik's Cube.
 * It uses raylib for 3D graphics and provides an interactive camera system.
 * 
 * ## Current Implementation
 * - Renders a 3x3x3 grid of cubelets with colored faces
 * - Uses a basic color scheme (currently static, not tied to cube state)
 * - Implements orbit camera controls with mouse input
 * - Provides smooth 60 FPS rendering with anti-aliasing
 * 
 * ## Future Enhancements
 * - Map actual RubiksCube state to cubelet colors
 * - Add keyboard controls for applying moves
 * - Implement move animations
 * - Add cube state display/debugging information
 */

#include "../include/Visualizer.hpp"
#include "../include/RubiksCube.hpp"

#include "raylib.h"
#include <cmath>

/**
 * @brief Draws a single cubelet (small cube) with colored faces
 * @param position 3D position of the cubelet center
 * @param size Side length of the cubelet
 * @param faceColors Array of 6 colors for the faces (+X, -X, +Y, -Y, +Z, -Z)
 * 
 * Each cubelet is rendered as a dark gray cube with colored stickers on each face.
 * The stickers are slightly smaller than the cube faces and offset outward to
 * create the appearance of gaps between stickers.
 */
static void DrawCubelet(Vector3 position, float size, Color faceColors[6]) {
    // Draw the cubelet body (dark gray base)
    DrawCubeV(position, { size, size, size }, DARKGRAY);
    DrawCubeWiresV(position, { size, size, size }, BLACK);

    // Draw colored stickers on each face
    // Stickers are slightly smaller and offset outward to create realistic gaps
    const float s = size * 0.98f;         // Sticker size (98% of face size)
    const float offset = size * 0.51f;    // Offset from center to face surface

    // +X face (right side)
    DrawCubeV({ position.x + offset, position.y, position.z }, { size * 0.02f, s, s }, faceColors[0]);
    // -X face (left side)
    DrawCubeV({ position.x - offset, position.y, position.z }, { size * 0.02f, s, s }, faceColors[1]);
    // +Y face (top)
    DrawCubeV({ position.x, position.y + offset, position.z }, { s, size * 0.02f, s }, faceColors[2]);
    // -Y face (bottom)
    DrawCubeV({ position.x, position.y - offset, position.z }, { s, size * 0.02f, s }, faceColors[3]);
    // +Z face (front)
    DrawCubeV({ position.x, position.y, position.z + offset }, { s, s, size * 0.02f }, faceColors[4]);
    // -Z face (back)
    DrawCubeV({ position.x, position.y, position.z - offset }, { s, s, size * 0.02f }, faceColors[5]);
}

Visualizer::Visualizer(RubiksCube& cubeInstance)
    : cube(cubeInstance) {}

Visualizer::~Visualizer() = default;

void Visualizer::run() {
    // Initialize raylib window with quality settings
    const int screenWidth = 1000;
    const int screenHeight = 800;
    SetConfigFlags(FLAG_MSAA_4X_HINT | FLAG_VSYNC_HINT);  // Anti-aliasing and V-sync
    InitWindow(screenWidth, screenHeight, "Rubik's Cube Visualizer");

    // Set up 3D camera
    Camera3D camera{};
    camera.target = { 0.0f, 0.0f, 0.0f };    // Look at cube center
    camera.up = { 0.0f, 1.0f, 0.0f };        // Y-axis is up
    camera.fovy = 45.0f;                      // Field of view
    camera.projection = CAMERA_PERSPECTIVE;   // Perspective projection

    // Cube rendering parameters
    Vector3 cubeCenter = { 0.0f, 0.0f, 0.0f };
    float cubeletSize = 0.95f;  // Size of each small cube (with gaps)

    // Orbit camera control state
    float orbitYawDeg = 45.0f;    // Horizontal rotation angle (degrees)
    float orbitPitchDeg = 25.0f;  // Vertical rotation angle (degrees)  
    float orbitRadius = 8.0f;     // Distance from target

    // Input sensitivity settings
    const float rotateSensitivity = 0.3f; // Degrees per pixel of mouse movement
    const float zoomSensitivity = 0.7f;   // Units per mouse wheel step

    SetTargetFPS(60);  // Target 60 FPS for smooth animation

    // Main rendering loop
    while (!WindowShouldClose()) {
        // Handle mouse orbit controls
        if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
            const Vector2 delta = GetMouseDelta();
            orbitYawDeg += delta.x * rotateSensitivity;   // Drag right -> rotate right
            orbitPitchDeg += delta.y * rotateSensitivity; // Drag up -> tilt up
            
            // Clamp pitch to prevent camera flipping
            if (orbitPitchDeg > 85.0f) orbitPitchDeg = 85.0f;
            if (orbitPitchDeg < -85.0f) orbitPitchDeg = -85.0f;
        }

        // Handle mouse wheel zoom
        orbitRadius -= GetMouseWheelMove() * zoomSensitivity;
        if (orbitRadius < 2.0f) orbitRadius = 2.0f;      // Minimum zoom distance
        if (orbitRadius > 30.0f) orbitRadius = 30.0f;    // Maximum zoom distance

        // Calculate camera position from orbit parameters
        const float yawRad = orbitYawDeg * DEG2RAD;
        const float pitchRad = orbitPitchDeg * DEG2RAD;
        camera.position.x = camera.target.x + orbitRadius * cosf(pitchRad) * cosf(yawRad);
        camera.position.y = camera.target.y + orbitRadius * sinf(pitchRad);
        camera.position.z = camera.target.z + orbitRadius * cosf(pitchRad) * sinf(yawRad);

        // Begin frame rendering
        BeginDrawing();
        ClearBackground(RAYWHITE);

        BeginMode3D(camera);
        DrawGrid(20, 1.0f);  // Draw reference grid

        // Standard Rubik's Cube color scheme
        // Currently static - future enhancement: map to actual cube state
        Color defaultFaces[6] = { RED, ORANGE, WHITE, YELLOW, GREEN, BLUE };

        // Render 3x3x3 grid of cubelets
        // Note: Currently renders all 27 cubelets including inner ones for simplicity
        // Future optimization: only render visible pieces
        for (int x = -1; x <= 1; ++x) {
            for (int y = -1; y <= 1; ++y) {
                for (int z = -1; z <= 1; ++z) {
                    Vector3 pos = { 
                        cubeCenter.x + x * 1.0f, 
                        cubeCenter.y + y * 1.0f, 
                        cubeCenter.z + z * 1.0f 
                    };

                    // Apply standard color scheme to all cubelets
                    // TODO: Map colors based on actual RubiksCube state
                    Color faces[6] = {
                        defaultFaces[0], defaultFaces[1],  // Right, Left
                        defaultFaces[2], defaultFaces[3],  // Top, Bottom
                        defaultFaces[4], defaultFaces[5]   // Front, Back
                    };

                    DrawCubelet(pos, cubeletSize, faces);
                }
            }
        }

        EndMode3D();

        // Draw UI overlay
        DrawFPS(10, 10);
        DrawText("Left drag: orbit  |  Wheel: zoom", 10, 30, 14, DARKGRAY);
        
        // TODO: Add cube state information display
        // TODO: Add move input instructions

        EndDrawing();
    }

    CloseWindow();  // Clean up raylib resources
}


