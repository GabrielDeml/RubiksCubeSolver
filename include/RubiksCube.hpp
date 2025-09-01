#ifndef Rubiks_CUBE_HPP
#define Rubiks_CUBE_HPP

#include <array>
#include <cstdint>
#include <string>

/**
 * @file RubiksCube.hpp
 * @brief A complete 3x3x3 Rubik's Cube implementation with move application and scrambling
 */

/**
 * @class RubiksCube
 * @brief Represents a 3x3x3 Rubik's Cube using a piece-based (cubie) model
 * 
 * This class models the cube state by tracking which physical piece occupies each 
 * slot and that piece's orientation relative to the solved state.
 * 
 * ## Cube Representation
 * - **8 Corner pieces**: Each has 3 possible orientations (0, 1, 2)
 * - **12 Edge pieces**: Each has 2 possible orientations (0, 1)
 * 
 * ## Coordinate System
 * The cube uses a standard coordinate system where:
 * - **Corner indices 0-7**: Fixed positions on the cube
 * - **Edge indices 0-11**: Fixed positions on the cube
 * 
 * In the solved state, slot `i` contains piece `i` with orientation 0.
 * 
 * ## Corner Indexing (0-7)
 * ```
 *     4 --- 5
 *    /|    /|
 *   0 --- 1 |
 *   | 7 --|-6
 *   |/    |/
 *   3 --- 2
 * ```
 * 
 * ## Edge Indexing (0-11)
 * ```
 *     +--4--+
 *    /|    /|
 *   8 0   9 1
 *   | +--5-|-+
 *   |/    |/
 *  11 3--10 2
 *   Back: 6,7 (not visible)
 * ```
 * 
 * ## Move Notation
 * Standard Singmaster notation is supported:
 * - **Basic moves**: U, D, R, L, F, B (clockwise quarter turns)
 * - **Inverse moves**: U', D', R', L', F', B' (counterclockwise)
 * - **Double moves**: U2, D2, R2, L2, F2, B2 (180-degree turns)
 * 
 * Where:
 * - U = Up face, D = Down face
 * - R = Right face, L = Left face  
 * - F = Front face, B = Back face
 */
class RubiksCube {
private:
    /**
     * @brief Represents a corner piece with its position and orientation
     */
    struct CornerPiece {
        uint8_t index;       ///< Which corner piece occupies this slot (0-7)
        uint8_t orientation; ///< Twist state relative to solved (0-2)
    };

    /**
     * @brief Represents an edge piece with its position and orientation  
     */
    struct EdgePiece {
        uint8_t index;       ///< Which edge piece occupies this slot (0-11)
        uint8_t orientation; ///< Flip state relative to solved (0-1)
    };

    std::array<CornerPiece, 8> corners;  ///< Array of corner pieces
    std::array<EdgePiece, 12> edges;     ///< Array of edge pieces

public:
    /**
     * @brief Enumerates all supported face turns in Singmaster notation
     * 
     * The values represent quarter-turn clockwise, counter-clockwise (prime),
     * and double turns for each face. Use with applyMove(Move).
     */
    enum class Move {
        U, U_PRIME, U2,
        D, D_PRIME, D2,
        R, R_PRIME, R2,
        L, L_PRIME, L2,
        F, F_PRIME, F2,
        B, B_PRIME, B2
    };
    /**
     * @brief Constructs a new RubiksCube in the solved state
     */
    RubiksCube();
    
    /**
     * @brief Destructor
     */
    ~RubiksCube();

    /**
     * @brief Resets the cube to the solved state
     * 
     * In the solved state, each slot i contains piece i with orientation 0.
     */
    void reset();

    /**
     * @brief Checks if the cube is in the solved state
     * @return true if solved, false otherwise
     */
    bool isSolved() const;

    /**
     * @brief Rotates a face of the cube
     * @param face Face to rotate (0=U, 1=D, 2=R, 3=L, 4=F, 5=B)
     * @param direction Direction of rotation (1=clockwise, -1=counter-clockwise, 2/-2=double)
     */
    void rotate(int face, int direction);
    
    /**
     * @brief Applies a single move in Singmaster notation
     * @param move Move string (e.g., "U", "R'", "F2")
     * @throws std::invalid_argument if move is not recognized
     * 
     * Supported moves:
     * - Base moves: U, D, R, L, F, B (clockwise quarter turns)
     * - Inverse moves: U', D', R', L', F', B' (counter-clockwise)
     * - Double moves: U2, D2, R2, L2, F2, B2 (180-degree turns)
     */
    void applyMove(const std::string& move);

    /**
     * @brief Applies a single move specified by the Move enum
     * @param move Enumerated move value (e.g., Move::U, Move::R_PRIME, Move::F2)
     */
    void applyMove(Move move);

    /**
     * @brief Applies a sequence of space-separated moves
     * @param moves String containing multiple moves separated by spaces
     * @throws std::invalid_argument if any move is not recognized
     * 
     * Example: "R U R' U' R U R' F' R U R' U' R' F R"
     */
    void applyMoves(const std::string& moves);

    /**
     * @brief Scrambles the cube with random moves
     * @param length Number of random moves to apply (default: 25)
     * @return String containing the scramble sequence applied
     * 
     * Applies random moves from the complete move set and returns the 
     * sequence for reference. A length of 20-25 moves typically provides
     * a well-scrambled cube.
     */
    std::string scramble(int length = 25);

    /**
     * @brief Returns a string representation of the cube state
     * @return Debug string showing piece positions and orientations
     * 
     * Format: "Corners: (piece,orientation) ... \nEdges: (piece,orientation) ..."
     * Useful for debugging and verifying cube state.
     */
    std::string toString() const;
        
};

#endif
