// moves.hpp
#ifndef MOVES_HPP
#define MOVES_HPP

#include <vector>
#include "piece.hpp"

struct LastMove {
    int startRow{-1}, startCol{-1};
    int endRow{-1}, endCol{-1};
    Piece movedPiece;
};

extern LastMove lastMove; // déclaration globale

std::vector<std::pair<int, int>> getValidMoves(const Piece& piece, int row, int col, const std::vector<std::vector<Piece>>& board, const LastMove& lastMove);

bool isValidMove(const Piece& piece, int startRow, int startCol, int endRow, int endCol, const std::vector<std::vector<Piece>>& board, const LastMove& lastMove);

#endif // MOVES_HPP
