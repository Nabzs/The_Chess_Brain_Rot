#include "chess_controller.hpp"
#include <cmath>
#include <iostream>
#include "chess_renderer.hpp"
#include "game_state.hpp"
#include "moves.hpp"
#include "sound_manager.hpp"

int turnCount = 0;

struct PendingPromotion {
    bool active = false;
    int  row;
    int  col;
    bool isWhite;
} pendingPromotion;

bool isWhiteTurn = true;

int drawFatigueDuration()
{
    float u1      = RAND();
    float u2      = RAND();
    float fatigue = 1 + (u1 + u2) * 1.5f;
    return static_cast<int>(std::round(fatigue));
}

struct LastMovedPiece {
    int  row      = -1;
    int  col      = -1;
    bool isWhite  = true;
    bool active   = false;
    int  uniqueID = -1;
};
LastMovedPiece lastMovedPiece;

void tryExpandBoard(Board& board)
{
    float chance = std::min(0.05f + 0.005f * turnCount, 0.5f);
    if (RAND() < chance)
    {
        auto& grid = board.getBoard();

        bool addLeft = RAND() < 0.5f;

        for (auto& row : grid)
        {
            if (addLeft)
                row.insert(row.begin(), Piece());
            else
                row.push_back(Piece());
        }

        // When adding to the left, all columns shift right — fix tracked indices
        if (addLeft)
        {
            if (pendingPromotion.active)
                pendingPromotion.col++;
            if (lastMovedPiece.active)
                lastMovedPiece.col++;
            if (lastMove.startCol >= 0)
            {
                lastMove.startCol++;
                lastMove.endCol++;
            }
        }

        std::cout << "Skibidi Expansion! Le plateau s'agrandit vers la "
                  << (addLeft ? "gauche" : "droite") << "! Plus de rizz, plus d'espace!" << std::endl;
        playEventSound("expand");
    }
}

void selectPiece(Board& board, SelectedPiece& selected, int row, int col)
{
    const Piece& piece = board.getBoard()[row][col];

    if (!piece.type.empty() && ((piece.isWhite && isWhiteTurn) || (!piece.isWhite && !isWhiteTurn)))
    {
        if (piece.fatigueRemaining > 0)
        {
            std::cout << "Cette piece n'a plus de rizz! Elle est fanum taxee!" << std::endl;
            selected.isSelected = false;
            return;
        }

        selected.isSelected = true;
        selected.row        = row;
        selected.col        = col;
    }
    else
    {
        selected.isSelected = false;
    }
}

void checkTwinTowers(Board& board)
{
    auto& grid    = board.getBoard();
    int   numRows = grid.size();
    int   numCols = grid[0].size();
    for (int r = 0; r < numRows; ++r)
    {
        for (int c = 0; c < numCols; ++c)
        {
            Piece& p = grid[r][c];
            if (p.type == "R")
            {
                const std::vector<std::pair<int, int>> dirs = {{0, 1}, {1, 0}, {0, -1}, {-1, 0}};
                for (auto [dr, dc] : dirs)
                {
                    int nr = r + dr, nc = c + dc;
                    if (nr >= 0 && nr < numRows && nc >= 0 && nc < numCols)
                    {
                        Piece& neighbor = grid[nr][nc];
                        if (neighbor.type == "R" && neighbor.isWhite == p.isWhite)
                        {
                            if (RAND() < 0.05f)
                            {
                                std::cout << "Les Tours Jumelles de l'Ohio se sont effondrees! Six Seven Eight Nine Ten!" << std::endl;
                                playEventSound("twin_tower");
                                p        = Piece();
                                neighbor = Piece();
                            }
                        }
                    }
                }
            }
        }
    }
}

void checkForPawnPromotion(Board& board, int row, int col)
{
    Piece& piece = board.getBoard()[row][col];

    if (piece.type == "P" && (row == 0 || row == 7))
    {
        pendingPromotion.active  = true;
        pendingPromotion.row     = row;
        pendingPromotion.col     = col;
        pendingPromotion.isWhite = piece.isWhite;
    }
}

