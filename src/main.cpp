#include <imgui.h>
#include "2D/board.hpp"
#include "2D/chess_renderer.hpp"
#include "quick_imgui/quick_imgui.hpp"

int main()
{
    Board chessBoard;

    quick_imgui::loop(
        "Chess",
        {
            .init = [&]() {},
            .loop = [&]() { renderChessBoard(chessBoard); },
        }
    );

    return 0;
}
