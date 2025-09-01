/**
 * @file RubiksCube.cpp
 * @brief Implementation of the RubiksCube class with move tables and algorithms
 * 
 * This file contains the complete implementation of a 3x3x3 Rubik's Cube simulator
 * including move application, scrambling, and state management.
 * 
 * ## Implementation Details
 * - Uses piece-based representation with corner and edge pieces
 * - Move tables define permutations and orientation changes for each face turn
 * - Supports standard Singmaster notation with automatic derivation of inverse/double moves
 * - Thread-safe move table initialization using static local variables
 */

#include "../include/RubiksCube.hpp"
#include <iostream>
#include <unordered_map>
#include <string>
#include <vector>
#include <random>
#include <sstream>

RubiksCube::RubiksCube() {
    reset();
}

RubiksCube::~RubiksCube() {
    std::cout << "RubiksCube destructor" << std::endl;
}

void RubiksCube::reset() {
    // Initialize each slot with the matching piece index and orientation 0.
    // In the solved state, slot i contains piece i with zero orientation.
    for (uint8_t i = 0; i < 8; ++i) {
        this->corners[i].index = i;
        this->corners[i].orientation = 0;
    }
    for (uint8_t i = 0; i < 12; ++i) {
        this->edges[i].index = i;
        this->edges[i].orientation = 0;
    }
}

bool RubiksCube::isSolved() const {
    // The cube is solved only if every slot contains its matching piece
    // with orientation 0.
    for (uint8_t i = 0; i < 8; ++i) {
        if (this->corners[i].index != i || this->corners[i].orientation != 0) return false;
    }
    for (uint8_t i = 0; i < 12; ++i) {
        if (this->edges[i].index != i || this->edges[i].orientation != 0) return false;
    }
    return true;
}

namespace {
    /**
     * @brief Defines a move in terms of piece permutations and orientation changes
     * 
     * Each move is represented by how pieces are permuted and how their orientations
     * change. For corners, orientation ranges 0-2; for edges, 0-1.
     */
    struct MoveDef {
        std::array<int, 8> corner_perm;      ///< Where each corner slot gets its piece from
        std::array<int, 8> corner_ori_delta; ///< Orientation change for each corner (0-2)
        std::array<int, 12> edge_perm;       ///< Where each edge slot gets its piece from  
        std::array<int, 12> edge_ori_delta;  ///< Orientation change for each edge (0-1)
    };

    /// Global move table mapping move names to their definitions
    std::unordered_map<std::string, MoveDef> g_moveTables;