void handlePawnPromotion(Board& board, tgui::Gui& gui)
{
    if (!pendingPromotion.active)
        return;

    if (gui.get("promotionPopup") != nullptr)
        return;

    auto popup = tgui::ChildWindow::create("Skibidi Evolution");
    popup->setSize({250, 250});
    popup->setPosition("(&.width - width) / 2", "(&.height - height) / 2");
    popup->setTitleButtons(tgui::ChildWindow::TitleButton::None);

    auto label = tgui::Label::create("Choisis ton evolution sigma :");
    label->setPosition({"(&.width - width) / 2", 10});
    label->setTextSize(16);
    popup->add(label);

    auto makeButton = [&](const std::string& text, const std::string& pieceType, float yPos) {
        auto btn = tgui::Button::create(text);
        btn->setSize({180, 30});
        btn->setPosition({"(&.width - width) / 2", yPos});
        btn->onPress([&board, &gui, pieceType]() {
            Piece& piece = board.getBoard()[pendingPromotion.row][pendingPromotion.col];
            piece.type = pieceType;
            pendingPromotion.active = false;
            gui.remove(gui.get("promotionPopup"));
            playEventSound("promotion");
        });
        popup->add(btn);
    };

    makeButton("Dame",     "Q",  50);
    makeButton("Tour",     "R",  90);
    makeButton("Fou",      "B",  130);
    makeButton("Cavalier", "Kn", 170);

    gui.add(popup, "promotionPopup");
}

void tryNervousKingMove(Board& board, bool isWhite)
{
    if (RAND() > 0.55f)
        return;

    auto& grid    = board.getBoard();
    int   numRows = grid.size();
    int   numCols = grid[0].size();
    int   kingRow = -1, kingCol = -1;

    for (int r = 0; r < numRows; ++r)
    {
        for (int c = 0; c < numCols; ++c)
        {
            const Piece& p = grid[r][c];
            if (p.type == "K" && p.isWhite == isWhite)
            {
                kingRow = r;
                kingCol = c;
                break;
            }
        }
        if (kingRow != -1)
            break;
    }

    if (kingRow == -1)
        return;

    std::vector<std::pair<int, int>> directions = {
        {-1, -1}, {-1, 0}, {-1, 1}, {0, -1}, {0, 1}, {1, -1}, {1, 0}, {1, 1}
    };

    std::vector<std::pair<int, int>> validMoves;
    for (auto& [dr, dc] : directions)
    {
        int newRow = kingRow + dr;
        int newCol = kingCol + dc;

        if (newRow >= 0 && newRow < numRows && newCol >= 0 && newCol < numCols)
        {
            const Piece& dest = grid[newRow][newCol];
            if (dest.type.empty() || dest.isWhite != isWhite)
            {
                validMoves.emplace_back(newRow, newCol);
            }
        }
    }

    if (validMoves.empty())
        return;

    auto [r, c]            = validMoves[randInt(validMoves.size())];
    grid[r][c]             = grid[kingRow][kingCol];
    grid[kingRow][kingCol] = Piece();

    std::cout << "Le Roi " << (isWhite ? "Blanc" : "Noir") << " a eu la Skibidi Panique! Il s'est deplace tout seul comme un giga chad!" << std::endl;
    playEventSound("panic");
}

