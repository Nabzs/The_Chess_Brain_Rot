// piece.hpp
#ifndef PIECE_HPP
#define PIECE_HPP

#include <string>

struct Piece {
    std::string type;
    bool isWhite;
    int uniqueID = -1;
    int explosionCountdown = -1;
    int fatigueRemaining = 0;
    bool justMovedTwoSquares = false;

    Piece(std::string t = "", bool white = true)
        : type(t), isWhite(white) {}
        float r = 1.0f;
float g = 1.0f;
float b = 1.0f;
bool colorInitialized = false;


};


#endif // PIECE_HPP