    /**
     * @brief Initializes the move tables with all 18 possible moves
     * 
     * Creates move definitions for:
     * - 6 base moves (U, D, R, L, F, B) - clockwise quarter turns
     * - 6 inverse moves (U', D', R', L', F', B') - counter-clockwise
     * - 6 double moves (U2, D2, R2, L2, F2, B2) - 180-degree turns
     * 
     * Base moves are defined manually, inverse and double moves are derived
     * by applying the base move multiple times to track the resulting permutation.
     */
    void initMoveTablesOnce() {
        if (!g_moveTables.empty()) return;

        std::array<int, 8> c_zero{};   // All zeros for corner orientations
        std::array<int, 12> e_zero{};  // All zeros for edge orientations

        // Base quarter-turn moves (clockwise when viewing the face)
        // Each array shows where slot i gets its piece from
        
        // U (Up) face: rotates top layer clockwise
        g_moveTables["U"] = MoveDef{
            {3,0,1,2,4,5,6,7}, c_zero,
            {3,0,1,2,4,5,6,7,8,9,10,11}, e_zero
        };
        
        // D (Down) face: rotates bottom layer clockwise  
        g_moveTables["D"] = MoveDef{
            {0,1,2,3,5,6,7,4}, c_zero,
            {0,1,2,3,5,6,7,4,8,9,10,11}, e_zero
        };
        
        // R (Right) face: rotates right layer clockwise
        g_moveTables["R"] = MoveDef{
            {0,1,2,3,5,6,7,4}, {0,0,0,0,2,1,2,1},
            {0,9,2,3,4,5,6,7,8,11,10,1}, e_zero
        };
        
        // L (Left) face: rotates left layer clockwise
        g_moveTables["L"] = MoveDef{
            {2,6,7,3,4,0,1,5}, {1,2,1,0,0,2,1,0},
            {0,1,10,3,4,5,9,7,8,2,6,11}, e_zero
        };
        
        // F (Front) face: rotates front layer clockwise
        g_moveTables["F"] = MoveDef{
            {1,5,2,3,0,4,6,7}, {1,2,0,0,2,1,0,0},
            {0,8,2,3,4,9,6,7,5,1,10,11}, {0,1,0,0,0,1,0,0,1,1,0,0}
        };
        
        // B (Back) face: rotates back layer clockwise
        g_moveTables["B"] = MoveDef{
            {0,1,3,7,4,5,2,6}, {0,0,1,2,0,0,2,1},
            {0,1,2,11,4,5,6,10,8,9,3,7}, {0,0,0,1,0,0,0,1,0,0,1,1}
        };

        /**
         * @brief Generate derived moves (inverse and double) from base moves
         * 
         * For each base move, we generate:
         * - Double move (m2): Apply base move twice
         * - Inverse move (m'): Apply base move three times (equivalent to one counter-clockwise)
         * 
         * This is done by simulating the moves on a temporary cube state and recording
         * the resulting permutation and orientation changes.
         */
        const std::vector<std::string> baseMoves = {"U","D","R","L","F","B"};
        for (const auto& m : baseMoves) {
            // Set up temporary cube state for simulation
            std::array<uint8_t, 8> corner_index{};
            std::array<uint8_t, 8> corner_ori{};
            std::array<uint8_t, 12> edge_index{};
            std::array<uint8_t, 12> edge_ori{};
            
            // Initialize to solved state
            for (int i = 0; i < 8; ++i) { corner_index[i] = (uint8_t)i; corner_ori[i] = 0; }
            for (int i = 0; i < 12; ++i) { edge_index[i] = (uint8_t)i; edge_ori[i] = 0; }

            // Lambda to apply a move definition to the temporary state
            auto applyLocal = [&](const MoveDef& def) {
                std::array<uint8_t, 8> new_c_idx{};
                std::array<uint8_t, 8> new_c_ori{};
                for (int i = 0; i < 8; ++i) {
                    new_c_idx[i] = corner_index[def.corner_perm[i]];
                    new_c_ori[i] = (uint8_t)((corner_ori[def.corner_perm[i]] + def.corner_ori_delta[i]) % 3);
                }
                corner_index = new_c_idx;
                corner_ori = new_c_ori;

                std::array<uint8_t, 12> new_e_idx{};
                std::array<uint8_t, 12> new_e_ori{};
                for (int i = 0; i < 12; ++i) {
                    new_e_idx[i] = edge_index[def.edge_perm[i]];
                    new_e_ori[i] = (uint8_t)((edge_ori[def.edge_perm[i]] + def.edge_ori_delta[i]) % 2);
                }
                edge_index = new_e_idx;
                edge_ori = new_e_ori;
            };

            // Generate double move (m2): apply base move twice
            applyLocal(g_moveTables[m]);
            applyLocal(g_moveTables[m]);

            MoveDef m2{};
            for (int i = 0; i < 8; ++i) { m2.corner_perm[i] = corner_index[i]; m2.corner_ori_delta[i] = corner_ori[i]; }
            for (int i = 0; i < 12; ++i) { m2.edge_perm[i] = edge_index[i]; m2.edge_ori_delta[i] = edge_ori[i]; }
            g_moveTables[m + "2"] = m2;

            // Generate inverse move (m'): apply base move three times
            // Reset to solved state first
            for (int i = 0; i < 8; ++i) { corner_index[i] = (uint8_t)i; corner_ori[i] = 0; }
            for (int i = 0; i < 12; ++i) { edge_index[i] = (uint8_t)i; edge_ori[i] = 0; }
            
            applyLocal(g_moveTables[m]);
            applyLocal(g_moveTables[m]);
            applyLocal(g_moveTables[m]);
            
            MoveDef inv{};
            for (int i = 0; i < 8; ++i) { inv.corner_perm[i] = corner_index[i]; inv.corner_ori_delta[i] = corner_ori[i]; }
            for (int i = 0; i < 12; ++i) { inv.edge_perm[i] = edge_index[i]; inv.edge_ori_delta[i] = edge_ori[i]; }
            g_moveTables[m + "'"] = inv;
        }
    }
}

