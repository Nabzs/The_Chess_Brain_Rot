// piece.hpp
#ifndef PIECE_HPP
#define PIECE_HPP

#include <string>

struct Piece {
    std::string type;
    bool        isWhite = true;
    bool        justMovedTwoSquares = false;

    Piece(std::string t = "", bool white = true)
        : type(t), isWhite(white) {}
};

#endif // PIECE_HPP
