#ifndef GAME_STATE_HPP
#define GAME_STATE_HPP

#include <string>
#include "board.hpp"
#include "imgui.h"

// Structure pour stocker l'état du jeu
struct GameState {
    bool        gameOver = false;
    std::string winner   = "";
};

// Déclaration d'une instance globale de l'état du jeu
extern GameState gameState;

// Vérifie si la partie est terminée (roi capturé)
void checkVictory(Board& board, bool isWhiteTurn);

// Affiche la popup de victoire si la partie est terminée
void showVictoryPopup(Board& board);

#endif // GAME_STATE_HPP
