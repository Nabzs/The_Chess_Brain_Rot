#ifndef CHESS_RENDERER_HPP
#define CHESS_RENDERER_HPP

#include "board.hpp"
#include "chess_controller.hpp"
#include <SFML/Graphics.hpp>
#include <TGUI/TGUI.hpp>
#include <TGUI/Backend/SFML-Graphics.hpp>

void renderChessBoard(Board& chessBoard, sf::RenderWindow& window, tgui::Gui& gui, SelectedPiece& selectedPiece);
void getBoardOffset(const sf::RenderWindow& window, int numRows, int numCols, float& offsetX, float& offsetY, float& tileSize);
float RAND();
int randInt(int n);

#endif // CHESS_RENDERER_HPP
