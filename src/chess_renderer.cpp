#include "chess_renderer.hpp"
#include <vector>
#include "board.hpp"
#include "chess_controller.hpp"
#include "game_state.hpp"
#include "texture_manager.hpp"
#include <array>
#include <random>
#include <cmath>
#include <algorithm>

extern int turnCount;

float drawNormal(float mu, float sigma) {
    float u1 = RAND();
    // Avoid log(0) which gives -infinity
    if (u1 < 1e-7f) u1 = 1e-7f;
    float u2 = RAND();
    float z = sqrt(-2.0f * log(u1)) * cos(2.0f * 3.1415926f * u2);
    return mu + sigma * z;
}

static std::mt19937& getEngine()
{
    static std::mt19937 gen(std::random_device{}());
    return gen;
}

float RAND()
{
    static std::uniform_real_distribution<float> dist(0.0f, 1.0f);
    return dist(getEngine());
}

int randInt(int n)
{
    std::uniform_int_distribution<int> dist(0, n - 1);
    return dist(getEngine());
}

static sf::Color generateRandomColor(float r_min, float r_max, float g_min, float g_max, float b_min, float b_max)
{
    float r = r_min + RAND() * (r_max - r_min);
    float g = g_min + RAND() * (g_max - g_min);
    float b = b_min + RAND() * (b_max - b_min);
    return sf::Color(
        static_cast<sf::Uint8>(r * 255),
        static_cast<sf::Uint8>(g * 255),
        static_cast<sf::Uint8>(b * 255)
    );
}

void getBoardOffset(const sf::RenderWindow& window, int numRows, int numCols, float& offsetX, float& offsetY, float& tileSize)
{
    sf::Vector2u winSize = window.getSize();
    tileSize = std::min(
        static_cast<float>(winSize.x) / numCols,
        static_cast<float>(winSize.y) / numRows
    );
    offsetX = (static_cast<float>(winSize.x) - tileSize * numCols) / 2.0f;
    offsetY = (static_cast<float>(winSize.y) - tileSize * numRows) / 2.0f;
}

