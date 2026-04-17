#include <SFML/Graphics.hpp>
#include <TGUI/TGUI.hpp>
#include <TGUI/Backend/SFML-Graphics.hpp>
#include <iostream>
#include <fstream>
#include "board.hpp"
#include "chess_renderer.hpp"
#include "chess_controller.hpp"
#include "texture_manager.hpp"
#include "sound_manager.hpp"

#ifdef _WIN32
#include <windows.h>
#endif

// Get the directory where the .exe lives
static std::string getExeDir()
{
#ifdef _WIN32
    char buf[MAX_PATH];
    GetModuleFileNameA(NULL, buf, MAX_PATH);
    std::string path(buf);
    auto pos = path.find_last_of("\\/");
    return (pos != std::string::npos) ? path.substr(0, pos) : ".";
#else
    return ".";
#endif
}

int main()
{
    std::string exeDir = getExeDir();
    std::ofstream logFile(exeDir + "/chess_log.txt");

    try {
        logFile << "Exe dir: " << exeDir << std::endl;
        logFile << "Creating window..." << std::endl;
        logFile.flush();

        sf::VideoMode desktop = sf::VideoMode::getDesktopMode();
        sf::RenderWindow window(desktop, "Chess Brainrot", sf::Style::Fullscreen);
        window.setFramerateLimit(60);
        logFile << "Window OK" << std::endl;
        logFile.flush();

        tgui::Gui gui(window);
        logFile << "TGUI OK" << std::endl;
        logFile.flush();

        std::string assetsPath = exeDir + "/assets";
        loadAllTextures(assetsPath);
        logFile << "Textures OK" << std::endl;
        logFile.flush();

        loadAllSounds(assetsPath);
        logFile << "Sounds OK" << std::endl;
        logFile.flush();

        Board chessBoard;
        chessBoard.initBoardRandom(8, 4, 3);
        logFile << "Board OK" << std::endl;
        logFile.flush();

        SelectedPiece selectedPiece;

        while (window.isOpen())
        {
            sf::Event event;
            while (window.pollEvent(event))
            {
                gui.handleEvent(event);

                if (event.type == sf::Event::Closed)
                    window.close();

                if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape)
                    window.close();

                if (event.type == sf::Event::MouseButtonPressed)
                {
                    if (gui.get("promotionPopup") != nullptr || gui.get("victoryPopup") != nullptr)
                        continue;

                    auto& board = chessBoard.getBoard();
                    int numRows = board.size();
                    int numCols = board[0].size();

                    float tileSize, offsetX, offsetY;
                    getBoardOffset(window, numRows, numCols, offsetX, offsetY, tileSize);

                    int clickedCol = static_cast<int>((event.mouseButton.x - offsetX) / tileSize);
                    int clickedRow = static_cast<int>((event.mouseButton.y - offsetY) / tileSize);

                    if (clickedRow >= 0 && clickedRow < numRows && clickedCol >= 0 && clickedCol < numCols)
                    {
                        if (event.mouseButton.button == sf::Mouse::Left)
                        {
                            if (selectedPiece.isSelected)
                                movePiece(chessBoard, selectedPiece, clickedRow, clickedCol);
                            else
                                selectPiece(chessBoard, selectedPiece, clickedRow, clickedCol);
                        }
                        else if (event.mouseButton.button == sf::Mouse::Right)
                        {
                            deselectPiece(selectedPiece);
                        }
                    }
                }
            }

            window.clear();
            renderChessBoard(chessBoard, window, gui, selectedPiece);
            gui.draw();
            window.display();
        }

    } catch (const std::exception& e) {
        logFile << "FATAL: " << e.what() << std::endl;
#ifdef _WIN32
        MessageBoxA(NULL, e.what(), "Chess Brainrot - Error", MB_OK | MB_ICONERROR);
#endif
        return 1;
    }

    logFile << "Clean exit." << std::endl;
    return 0;
}