void RubiksCube::applyMove(const std::string& move) {
    initMoveTablesOnce();
    auto it = g_moveTables.find(move);
    if (it == g_moveTables.end()) {
        throw std::invalid_argument("Invalid move: " + move);
    }
    const MoveDef& def = it->second;

    // Apply corner permutation and orientation changes
    // For each slot i, get the piece from slot def.corner_perm[i] and add orientation delta
    std::array<CornerPiece, 8> newCorners{};
    for (int i = 0; i < 8; ++i) {
        const int from = def.corner_perm[i];
        newCorners[i].index = this->corners[from].index;
        newCorners[i].orientation = (uint8_t)((this->corners[from].orientation + def.corner_ori_delta[i]) % 3);
    }
    this->corners = newCorners;

    // Apply edge permutation and orientation changes
    // Same logic as corners but with mod 2 for edge orientations
    std::array<EdgePiece, 12> newEdges{};
    for (int i = 0; i < 12; ++i) {
        const int from = def.edge_perm[i];
        newEdges[i].index = this->edges[from].index;
        newEdges[i].orientation = (uint8_t)((this->edges[from].orientation + def.edge_ori_delta[i]) % 2);
    }
    this->edges = newEdges;
}

static std::string moveEnumToString(RubiksCube::Move move) {
    switch (move) {
        case RubiksCube::Move::U: return "U";
        case RubiksCube::Move::U_PRIME: return "U'";
        case RubiksCube::Move::U2: return "U2";
        case RubiksCube::Move::D: return "D";
        case RubiksCube::Move::D_PRIME: return "D'";
        case RubiksCube::Move::D2: return "D2";
        case RubiksCube::Move::R: return "R";
        case RubiksCube::Move::R_PRIME: return "R'";
        case RubiksCube::Move::R2: return "R2";
        case RubiksCube::Move::L: return "L";
        case RubiksCube::Move::L_PRIME: return "L'";
        case RubiksCube::Move::L2: return "L2";
        case RubiksCube::Move::F: return "F";
        case RubiksCube::Move::F_PRIME: return "F'";
        case RubiksCube::Move::F2: return "F2";
        case RubiksCube::Move::B: return "B";
        case RubiksCube::Move::B_PRIME: return "B'";
        case RubiksCube::Move::B2: return "B2";
    }
    return "";
}

void RubiksCube::applyMove(RubiksCube::Move move) {
    applyMove(moveEnumToString(move));
}

void RubiksCube::applyMoves(const std::string& moves) {
    // Parse space-separated moves and apply each one sequentially
    std::stringstream ss(moves);
    std::string m;
    while (ss >> m) {
        applyMove(m);
    }
}

std::string RubiksCube::scramble(int length) {
    initMoveTablesOnce();
    
    // Build list of all available moves (cached after first call)
    static std::vector<std::string> allMoves;
    if (allMoves.empty()) {
        allMoves.reserve(g_moveTables.size());
        for (const auto& kv : g_moveTables) {
            allMoves.push_back(kv.first);
        }
    }

    // Set up random number generation
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, (int)allMoves.size() - 1);

    // Generate and apply random moves
    std::stringstream ss;
    for (int i = 0; i < length; ++i) {
        const std::string& m = allMoves[(size_t)dis(gen)];
        ss << m;
        if (i + 1 < length) ss << ' ';  // Add space between moves
        applyMove(m);
    }
    return ss.str();
}

std::string RubiksCube::toString() const {
    // Create a human-readable representation of the cube state
    // Format: piece_index,orientation for each slot
    std::stringstream ss;
    ss << "Corners: ";
    for (int i = 0; i < 8; ++i) {
        ss << "(" << (int)corners[i].index << "," << (int)corners[i].orientation << ") ";
    }
    ss << "\nEdges:   ";
    for (int i = 0; i < 12; ++i) {
        ss << "(" << (int)edges[i].index << "," << (int)edges[i].orientation << ") ";
    }
    return ss.str();
}

void RubiksCube::rotate(int face, int direction) {
    /**
     * Maps numeric face/direction to Singmaster notation and applies the move.
     * 
     * Face mapping: 0=U, 1=D, 2=R, 3=L, 4=F, 5=B
     * Direction: 1=clockwise, -1=counter-clockwise, 2/-2=double turn
     * 
     * This provides a convenient numeric interface for external callers
     * while internally using the string-based move system.
     */
    static const char* names[] = {"U","D","R","L","F","B"};
    if (face < 0 || face > 5) return;  // Ignore invalid face indices
    
    std::string move = names[face];
    if (direction == -1) move += "'";          // Counter-clockwise
    else if (direction == 2 || direction == -2) move += "2";  // Double turn
    // Default case (direction == 1): use base move (clockwise)
    
    applyMove(move);
}
