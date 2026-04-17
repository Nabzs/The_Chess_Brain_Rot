#ifndef GAME_STATE_HPP
#define GAME_STATE_HPP

#include <string>
#include "board.hpp"
#include <TGUI/TGUI.hpp>
#include <TGUI/Backend/SFML-Graphics.hpp>

struct GameState {
    bool        gameOver = false;
    std::string winner   = "";
};

extern GameState gameState;

void checkVictory(Board& board, bool isWhiteTurn);
void showVictoryPopup(Board& board, tgui::Gui& gui);

#endif // GAME_STATE_HPP