bool movePiece(Board& board, SelectedPiece& selected, int newRow, int newCol)
{
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

    Piece& selectedPieceRef = board.getBoard()[selected.row][selected.col];

    if (selectedPieceRef.fatigueRemaining > 0)
    {
        std::cout << "Cette piece est en mode NPC! Encore "
                  << selectedPieceRef.fatigueRemaining << " tours de cooldown skibidi!" << std::endl;
        return false;
    }

    if (isValidMove(selectedPieceRef, selected.row, selected.col, newRow, newCol, board.getBoard(), lastMove))
    {
        if (selectedPieceRef.type == "P" && abs(newCol - selected.col) == 1
            && board.getBoard()[newRow][newCol].type.empty())
        {
            board.getBoard()[selected.row][newCol] = Piece();
        }

        int savedID = selectedPieceRef.uniqueID;
        bool wasCapture = !board.getBoard()[newRow][newCol].type.empty();
        std::string movingType = selectedPieceRef.type;

        Piece moved                                  = std::move(selectedPieceRef);
        board.getBoard()[selected.row][selected.col] = Piece();
        board.getBoard()[newRow][newCol]             = std::move(moved);

        board.getBoard()[newRow][newCol].uniqueID = savedID;

        Piece& movedPiece = board.getBoard()[newRow][newCol];

        // Son de deplacement (par type de piece) + son de capture
        playMoveSound(movingType);
        if (wasCapture)
            playEventSound("capture");

        bool wasSameAsLast = lastMovedPiece.active && lastMovedPiece.uniqueID == movedPiece.uniqueID;
        lastMovedPiece     = {newRow, newCol, movedPiece.isWhite, true, movedPiece.uniqueID};

        if (wasSameAsLast)
        {
            int fatigue                 = drawFatigueDuration();
            movedPiece.fatigueRemaining = fatigue;
            std::cout << "Fanum Tax! Cette piece a ete gyattee deux fois de suite, fatiguee pour "
                      << fatigue << " tours!" << std::endl;
            playEventSound("fatigue");
        }

        if (movedPiece.type == "B" && movedPiece.explosionCountdown > 0)
        {
            movedPiece.explosionCountdown--;
        }

        if (movedPiece.type == "B" && movedPiece.explosionCountdown == 0)
        {
            std::cout << "SKIBIDI BOOM! Le fou a explose comme un Skibidi Toilet!" << std::endl;
            playEventSound("explosion");

            int centerRow = newRow;
            int centerCol = newCol;
            int numRows   = board.getBoard().size();
            int numCols   = board.getBoard()[0].size();

            for (int dr = -2; dr <= 2; ++dr)
            {
                for (int dc = -2; dc <= 2; ++dc)
                {
                    if (abs(dr) + abs(dc) <= 2)
                    {
                        int r = centerRow + dr;
                        int c = centerCol + dc;

                        if (r >= 0 && r < numRows && c >= 0 && c < numCols)
                        {
                            board.getBoard()[r][c] = Piece();
                        }
                    }
                }
            }

            deselectPiece(selected);
            isWhiteTurn = !isWhiteTurn;
            return true;
        }

        lastMove = {selected.row, selected.col, newRow, newCol};

        if (movedPiece.type == "P" && abs(newRow - selected.row) == 2)
            movedPiece.justMovedTwoSquares = true;

        selected.isSelected = false;
        isWhiteTurn         = !isWhiteTurn;

        for (auto& row : board.getBoard())
        {
            for (auto& p : row)
            {
                if (!p.type.empty() && p.isWhite == isWhiteTurn && p.fatigueRemaining > 0)
                {
                    p.fatigueRemaining--;
                }
            }
        }

        checkVictory(board, isWhiteTurn);
        checkForPawnPromotion(board, newRow, newCol);
        tryNervousKingMove(board, isWhiteTurn);
        checkTwinTowers(board);
        tryExpandBoard(board);

        turnCount++;

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

std::vector<std::pair<int, int>> getValidMovesForSelected(const Board& board, const SelectedPiece& selectedPiece)
{
    if (!selectedPiece.isSelected)
        return {};

    const Piece& piece = board.getBoard()[selectedPiece.row][selectedPiece.col];
    return getValidMoves(piece, selectedPiece.row, selectedPiece.col, board.getBoard(), lastMove);
}

void resetGame(Board& board)
{
    board.initBoardRandom(8, 4, 3);
    gameState.gameOver = false;
    gameState.winner   = "";
    isWhiteTurn        = true;
    turnCount          = 0;
    lastMovedPiece     = {-1, -1, true, false, -1};
    lastMove           = {};
}