void renderChessBoard(Board& chessBoard, sf::RenderWindow& window, tgui::Gui& gui, SelectedPiece& selectedPiece)
{
    // Background
    sf::Vector2u winSize = window.getSize();
    sf::RectangleShape bg(sf::Vector2f(static_cast<float>(winSize.x), static_cast<float>(winSize.y)));
    bg.setFillColor(sf::Color(30, 30, 50));
    window.draw(bg);

    auto& board = chessBoard.getBoard();
    int numRows = board.size();
    int numCols = board[0].size();

    // Tile color cache — regenerate when board size changes (expansion can shift columns)
    static std::vector<std::vector<sf::Color>> tileColors;
    static int cachedRows = 0, cachedCols = 0;

    if (numRows != cachedRows || numCols != cachedCols)
    {
        tileColors.resize(numRows);
        for (int row = 0; row < numRows; ++row)
        {
            tileColors[row].resize(numCols);
            for (int col = 0; col < numCols; ++col)
            {
                if ((row + col) % 2 == 0)
                    tileColors[row][col] = generateRandomColor(0.9f, 1.0f, 0.7f, 0.9f, 0.7f, 0.9f);
                else
                    tileColors[row][col] = generateRandomColor(0.2f, 0.4f, 0.4f, 0.6f, 0.8f, 1.0f);
            }
        }
        cachedRows = numRows;
        cachedCols = numCols;
    }

    // Dynamic tile size
    float tileSize = std::min(
        static_cast<float>(winSize.x) / numCols,
        static_cast<float>(winSize.y) / numRows
    );

    // Center the board on screen
    float boardW = tileSize * numCols;
    float boardH = tileSize * numRows;
    float offsetX = (static_cast<float>(winSize.x) - boardW) / 2.0f;
    float offsetY = (static_cast<float>(winSize.y) - boardH) / 2.0f;

    sf::Color highlightColor(255, 255, 0, 180);
    sf::Color validMoveColor(0, 255, 0, 128);
    sf::Color grayOverlay(128, 128, 128, 128);

    // Precompute valid moves once (not per tile)
    std::vector<std::pair<int, int>> validMoves;
    if (selectedPiece.isSelected)
    {
        const Piece& selected = chessBoard.getBoard()[selectedPiece.row][selectedPiece.col];
        if (selected.fatigueRemaining <= 0)
            validMoves = getValidMovesForSelected(chessBoard, selectedPiece);
    }

    for (int row = 0; row < numRows; ++row)
    {
        for (int col = 0; col < numCols; ++col)
        {
            float x = offsetX + col * tileSize;
            float y = offsetY + row * tileSize;

            // Draw tile
            sf::RectangleShape tile(sf::Vector2f(tileSize, tileSize));
            tile.setPosition(x, y);

            sf::Color tileColor = tileColors[row][col];
            if (selectedPiece.isSelected && selectedPiece.row == row && selectedPiece.col == col)
                tileColor = highlightColor;

            tile.setFillColor(tileColor);
            window.draw(tile);

            // Draw valid moves
            for (const auto& move : validMoves)
            {
                if (move.first == row && move.second == col)
                {
                    float radius = tileSize / 4.0f;
                    sf::CircleShape circle(radius);
                    circle.setPosition(x + tileSize / 2.0f - radius, y + tileSize / 2.0f - radius);
                    circle.setFillColor(validMoveColor);
                    window.draw(circle);
                }
            }

            // Draw piece
            if (!board[row][col].type.empty())
            {
                Piece& p = board[row][col];

                // Initialize piece color (Box-Muller) — kept for sprite tinting
                if (!p.colorInitialized)
                {
                    float sigma = 0.07f;
                    float baseColor = std::clamp(drawNormal(p.isWhite ? 1.0f : 0.0f, sigma), 0.0f, 1.0f);
                    p.r = baseColor;
                    p.g = baseColor;
                    p.b = baseColor;
                    p.colorInitialized = true;
                }

                // Get texture and draw sprite
                try {
                    const sf::Texture& tex = getTexture(p.type, p.isWhite);
                    sf::Sprite sprite(tex);

                    float padding = tileSize * 0.1f;
                    float spriteSize = tileSize - 2.0f * padding;
                    float scaleX = spriteSize / tex.getSize().x;
                    float scaleY = spriteSize / tex.getSize().y;
                    sprite.setScale(scaleX, scaleY);
                    sprite.setPosition(x + padding, y + padding);

                    window.draw(sprite);
                } catch (...) {
                    // Fallback: draw colored rectangle with text if texture missing
                    sf::RectangleShape pieceRect(sf::Vector2f(tileSize * 0.7f, tileSize * 0.7f));
                    pieceRect.setPosition(x + tileSize * 0.15f, y + tileSize * 0.15f);
                    pieceRect.setFillColor(sf::Color(
                        static_cast<sf::Uint8>(p.r * 255),
                        static_cast<sf::Uint8>(p.g * 255),
                        static_cast<sf::Uint8>(p.b * 255)
                    ));
                    window.draw(pieceRect);
                }

                // Bishop explosion countdown overlay
                if (p.type == "B" && p.explosionCountdown > 0)
                {
                    sf::RectangleShape overlay(sf::Vector2f(tileSize, tileSize));
                    overlay.setPosition(x, y);
                    if (p.explosionCountdown == 3)
                        overlay.setFillColor(sf::Color(255, 255, 0, 80));
                    else if (p.explosionCountdown == 2)
                        overlay.setFillColor(sf::Color(255, 128, 0, 100));
                    else if (p.explosionCountdown == 1)
                        overlay.setFillColor(sf::Color(255, 0, 0, 120));
                    window.draw(overlay);
                }

                // Fatigue overlay
                if (p.fatigueRemaining > 0)
                {
                    sf::RectangleShape fatigueRect(sf::Vector2f(tileSize, tileSize));
                    fatigueRect.setPosition(x, y);
                    fatigueRect.setFillColor(grayOverlay);
                    window.draw(fatigueRect);
                }
            }
        }
    }

    handlePawnPromotion(chessBoard, gui);
    showVictoryPopup(chessBoard, gui);
}
