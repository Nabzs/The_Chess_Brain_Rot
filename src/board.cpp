#include "board.hpp"
#include <vector>
#include <algorithm>  // std::swap
#include "chess_renderer.hpp"
#include <cmath>


static int GLOBAL_PIECE_ID = 0;

int drawPoisson(float lambda) {
    float L = exp(-lambda);
    int k = 0;
    float p = 1.0;

    do {
        k++;
        p *= RAND();
    } while (p > L);

    int result = k - 1;
    if (result < 2) result = 2;  // minimum 2 mouvements avant explosion
    return result;
}




// Constructeur classique inchangé
Board::Board()
{
    initBoardRandom(3, 2, 4);  
    // Exemple clair : 3 échanges de pions, 2 échanges de majeures, 4 déplacements éloignés très visibles
}

// Initialisation avec permutations aléatoires 
void Board::initBoardRandom(int numPawnSwaps, int numMajorSwaps, int numFarSwaps)
{
    resetBoard();
    

    // Positions initiales des pions
    std::vector<std::pair<int, int>> whitePawnPositions = {{6,0}, {6,1}, {6,2}, {6,3}, {6,4}, {6,5}, {6,6}, {6,7}};
    std::vector<std::pair<int, int>> blackPawnPositions = {{1,0}, {1,1}, {1,2}, {1,3}, {1,4}, {1,5}, {1,6}, {1,7}};

    // Positions des pièces majeures
    std::vector<std::pair<int, int>> whiteMajorPositions = {{7,0}, {7,1}, {7,2}, {7,3}, {7,4}, {7,5}, {7,6}, {7,7}};
    std::vector<std::pair<int, int>> blackMajorPositions = {{0,0}, {0,1}, {0,2}, {0,3}, {0,4}, {0,5}, {0,6}, {0,7}};

    // Positions vides éloignées (rangée 5 pour blancs, rangée 2 pour noirs)
    std::vector<std::pair<int, int>> whiteFarEmptyPositions = {{5,0}, {5,1}, {5,2}, {5,3}, {5,4}, {5,5}, {5,6}, {5,7}};
    std::vector<std::pair<int, int>> blackFarEmptyPositions = {{2,0}, {2,1}, {2,2}, {2,3}, {2,4}, {2,5}, {2,6}, {2,7}};

    // Permutations aléatoires des pions entre eux
    for (int i = 0; i < numPawnSwaps; ++i)
    {
        std::swap(whitePawnPositions[randInt(8)], whitePawnPositions[randInt(8)]);
        std::swap(blackPawnPositions[randInt(8)], blackPawnPositions[randInt(8)]);
    }

    // Permutations aléatoires des pièces majeures
    for (int i = 0; i < numMajorSwaps; ++i)
    {
        std::swap(whiteMajorPositions[randInt(8)], whiteMajorPositions[randInt(8)]);
        std::swap(blackMajorPositions[randInt(8)], blackMajorPositions[randInt(8)]);
    }

    // Déplacement très visible de pions et majeures sur la rangée éloignée vide
    for (int i = 0; i < numFarSwaps; ++i)
    {
        // Pour les blancs
        if(RAND() < 0.5f) // 50% chance déplacer pion loin
            std::swap(whitePawnPositions[randInt(8)], whiteFarEmptyPositions[randInt(8)]);
        else // 50% déplacer pièce majeure loin
            std::swap(whiteMajorPositions[randInt(8)], whiteFarEmptyPositions[randInt(8)]);

        // Pour les noirs
        if(RAND() < 0.5f)
            std::swap(blackPawnPositions[randInt(8)], blackFarEmptyPositions[randInt(8)]);
        else
            std::swap(blackMajorPositions[randInt(8)], blackFarEmptyPositions[randInt(8)]);
    }

    // Vider les lignes 0,1,2,5,6,7
    for (int col = 0; col < 8; ++col)
    {
        board[0][col] = Piece();
        board[1][col] = Piece();
        board[2][col] = Piece();
        board[5][col] = Piece();
        board[6][col] = Piece();
        board[7][col] = Piece();
    }

    // Replacer les pions blancs
    for (const auto& pos : whitePawnPositions) {
        board[pos.first][pos.second] = Piece("P", true);
        board[pos.first][pos.second].uniqueID = GLOBAL_PIECE_ID++;
    }
    

    // Replacer les pions noirs
    for (const auto& pos : blackPawnPositions) {
        board[pos.first][pos.second] = Piece("P", false);
        board[pos.first][pos.second].uniqueID = GLOBAL_PIECE_ID++;
    }
    

    // Pièces majeures blanches (ordre initial)
    std::vector<std::string> whiteMajors = {"R","Kn","B","Q","K","B","Kn","R"};
for (size_t i = 0; i < whiteMajorPositions.size(); ++i) {
    Piece piece(whiteMajors[i], true);
    piece.uniqueID = GLOBAL_PIECE_ID++;
    
    if (whiteMajors[i] == "B") {
        piece.explosionCountdown = drawPoisson(4); // moyenne lambda = 4
    }
    
    board[whiteMajorPositions[i].first][whiteMajorPositions[i].second] = piece;
    
}


    // Pièces majeures noires (ordre initial)
    std::vector<std::string> blackMajors = {"R","Kn","B","Q","K","B","Kn","R"};
for (size_t i = 0; i < blackMajorPositions.size(); ++i) {
    Piece piece(blackMajors[i], false);
    piece.uniqueID = GLOBAL_PIECE_ID++;
    
    if (blackMajors[i] == "B") {
        piece.explosionCountdown = drawPoisson(4);
    }
    
    board[blackMajorPositions[i].first][blackMajorPositions[i].second] = piece;
    
}

}

void Board::resetBoard()
{
    board = std::vector<std::vector<Piece>>(8, std::vector<Piece>(8));

    // Rangée 0 : pièces noires
    std::vector<std::string> blackMajors = {"R", "Kn", "B", "Q", "K", "B", "Kn", "R"};
    for (int col = 0; col < 8; ++col) {
        board[0][col] = Piece{blackMajors[col], false};
        board[0][col].uniqueID = GLOBAL_PIECE_ID++;
    }

    // Rangée 1 : pions noirs
    for (int col = 0; col < 8; ++col) {
        board[1][col] = Piece{"P", false};
        board[1][col].uniqueID = GLOBAL_PIECE_ID++;
    }

    // Rangées 2 à 5 : cases vides
    for (int row = 2; row <= 5; ++row) {
        for (int col = 0; col < 8; ++col) {
            board[row][col] = Piece{"", false}; // peu importe le bool ici
        }
    }

    // Rangée 6 : pions blancs
    for (int col = 0; col < 8; ++col) {
        board[6][col] = Piece{"P", true};
        board[6][col].uniqueID = GLOBAL_PIECE_ID++;
    }

    // Rangée 7 : pièces blanches
    std::vector<std::string> whiteMajors = {"R", "Kn", "B", "Q", "K", "B", "Kn", "R"};
    for (int col = 0; col < 8; ++col) {
        board[7][col] = Piece{whiteMajors[col], true};
        board[7][col].uniqueID = GLOBAL_PIECE_ID++;
    }
}




// Accesseurs inchangés
std::vector<std::vector<Piece>>& Board::getBoard()
{
    return board;
}

const std::vector<std::vector<Piece>>& Board::getBoard() const
{
    return board;
}
