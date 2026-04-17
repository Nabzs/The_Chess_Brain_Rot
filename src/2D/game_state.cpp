#include "2D/game_state.hpp"
#include <iostream>
#include "2D/chess_controller.hpp"

// Déclaration de l'instance globale de l'état du jeu
GameState gameState;

void checkVictory(Board& board, bool isWhiteTurn)
{
    bool whiteKingAlive = false;
    bool blackKingAlive = false;

    for (const auto& row : board.getBoard())
    {
        for (const auto& piece : row)
        {
            if (piece.type == "K")
            {
                if (piece.isWhite)
                    whiteKingAlive = true;
                else
                    blackKingAlive = true;
            }
        }
    }

    if (!whiteKingAlive)
    {
        gameState.gameOver = true;
        gameState.winner   = "Les Noirs gagnent !";
        std::cout << gameState.winner << std::endl;
    }
    else if (!blackKingAlive)
    {
        gameState.gameOver = true;
        gameState.winner   = "Les Blancs gagnent !";
        std::cout << gameState.winner << std::endl;
    }
}

void showVictoryPopup(Board& board)
{
    if (gameState.gameOver)
    {
        ImGui::OpenPopup("Partie terminée");
    }

    if (ImGui::BeginPopupModal("Partie terminée", NULL, ImGuiWindowFlags_AlwaysAutoResize))
    {
        ImGui::Text("%s", gameState.winner.c_str());
        ImGui::Separator();

        if (ImGui::Button("Rejouer"))
        {
            resetGame(board);
            ImGui::CloseCurrentPopup();
        }

        ImGui::SameLine();

        if (ImGui::Button("Quitter"))
        {
            exit(0); // Quitter l'application
        }

        ImGui::EndPopup();
    }
}
