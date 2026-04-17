#include "2D/board.hpp"

Board::Board()
{
    // Initialisation du plateau avec les pièces en position de départ
    board = {
        {{"R", false}, {"Kn", false}, {"B", false}, {"Q", false}, {"K", false}, {"B", false}, {"Kn", false}, {"R", false}},
        {{"P", false}, {"P", false}, {"P", false}, {"P", false}, {"P", false}, {"P", false}, {"P", false}, {"P", false}},
        {{"", false}, {"", false}, {"", false}, {"", false}, {"", false}, {"", false}, {"", false}, {"", false}},
        {{"", false}, {"", false}, {"", false}, {"", false}, {"", false}, {"", false}, {"", false}, {"", false}},
        {{"", false}, {"", false}, {"", false}, {"", false}, {"", false}, {"", false}, {"", false}, {"", false}},
        {{"", false}, {"", false}, {"", false}, {"", false}, {"", false}, {"", false}, {"", false}, {"", false}},
        {{"P", true}, {"P", true}, {"P", true}, {"P", true}, {"P", true}, {"P", true}, {"P", true}, {"P", true}},
        {{"R", true}, {"Kn", true}, {"B", true}, {"Q", true}, {"K", true}, {"B", true}, {"Kn", true}, {"R", true}}
    };
}

std::vector<std::vector<Piece>>& Board::getBoard()
{
    return board;
}

const std::vector<std::vector<Piece>>& Board::getBoard() const
{
    return board;
}

void Board::resetBoard()
{
    board = {
        {{"R", false}, {"Kn", false}, {"B", false}, {"Q", false}, {"K", false}, {"B", false}, {"Kn", false}, {"R", false}},
        {{"P", false}, {"P", false}, {"P", false}, {"P", false}, {"P", false}, {"P", false}, {"P", false}, {"P", false}},
        {{"", false}, {"", false}, {"", false}, {"", false}, {"", false}, {"", false}, {"", false}, {"", false}},
        {{"", false}, {"", false}, {"", false}, {"", false}, {"", false}, {"", false}, {"", false}, {"", false}},
        {{"", false}, {"", false}, {"", false}, {"", false}, {"", false}, {"", false}, {"", false}, {"", false}},
        {{"", false}, {"", false}, {"", false}, {"", false}, {"", false}, {"", false}, {"", false}, {"", false}},
        {{"P", true}, {"P", true}, {"P", true}, {"P", true}, {"P", true}, {"P", true}, {"P", true}, {"P", true}},
        {{"R", true}, {"Kn", true}, {"B", true}, {"Q", true}, {"K", true}, {"B", true}, {"Kn", true}, {"R", true}}
    };
}
