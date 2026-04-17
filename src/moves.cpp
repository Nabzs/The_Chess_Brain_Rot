#include "moves.hpp"
#include <cmath>
#include <iostream> // Ajout pour le debug

LastMove lastMove;

std::vector<std::pair<int, int>> getValidMoves(const Piece& piece, int row, int col, const std::vector<std::vector<Piece>>& board, const LastMove& lastMove)
{
    std::vector<std::pair<int, int>> moves;

    int numRows = board.size();
    int numCols = board[0].size();

    for (int r = 0; r < numRows; ++r)
    {
        for (int c = 0; c < numCols; ++c)
        {
            if (isValidMove(piece, row, col, r, c, board, lastMove))
            {
                moves.emplace_back(r, c);
            }
        }
    }
    return moves;
}

bool isValidMove(const Piece& piece, 
    int startRow, int startCol, 
    int endRow, int endCol, 
    const std::vector<std::vector<Piece>>& board, 
    const LastMove& lastMove)
{
    int numRows = board.size();
    int numCols = board[0].size();

    if (endRow < 0 || endRow >= numRows || endCol < 0 || endCol >= numCols)
        return false;

    // Gestion des mouvements des pions
    if (piece.type == "P")
    {
        int direction = piece.isWhite ? -1 : 1;

        if (startCol == endCol && startRow + direction == endRow && board[endRow][endCol].type.empty())
            return true;

        if (startCol == endCol && startRow + 2 * direction == endRow && board[endRow][endCol].type.empty() && board[startRow + direction][endCol].type.empty())
        {
            if ((piece.isWhite && startRow == 6) || (!piece.isWhite && startRow == 1))
                return true;
        }

        if (abs(endCol - startCol) == 1 && endRow == startRow + direction && board[endRow][endCol].type.empty()) {
            const Piece& adjacentPiece = board[startRow][endCol];
            if (adjacentPiece.type == "P" && 
                abs(lastMove.startRow - lastMove.endRow) == 2 &&
                lastMove.endRow == startRow && lastMove.endCol == endCol &&
                board[endRow][endCol].type.empty()) {
                return true;
            }
        }

        if (std::abs(startCol - endCol) == 1 && startRow + direction == endRow && !board[endRow][endCol].type.empty() && board[endRow][endCol].isWhite != piece.isWhite)
            return true;
    }

    if (piece.type == "R")
    {
        if (startRow == endRow || startCol == endCol)
        {
            int rowStep = (endRow == startRow) ? 0 : (endRow > startRow ? 1 : -1);
            int colStep = (endCol == startCol) ? 0 : (endCol > startCol ? 1 : -1);

            int row = startRow + rowStep;
            int col = startCol + colStep;

            while (row != endRow || col != endCol)
            {
                if (!board[row][col].type.empty())
                    return false;
                row += rowStep;
                col += colStep;
            }
            return board[endRow][endCol].type.empty() || board[endRow][endCol].isWhite != piece.isWhite;
        }
    }

    if (piece.type == "B")
    {
        if (std::abs(startRow - endRow) == std::abs(startCol - endCol) && startRow != endRow && startCol != endCol)
        {
            int rowStep = (endRow > startRow) ? 1 : -1;
            int colStep = (endCol > startCol) ? 1 : -1;

            int row = startRow + rowStep;
            int col = startCol + colStep;

            while (row != endRow && col != endCol)
            {
                if (row < 0 || row >= numRows || col < 0 || col >= numCols)
                    return false;
                if (!board[row][col].type.empty())
                    return false;
                row += rowStep;
                col += colStep;
            }
            return board[endRow][endCol].type.empty() || board[endRow][endCol].isWhite != piece.isWhite;
        }
    }

    if (piece.type == "Kn")
    {
        int knightMoves[8][2] = {{-2, -1}, {-2, 1}, {2, -1}, {2, 1},
                                 {-1, -2}, {-1, 2}, {1, -2}, {1, 2}};

        for (auto& move : knightMoves)
        {
            if (startRow + move[0] == endRow && startCol + move[1] == endCol)
            {
                return board[endRow][endCol].type.empty() || board[endRow][endCol].isWhite != piece.isWhite;
            }
        }
    }

    if (piece.type == "K")
    {
        if (std::abs(startRow - endRow) <= 1 && std::abs(startCol - endCol) <= 1)
        {
            return board[endRow][endCol].type.empty() || board[endRow][endCol].isWhite != piece.isWhite;
        }
    }

    if (piece.type == "Q")
    {
        if (startRow == endRow || startCol == endCol)
        {
            int rowStep = (endRow == startRow) ? 0 : (endRow > startRow ? 1 : -1);
            int colStep = (endCol == startCol) ? 0 : (endCol > startCol ? 1 : -1);

            int row = startRow + rowStep;
            int col = startCol + colStep;

            while (row != endRow || col != endCol)
            {
                if (!board[row][col].type.empty())
                    return false;
                row += rowStep;
                col += colStep;
            }
            return board[endRow][endCol].type.empty() || board[endRow][endCol].isWhite != piece.isWhite;
        }

        if (std::abs(startRow - endRow) == std::abs(startCol - endCol))
        {
            int rowStep = (endRow > startRow) ? 1 : -1;
            int colStep = (endCol > startCol) ? 1 : -1;

            int row = startRow + rowStep;
            int col = startCol + colStep;

            while (row != endRow && col != endCol)
            {
                if (row < 0 || row >= numRows || col < 0 || col >= numCols)
                    return false;
                if (!board[row][col].type.empty())
                    return false;
                row += rowStep;
                col += colStep;
            }
            return board[endRow][endCol].type.empty() || board[endRow][endCol].isWhite != piece.isWhite;
        }
    }

    return false;
}
