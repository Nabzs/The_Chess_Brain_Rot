#include "game_state.hpp"
#include <iostream>
#include "chess_controller.hpp"
#include "sound_manager.hpp"

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
        gameState.winner   = "Les Noirs sont sigma! GG no rizz!";
        std::cout << gameState.winner << std::endl;
        playEventSound("victory");
    }
    else if (!blackKingAlive)
    {
        gameState.gameOver = true;
        gameState.winner   = "Les Blancs sont sigma! GG no rizz!";
        std::cout << gameState.winner << std::endl;
        playEventSound("victory");
    }
}

void showVictoryPopup(Board& board, tgui::Gui& gui)
{
    if (!gameState.gameOver)
        return;

    if (gui.get("victoryPopup") != nullptr)
        return;

    auto popup = tgui::ChildWindow::create("Skibidi Game Over");
    popup->setSize({300, 180});
    popup->setPosition("(&.width - width) / 2", "(&.height - height) / 2");
    popup->setTitleButtons(tgui::ChildWindow::TitleButton::None);

    auto label = tgui::Label::create(gameState.winner);
    label->setPosition({"(&.width - width) / 2", 20});
    label->setTextSize(20);
    popup->add(label);

    auto btnReplay = tgui::Button::create("Skibidi Retry");
    btnReplay->setSize({100, 35});
    btnReplay->setPosition({30, 100});
    btnReplay->onPress([&board, &gui]() {
        resetGame(board);
        gui.remove(gui.get("victoryPopup"));
    });
    popup->add(btnReplay);

    auto btnQuit = tgui::Button::create("Rage Quit");
    btnQuit->setSize({100, 35});
    btnQuit->setPosition({170, 100});
    btnQuit->onPress([]() {
        exit(0);
    });
    popup->add(btnQuit);

    gui.add(popup, "victoryPopup");
}
