#include "../include/Visualizer.hpp"
#include "../include/RubiksCube.hpp"

#include "raylib.h"
#include <cmath>

static void DrawCubelet(Vector3 position, float size, Color faceColors[6]) {
    // Draw cubelet body
    DrawCubeV(position, { size, size, size }, DARKGRAY);
    DrawCubeWiresV(position, { size, size, size }, BLACK);

    // Slightly offset stickers from the surface
    const float s = size * 0.98f;
    const float offset = size * 0.51f;

    // +X (right)
    DrawCubeV({ position.x + offset, position.y, position.z }, { size * 0.02f, s, s }, faceColors[0]);
    // -X (left)
    DrawCubeV({ position.x - offset, position.y, position.z }, { size * 0.02f, s, s }, faceColors[1]);
    // +Y (top)
    DrawCubeV({ position.x, position.y + offset, position.z }, { s, size * 0.02f, s }, faceColors[2]);
    // -Y (bottom)
    DrawCubeV({ position.x, position.y - offset, position.z }, { s, size * 0.02f, s }, faceColors[3]);
    // +Z (front)
    DrawCubeV({ position.x, position.y, position.z + offset }, { s, s, size * 0.02f }, faceColors[4]);
    // -Z (back)
    DrawCubeV({ position.x, position.y, position.z - offset }, { s, s, size * 0.02f }, faceColors[5]);
}

Visualizer::Visualizer(RubiksCube& cubeInstance)
    : cube(cubeInstance) {}

Visualizer::~Visualizer() = default;

void Visualizer::run() {
    const int screenWidth = 1000;
    const int screenHeight = 800;
    SetConfigFlags(FLAG_MSAA_4X_HINT | FLAG_VSYNC_HINT);
    InitWindow(screenWidth, screenHeight, "Rubik's Cube Visualizer");

    Camera3D camera{};
    camera.target = { 0.0f, 0.0f, 0.0f };
    camera.up = { 0.0f, 1.0f, 0.0f };
    camera.fovy = 45.0f;
    camera.projection = CAMERA_PERSPECTIVE;

    Vector3 cubeCenter = { 0.0f, 0.0f, 0.0f };
    float cubeletSize = 0.95f;

    // Simple orbit camera state
    float orbitYawDeg = 45.0f;   // horizontal angle
    float orbitPitchDeg = 25.0f; // vertical angle
    float orbitRadius = 8.0f;    // distance from target

    const float rotateSensitivity = 0.3f; // degrees per pixel
    const float zoomSensitivity = 0.7f;   // units per wheel step

    SetTargetFPS(60);

    while (!WindowShouldClose()) {
        // Mouse orbit controls
        if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
            const Vector2 delta = GetMouseDelta();
            orbitYawDeg += delta.x * rotateSensitivity;   // drag right -> rotate right
            orbitPitchDeg += delta.y * rotateSensitivity; // drag up -> tilt up
            if (orbitPitchDeg > 85.0f) orbitPitchDeg = 85.0f;
            if (orbitPitchDeg < -85.0f) orbitPitchDeg = -85.0f;
        }

        // Mouse wheel zoom
        orbitRadius -= GetMouseWheelMove() * zoomSensitivity;
        if (orbitRadius < 2.0f) orbitRadius = 2.0f;
        if (orbitRadius > 30.0f) orbitRadius = 30.0f;

        const float yawRad = orbitYawDeg * DEG2RAD;
        const float pitchRad = orbitPitchDeg * DEG2RAD;
        camera.position.x = camera.target.x + orbitRadius * cosf(pitchRad) * cosf(yawRad);
        camera.position.y = camera.target.y + orbitRadius * sinf(pitchRad);
        camera.position.z = camera.target.z + orbitRadius * cosf(pitchRad) * sinf(yawRad);

        BeginDrawing();
        ClearBackground(RAYWHITE);

        BeginMode3D(camera);
        DrawGrid(20, 1.0f);

        // Basic color scheme for faces: +X, -X, +Y, -Y, +Z, -Z
        Color defaultFaces[6] = { RED, ORANGE, WHITE, YELLOW, GREEN, BLUE };

        // Draw 3x3x3 cubelets (skip the core if desired; we draw all for simplicity)
        for (int x = -1; x <= 1; ++x) {
            for (int y = -1; y <= 1; ++y) {
                for (int z = -1; z <= 1; ++z) {
                    Vector3 pos = { cubeCenter.x + x * 1.0f, cubeCenter.y + y * 1.0f, cubeCenter.z + z * 1.0f };

                    Color faces[6] = {
                        defaultFaces[0], defaultFaces[1],
                        defaultFaces[2], defaultFaces[3],
                        defaultFaces[4], defaultFaces[5]
                    };

                    DrawCubelet(pos, cubeletSize, faces);
                }
            }
        }

        EndMode3D();

        DrawFPS(10, 10);
        DrawText("Left drag: orbit  |  Wheel: zoom", 10, 30, 14, DARKGRAY);

        EndDrawing();
    }

    CloseWindow();
}


