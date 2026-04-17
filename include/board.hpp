#ifndef BOARD_HPP
#define BOARD_HPP

#include <vector>
#include "piece.hpp"

class Board
{
public:
    Board();

    void resetBoard();  
    void initBoardRandom(int numPawnSwaps, int numMajorSwaps, int numFarSwaps);
    
    std::vector<std::vector<Piece>>& getBoard();
    const std::vector<std::vector<Piece>>& getBoard() const;

private:
    std::vector<std::vector<Piece>> board;
};



#endif
