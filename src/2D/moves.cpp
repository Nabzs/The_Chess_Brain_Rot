#include "2D/moves.hpp"
#include <cmath>
#include <iostream> // Ajout pour le debug

LastMove lastMove;

std::vector<std::pair<int, int>> getValidMoves(const Piece& piece, int row, int col, const std::vector<std::vector<Piece>>& board, const LastMove& lastMove)
{
    std::vector<std::pair<int, int>> moves;

    for (int r = 0; r < 8; ++r)
    {
        for (int c = 0; c < 8; ++c)
        {
            if (isValidMove(piece, row, col, r, c, board, lastMove))
            {
                moves.emplace_back(r, c);
            }
        }
    }
    return moves;
}

bool isValidMove(const Piece& piece, int startRow, int startCol, int endRow, int endCol, const std::vector<std::vector<Piece>>& board, const LastMove& lastMove)
{
    // Vérifier que la case de destination est bien dans les limites du plateau
    if (endRow < 0 || endRow >= 8 || endCol < 0 || endCol >= 8)
        return false;

    // Gestion des mouvements des pions
    if (piece.type == "P")
    {
        int direction = piece.isWhite ? -1 : 1; // Blancs avancent vers le haut (-1), noirs vers le bas (+1)

        // Déplacement simple d'une case vers l'avant
        if (startCol == endCol && startRow + direction == endRow && board[endRow][endCol].type.empty())
            return true;

        // Premier déplacement : possibilité d'avancer de deux cases
        if (startCol == endCol && startRow + 2 * direction == endRow && board[endRow][endCol].type.empty() && board[startRow + direction][endCol].type.empty())
        {
            if ((piece.isWhite && startRow == 6) || (!piece.isWhite && startRow == 1))
                return true;
        }

        if (abs(endCol - startCol) == 1 && endRow == startRow + direction && board[endRow][endCol].type.empty())
        {
            const Piece& adjacentPiece = board[startRow][endCol];
            if (adjacentPiece.type == "P"
                && adjacentPiece.isWhite != piece.isWhite
                && abs(lastMove.startRow - lastMove.endRow) == 2
                && lastMove.endRow == startRow
                && lastMove.endCol == endCol)
            {
                return true;
            }
        }

        // Capture diagonale
        if (std::abs(startCol - endCol) == 1 && startRow + direction == endRow && !board[endRow][endCol].type.empty() && board[endRow][endCol].isWhite != piece.isWhite)
            return true;
    }

    // Gestion des mouvements des tours
    if (piece.type == "R") // "R" pour Rook (Tour)
    {
        // Déplacement en ligne droite (horizontal ou vertical)
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

    // Gestion des mouvements des fous
    if (piece.type == "B") // "B" pour Bishop (Fou)
    {
        // Vérification du déplacement en diagonale
        if (std::abs(startRow - endRow) == std::abs(startCol - endCol) && startRow != endRow && startCol != endCol)
        {
            int rowStep = (endRow > startRow) ? 1 : -1;
            int colStep = (endCol > startCol) ? 1 : -1;

            int row = startRow + rowStep;
            int col = startCol + colStep;

            while (row != endRow && col != endCol)
            {
                if (row < 0 || row >= 8 || col < 0 || col >= 8) // Sécurité contre les indices hors limites
                    return false;
                if (!board[row][col].type.empty())
                    return false;
                row += rowStep;
                col += colStep;
            }
            return board[endRow][endCol].type.empty() || board[endRow][endCol].isWhite != piece.isWhite;
        }
    }

    // Gestion des mouvements du cavalier
    if (piece.type == "Kn") // "N" pour Knight (Cavalier)
    {
        int knightMoves[8][2] = {{-2, -1}, {-2, 1}, {2, -1}, {2, 1}, {-1, -2}, {-1, 2}, {1, -2}, {1, 2}};

        for (auto& move : knightMoves)
        {
            if (startRow + move[0] == endRow && startCol + move[1] == endCol)
            {
                return board[endRow][endCol].type.empty() || board[endRow][endCol].isWhite != piece.isWhite;
            }
        }
    }

    // Gestion des mouvements du roi
    if (piece.type == "K") // "K" pour King (Roi)
    {
        // Le roi peut se déplacer d'une case dans toutes les directions
        if (std::abs(startRow - endRow) <= 1 && std::abs(startCol - endCol) <= 1)
        {
            return board[endRow][endCol].type.empty() || board[endRow][endCol].isWhite != piece.isWhite;
        }
    }

    // Gestion des mouvements de la reine
    if (piece.type == "Q") // "Q" pour Queen (Reine)
    {
        // La reine se déplace en ligne droite (horizontalement ou verticalement)
        if (startRow == endRow || startCol == endCol)
        {
            int rowStep = (endRow == startRow) ? 0 : (endRow > startRow ? 1 : -1);
            int colStep = (endCol == startCol) ? 0 : (endCol > startCol ? 1 : -1);

            int row = startRow + rowStep;
            int col = startCol + colStep;

            while (row != endRow || col != endCol)
            {
                if (!board[row][col].type.empty())
                    return false; // Bloqué par une pièce
                row += rowStep;
                col += colStep;
            }
            return board[endRow][endCol].type.empty() || board[endRow][endCol].isWhite != piece.isWhite;
        }

        // La reine se déplace aussi en diagonale (comme un fou)
        if (std::abs(startRow - endRow) == std::abs(startCol - endCol))
        {
            int rowStep = (endRow > startRow) ? 1 : -1;
            int colStep = (endCol > startCol) ? 1 : -1;

            int row = startRow + rowStep;
            int col = startCol + colStep;

            while (row != endRow && col != endCol)
            {
                if (row < 0 || row >= 8 || col < 0 || col >= 8) // Sécurité contre les indices hors limites
                    return false;
                if (!board[row][col].type.empty())
                    return false; // Bloqué par une pièce
                row += rowStep;
                col += colStep;
            }
            return board[endRow][endCol].type.empty() || board[endRow][endCol].isWhite != piece.isWhite;
        }
    }

    return false; // Mouvement invalide par défaut
}
