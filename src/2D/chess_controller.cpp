#include "2D/chess_controller.hpp"
#include "2D/game_state.hpp"
#include "2D/moves.hpp"
#include "imgui.h"

PendingPromotion pendingPromotion;

bool isWhiteTurn = true;

void selectPiece(Board& board, SelectedPiece& selected, int row, int col)
{
    const Piece& piece = board.getBoard()[row][col];

    // Vérifier si la pièce appartient au joueur dont c'est le tour
    if (!piece.type.empty() && ((piece.isWhite && isWhiteTurn) || (!piece.isWhite && !isWhiteTurn)))
    {
        selected.isSelected = true;
        selected.row        = row;
        selected.col        = col;
    }
    else
    {
        selected.isSelected = false; // Désélectionner si la pièce ne correspond pas au tour
    }
}

// Fonction pour vérifier si un pion atteint la dernière rangée
void checkForPawnPromotion(Board& board, int row, int col)
{
    Piece& piece = board.getBoard()[row][col];

    if (piece.type == "P" && (row == 0 || row == 7)) // Si un pion atteint la dernière rangée
    {
        // Stocker les informations pour la promotion
        pendingPromotion.active  = true;
        pendingPromotion.row     = row;
        pendingPromotion.col     = col;
        pendingPromotion.isWhite = piece.isWhite;

        // Ouvrir la fenêtre modale
        ImGui::OpenPopup("Promotion");
    }
}

// Fonction pour gérer la promotion du pion
void handlePawnPromotion(Board& board)
{
    if (pendingPromotion.active && ImGui::BeginPopupModal("Promotion", NULL, ImGuiWindowFlags_AlwaysAutoResize))
    {
        ImGui::Text("Choisissez une pièce pour la promotion :");

        Piece& piece = board.getBoard()[pendingPromotion.row][pendingPromotion.col];

        if (ImGui::Button("Dame"))
        {
            piece.type              = "Q";
            pendingPromotion.active = false;
            ImGui::CloseCurrentPopup();
        }
        if (ImGui::Button("Tour"))
        {
            piece.type              = "R";
            pendingPromotion.active = false;
            ImGui::CloseCurrentPopup();
        }
        if (ImGui::Button("Fou"))
        {
            piece.type              = "B";
            pendingPromotion.active = false;
            ImGui::CloseCurrentPopup();
        }
        if (ImGui::Button("Cavalier"))
        {
            piece.type              = "Kn";
            pendingPromotion.active = false;
            ImGui::CloseCurrentPopup();
        }

        ImGui::EndPopup();
    }
}

bool movePiece(Board& board, SelectedPiece& selected, int newRow, int newCol)
{
    // Ajouter ceci au début de la fonction movePiece
    for (auto& row : board.getBoard())
    {
        for (auto& p : row)
        {
            if (p.type == "P")
            {
                p.justMovedTwoSquares = false;
            }
        }
    }

    if (!selected.isSelected)
        return false;

    Piece& piece = board.getBoard()[selected.row][selected.col];

    if (isValidMove(piece, selected.row, selected.col, newRow, newCol, board.getBoard(), lastMove))
    {
        // Gérer la prise en passant (effacer le pion adverse)
        if (piece.type == "P" && abs(newCol - selected.col) == 1 && board.getBoard()[newRow][newCol].type.empty())
        {
            board.getBoard()[selected.row][newCol] = {"", false};
        }

        // Effectuer le déplacement
        board.getBoard()[newRow][newCol]             = piece;
        board.getBoard()[selected.row][selected.col] = {"", false};

        // Mettre à jour le dernier coup
        lastMove = {selected.row, selected.col, newRow, newCol, board.getBoard()[newRow][newCol]};

        // Marquer le pion si déplacement de deux cases
        if (piece.type == "P" && abs(newRow - selected.row) == 2)
            board.getBoard()[newRow][newCol].justMovedTwoSquares = true;

        selected.isSelected = false;
        isWhiteTurn         = !isWhiteTurn;

        checkVictory(board, isWhiteTurn);
        checkForPawnPromotion(board, newRow, newCol);

        return true;
    }

    return false;
}

void deselectPiece(SelectedPiece& selected)
{
    selected.isSelected = false;
    selected.row        = -1;
    selected.col        = -1;
}

// chess_controller.cpp
std::vector<std::pair<int, int>> getValidMovesForSelected(const Board& board, const SelectedPiece& selectedPiece)
{
    if (!selectedPiece.isSelected)
        return {};

    const Piece& piece = board.getBoard()[selectedPiece.row][selectedPiece.col];
    return getValidMoves(piece, selectedPiece.row, selectedPiece.col, board.getBoard(), lastMove);
}

void resetGame(Board& board)
{
    board.resetBoard();
    gameState.gameOver = false;
    gameState.winner   = "";
    isWhiteTurn        = true;
    lastMove           = {};
}