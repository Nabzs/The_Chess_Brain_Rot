#ifndef CHESS_CONTROLLER_HPP
#define CHESS_CONTROLLER_HPP

#include "board.hpp"
#include <TGUI/TGUI.hpp>
#include <TGUI/Backend/SFML-Graphics.hpp>

struct SelectedPiece {
    int  row = -1, col = -1;
    bool isSelected = false;
};

extern bool isWhiteTurn;

void                             selectPiece(Board& board, SelectedPiece& selected, int row, int col);
bool                             movePiece(Board& board, SelectedPiece& selected, int newRow, int newCol);
void                             deselectPiece(SelectedPiece& selected);
std::vector<std::pair<int, int>> getValidMovesForSelected(const Board& board, const SelectedPiece& selectedPiece);
void                             handlePawnPromotion(Board& board, tgui::Gui& gui);
void                             resetGame(Board& board);

#endif // CHESS_CONTROLLER_HPP
